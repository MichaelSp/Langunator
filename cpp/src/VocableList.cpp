#include "VocableList.h"
#include "vocabel.h"

VocableList::VocableList(CategoryPtr cat,  QObject *parent) :
    QSqlQueryModel(parent),
    category(cat)
{
    vocabularyListQuery.prepare("SELECT language1,language2,lesson FROM " + Vocable::TableName() + " WHERE category = :cat" );
    vocabularyListQuery.bindValue(":cat", category->id.get().toInt() );
    refresh();
}

QVariant VocableList::data(const QModelIndex &index, int role) const
{
   if (index.isValid() && role==Qt::FontRole) {
        switch(index.column()){
        case 0: return category->fontFrom();
        case 1: return category->fontTo();
        default:
            return QSqlQueryModel::data(index,role);
        }
    }
   return QSqlQueryModel::data(index,role);
}

void VocableList::refresh()
{
    if (!vocabularyListQuery.exec())
        qDebug() << vocabularyListQuery.lastError().databaseText();
    setQuery(vocabularyListQuery);
}
