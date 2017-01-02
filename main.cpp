#include "clipboardmanager.h"
#include "cliptrayicon.h"

#include <QJsonDocument>
#include <QJsonObject>

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

  QString dbpath = QDir::homePath(); dbpath.append(QDir::separator());
  QString cfpath = QString(dbpath);

    dbpath.append(".zclip.db");
    cfpath.append(".zclip");

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

  ClipTrayIcon *tray = new ClipTrayIcon(new ClipboardManager(dbman), dbman);

  QByteArray jsonconf = "{\"hotkey\":\"Alt+X\"}";

  QFile jsonf(cfpath);
  if (QFile::exists(cfpath)) {
      if (jsonf.open(QIODevice::ReadOnly)) {
          jsonconf = jsonf.readAll();
      }
  } else {
      if (jsonf.open(QIODevice::WriteOnly)) {
          jsonf.write(jsonconf);
      }
  }

  QJsonObject config = (new QJsonDocument(QJsonDocument::fromJson(jsonconf)))->object();

  tray->setHotkey(config["hotkey"].toString());

  return a.exec();
}
