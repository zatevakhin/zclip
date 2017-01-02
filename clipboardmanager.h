#ifndef CLIPBOARDMANAGER_H
#define CLIPBOARDMANAGER_H

#include <QApplication>
#include <QClipboard>
#include <QDebug>

#include "database.h"

class ClipboardManager : public QObject {
    Q_OBJECT
  public:
    explicit ClipboardManager(DataBase* dbman, QObject *parent = 0);

    void insertClip(QString clip);
    void setToClipboard(int index);
    int getElementsCount();

    int lastcount = 0;

  private:
    QClipboard* clipboard;
    DataBase *dbman;

  public slots:
    void onClipboardDataChanged();

  signals:
    void inserted();
};

#endif // CLIPBOARDMANAGER_H
