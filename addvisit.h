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
    void setEditableData(int visitId);
    QStringList getDatabaseData(int id);
    bool editMode = false;
    int publicVisitId;

private slots:
    void on_saveBtn_clicked();

    void on_cancelBtn_clicked();

    void on_calendarWidget_clicked(const QDate &date);

private:
    Ui::AddVisit *ui;
    QStringList getData();
    QString selectedDate;
    int customerId;
};

#endif // ADDVISIT_H
