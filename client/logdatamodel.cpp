#include "logdatamodel.h"

LogDataModel::LogDataModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

int LogDataModel::columnCount(const QModelIndex &) const
{
    return 1;
}

int LogDataModel::rowCount(const QModelIndex &) const
{
    return dataList.size();
}

QVariant LogDataModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return dataList.at(index.row());
    }
    return QVariant();
}

QModelIndex LogDataModel::index(int row, int column, const QModelIndex &) const
{
    return createIndex(row, column, (void *) &dataList[row]);
}

QModelIndex LogDataModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

void LogDataModel::addMessageToLog(const QString &message)
{
    dataList.append(message);
    emit layoutChanged();
}
