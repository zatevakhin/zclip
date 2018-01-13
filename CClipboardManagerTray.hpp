#ifndef CCLIPBOARDMANAGERTRAY_HPP
#define CCLIPBOARDMANAGERTRAY_HPP

#include "CHistoryDialog.hpp"

#include <qxtglobalshortcut.h>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include <QJsonDocument>
#include <QJsonObject>

#include <QSystemTrayIcon>
#include <QApplication>

#include <QClipboard>
#include <QMimeData>
#include <QMenu>

#include <QFile>
#include <QDir>


class CClipboardManagerTray
      : public QSystemTrayIcon
{
public:
   CClipboardManagerTray();

private:
   QxtGlobalShortcut* m_globalShortCut;
   CHistoryDialog* m_historyDialog;
   QClipboard* m_clipboard;
   QJsonObject m_settings;
   QSqlDatabase m_db;
   QMenu m_menu;

private:
   void loadSettings();

   void initializeDataBase();
   void initializeMenu();
   void initializeTray();
   void initializeDialog();

   void showHistoryDialog();
   void refreshTooltipText();

   static QString getSettingsdPath();
   static QString getDataBasePath();

   void insertText(QString text);
   /// @todo Other content types.

public slots:
   void setItemToClipboard(const int& index);

private slots:
   void onClipboardChanged();

};

#endif // CCLIPBOARDMANAGERTRAY_HPP
