#pragma once

#include "category.h"
#include <QAbstractItemModel>

class VocableList : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit VocableList(CategoryPtr cat, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    void refresh();
private:
    CategoryPtr category;
    QSqlQuery vocabularyListQuery;
};

typedef QSharedPointer<VocableList> VocableListPtr;

