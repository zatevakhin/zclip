#ifndef DATABASE_H
#define DATABASE_H

#include <QStringListModel>
#include <QStandardItemModel>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QObject>
#include <QDebug>

class DataBase : public QObject {
    Q_OBJECT

  public:
    explicit DataBase(QString path, QObject *parent = 0);
    bool open();
    bool isOpen();
    QSqlQuery* dbquery();

  private:
    QSqlDatabase db;
};

#endif // DATABASE_H
