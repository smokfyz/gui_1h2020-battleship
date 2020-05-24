#include "connectiondialog.h"

ConnectionDialog::ConnectionDialog(QWidget* parent) :
    QDialog(parent)
{
    this->setFixedSize(280, 150);
    this->setWindowTitle("Start the game");

    applyButton = new QPushButton("Apply", this);
    cancelButton = new QPushButton("Cancel", this);

    QLabel *addrLabel = new QLabel("Host:", this);
    QLabel *loginLabel = new QLabel("Login:", this);
    QLabel *passLabel = new QLabel("Password:", this);

    addressTextBox = new QLineEdit(this);
    loginTextBox = new QLineEdit(this);
    passwordTextBox = new QLineEdit(this);

    QGridLayout *layout = new QGridLayout(this);
    QHBoxLayout *buttonsLayout = new QHBoxLayout(this);
    layout->addWidget(addrLabel, 0, 0);
    layout->addWidget(addressTextBox, 0, 1);
    layout->addWidget(loginLabel, 1, 0);
    layout->addWidget(loginTextBox, 1, 1);
    layout->addWidget(passLabel, 2, 0);
    layout->addWidget(passwordTextBox, 2, 1);
    buttonsLayout->addWidget(applyButton);
    buttonsLayout->addWidget(cancelButton);
    layout->addItem(buttonsLayout, 3, 0, 1, 2);
    setLayout(layout);

    passwordTextBox->setEchoMode(QLineEdit::Password);
    applyButton->setDefault(true);
    loginTextBox->installEventFilter(this);
    passwordTextBox->installEventFilter(this);

    addrLabel->setVisible(false);
    addressTextBox->setVisible(false);

    connect(applyButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

ConnectionDialog::~ConnectionDialog()
{
}

void ConnectionDialog::setAddressString(const QString &address, quint16 port)
{
    setAddressString(QHostAddress(address), port);
}

void ConnectionDialog::setAddressString(const QHostAddress &address, quint16 port)
{
    if (!addressTextBox)
    {
        return;
    }
    addressTextBox->setText(QString("%1:%2").arg(address.toString()).arg(port));
}

void ConnectionDialog::setLogin(const QString &login)
{
    loginTextBox->setText(login);
}

void ConnectionDialog::setPassword(const QString &password)
{
    passwordTextBox->setText(password);
}

QString ConnectionDialog::getAddress() const
{
    return address;
}

QString ConnectionDialog::getLogin() const
{
    return login;
}

QString ConnectionDialog::getPassword() const
{
    return password;
}

quint16 ConnectionDialog::getPort()
{
    return port;
}

void ConnectionDialog::accept()
{
    QHostAddress addr( addressTextBox->text().replace(QRegExp(":.*"), ""));
    bool isOk;
    port = addressTextBox->text().replace(QRegExp(".*:"), "").toUShort(&isOk);
    login = loginTextBox->text();
    password = passwordTextBox->text();

    if (!addr.isNull() && isOk)
    {
        address = addr.toString();
        qDebug("Server address and port: %s:%d", qPrintable(address), port);
        done(QDialog::Accepted);
        return;
    }
    QMessageBox::warning(this, "Warning", "Specify the valid IPv4 address");
    addressTextBox->setFocus();
}

bool ConnectionDialog::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        if (object == loginTextBox)
        {
            loginTextBox->selectAll();
            return true;
        }
        if (object == passwordTextBox)
        {
            passwordTextBox->selectAll();
            return true;
        }
        return false;
    }
    return QDialog::eventFilter( object, event );
}
