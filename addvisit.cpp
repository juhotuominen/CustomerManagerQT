#include "addvisit.h"
#include "ui_addvisit.h"


AddVisit::AddVisit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddVisit)
{
    ui->setupUi(this);
}

AddVisit::~AddVisit()
{
    delete ui;
}


/**********
 * FUNCTION
 * Set selected customers id to customerId
***********/

void AddVisit::setCustomerId(int id)
{
    customerId = id;
}


/**********
 * FUNCTION
 * Add visit to Information table on database
***********/

void AddVisit::on_saveBtn_clicked()
{
    QStringList data = getData();

    QSqlQuery query;
    query.prepare("INSERT INTO Information (customer_id, date, reason, operation, plan) "
                  "VALUES (:customer_id, :date, :reason, :operation, :plan)");
    query.bindValue(":customer_id", customerId);
    query.bindValue(":date", data[0]);
    query.bindValue(":reason", data[1]);
    query.bindValue(":operation", data[2]);
    query.bindValue(":plan", data[3]);


    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    emit visitAdded();

    // Clear inputs and close
    ui->lineEditDate->clear();
    ui->plainTextEditReason->clear();
    ui->plainTextEditOperation->clear();
    ui->plainTextEditPlan->clear();
    this->close();
}


/**********
 * FUNCTION
 * Get data from lineEdits and textEdits to be added for new customer
***********/

QStringList AddVisit::getData()
{
    QStringList data;
    data << selectedDate
         << ui->plainTextEditReason->toPlainText()
         << ui->plainTextEditOperation->toPlainText()
         << ui->plainTextEditPlan->toPlainText();

    return data;
}


/**********
 * FUNCTION
 * Cleare inputs and close without saving
***********/

void AddVisit::on_cancelBtn_clicked()
{
    ui->lineEditDate->clear();
    ui->plainTextEditReason->clear();
    ui->plainTextEditOperation->clear();
    ui->plainTextEditPlan->clear();
    this->close();
}



void AddVisit::on_calendarWidget_clicked(const QDate &date)
{

    selectedDate = date.toString("dd.MM.yyyy");
    ui->lineEditDate->setText(selectedDate);
    selectedDate = date.toString("yyyy-MM-dd");
    qDebug() << "Double-clicked Date: " << selectedDate;
}

