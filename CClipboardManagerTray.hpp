#ifndef CCLIPBOARDMANAGERTRAY_HPP
#define CCLIPBOARDMANAGERTRAY_HPP

#include "CHistoryDialog.hpp"

#include <qxtglobalshortcut.h>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include <QSystemTrayIcon>
#include <QApplication>

#include <QClipboard>
#include <QSettings>
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
   QClipboard* m_clipboard;
   QSqlDatabase m_db;

   QMenu m_menu;
   QSettings m_settings;

   CHistoryDialog* m_historyDialog;

private:
   void initializeSettings();

   void initializeDataBase();
   void initializeMenu();
   void initializeTray();

   void initializeHistoryDialog();
   void initializeSettingsDialog();

   void showHistoryDialog();
   void showSettingsDialog();

   void refreshTooltipText();

   static QString getDataBasePath();

   void insertText(QString text);
   /// @todo Other content types.

public slots:
   void setItemToClipboard(const int& index);
   void removeSavedItem(const int& index);

private slots:
   void onClipboardChanged();

};

#endif // CCLIPBOARDMANAGERTRAY_HPP
