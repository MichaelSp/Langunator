#include "db.h"
#include "pch.h"

#include "Vocable.h"
#include "category.h"


DB::DB():
    _isLoaded(false)
{
    init();
}

void DB::init()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QDir appDataDir( QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) );
#else
    QDir appDataDir = QDir( QDir::home().absoluteFilePath( "." + QApplication::applicationName() ));
#endif
    if (!appDataDir.exists())
        appDataDir.mkpath(".");
    QString sqlDBFile = appDataDir.absoluteFilePath(QApplication::applicationName() + ".sqlite");
    qDebug() << "SQLite DB: " <<sqlDBFile;
    db.setDatabaseName(sqlDBFile);
    if (!db.open()){
        QMessageBox::warning(NULL,"DB Error", db.lastError().text() + "\nSQLite Driver is required!" );
        qApp->quit();
    }
    else
        qDebug() << "Database opened";

    connection = new DQConnection();
    connection->open(db);

    connection->addModel<Category>();
    connection->addModel<Vocable>();
    connection->addModel<LookupTable>();

    connection->createTables();

    populateLookupTable();

    emit dbLoaded();
    _isLoaded=true;
}

void DB::populateLookupTable()
{
    DQQuery<LookupTable> lookupTable;
    if (lookupTable.count() >=2 )
        return;

    DQList<LookupTable> initialData;
    DQListWriter writer(&initialData);

    for(double i=0;i<20;i++)
        writer << i << pow(2.0,i);

    initialData.save();
}
