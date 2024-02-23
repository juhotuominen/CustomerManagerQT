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

    if (editMode == false){

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
    else if(editMode == true){
        QSqlQuery query;
        query.prepare("UPDATE Information SET date = :date, reason = :reason, operation = :operation, plan = :plan WHERE id = :id");
        query.bindValue(":id", publicVisitId);
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

        editMode = false;
        this->close();
    }

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


QStringList AddVisit::getDatabaseData(int id)
{
    QSqlQuery query;
    query.prepare("SELECT reason, operation, plan, date FROM Information WHERE id = :id");
    query.bindValue(":id", id);

    QStringList data;

    if (query.exec() && query.next()) {
        on_calendarWidget_clicked(query.value("date").toDate());
        // Assuming query has only one row for the specified customer
        data << query.value("date").toString()
             << query.value("reason").toString()
             << query.value("operation").toString()
             << query.value("plan").toString();
    } else {
        qDebug() << "Error retrieving customer information:" << query.lastError().text();
    }

    return data;
}

void AddVisit::setEditableData(int visitId)
{
    editMode = true;
    publicVisitId = visitId;
    QStringList dataList = getDatabaseData(visitId);
    qDebug() << selectedDate;

    ui->lineEditDate->setText(dataList[0]);
    ui->plainTextEditReason->setPlainText(dataList[1]);
    ui->plainTextEditOperation->setPlainText(dataList[2]);
    ui->plainTextEditPlan->setPlainText(dataList[3]);
}
