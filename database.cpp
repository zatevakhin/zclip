#include "database.h"

DataBase::DataBase(QString path, QObject *parent) : QObject(parent) {
  db = QSqlDatabase::addDatabase("QSQLITE");//, "mainconnection");
  db.setDatabaseName(path);
}

QSqlQuery* DataBase::dbquery() {
  return new QSqlQuery(db);
}

bool DataBase::isOpen() {
  return db.isOpen();
}

bool DataBase::open() {
  return db.open();
}


