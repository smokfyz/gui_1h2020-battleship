#include <QAbstractSocket>
#include "controller.h"

const QString DEFAULT_HOST_ADDRESS = "193.169.33.254";
const quint16 DEFAULT_SERVER_PORT = 1234;
const quint16 DEFAULT_SERVER_TIMEOUT = 5000;

Controller::Controller(Model* model, LogDataModel *log):
    model(model),
    gameLog(log),
    serverAddress(QHostAddress(DEFAULT_HOST_ADDRESS)),
    serverPort(DEFAULT_SERVER_PORT)
{
    client = new QTcpSocket(this);
    connect(client, SIGNAL(readyRead()), this, SLOT(onDataReceived()));
    connect(client, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onError(QAbstractSocket::SocketError)));
}

Controller::~Controller()
{
}

void Controller::onMousePressed(const QPoint& position, bool setShip)
{
    if (model->getState() == ST_PLACING_SHIPS)
    {
        QPoint point = getMyFieldCoord(position);
        if (point.x() == -1 || point.y() == -1)
        {
            return;
        }
        qDebug() << "Ship at" << point.x() << point.y();
        model->setMyCell(point.x(), point.y(), setShip ? CL_SHIP : CL_CLEAR);
        emit stateChanged();
        return;
    }
    else if (model->getState() == ST_MAKING_STEP)
    {
        QPoint point = getEnemyFieldCoord(position);
        if (point.x() == -1 || point.y() == -1)
        {
            return;
        }
        qDebug() << "Going to" << point.x() << point.y();
        Cell cell = model->getEnemyCell(point.x(), point.y());
        if (cell != CL_CLEAR)
        {
            return;
        }
        model->setEnemyCell(point.x(), point.y(), CL_DOT);

        QString gameStep;
        gameStep = QString("step:%1:%2:").arg(point.x()).arg(point.y());
        qDebug() << gameStep;
        client->write(gameStep.toLocal8Bit());
        model->setState(ST_WAITING_STEP);
        emit stateChanged();
        return;
    }
}

void Controller::onDataReceived()
{
    QString data;
    data = client->readAll();
    parseData(data);
    emit stateChanged();
}

void Controller::parseData(const QString& data)
{
    parseFound(data);
    parseWrongField(data);
    parseWrongStep(data);
    parseFields(data);
    parseGo(data);
    parseGameResult(data);
    parseErrorInfo(data);
    parsePing(data);
}

bool Controller::parsePing(const QString& data)
{
    QRegExp rx("ping:");
    if (rx.indexIn(data) == -1)
    {
        return false;
    }
    qDebug() << "Ping request";
    client->write("pong:\n");
    return true;
}

bool Controller::parseFound(const QString& data)
{
    QRegExp rx("found:((\\w|\\d)+):");
    if (rx.indexIn(data) == -1)
    {
        return false;
    }
    gameLog->addMessageToLog(QTime::currentTime().toString("hh:mm:ss") + ": Игрок " +
                             rx.cap(1) + " подключился к Вам!");
    emit gameOpponent(rx.cap(1));
    return true;
}

bool Controller::parseGo(const QString& data)
{
    QRegExp rx("go:");
    if (rx.indexIn(data) == -1)
    {
        return false;
    }
    gameLog->addMessageToLog(QTime::currentTime().toString("hh:mm:ss") + ": Ваш ход!");
    model->setState(ST_MAKING_STEP);
    return true;
}

bool Controller::parseErrorInfo(const QString& data)
{
    if (checkError("wronguser:", data))
    {
        emitError(GEM_WRONG_USER);
    }
    else if (checkError("alreadyauth:", data))
    {
        emitError(GEM_ALREADY_CONNECTED);
    }
    return true;
}

bool Controller::checkError(const QString& error, const QString& data)
{
    QRegExp rx(error);
    if (rx.indexIn(data) == -1)
    {
        return false;
    }
    return true;
}

void Controller::emitError(GameErrorMessage error)
{
    qDebug() << "Error:" << error;
    model->setState(ST_PLACING_SHIPS);
    connectionError = true;
    emit gameError(error);
}

bool Controller::parseWrongStep(const QString& data)
{
    QRegExp rx("wrongstep:");
    if (rx.indexIn(data) == -1)
    {
        return false;
    }
    qDebug() << "Maked wrong step";
    model->setState(ST_MAKING_STEP);
    return true;
}

bool Controller::parseWrongField(const QString& data)
{
    QRegExp rx("wrongfield:");
    if(rx.indexIn(data) == -1)
    {
        return false;
    }
    qDebug() << "Maked wrong field";
    model->setState(ST_PLACING_SHIPS);
    model->clearEnemyField();
    model->clearMyField();
    emit gameError(GEM_WRONG_FIELD);
    return true;
}

bool Controller::parseFields(const QString& data)
{
    QRegExp rx("field(\\d):(\\w+):(\\d):(\\d):");
    int pos = 0;
    while ((pos = rx.indexIn(data, pos)) != -1)
    {
        const QString& type = rx.cap(2);
        int field = rx.cap(1).toInt();
        int xpos = rx.cap(3).toInt();
        int ypos = rx.cap(4).toInt();
        Cell cell = type == "half" ? CL_HALF : (type == "kill" ? CL_SHIP : CL_DOT);

        if (type == "half" || type == "kill")
        {
            playHitSound();
        }
        else
        {
            playMissSound();
        }
        markShip(xpos, ypos, cell, field == 2);
        logGameStep(xpos, ypos, type, field == 2);
        pos += rx.matchedLength();
    }
    return pos;
}

void Controller::markShip(int x, int y, Cell cell, bool atEnemyField)
{
    if (cell == CL_SHIP)
    {
        if (!atEnemyField)
        {
            cell = CL_HALF;
        }
        else
        {
            for (int i = -1; i <= 1; i++)
            {
                markEnemyPoint(x + 1, y + i, CL_DOT);
                markEnemyPoint(x - 1, y + i, CL_DOT);
                markEnemyPoint(x + i, y + 1, CL_DOT);
                markEnemyPoint(x + i, y - 1, CL_DOT);
            }
        }
    }
    if (atEnemyField)
    {
        model->setEnemyCell(x, y, cell);
    }
    else
    {
        model->setMyCell(x, y, cell);
    }
}

void Controller::logGameStep(int x, int y, const QString& type, bool enemyStep)
{
    char logX = static_cast<char>(x + 'A');
    int logY = y + 1;
    QString playerString = (enemyStep) ? "Ход противника: " : "Ваш ход: ";
    QString stepType = (type == "half") ? "Ранен!" : ((type == "kill") ? "Убит!"  : "Мимо!");
    gameLog->addMessageToLog(QTime::currentTime().toString("hh:mm:ss") + ": " +
                             playerString + logX + QString::number(logY) + ". " + stepType);
}

void Controller::markEnemyPoint(int x, int y, Cell cell)
{
    if (model->getEnemyCell(x, y) == CL_CLEAR)
    {
        model->setEnemyCell(x, y, cell);
    }
}

bool Controller::parseGameResult(const QString& data)
{
    QRegExp rx("win:"), rx2("lose:");
    if (rx.indexIn(data) != -1)
    {
        gameLog->addMessageToLog(QTime::currentTime().toString("hh:mm:ss") + ": Вы победили! Поздравляем!");
        emit gameResult(GR_WON);
        model->setState(ST_PLACING_SHIPS);
        model->clearMyField();
        model->clearEnemyField();
        return true;
    }
    else if (rx2.indexIn(data) != -1)
    {
        gameLog->addMessageToLog(QTime::currentTime().toString("hh:mm:ss") +
                                 ": Вы проиграли! Но возможно Вам повезёт в следующий раз...");
        emit gameResult(GR_LOST);
        model->setState(ST_PLACING_SHIPS);
        model->clearMyField();
        model->clearEnemyField();
        return true;
    }
    return false;
}

void Controller::onGameStart()
{
    if (!model->checkMyField())
    {
        emit gameError( GEM_WRONG_FIELD );
        return;
    }
    if (client->state() == QAbstractSocket::ConnectedState)
    {
        emit gameError(GEM_ALREADY_CONNECTED);
        return;
    }
    client->connectToHost(serverAddress, serverPort);
    if (!client->waitForConnected(DEFAULT_SERVER_TIMEOUT))
    {
        if (client->error() == QAbstractSocket::SocketTimeoutError)
        {
            emit gameError(GEM_SERVER_CONNECTION_TIMEOUT);
        }
        return;
    }
    gameLog->addMessageToLog(QTime::currentTime().toString("hh:mm:ss") + ": Подключение к серверу " +
                             qPrintable(serverAddress.toString()) + ":" + QString::number(serverPort) +
                             " под логином " + qPrintable(model->getLogin()) + "...");
}

void Controller::onGameQuit()
{
    if (client->state() == QAbstractSocket::ConnectedState)
    {
        gameLog->addMessageToLog(QTime::currentTime().toString("hh:mm:ss") + ": Отключение от сервера");
        client->write("disconnect:");
        client->flush();
        client->disconnectFromHost();
        model->clearEnemyField();
        model->clearMyField();
        model->setState(ST_PLACING_SHIPS);
    }
}

void Controller::clearFields()
{
    if (model->getState() != ST_PLACING_SHIPS)
    {
        return;
    }
    model->clearEnemyField();
    model->clearMyField();
}

void Controller::randomField()
{
    if (model->getState() != ST_PLACING_SHIPS)
    {
        return;
    }
    model->clearMyField();
    for (int i = 1, k = 4; i <= 4; i++, k--)
    {
        for (int j = 0; j < i; j++)
        {
            placeShipAtRandom(k);
        }
    }
}

void Controller::placeShipAtRandom(int size)
{
    int p = 0;
    int q = 0;
    bool r = true;
    bool isOk = true;

    while (isOk)
    {
        p = qrand() % (10 - size + 1);
        q = qrand() % (10 - size + 1);
        r = qrand() % 2;
        for (int k = r * p + !r * q - 1; k < (r * p + !r * q + size + 1); k++)
        {
            if (model->getMyCell(r * k + !r * p, r * q + !r * k) == CL_SHIP ||
                model->getMyCell(r * k + !r * (p - 1), r * (q - 1) + !r * k) == CL_SHIP ||
                model->getMyCell(r * k + !r * (p + 1), r * (q + 1) + !r * k) == CL_SHIP)
            {
                isOk = false;
            }
        }
        isOk = ! isOk;
    }
    for(int k = r * p + !r * q; k < (r * p + !r * q + size); k++)
    {
        model->setMyCell(r * k + !r * p, r * q + !r * k, CL_SHIP);
    }
}

void Controller::onError(QAbstractSocket::SocketError socketError)
{
    qDebug() << client->errorString();
    if (model->getState() == ST_WAITING_STEP ||
        model->getState() == ST_MAKING_STEP)
    {
        model->setState(ST_PLACING_SHIPS);
    }
    if (socketError == QAbstractSocket::ConnectionRefusedError)
    {
        emit gameError(GEM_SERVER_CONNECTION_REFUSED);
    }

    if (socketError == QAbstractSocket::HostNotFoundError ||
        socketError == QAbstractSocket::SocketTimeoutError)
    {
        emit gameError(GEM_SERVER_UNAVAILABLE);
    }
}

void Controller::onConnected()
{
    QString response;
    QString request;
    connectionError = false;

    request = QString("mbclient:2:%1:%2:")
        .arg(model->getLogin())
        .arg(model->getPassword());
    client->write(request.toLocal8Bit());
    if (!client->waitForReadyRead(DEFAULT_SERVER_TIMEOUT))
    {
        return;
    }
    if (connectionError)
    {
        return;
    }
    response = client->readAll();
    qDebug() << response;

    request = "field:4:" + model->getMyField() + ":";
    client->write(request.toLocal8Bit());

    qDebug() << request;
    model->setState(ST_WAITING_STEP);
}

State Controller::getState() const
{
    return model->getState();
}

void Controller::setConnectionInfo(const QString& address, quint16 port, const QString& login, const QString& password)
{
    serverAddress = QHostAddress(address);
    serverPort = port;
    model->setLogin(login);
    model->setPassword(password);
}

QString Controller::getServerAddress() const
{
    return serverAddress.toString();
}

quint16 Controller::getServerPort() const
{
    return serverPort;
}

QString Controller::getUserLogin() const
{
    return model->getLogin();
}

QString Controller::getUserPassword() const
{
    return model->getPassword();
}
