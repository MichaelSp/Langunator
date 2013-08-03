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
    Category():aboutToDelete(false){}

    explicit Category(QString from, QString to);
    Category(const Category&);

    QString categoryName()const{return langFrom + " -> " + langTo;}

    bool isValid()const{return !(((QString)langFrom).isEmpty() || ((QString)langTo).isEmpty());}

    void setLanguageFrom(QString from){langFrom=from;}
    void setLanguageTo(QString to){langTo=to;}
    QString languageFrom(){return langFrom;}
    QString languageTo(){return langTo;}

    void setFontFrom(QFont from){fontTypeFrom=QVariant::fromValue(from);}
    void setFontTo(QFont to){fontTypeTo=QVariant::fromValue(to);}
    QFont fontFrom(){return fontTypeFrom.get().value<QFont>();}
    QFont fontTo(){return fontTypeTo.get().value<QFont>();}

    DQList<Vocable> vocables();

    bool addVocable(QString lang1, QString lang2, int lektion);
    virtual bool save(bool forceInsert = false, bool forceAllField=false);
    virtual bool remove();

    int keyboardLayoutFrom() const;
    void setKeyboardLayoutFrom(const int &value);
    int keyboardLayoutTo() const;
    void setKeyboardLayoutTo(const int &value);

private:
    bool aboutToDelete;
    DQField<QString> langFrom;
    DQField<QString> langTo;
    DQField<QString> fontTypeFrom;
    DQField<QString> fontTypeTo;
    DQField<int> keyboardFrom;
    DQField<int> keyboardTo;
};

DQ_DECLARE_MODEL(Category,
                 "category",
                 DQ_FIELD(langFrom),
                 DQ_FIELD(langTo),
                 DQ_FIELD(fontTypeFrom),
                 DQ_FIELD(fontTypeTo),
                 DQ_FIELD(keyboardFrom, DQDefault(0)),
                 DQ_FIELD(keyboardTo, DQDefault(0))
                 )


typedef QList<Category> Categories;
typedef QSharedPointer<Category> CategoryPtr;
typedef QList<CategoryPtr> CategoriesPtr;

Q_DECLARE_METATYPE(Category)
Q_DECLARE_METATYPE(CategoryPtr)
Q_DECLARE_METATYPE(Categories)
Q_DECLARE_METATYPE(CategoriesPtr)

#endif // CATEGORY_H
