#pragma once

#include "pch.h"

class Backend;

class DB: public QObject
{
    friend class Backend;
    Q_OBJECT
public:

    bool isLoaded(){return _isLoaded;}
    void populateLookupTable();
signals:
    void dbLoaded();

private:
    DB();

    void init();

    bool _isLoaded;
    DQConnection *connection;
};

class LookupTable: public DQModel {
    DQ_MODEL
public:
    DQField<double> exponent;
    DQField<double> power;
};

DQ_DECLARE_MODEL(LookupTable,
                 "LookupTable", // the table name.
                 DQ_FIELD(exponent, DQNotNull | DQUnique),
                 DQ_FIELD(power, DQNotNull)
                 )
