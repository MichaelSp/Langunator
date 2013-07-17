#ifndef CATEGORY_H
#define CATEGORY_H

#include "pch.h"
#include <dquest.h>
#include <dqsql.h>
#include <dqclause.h>
#include <QString>

#include <QObject>

class Vocable;
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

    DQList<Vocable> vocables();

    bool addVocable(QString lang1, QString lang2, int lektion);
    virtual bool save(bool forceInsert = false, bool forceAllField=false);

    int keyboardLayoutFrom() const;
    void setKeyboardLayoutFrom(const int &value);
    int keyboardLayoutTo() const;
    void setKeyboardLayoutTo(const int &value);

private:
    DQField<QString> langFrom;
    DQField<QString> langTo;
    DQField<int> keyboardFrom;
    DQField<int> keyboardTo;
};

DQ_DECLARE_MODEL(Category,
                 "category",
                 DQ_FIELD(langFrom),
                 DQ_FIELD(langTo),
                 DQ_FIELD(keyboardFrom),
                 DQ_FIELD(keyboardTo)
                 )


typedef QList<Category> Categories;
typedef QSharedPointer<Category> CategoryPtr;
typedef QList<CategoryPtr> CategoriesPtr;

Q_DECLARE_METATYPE(Category)
Q_DECLARE_METATYPE(CategoryPtr)
Q_DECLARE_METATYPE(Categories)
Q_DECLARE_METATYPE(CategoriesPtr)

#endif // CATEGORY_H
