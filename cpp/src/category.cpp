
#include "category.h"
#include "db.h"
#include "vokabel.h"

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

bool Category::add(QString lang1, QString lang2)
{
    Vokabel vok;
    vok.language1 = lang1;
    vok.language2 = lang2;
    return vok.save();
}

bool Category::save(bool forceInsert, bool forceAllField)
{
    bool returnValue = DQModel::save(forceInsert, forceAllField);

    return returnValue;
}


