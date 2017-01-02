#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QSortFilterProxyModel>
#include <QtSql/QSqlTableModel>
#include <QStandardItemModel>
#include <QCloseEvent>
#include <QDialog>
#include <QDebug>
#include <QFile>

#include "clipboardmanager.h"
#include "database.h"

namespace Ui {
  class HistoryDialog;
}

class HistoryDialog : public QDialog {
    Q_OBJECT

  public:
    explicit HistoryDialog(DataBase* dbman, ClipboardManager *clipboard, QWidget *parent = 0);
    ~HistoryDialog();

    void fillTable();
    void initialFocus();

  private:
    Ui::HistoryDialog *ui;
    QSqlTableModel *histmodel;
    ClipboardManager *clipboard;
    DataBase* dbman;

    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *, QEvent *event);

  private slots:
    void onTableDoubleClicked(const QModelIndex &index);
    void onTextChanged(const QString &text);
};

#endif // HISTORYDIALOG_H
