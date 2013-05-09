#ifndef VOKABEL_H
#define VOKABEL_H

#include "pch.h"
#include "box.h"

class Vokabel: public DQModel {
    DQ_MODEL
public:
    DQField<QString> language1;
    DQField<QString> language2;
    DQForeignKey<Box> box;
};

DQ_DECLARE_MODEL(Vokabel,
                 "vokabel", // the table name.
                 DQ_FIELD(language1, DQNotNull),
                 DQ_FIELD(language2, DQNotNull),
                 DQ_FIELD(box, DQNotNull)
                 )

#endif // VOKABEL_H
