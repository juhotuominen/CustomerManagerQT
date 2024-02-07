#ifndef ADDCUSTOMER_H
#define ADDCUSTOMER_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class AddCustomer;
}

class AddCustomer : public QDialog
{
    Q_OBJECT

public:
    explicit AddCustomer(QWidget *parent = nullptr);
    ~AddCustomer();

private slots:
    void on_btnSave_clicked();

private:
    Ui::AddCustomer *ui;
    QStringList getData();
};

#endif // ADDCUSTOMER_H
