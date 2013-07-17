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
        switch(index.column()){
        default:
        case 0: return vocListCache.at(index.row())->language1;
        case 1: return vocListCache.at(index.row())->language2;
        case 2: return vocListCache.at(index.row())->lektion;
        }
    }
    else if (role==Qt::UserRole) {
        Vocable *voc = vocListCache.at(index.row());
        return QVariant::fromValue( voc );
    }
    return QVariant();
}

QVariant VocableList::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QAbstractItemModel::headerData(section, orientation, role);

    switch (section) {
    case 0: return category->languageFrom();
    case 1: return category->languageTo();
    default:
    case 2: return "Lektion";
    }
}

void VocableList::refreshCache()
{
    vocListCache = Vocable::objects().filter(DQWhere("category = ", category->id)).all();
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
    return 3;
}
