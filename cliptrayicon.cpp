#include "cliptrayicon.h"

ClipTrayIcon::ClipTrayIcon(ClipboardManager *clipboard, DataBase* dbman, QObject *parent) : QObject(parent) {
  trayMenu = new QMenu();
  this->dbman = dbman;
  this->clipboard = clipboard;

  QAction *quitAction = new QAction(tr("&Quit"), this);
  QObject::connect(quitAction, &QAction::triggered, this, &QCoreApplication::quit);

  trayMenu->addAction(quitAction);

  trayIcon = new QSystemTrayIcon();
  trayIcon->setContextMenu(trayMenu);

  // -----------------------------------------------------------------------------------------------------------------
  shortkey = new QxtGlobalShortcut();
  shortkey->setShortcut(QKeySequence("Alt+X"));

  // -----------------------------------------------------------------------------------------------------------------
  QIcon icon = QIcon(":/icon/clipboard.svg");
  trayIcon->setIcon(icon);
  this->onInsert();
  trayIcon->show();
  // -----------------------------------------------------------------------------------------------------------------
  QObject::connect(shortkey, SIGNAL(activated()), this, SLOT(showHistoryDialog()));
  QObject::connect(clipboard, SIGNAL(inserted()), this, SLOT(onInsert()));
  // -----------------------------------------------------------------------------------------------------------------
  history = new HistoryDialog(dbman, clipboard);
  history->setWindowIcon(icon);
}

void ClipTrayIcon::onInsert() {
  QString formatted = "Менеджер буфера обмена: \n - записей в базе '%1';\n - Хоткей: '%2'.";
    formatted = formatted.arg(clipboard->lastcount);
    formatted = formatted.arg(shortkey->shortcut().toString());
  this->setTooltipText(formatted);
}

void ClipTrayIcon::setTooltipText(QString s) {
  trayIcon->setToolTip(s);
}

void ClipTrayIcon::showHistoryDialog() {
  history->show();
  history->fillTable();
}
