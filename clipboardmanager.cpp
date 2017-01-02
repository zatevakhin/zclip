#include "clipboardmanager.h"

ClipboardManager::ClipboardManager(DataBase* dbman, QObject *parent) : QObject(parent) {
  this->dbman = dbman;

  clipboard = QApplication::clipboard();

  QObject::connect(clipboard, SIGNAL(dataChanged()), this, SLOT(onClipboardDataChanged()));

  this->lastcount = this->getElementsCount();
}

void ClipboardManager::insertClip(QString clip) {
  if (!clip.length()) { return; }

  QSqlQuery* q = dbman->dbquery();
  q->prepare("INSERT OR REPLACE INTO `clipboard` (`data`) VALUES (?);");
      q->bindValue(0, clip);
      q->exec();

  this->lastcount = this->getElementsCount();
};

void ClipboardManager::setToClipboard(int index) {
  clipboard->clear();

  QSqlQuery * query = dbman->dbquery();
    query->prepare("SELECT data FROM clipboard WHERE id = ?");
    query->bindValue(0, index);
    query->exec();

  if(query->next()) {
      // bool bs = this->clipboard->blockSignals(true);
      clipboard->setText(query->value(0).toString());
      // this->clipboard->blockSignals(bs);
  }
}

void ClipboardManager::onClipboardDataChanged() {
  insertClip(clipboard->text());
  emit inserted();
}

int ClipboardManager::getElementsCount() {
  QSqlQuery * query = dbman->dbquery();
    query->exec("SELECT COUNT(*) as count FROM clipboard");
    query->next();

    return query->value(0).toInt();
}
