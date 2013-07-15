#pragma once

#include "pch.h"

class Backend;

class DB: public QObject
{
    friend class Backend;
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
