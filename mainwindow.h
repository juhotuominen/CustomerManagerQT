#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardPaths>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>

#include "addvisit.h"
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
    Ui::MainWindow *ui;

private slots:

    void setupTableWidget();

    void on_btnRemove_clicked();

    void on_btnGet_clicked();

    void on_btnSearch_clicked();

    void on_btnAddCustomer_clicked();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_lineEditSearch_editingFinished();

    void onCustomerAdded();


    void onLineEditTextChanged(const QString &arg1);

    void on_cancelButton_clicked();

    void on_saveButton_clicked();

    void on_addVisitButton_clicked();

    void on_refreshButton_clicked();

    void onVisitAdded();

    void on_btnSave_clicked();

    void on_btnCancel_clicked();

private:

    AddCustomer *ptrAddCustomer;
    CustomerInfo *ptrCustomerInfo;
    AddVisit *ptrAddVisit;
    QSqlDatabase DB;

    //QString path_to_db = "./database/CustomersDeploy.db"; //RELEASE
    QString path_to_db = "D:/SQLite/Customers.db"; //DEV
};
#endif // MAINWINDOW_H
