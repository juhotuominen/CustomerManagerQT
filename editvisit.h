#ifndef EDITVISIT_H
#define EDITVISIT_H

#include "addvisit.h"
#include "customerinfo.h"
#include "qtablewidget.h"
#include <QDialog>

namespace Ui {
class EditVisit;
}

class EditVisit : public QDialog
{
    Q_OBJECT

signals:
    void visitEdited();

public:
    explicit EditVisit(QWidget *parent = nullptr);
    ~EditVisit();
    void setCustomerVisitInfo(QStringList customerVisitInfo);
    void getCustomerVisitInfo(int customerId);

private slots:
    void on_btnRemove_clicked();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_btnCancel_clicked();

    void onVisitAdded();

private:
    Ui::EditVisit *ui;
    AddVisit *ptrAddVisit;
    int privateCustomerId;
    void setupTableWidget();

};

#endif // EDITVISIT_H
