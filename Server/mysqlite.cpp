#include "mysqlite.h"

mysqlite::mysqlite()
{

}

void mysqlite::initDb(){
    if(QSqlDatabase::contains("qt_sql_default_connection")) {
            db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("chatroom.db");
        db.open();
    }
}

bool mysqlite::openDb()
{
    if(!db.open())
       {
           qDebug() << "Error: Failed to connect database." << db.lastError();
           return false;
       }

    return true;
}

void mysqlite::closeDb()
{
   db.close();
}

void mysqlite::createTable()
{
    QSqlQuery query;
    QString sql = "create table message(id int primary key, name varchar(500))";
    query.prepare(sql);
    if (!query.exec()) {
        qDebug() << "Error: Fail to create table." << query.lastError();
    } else {
        qDebug() << "Table created!";
    }
}

void mysqlite::insertData()
{
    QString insert_sql = "insert into student values (?, ?)";
    QSqlQuery sql_query;
    sql_query.prepare(insert_sql);

    sql_query.addBindValue("Wang");
    sql_query.addBindValue(25);
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        qDebug() << "inserted Wang!";
    }
}

void mysqlite::deleteData()
{
   QSqlQuery query;
   QString sql = "delete" ;

}

void mysqlite::updateData()
{
    QString update_sql = "update student set name = :name where id = :id";
    QSqlQuery sql_query;
    sql_query.prepare(update_sql);
    sql_query.bindValue(":name", "Qt");
    sql_query.bindValue(":id", 1);
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        qDebug() << "updated!";
    }
}



