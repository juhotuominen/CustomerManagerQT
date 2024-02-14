#ifndef ADDVISIT_H
#define ADDVISIT_H


#include <QDialog>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class AddVisit;
}

class AddVisit : public QDialog
{
    Q_OBJECT

public:
    explicit AddVisit(QWidget *parent = nullptr);
    ~AddVisit();
    void setCustomerId(int id);

private slots:
    void on_saveBtn_clicked();


    void on_cancelBtn_clicked();

private:
    Ui::AddVisit *ui;
    QStringList getData();
    int customerId;
};

#endif // ADDVISIT_H
