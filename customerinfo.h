#ifndef CUSTOMERINFO_H
#define CUSTOMERINFO_H

#include "addvisit.h"
#include <QDialog>

namespace Ui {
class CustomerInfo;
}

class CustomerInfo : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerInfo(QWidget *parent = nullptr);
    ~CustomerInfo();
    void setCustomerInfo(QStringList customerInfo);
    void setCustomerVisitInfo(QStringList customerVisitInfo);
    QStringList getCustomerInfo(int customerId);
    void getCustomerVisitInfo(int customerId);
    int getCustomerId();

private slots:
    void onLineEditTextChanged(const QString &text);

    void setCustomerId(int id);


    void on_cancelButton_clicked();

    void on_saveButton_clicked();

    void on_addVisitButton_clicked();

private:
    Ui::CustomerInfo *ui;
    AddVisit *ptrAddVisit;
    int publicId;

};

#endif // CUSTOMERINFO_H
