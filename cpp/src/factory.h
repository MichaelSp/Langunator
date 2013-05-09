#ifndef CATEGORIES_H
#define CATEGORIES_H

#include "pch.h"
#include "category.h"


typedef QList<Category> Categories;
Q_DECLARE_METATYPE(Categories)

class Factory: public QObject {
    Q_OBJECT
public:
    static Factory *instance(QObject*p=nullptr);
    static DB *db(){return Factory::instance()->dbInstance;}

    Categories categories();
signals:
    void categoriesUpdated(Categories&);

private slots:
    void dbLoaded();

private:
    explicit Factory(QObject*p=nullptr);
    DB *dbInstance;
    static Factory *_instance;
};


#endif // CATEGORIES_H
