#ifndef MYSQLITE_H
#define MYSQLITE_H
#include<QDebug>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>

class mysqlite
{
public:
    mysqlite();
    QSqlDatabase db;
    void initDb();
    bool openDb();
    void closeDb();
    void createTable();

    void insertData();
    void deleteData();
    void updateData();
    void queryData();

};

#endif // MYSQLITE_H
