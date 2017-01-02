#ifndef CLIPTRAYICON_H
#define CLIPTRAYICON_H

#include <qxt/qxtglobal.h>
#include <qxt/qxtglobalshortcut.h>
#include <qxt/qxtglobalshortcut_p.h>

#include <QSystemTrayIcon>
#include <QApplication>
#include <QMessageBox>
#include <QMenu>

#include "clipboardmanager.h"
#include "historydialog.h"
#include "database.h"

class ClipTrayIcon : public QObject {
    Q_OBJECT
  public:
    explicit ClipTrayIcon(ClipboardManager *clipboard, DataBase* dbman, QObject *parent = 0);
    void setTooltipText(QString s);
    void setHotkey(QString s);

  private:
    QxtGlobalShortcut *shortkey;
    ClipboardManager *clipboard;
    QSystemTrayIcon *trayIcon;
    HistoryDialog *history;
    QMenu *trayMenu;
    DataBase* dbman;

public slots:
    void showHistoryDialog();
    void onInsert();

};

#endif // CLIPTRAYICON_H
