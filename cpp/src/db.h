#ifndef DB_H
#define DB_H

#include "factory.h"


class DB: public QObject
{
    friend class Factory;
    Q_OBJECT
public:

    bool isLoaded(){return _isLoaded;}
signals:
    void dbLoaded();

private:
    DB();

    void init();

    bool _isLoaded;
    DQConnection *connection;
};

#endif // DB_H
