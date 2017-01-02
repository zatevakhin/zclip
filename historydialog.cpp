#include "historydialog.h"
#include "ui_historydialog.h"
#include <QAbstractTableModel>

HistoryDialog::HistoryDialog(DataBase* dbman, ClipboardManager *clipboard, QWidget *parent) :
  QDialog(parent), ui(new Ui::HistoryDialog) {

  ui->setupUi(this);
  this->clipboard = clipboard;
  this->dbman = dbman;

  // -----------------------------------------------------------------------------------------------------------------
  QString style = ""
      "QLineEdit {"
        "border-style: none;"
        "color: #8F8F8F;"
        "selection-color: gray;"
        "background: #1F1F1F;"
        "selection-background-color: #1E1542;"
        "border-top: 1px solid black;"
        "border-bottom: 1px solid black;"
      "}"
      ""
      "QTableView {"
        "background: #1F1F1F;"
        "alternate-background-color: #2F2F2F;"
        "color: #8F8F8F;"
        "border-style: none;"
      "}"
      ""
      "QHeaderView {"
        "background: #1F1F1F;"
        "color: #8F8F8F;"
        "border-style: none;"
      "}"
      ""
      "QTableView::item:selected {"
        "background: #1E1542;"
        "color: gray;"
      "}"
      ""
      "QScrollBar:vertical {"
        "background: #1F1F1F;"
        "border-style: none;"
        "border-left: 1px solid #0F0F0F;"
        "width: 6px;"
      "}"
      ""
      "QScrollBar::handle:vertical {"
        "background: #2F2F2F;"
        "min-height: 20px;"
      "}"
      "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
         "background: #1F1F1F;"
        "border-left: 1px solid #0F0F0F;"
      "}"
      "";

  this->setStyleSheet(style);
  // -----------------------------------------------------------------------------------------------------------------
  this->setWindowTitle("zClip");
  this->setMinimumSize(400, 500);
  this->setLayout(ui->verticalLayout);

  // -----------------------------------------------------------------------------------------------------------------
  this->ui->lineEdit->setPlaceholderText("Search...");

  // -----------------------------------------------------------------------------------------------------------------
  histmodel = new QSqlTableModel();

  histmodel->setTable("clipboard");
  histmodel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
  histmodel->setHeaderData(1, Qt::Horizontal, QObject::tr("DATA"));

  QSortFilterProxyModel *proxymodel = new QSortFilterProxyModel();
  proxymodel->setSourceModel(histmodel);
  ui->tableView->setModel(proxymodel);

  ui->tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
  ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  ui->tableView->horizontalHeader()->setStretchLastSection(true);
  ui->tableView->setAlternatingRowColors(true);
  ui->tableView->setColumnHidden(0, true);
  ui->tableView->setColumnHidden(2, true);
  ui->tableView->setSortingEnabled(true);
  ui->tableView->sortByColumn(0);

  ui->tableView->verticalHeader()->setDefaultSectionSize(20);
  ui->tableView->horizontalHeader()->hide();
  ui->tableView->verticalHeader()->hide();

  ui->tableView->installEventFilter(this);

  // -----------------------------------------------------------------------------------------------------------------
  QObject::connect(ui->tableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onTableDoubleClicked(const QModelIndex &)));
  // -----------------------------------------------------------------------------------------------------------------
  QObject::connect(ui->lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onTextChanged(const QString &)));

}

bool HistoryDialog::eventFilter(QObject* obj, QEvent* event){
    if(event->type() == QEvent::KeyPress) {

        QKeyEvent* key = static_cast<QKeyEvent*>(event);

        if ((key->key() == Qt::Key_Enter) || (key->key() == Qt::Key_Return) ) {

            QModelIndexList selection = ui->tableView->selectionModel()->selectedIndexes();
            clipboard->setToClipboard(selection.at(0).data().toInt());
            this->hide();
        }
    }
    return QObject::eventFilter(obj, event);
}


void HistoryDialog::onTableDoubleClicked(const QModelIndex &index) {
  QModelIndexList selection = ui->tableView->selectionModel()->selectedIndexes();
  clipboard->setToClipboard(selection.at(0).data().toInt());
  this->hide();
}

void HistoryDialog::onTextChanged(const QString &text) {
  histmodel->setFilter(QString("data LIKE '%%1%'").arg(text));
}

void HistoryDialog::fillTable() {
  histmodel->setSort(0, Qt::DescendingOrder);
  histmodel->select();
}

void HistoryDialog::closeEvent(QCloseEvent *event) {
  this->hide();
  event->ignore();
}

void HistoryDialog::initialFocus() {
  ui->lineEdit->setFocus();
}

HistoryDialog::~HistoryDialog() {
  delete ui;
}
