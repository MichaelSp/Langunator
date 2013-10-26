#include "category.h"
#include "db.h"
#include "Vocable.h"

Category::Category(const QJsonObject &obj)
    :aboutToDelete(false)
{
   *this << obj;
}

Category::Category(QString from, QString to)
    :DQModel(),
     aboutToDelete(false)
{
    langFrom = from;
    langTo = to;
}

Category::Category(const Category &cat):DQModel()
{
    langFrom = cat.langFrom;
    langTo = cat.langTo;
}


bool Category::addVocable(QString lang1, QString lang2, int lektion)
{
    Vocable vok;
    vok.language1 = lang1;
    vok.language2 = lang2;
    vok.lesson = lektion;
    vok.category = *this;
    vok.lastAsked = vok.getTimestamp();
    vok.rightInRow = 0;
    vok.nextClicks = 0;
    vok.backClicks = 0;
    vok.stayClicks = 0;
    vok.backTo0Clicks = 0;
    return vok.save();
}

bool Category::save(bool forceInsert, bool forceAllField)
{
    if (aboutToDelete)
        return true;
    return DQModel::save(forceInsert, forceAllField);
}

bool Category::remove()
{
    aboutToDelete =true;
    return DQModel::remove();
}

DQList<Vocable> Category::vocables()
{
    DQList<Vocable> list = Vocable::objects().filter(DQWhere("category = ", this->id) ).all();
    return list;
}


int Category::keyboardLayoutTo() const
{
    return keyboardTo;
}

void Category::setKeyboardLayoutTo(const int &value)
{
    keyboardTo = value;
}

void Category::refreshNumbers()
{
    DQQuery<Vocable> query;
    count = query.filter(DQWhere("category") == id).count();
    lesson = query.call("avg","lesson").toInt();
}

int Category::keyboardLayoutFrom() const
{
    return keyboardFrom;
}

void Category::setKeyboardLayoutFrom(const int &value)
{
    keyboardFrom = value;
}

