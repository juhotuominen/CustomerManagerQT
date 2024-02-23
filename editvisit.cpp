#include "editvisit.h"
#include "qdatetime.h"
#include "qmessagebox.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "ui_editvisit.h"

EditVisit::EditVisit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditVisit)
{
    ui->setupUi(this);
    ptrAddVisit = new AddVisit();
    connect(ptrAddVisit, &AddVisit::visitAdded, this, &EditVisit::onVisitAdded);
}

EditVisit::~EditVisit()
{
    delete ui;
}


void EditVisit::setupTableWidget()
{
    ui->tableWidget->setColumnCount(5);

    QStringList headers;
    headers << "Pvm" << "Tulo syy" << "Toimenpiteet" << "Jatkosuunnitelma" << "ID";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void EditVisit::getCustomerVisitInfo(int customerId)
{
    privateCustomerId = customerId;
    QSqlQuery query;
    query.prepare("SELECT id, reason, operation, plan, date FROM Information WHERE customer_id = :customerid ORDER BY STRFTIME('%Y-%m-%d', date) DESC");
    query.bindValue(":customerid", customerId);

    QStringList visitData;

    if (query.exec()) {
        while (query.next()) {
            // Iterate through all rows for the specified customer
            visitData << query.value("date").toString()
                      << query.value("reason").toString()
                      << query.value("operation").toString()
                      << query.value("plan").toString()
                      << query.value("id").toString();
        }

        if (visitData.isEmpty()) {
            qDebug() << "No data found for customer_id:" << customerId;
        }
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    setCustomerVisitInfo(visitData);
}

void EditVisit::setCustomerVisitInfo(QStringList customerVisitInfo)
{
    // Assuming ui->tableWidget is a QTableWidget
    ui->tableWidget->clear();  // Clear previous content

    int numVisits = customerVisitInfo.size() / 5;
    // Set the number of rows in the table
    ui->tableWidget->setRowCount(numVisits);

    // Set the number of columns in the table
    setupTableWidget();

    for (int visitIndex = 0; visitIndex < numVisits; ++visitIndex)
    {
        // Make the first row (date row) bold
        QString dateString = customerVisitInfo[visitIndex * 5];
        QDate isoDate = QDate::fromString(dateString, "yyyy-MM-dd");
        QString displayDate = isoDate.toString("dd.MM.yyyy");

        QTableWidgetItem *dateItem = new QTableWidgetItem(displayDate);
        dateItem->setFlags(dateItem->flags() & ~Qt::ItemIsEditable);  // Make date row read-only
        ui->tableWidget->setItem(visitIndex, 0, dateItem);

        // Add editable fields for other rows
        for (int rowIndex = 1; rowIndex < 5; ++rowIndex)
        {
            QString fieldValue = customerVisitInfo[visitIndex * 5 + rowIndex];

            QTableWidgetItem *item = new QTableWidgetItem(fieldValue);
            ui->tableWidget->setItem(visitIndex, rowIndex, item);
        }
    }
}

void EditVisit::on_btnRemove_clicked()
{
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow >= 0)
    {
        // Ask for confirmation
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm Deletion", "Haluatko varmasti poistaa käynnin?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            QString id = ui->tableWidget->item(currentRow, 4)->text();
            qDebug() << id;

            QSqlQuery query;
            query.prepare("DELETE FROM Information WHERE id = :id");
            query.bindValue(":id", id);

            if (query.exec())
            {
                ui->tableWidget->removeRow(currentRow);
                QMessageBox::information(this, "Remove Row", "Käynti poistettu");
                emit visitEdited();
            }
            else
            {
                QMessageBox::critical(this, "Database Error", "Failed to remove row from the database:\n" + query.lastError().text());
            }
        }
    }
    else
    {
        QMessageBox::information(this, "Remove Row", "Poistettavaa käyntiä ei valittu");
    }
}





void EditVisit::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    int visitId = ui->tableWidget->item(item->row(), 4)->text().toInt();

    ptrAddVisit->setEditableData(visitId);

    // Update CustomerInfoTab with the retrieved information
    ptrAddVisit->show();
}


void EditVisit::on_btnCancel_clicked()
{
    QTableWidgetItem *item = ui->tableWidget->currentItem();

    if (item != nullptr) {
        on_tableWidget_itemDoubleClicked(item);
    } else {
        // Handle the case where no item is selected
        qDebug() << "No item selected.";
        QMessageBox::information(this, "Edit Row", "Muokattavaa käyntiä ei valittu");
    }
}

void EditVisit::onVisitAdded()
{
    getCustomerVisitInfo(privateCustomerId);
    emit visitEdited();

}

