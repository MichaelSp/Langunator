#ifndef VOKABEL_H
#define VOKABEL_H

#include "pch.h"
#include "category.h"

class Vocabel: public DQModel {
    DQ_MODEL
public:
    DQForeignKey<Category> category;
    DQField<QString> language1;
    DQField<QString> language2;
    DQField<int> lektion;
    DQField<int> right;
    DQField<int> wrong;
};

DQ_DECLARE_MODEL(Vocabel,
                 "vocabel", // the table name.
                 DQ_FIELD(language1, DQNotNull),
                 DQ_FIELD(language2, DQNotNull),
                 DQ_FIELD(lektion, DQDefault(0)),
                 DQ_FIELD(right, DQDefault(0)),
                 DQ_FIELD(wrong, DQDefault(0)),
                 DQ_FIELD(category, DQNotNull)
                 )

#endif // VOKABEL_H
