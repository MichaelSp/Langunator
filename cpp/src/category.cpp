
#include "category.h"
#include "db.h"
#include "vokabel.h"

Category Category::_currentCategory;

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

void Category::add(QString lang1, QString lang2, int box)
{
    Vokabel vok;
    vok.language1 = lang1;
    vok.language2 = lang2;
    vok.box = box;
}

bool Category::save(bool forceInsert, bool forceAllField)
{
    bool returnValue = DQModel::save(forceInsert, forceAllField);

    return returnValue;
}

Category &Category::currentCategory()
{
    return _currentCategory;
}

