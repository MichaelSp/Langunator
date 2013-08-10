#include "vocabel.h"


Vocable::Vocable(const QJsonObject &obj, CategoryPtr cat)
{
    *this << obj;
    Category localCat;
    localCat.load( DQWhere("langFrom") == cat->languageFrom() && DQWhere("langTo") == cat->languageTo() ||
                   DQWhere("langTo") == cat->languageFrom() && DQWhere("langFrom") == cat->languageTo());
    if (!localCat.isValid()) {
        localCat.setLanguageFrom(cat->languageFrom());
        localCat.setLanguageTo(cat->languageTo());
        localCat.save();
    }
    category = localCat.id;
    save();
}

void Vocable::next()
{
    rightInRow.set( rightInRow().value<int>() +1 );
    nextClicks.set( nextClicks().value<int>() +1 );
    lastAsked = QDateTime::currentDateTime();
    save();
}

void Vocable::back()
{
    backClicks.set( backClicks().value<int>() +1 );
    rightInRow.set( rightInRow().value<int>() - 1 );
    lastAsked = QDateTime::currentDateTime();
    save();
}

void Vocable::backTo0()
{
    rightInRow.set( 0 );
    backTo0Clicks.set( backTo0Clicks().value<int>() +1 );
    lastAsked = QDateTime::currentDateTime();
    save();
}

void Vocable::stay()
{
    stayClicks.set( stayClicks().value<int>() +1 );
    lastAsked = QDateTime::currentDateTime();
    save();
}
