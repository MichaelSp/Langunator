#pragma once

#include "pch.h"
#include "category.h"

class Vocabel: public DQModel {
    DQ_MODEL
public:

    void next();
    void previous();
    void back();
    void stay();

    DQForeignKey<Category> category;
    DQField<QString> language1;
    DQField<QString> language2;
    DQField<int> lektion;
    DQField<int> nextClicks;
    DQField<int> previousClicks;
    DQField<int> stayClicks;
    DQField<int> backClicks;
    DQField<int> rightInRow;
    DQField<QDateTime> lastAsked;
};

DQ_DECLARE_MODEL(Vocabel,
                 "vocabel", // the table name.
                 DQ_FIELD(language1, DQNotNull),
                 DQ_FIELD(language2, DQNotNull),
                 DQ_FIELD(lektion, DQDefault(0)),
                 DQ_FIELD(nextClicks, DQDefault(0)),
                 DQ_FIELD(previousClicks, DQDefault(0)),
                 DQ_FIELD(stayClicks, DQDefault(0)),
                 DQ_FIELD(backClicks, DQDefault(0)),
                 DQ_FIELD(category, DQNotNull)
                 )
