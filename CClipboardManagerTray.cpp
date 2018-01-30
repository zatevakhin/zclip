#include "CClipboardManagerTray.hpp"


CClipboardManagerTray::CClipboardManagerTray()
   : QSystemTrayIcon(Q_NULLPTR)
   , m_globalShortCut(new QxtGlobalShortcut(this))
   , m_clipboard(QApplication::clipboard())
   , m_settings(
        QSettings::IniFormat,
        QSettings::UserScope,
        QCoreApplication::organizationName(),
        QCoreApplication::applicationName()
        )
{
   initializeSettings();
   initializeDataBase();

   initializeHistoryDialog();

   initializeMenu();
   initializeTray();
}

void CClipboardManagerTray::initializeSettings()
{
   if (!m_settings.contains(NConstants::SHOW_HISTORY))
   {
      m_settings.setValue(NConstants::SHOW_HISTORY, "Alt+X");
   }
}

void CClipboardManagerTray::initializeDataBase()
{
   m_db = QSqlDatabase::addDatabase("QSQLITE");
   m_db.setDatabaseName(CClipboardManagerTray::getDataBasePath());

   if (!m_db.open())
   {
      throw std::runtime_error("Can`t open database!");
   }

   m_db.exec(QString(NConstants::DATABASE_CREATE_QUERY)
             .arg(NConstants::TABLE_NAME));
}

void CClipboardManagerTray::initializeMenu()
{
   auto quitAction = new QAction(tr("&Quit"), this);
   QObject::connect(
            quitAction, &QAction::triggered,
            this, &QCoreApplication::quit
            );

   auto historyAction = new QAction(tr("&History"), this);
   QObject::connect(
            historyAction, &QAction::triggered,
            this, &CClipboardManagerTray::showHistoryDialog
            );

   m_menu.addAction(historyAction);
   m_menu.addAction(quitAction);

   setContextMenu(&m_menu);
}

void CClipboardManagerTray::initializeTray()
{
   auto trayIcon = QIcon(NConstants::ICON_MAIN);
   setIcon(trayIcon);

   refreshTooltipText();
}

void CClipboardManagerTray::initializeHistoryDialog()
{
   m_historyDialog = new CHistoryDialog();

   m_globalShortCut->setShortcut(
            QKeySequence(m_settings.value(NConstants::SHOW_HISTORY).toString())
            );

   QObject::connect(
            m_globalShortCut, &QxtGlobalShortcut::activated,
            this, &CClipboardManagerTray::showHistoryDialog
            );

   QObject::connect(
            m_clipboard, &QClipboard::dataChanged,
            this, &CClipboardManagerTray::onClipboardChanged
            );

   QObject::connect(
            m_historyDialog, &CHistoryDialog::getItemById,
            this, &CClipboardManagerTray::setItemToClipboard
            );

   QObject::connect(
            m_historyDialog, &CHistoryDialog::removeItemById,
            this, &CClipboardManagerTray::removeSavedItem
            );
}

void CClipboardManagerTray::showHistoryDialog()
{
   m_historyDialog->fillTable();
   m_historyDialog->show();
}

void CClipboardManagerTray::refreshTooltipText()
{
   QSqlQuery query;
   query.prepare(QString(NConstants::DATABASE_COUNT_QUERY)
                 .arg(NConstants::TABLE_NAME));
   query.exec();

   if (query.next()) {
      QString tooltip = QString(NConstants::TOOLTIP_TEXT_TEMPLATE)
            .arg(m_settings.value(NConstants::SHOW_HISTORY).toString())
            .arg(query.value(0).toInt());

      setToolTip(tooltip);
   }
}

QString CClipboardManagerTray::getDataBasePath()
{
   auto dbPath = QString("%1%2%3")
         .arg(QDir::homePath())
         .arg(QDir::separator())
         .arg(NConstants::DATABASE_FILE);

   return std::move(dbPath);
}

void CClipboardManagerTray::insertText(QString text)
{
   if (text.length()) {

      QSqlQuery query;
      query.prepare(QString(NConstants::DATABASE_INSERT_TEXT_QUERY)
                     .arg(NConstants::TABLE_NAME));

      query.bindValue(0, text);
      query.exec();
   }
}

void CClipboardManagerTray::onClipboardChanged()
{
   auto mime = m_clipboard->mimeData();

   if (mime->hasText()) {
      insertText(std::move(m_clipboard->text()));
   }
   /// @todo Other content types.
}

void CClipboardManagerTray::setItemToClipboard(const int &index)
{
   m_clipboard->clear();

   QSqlQuery query;
   query.prepare(QString(NConstants::DATABASE_SELECT_TEXT_QUERY)
                 .arg(NConstants::TABLE_NAME));

   query.bindValue(0, index);
   query.exec();

   if (query.next())
   {
      m_clipboard->setText(query.value(0).toString());
   }
}

void CClipboardManagerTray::removeSavedItem(const int& index)
{
   QSqlQuery query;
   query.prepare(QString(NConstants::DATABASE_REMOVE_ITEM)
                 .arg(NConstants::TABLE_NAME));

   query.bindValue(0, index);
   query.exec();
}
