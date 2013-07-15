#include "VocableList.h"
#include "vocabel.h"

VocableList::VocableList(CategoryPtr cat, QObject *parent) :
    QAbstractItemModel(parent),
    category(cat)
{
    refreshCache();
}

QVariant VocableList::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (index.column() == 0)
            return vocListCache.at(index.row())->language1;
        else
            return vocListCache.at(index.row())->language2;
    }
    return QVariant();
}

QVariant VocableList::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QAbstractItemModel::headerData(section, orientation, role);

    if (section == 0)
        return category->languageFrom();
    else
        return category->languageTo();
}

void VocableList::refreshCache()
{
    vocListCache = Vocabel::objects().filter(DQWhere("category = ", category->id)).all();
    emit dataChanged(index(0,0),index(rowCount(),columnCount()));
}

QModelIndex VocableList::index(int row, int column, const QModelIndex &) const
{
    return createIndex(row, column, row);
}

QModelIndex VocableList::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int VocableList::rowCount(const QModelIndex &) const
{
    return vocListCache.size();
}

int VocableList::columnCount(const QModelIndex &) const
{
    return 2;
}
