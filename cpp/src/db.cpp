#include "db.h"
#include "pch.h"

#include "vocabel.h"
#include "category.h"


DB::DB():
    _isLoaded(false)
{
    init();
}

void DB::init()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("test.sqlite");
    if (!db.open())
        QMessageBox::warning(NULL,"DB Error", db.lastError().text() );
    else
        qDebug() << "Database opened";

    connection = new DQConnection();
    connection->open(db);

    connection->addModel<Category>();
    connection->addModel<Vocable>();

    connection->createTables();

    emit dbLoaded();
    _isLoaded=true;
}
