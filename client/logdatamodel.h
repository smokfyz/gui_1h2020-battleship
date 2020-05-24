#ifndef LOGDATAMODEL_H
#define LOGDATAMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

class LogDataModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    LogDataModel(QObject *parent);

    //Model interface implementation
    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &index) const;

    void addMessageToLog(const QString &message);

private:
    QStringList dataList;
};

#endif // LOGDATAMODEL_H
