#ifndef DATABASE_H
#define DATABASE_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSql>
#include <QSqlError>

class Database
{
public:

    static Database *getDatabase(QString databaseName);
    static bool closeDatabase();
    static void createTable(QString tableName);
    static void insertData(QString tableName, QString userName, QString chatMessage);

private:
     Database();
     Database(QString databaseName);
     ~Database();
     // 类对象指针
     static Database *pDatabase;
     // 数据库指针
     static QSqlDatabase *pSqlite;

};

#endif // DATABASE_H
