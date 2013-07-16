#include "category.h"
#include "db.h"
#include "vocabel.h"

Category::Category(QString from, QString to)
{
    langFrom = from;
    langTo = to;
}

Category::Category(const Category &cat)
{
    langFrom = cat.langFrom;
    langTo = cat.langTo;
}

bool Category::addVocable(QString lang1, QString lang2)
{
    Vocable vok;
    vok.language1 = lang1;
    vok.language2 = lang2;
    vok.category = *this;
    return vok.save();
}

bool Category::save(bool forceInsert, bool forceAllField)
{
    bool returnValue = DQModel::save(forceInsert, forceAllField);
    return returnValue;
}

DQList<Vocable> Category::vocables()
{
    DQList<Vocable> list = Vocable::objects().filter(DQWhere("category = ", this->id) ).all();
    return list;
}


QString Category::keyboardLayoutTo() const
{
    return keyboardTo;
}

void Category::setKeyboardLayoutTo(const QString &value)
{
    keyboardTo = value;
}

QString Category::keyboardLayoutFrom() const
{
    return keyboardFrom;
}

void Category::setKeyboardLayoutFrom(const QString &value)
{
    keyboardFrom = value;
}

