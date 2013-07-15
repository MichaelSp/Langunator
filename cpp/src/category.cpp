
#include "category.h"
#include "db.h"
#include "vocabel.h"

Category::Category(QString from, QString to)
{
    langFrom = from;
    langTo = to;
}

Category::Category(const Category &cat)
{
    langFrom = cat.langFrom;
    langTo = cat.langTo;
}

bool Category::addVocable(QString lang1, QString lang2)
{
    Vocabel vok;
    vok.language1 = lang1;
    vok.language2 = lang2;
    vok.category = *this;
    return vok.save();
}

bool Category::save(bool forceInsert, bool forceAllField)
{
    bool returnValue = DQModel::save(forceInsert, forceAllField);
    return returnValue;
}

DQList<Vocabel> Category::vocables()
{
    DQList<Vocabel> list = Vocabel::objects().filter(DQWhere("category = ", this->id) ).all();
    return list;
}

