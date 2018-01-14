#include "CHistoryDialog.hpp"
#include "ui_CHistoryDialog.h"


CHistoryDialog::CHistoryDialog()
   : QDialog(Q_NULLPTR)
   , m_ui(new Ui::CHistoryDialog)
   , m_tableModel(new QSqlTableModel)
{
   m_ui->setupUi(this);
   setLayout(m_ui->verticalLayout);

   initializeTableModel();
   initializeSignals();
}

CHistoryDialog::~CHistoryDialog()
{
   delete m_ui;
}

void CHistoryDialog::fillTable()
{
   m_tableModel->setSort(0, Qt::DescendingOrder);
   m_tableModel->select();
}

void CHistoryDialog::keyPressEvent(QKeyEvent *event)
{
   switch (event->key())
   {
      case Qt::Key_Enter:
      case Qt::Key_Return:
      {
         auto index = m_ui->tableView->currentIndex();
         emit getItemById(index.sibling(index.row(), 0).data().toInt());
         hide();
         break;
      }
      case Qt::Key_Home:
         m_ui->tableView->scrollToTop();
         break;
      case Qt::Key_End:
         m_ui->tableView->scrollToBottom();
         break;
      case Qt::Key_Escape:
         hide();
         break;
      default:
         break;
   }
}

bool CHistoryDialog::eventFilter(QObject *obj, QEvent *event)
{
   if (event->type() == QEvent::KeyPress)
   {
      auto keyEvent = static_cast<QKeyEvent*>(event);

      bool isTab = keyEvent->key() == Qt::Key_Tab;
      bool isBackTab = keyEvent->key() == Qt::Key_Backtab;

      if (isBackTab || isTab)
      {
         focusNextChild();
         return true;
      }
   }

   return QObject::eventFilter(obj, event);
}

void CHistoryDialog::initializeTableModel()
{

   m_tableModel->setTable(NConstants::TABLE_NAME);
   m_tableModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
   m_tableModel->setHeaderData(1, Qt::Horizontal, QObject::tr("DATA"));
   m_tableModel->select();

   QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel();
   proxyModel->setSourceModel(m_tableModel);

   m_ui->tableView->setModel(proxyModel);
   m_ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
   m_ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
   m_ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
   m_ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
   m_ui->tableView->horizontalHeader()->setStretchLastSection(true);
   m_ui->tableView->setAlternatingRowColors(true);
   m_ui->tableView->setColumnHidden(0, true);
   m_ui->tableView->setColumnHidden(2, true);
   m_ui->tableView->setSortingEnabled(true);
   m_ui->tableView->sortByColumn(0);

   m_ui->tableView->verticalHeader()->setDefaultSectionSize(20);
   m_ui->tableView->horizontalHeader()->hide();
   m_ui->tableView->verticalHeader()->hide();

   m_ui->tableView->installEventFilter(this);
}

void CHistoryDialog::initializeSignals()
{
   QObject::connect(
            m_ui->lineEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(onSearch(const QString &))
            );

   QObject::connect(
            m_ui->tableView, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(onItemSelected(const QModelIndex &))
            );
}

void CHistoryDialog::onSearch(const QString &query)
{
   m_tableModel->setFilter(QString("data LIKE '%%1%'").arg(query));
}

void CHistoryDialog::onItemSelected(const QModelIndex& index)
{
   emit getItemById(index.sibling(index.row(), 0).data().toInt());
   hide();
}

