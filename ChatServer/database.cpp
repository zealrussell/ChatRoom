#include "database.h"

Database *Database::pDatabase = nullptr;
QSqlDatabase *Database::pSqlite = nullptr;

Database *Database::getDatabase(QString databaseName)
{
    if (Database::pDatabase == nullptr) {
        Database::pDatabase = new Database(databaseName);
    }
    return Database::pDatabase;
}

bool Database::closeDatabase()
{
    if(Database::pSqlite->isValid() && !Database::pSqlite->isOpen()) {
        Database::pSqlite->close();
        Database::pSqlite = nullptr;
        return true;
    } else if (Database::pSqlite->isValid() && Database::pSqlite->isOpen()) {
        Database::pSqlite = nullptr;
        return true;
    }
    return false;
}

void Database::createTable(QString tableName)
{
    QString sql = QString("CREATE TABLE if not exists %1(id integer primary key autoincrement, username varchar(20), chatmessage varchar(1000));").arg(tableName);
    QSqlQuery query(*Database::pSqlite);
    bool res = query.exec(sql);
    if (res)
        qDebug() << "创建表成功" << tableName;
    else
        qDebug() << "创建表失败";

}

void Database::insertData(QString tableName, QString userName, QString chatMessage)
{
    QString sql = QString("INSERT INTO %1(username, chatmessage) VALUES(%2, %3)").arg(tableName).arg(userName).arg(chatMessage);
    QSqlQuery query(*Database::pSqlite);
    bool res = query.exec(sql);
    if (res)
        qDebug() << "插入成功";
    else
        qDebug() << "插入失败";

}

Database::Database()
{

}

Database::Database(QString databaseName)
{

    *Database::pSqlite = QSqlDatabase::addDatabase("QSQLITE");
    // 未打开
    if( !Database::pSqlite->isOpen()) {
        Database::pSqlite->setDatabaseName(databaseName);
        Database::pSqlite->open();
        qDebug() << "打开数据库";

    }
}

