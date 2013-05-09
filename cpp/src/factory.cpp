#include "factory.h"
#include "db.h"

Factory* Factory::_instance=nullptr;

Factory::Factory(QObject *p):QObject(p)
{
    dbInstance = new DB();
    connect(dbInstance, SIGNAL(dbLoaded()), SLOT(dbLoaded()));
}


Categories Factory::categories()
{
    Categories lst;
    DQQuery<Category> query;
    auto allCategories = query.all();
    for(int i=0;i<allCategories.size();i++) {
        auto item = allCategories.at(i);
        qDebug()<<item;
        //lst.append(item);
    }
    return lst;
}

void Factory::dbLoaded()
{
    emit categoriesUpdated(categories());
}


Factory *Factory::instance(QObject *p)
{
    if (_instance==nullptr){
        _instance = new Factory(p);
        if (_instance->dbInstance->isLoaded())
            _instance->dbLoaded();
    }

    return _instance;
}
