#include "Vocable.h"


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
    lastAsked = getTimestamp();
    save();
}

void Vocable::back()
{
    backClicks.set( backClicks().value<int>() +1 );
    rightInRow.set( rightInRow().value<int>() - 1 );
    lastAsked = getTimestamp();
    save();
}

void Vocable::backTo0()
{
    rightInRow.set( 0 );
    backTo0Clicks.set( backTo0Clicks().value<int>() +1 );
    lastAsked = getTimestamp();
    save();
}

void Vocable::stay()
{
    stayClicks.set( stayClicks().value<int>() +1 );
    lastAsked = getTimestamp();
    save();
}

// returns current date with the time set to 0:00 am
QDateTime Vocable::getTimestamp()
{
    QDateTime dt = QDateTime::currentDateTime();
    dt.setTime( QTime(0,0) );
    return dt;
}
