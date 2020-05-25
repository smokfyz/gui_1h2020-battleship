#include "field.h"

Field::Field()
{
    clear();
}

Field::~Field()
{
}

Cell Field::getCell(int x, int y)
{
    int fieldIndex = y * 10 + x;
    if (x < 0 || y < 0 || x >= 10 || y >= 10)
    {
        return CL_CLEAR;
    }
    else if (fieldIndex >= 0 && fieldIndex < field.size())
    {
        return field[fieldIndex];
    }
    return CL_CLEAR;
}

void Field::setCell(int x, int y, Cell cell)
{
    int fieldIndex = y * 10 + x;
    if (x >= 0 && y >= 0 && x < 10 && y < 10 &&
            fieldIndex >= 0 && fieldIndex < field.size())
    {
        field[fieldIndex] = cell;
    }
    return;
}

QString Field::getField()
{
    QString stringifiedField;
    for (QVector<Cell>::iterator i = field.begin(); i != field.end(); i++)
    {
        if(*i == CL_CLEAR)
        {
            stringifiedField += "0";
        }
        else
        {
            stringifiedField += "1";
        }
    }
    return stringifiedField;
}

void Field::clear()
{
    field.fill(CL_CLEAR, 100);
}


