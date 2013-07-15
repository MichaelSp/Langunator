#include "vocabel.h"


void Vocabel::next()
{
    rightInRow++;
    next++;
    lastAsked = QDateTime::currentDateTime();
    save();
}

void Vocabel::previous()
{
    previous++;
    rightInRow--;
    lastAsked = QDateTime::currentDateTime();
    save();
}

void Vocabel::back()
{
    rightInRow = 0;
    back++;
    lastAsked = QDateTime::currentDateTime();
    save();
}

void Vocabel::stay()
{
    stay++;
    lastAsked = QDateTime::currentDateTime();
    save();
}
