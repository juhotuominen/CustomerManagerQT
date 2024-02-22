#ifndef ADDCUSTOMER_H
#define ADDCUSTOMER_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>

class MainWindow;

namespace Ui {
class AddCustomer;
}

class AddCustomer : public QDialog
{
    Q_OBJECT

signals:
    void customerAdded();

public:
    explicit AddCustomer(MainWindow *mainWindow, QWidget *parent = nullptr);
    ~AddCustomer();
    QStringList getData();

private slots:

private:
    Ui::AddCustomer *ui; 
    MainWindow *mainWindow;
};

#endif // ADDCUSTOMER_H
