#include "vocabel.h"
void Vocabel::next()
{
    rightInRow.set( rightInRow().value<int>() +1 );
    nextClicks.set( nextClicks().value<int>() +1 );
    lastAsked = QDateTime::currentDateTime();
    save();
}

void Vocabel::previous()
{
    previousClicks.set( previousClicks().value<int>() +1 );
    rightInRow.set( rightInRow().value<int>() - 1 );
    lastAsked = QDateTime::currentDateTime();
    save();
}

void Vocabel::back()
{
    rightInRow.set( 0 );
    backClicks.set( backClicks().value<int>() +1 );
    lastAsked = QDateTime::currentDateTime();
    save();
}

void Vocabel::stay()
{
    stayClicks.set( stayClicks().value<int>() +1 );
    lastAsked = QDateTime::currentDateTime();
    save();
}
