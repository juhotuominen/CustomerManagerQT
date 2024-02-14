#ifndef CUSTOMERINFO_H
#define CUSTOMERINFO_H

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
    int publicId = 0;

private slots:
    void onLineEditTextChanged(const QString &text);


    void on_cancelButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::CustomerInfo *ui;

};

#endif // CUSTOMERINFO_H
