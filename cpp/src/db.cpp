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
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QDir appDataDir( QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) );
#else
    QDir appDataDir = QDir::home() + QDir::separator() + "." + QApplication::applicationName();
#endif
    if (!appDataDir.exists())
        appDataDir.mkpath(".");
    QString sqlDBFile = appDataDir.absoluteFilePath("DB.sqlite");
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

    connection->createTables();

    emit dbLoaded();
    _isLoaded=true;
}
