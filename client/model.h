#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include <QObject>
#include "field.h"

enum State
{
    ST_PLACING_SHIPS,
    ST_WAITING_STEP,
    ST_MAKING_STEP
};

class Model: public QObject
{
    Q_OBJECT
public:
    Model();
    ~Model();

    State getState() const;
    Cell getMyCell(int x, int y) const;
    Cell getEnemyCell(int x, int y) const;
    QString getMyField() const;
    QString getEnemyField() const;
    QString getLogin() const;
    QString getPassword() const;

    void setState(State);
    void setMyCell(int x, int y, Cell cell);
    void setEnemyCell(int x, int y, Cell cell);
    void setLogin(const QString& str);
    void setPassword(const QString& str);

    bool checkMyField() const;
    void clearEnemyField();
    void clearMyField();

private:
    int shipNum(int size) const;
    bool isShip(int size, int x, int y) const;

private:
    Field* myField, *enemyField;
    State state;
    QString login, password;
};

#endif // MODEL_H
