#include "clipboardmanager.h"
#include "cliptrayicon.h"
#include <QApplication>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QApplication::setQuitOnLastWindowClosed(false);
  if (!QSystemTrayIcon::isSystemTrayAvailable()) {
    QMessageBox::critical(0, QObject::tr("Systray"), QObject::tr("I couldn't detect any system tray on this system."));
    return 1;
  }

  QString dbpath = QDir::homePath();
    dbpath.append(QDir::separator());
    dbpath.append(".zclip.db");

  DataBase* dbman = new DataBase(dbpath);

  if(!dbman->open()) {
     qDebug() << "Ошибка при открытии базы данных!";
     return 1;
  }

  QSqlQuery *q = dbman->dbquery();
    q->exec("CREATE TABLE IF NOT EXISTS `clipboard` ("
              "`id`    INTEGER   PRIMARY KEY NOT NULL,"
              "`data`  TEXT      UNIQUE      NOT NULL,"
              "`stamp` TIMESTAMP DEFAULT     CURRENT_TIMESTAMP"
            ");");

  ClipboardManager *cbm = new ClipboardManager(dbman);

  ClipTrayIcon *tray = new ClipTrayIcon(cbm, dbman);

  return a.exec();
}
