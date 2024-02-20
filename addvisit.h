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

signals:
    void visitAdded();

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
    QString sortDate(QString date);
    int customerId;
};

#endif // ADDVISIT_H
