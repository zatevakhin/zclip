#ifndef CHISTORYDIALOG_H
#define CHISTORYDIALOG_H

#include "Constants.hpp"

#include <QSortFilterProxyModel>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlQuery>
#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QKeyEvent>
#include <QDialog>

namespace Ui
{

class CHistoryDialog;

}

class CHistoryDialog : public QDialog
{
   Q_OBJECT

public:
   CHistoryDialog();
   ~CHistoryDialog();

   void fillTable();

private:
   Ui::CHistoryDialog *m_ui;
   QSqlTableModel* m_tableModel;

private:
   void keyPressEvent(QKeyEvent *event) override;
   bool eventFilter(QObject* obj, QEvent* event) override;

   void initializeTableModel();
   void initializeSignals();


private slots:
   void onSearch(const QString& query);
   void onItemSelected(const QModelIndex &index);
   void onItemRemove(const QModelIndex &index);

signals:
   void getItemById(const int& index);
   void removeItemById(const int& index);

};

#endif // CHISTORYDIALOG_H
