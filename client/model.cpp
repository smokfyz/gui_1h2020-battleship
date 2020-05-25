#include "model.h"

Model::Model()
{
    myField = new Field;
    enemyField = new Field;
    state = ST_PLACING_SHIPS;
}

Model::~Model()
{
    delete myField;
    delete enemyField;
}

Cell Model::getMyCell(int x, int y) const
{
    return myField->getCell(x, y);
}

Cell Model::getEnemyCell(int x, int y) const
{
    return enemyField->getCell(x, y);
}

QString Model::getMyField() const
{
    return myField->getField();
}

QString Model::getEnemyField() const
{
    return enemyField->getField();
}

State Model::getState() const
{
    return state;
}

QString Model::getLogin() const
{
    return login;
}

QString Model::getPassword() const
{
    return password;
}

void Model::setState(State st)
{
    state = st;
}

void Model::setMyCell(int x, int y, Cell cell)
{
    myField->setCell(x, y, cell);
}

void Model::setEnemyCell(int x, int y, Cell cell)
{
    enemyField->setCell(x, y, cell);
}

void Model::setLogin(const QString& str)
{
    login = str;
}

void Model::setPassword(const QString& str)
{
    password = str;
}

bool Model::checkMyField() const
{
    return (shipNum(1) == 4 && shipNum(2) == 3 &&
        shipNum(3) == 2 && shipNum(4) == 1);
}

void Model::clearEnemyField()
{
    enemyField->clear();
}

void Model::clearMyField()
{
    myField->clear();
}

int Model::shipNum(int size) const
{
    int shipNumber = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (isShip(size, i, j))
            {
                shipNumber++;
            }
        }
    }
    return shipNumber;
}

bool Model::isShip(int size, int x, int y) const
{
    if (x > 0 && myField->getCell(x - 1, y) != CL_CLEAR)
    {
        return false;
    }
    else if (y > 0 && myField->getCell(x, y - 1) != CL_CLEAR)
    {
        return false;
    }
    else if (myField->getCell(x, y) == CL_CLEAR)
    {
        return false;
    }
    int tmp = x;
    int num = 0;

    while (myField->getCell(tmp, y) != CL_CLEAR && tmp < 10)
    {
        tmp++;
        num++;
    }
    if (num == size)
    {
        if (myField->getCell(x, y + 1) != CL_CLEAR)
        {
            return false;
        }
        return true;
    }
    tmp = y;
    num = 0;

    while (myField->getCell(x, tmp) != CL_CLEAR && tmp < 10)
    {
        tmp++;
        num++;
    }
    if (num == size)
    {
        if (myField->getCell(x + 1, y) != CL_CLEAR)
        {
            return false;
        }
        return true;
    }
    return false;
}
