#pragma once

#include "pch.h"
#include "category.h"

class Vocable: public DQModel {
    DQ_MODEL
public:

    void next();
    void back();
    void backTo0();
    void stay();

    DQForeignKey<Category> category;
    DQField<QString> language1;
    DQField<QString> language2;
    DQField<int> lektion;
    DQField<int> nextClicks;
    DQField<int> backClicks;
    DQField<int> stayClicks;
    DQField<int> backTo0Clicks;
    DQField<int> rightInRow;
    DQField<QDateTime> lastAsked;
};

DQ_DECLARE_MODEL(Vocable,
                 "vocable", // the table name.
                 DQ_FIELD(language1, DQNotNull),
                 DQ_FIELD(language2, DQNotNull),
                 DQ_FIELD(lektion, DQDefault(0)),
                 DQ_FIELD(nextClicks, DQDefault(0)),
                 DQ_FIELD(backClicks, DQDefault(0)),
                 DQ_FIELD(stayClicks, DQDefault(0)),
                 DQ_FIELD(backTo0Clicks, DQDefault(0)),
                 DQ_FIELD(category, DQNotNull)
                 )

Q_DECLARE_METATYPE(Vocable)
Q_DECLARE_METATYPE(Vocable*)
