#ifndef BOX_H
#define BOX_H

#include "pch.h"
#include "category.h"

class Box: public DQModel {
    DQ_MODEL
public:
    DQForeignKey<Category> category;
};

DQ_DECLARE_MODEL(Box,
                 "box",
                 DQ_FIELD(category , DQNotNull)
                 )

#endif // BOX_H
