#pragma once

#include "category.h"
#include <QAbstractItemModel>

class VocableList : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit VocableList(CategoryPtr category, QObject *parent = 0);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void refreshCache();
public slots:

private:
    CategoryPtr category;
    DQList<Vocabel> vocListCache;
};

typedef QSharedPointer<VocableList> VocableListPtr;

