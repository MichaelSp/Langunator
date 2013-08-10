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
class TabImportExport;

class Category : public DQModel
{
    friend class TabImportExport;
    friend class GithubAPI;
    friend void operator <<(Category &cat, const QJsonObject &obj);
    friend void operator <<(QJsonObject &obj, const Category &cat);
    DQ_MODEL
public:
    Category(const QJsonObject &obj);
    Category():aboutToDelete(false){}

    explicit Category(QString from, QString to);
    Category(const Category&);

    QString categoryName()const{return langFrom + " -> " + langTo;}

    bool isValid()const{return !(((QString)langFrom).isEmpty() || ((QString)langTo).isEmpty());}

    void setLanguageFrom(QString from){langFrom=from;}
    void setLanguageTo(QString to){langTo=to;}
    QString languageFrom()const{return langFrom;}
    QString languageTo()const{return langTo;}

    void setFontFrom(QFont from){fontTypeFrom=QVariant::fromValue(from);}
    void setFontTo(QFont to){fontTypeTo=QVariant::fromValue(to);}
    QFont fontFrom()const {return fontTypeFrom.get().value<QFont>();}
    QFont fontTo()const {return fontTypeTo.get().value<QFont>();}

    DQList<Vocable> vocables();

    bool addVocable(QString lang1, QString lang2, int lektion);
    virtual bool save(bool forceInsert = false, bool forceAllField=false);
    virtual bool remove();

    int keyboardLayoutFrom() const;
    void setKeyboardLayoutFrom(const int &value);
    int keyboardLayoutTo() const;
    void setKeyboardLayoutTo(const int &value);

    void refreshNumbers();

private:
    bool aboutToDelete;
    DQField<QString> langFrom;
    DQField<QString> langTo;
    DQField<QString> fontTypeFrom;
    DQField<QString> fontTypeTo;
    DQField<int> keyboardFrom;
    DQField<int> keyboardTo;
    DQField<QString> author;
    DQField<QString> sourceFileName;
    /** only set whene used as downloadable package */
    int lesson, count;
};

inline void operator <<(Category &cat, const QJsonObject &obj){
    cat.langFrom = obj.value("lang1").toString("<undefined>");
    cat.langTo = obj.value("lang2").toString("<undefined>");
    cat.author = obj.value("author").toString("Anonymous");
    cat.lesson = obj.value("lesson").toDouble();
    cat.count = obj.value("count").toDouble();
    cat.sourceFileName = obj.value("file").toString("");
}

inline void operator <<(QJsonObject &obj, const Category &cat) {
    QVariantMap map;
    map.insert("lang1", cat.languageFrom());
    map.insert("lang2", cat.languageTo());
    map.insert("author", cat.author.get().toString());
    map.insert("lesson", cat.lesson);
    map.insert("count",  cat.count);
    map.insert("file", cat.sourceFileName.get().toString());
    obj = QJsonObject::fromVariantMap(map);
}

DQ_DECLARE_MODEL(Category,
                 "category",
                 DQ_FIELD(author),
                 DQ_FIELD(sourceFileName),
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
