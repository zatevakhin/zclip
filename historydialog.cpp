#include "historydialog.h"
#include "ui_historydialog.h"

HistoryDialog::HistoryDialog(DataBase* dbman, ClipboardManager *clipboard, QWidget *parent) :
  QDialog(parent), ui(new Ui::HistoryDialog) {

  ui->setupUi(this);
  this->clipboard = clipboard;
  this->dbman = dbman;

  // -----------------------------------------------------------------------------------------------------------------
  QFile fstyle(":/resources/qss/historydialog.qss");
    fstyle.open(QFile::ReadOnly);

  this->setStyleSheet(fstyle.readAll());
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
