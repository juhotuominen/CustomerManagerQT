#ifndef CUSTOMERINFO_H
#define CUSTOMERINFO_H

#include "addvisit.h"
#include <QDialog>

class MainWindow;

namespace Ui {
class CustomerInfo;
}

class CustomerInfo : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerInfo(MainWindow *mainWindow, QWidget *parent = nullptr);
    ~CustomerInfo();
    void setCustomerInfo(QStringList customerInfo);
    void setCustomerVisitInfo(QStringList customerVisitInfo);
    QStringList getCustomerInfo(int customerId);
    void getCustomerVisitInfo(int customerId);
    int getCustomerId();
    int publicId;

private slots:

private:
    Ui::CustomerInfo *ui;
    MainWindow *mainWindow;
    AddVisit *ptrAddVisit;
    void setCustomerId(int id);

    void handleAddVisitClosed(int result);

};

#endif // CUSTOMERINFO_H
