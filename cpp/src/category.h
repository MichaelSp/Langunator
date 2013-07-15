#ifndef CATEGORY_H
#define CATEGORY_H

#include "pch.h"
#include <dquest.h>
#include <dqsql.h>
#include <dqclause.h>
#include <QString>

#include <QObject>

class DB;
class Category : public DQModel
{
    DQ_MODEL
public:
    Category(){}
    explicit Category(QString from, QString to);
    Category(const Category&);

    QString categoryName()const{return langFrom + " -> " + langTo;}

    bool isValid()const{return !(((QString)langFrom).isEmpty() || ((QString)langTo).isEmpty());}

    void setLanguageFrom(QString from){langFrom=from;}
    void setLanguageTo(QString to){langTo=to;}
    QString languageFrom(){return langFrom;}
    QString languageTo(){return langTo;}

    bool add(QString lang1, QString lang2);
    virtual bool save(bool forceInsert,bool forceAllField);

private:
    DQField<QString> langFrom;
    DQField<QString> langTo;
};

DQ_DECLARE_MODEL(Category,
                 "category",
                 DQ_FIELD(langFrom),
                 DQ_FIELD(langTo)
                 )


typedef QList<Category> Categories;
typedef QSharedPointer<Category> CategoryPtr;
typedef QList<CategoryPtr> CategoriesPtr;

Q_DECLARE_METATYPE(Category)
Q_DECLARE_METATYPE(CategoryPtr)
Q_DECLARE_METATYPE(Categories)
Q_DECLARE_METATYPE(CategoriesPtr)

#endif // CATEGORY_H
