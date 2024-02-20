#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardPaths>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>

#include "addcustomer.h"
#include "customerinfo.h"
#include "qtablewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void setupTableWidget();

    void on_btnRemove_clicked();

    void on_btnGet_clicked();

    void on_btnSearch_clicked();

    void on_btnAddCustomer_clicked();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_lineEditSearch_editingFinished();

    void onCustomerAdded();

private:
    Ui::MainWindow *ui;

    AddCustomer *ptrAddCustomer;
    CustomerInfo *ptrCustomerInfo;
    QSqlDatabase DB;
    //RELEASE QString path_to_db = "./database/CustomersDeploy.db";
    QString path_to_db = "D:/SQLite/Customers.db";
};
#endif // MAINWINDOW_H
