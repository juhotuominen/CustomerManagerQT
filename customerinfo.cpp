#include "customerinfo.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "ui_customerinfo.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

CustomerInfo::CustomerInfo(MainWindow *mainWindow, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CustomerInfo)
    , mainWindow(mainWindow)
{
    ui->setupUi(this);
    ptrAddVisit = new AddVisit();

    ui->saveButton->setEnabled(false);
}

CustomerInfo::~CustomerInfo()
{
    delete ptrAddVisit;
    delete ui;
}


/**********
 * FUNCTION
 * Getter and Setter for customerID
***********/

void CustomerInfo::setCustomerId(int id)
{
    publicId = id;
}
int CustomerInfo::getCustomerId()
{
    return publicId;
}


/**********
 * FUNCTION
 * Get customer info from database and set publicID
***********/

QStringList CustomerInfo::getCustomerInfo(int customerId)
{
    setCustomerId(customerId);

    QSqlQuery query;
    query.prepare("SELECT Firstname, Lastname, SocialSecurity, Address, Phone, Email, Profession, Hobbies, Diseases, Medication FROM Customers WHERE id = :customerid");
    query.bindValue(":customerid", customerId);

    QStringList data;

    if (query.exec() && query.next()) {
        // Assuming query has only one row for the specified customer
        data << query.value("Firstname").toString()
             << query.value("Lastname").toString()
             << query.value("SocialSecurity").toString()
             << query.value("Address").toString()
             << query.value("Phone").toString()
             << query.value("Email").toString()
             << query.value("Profession").toString()
             << query.value("Hobbies").toString()
             << query.value("Diseases").toString()
             << query.value("Medication").toString();
    } else {
        qDebug() << "Error retrieving customer information:" << query.lastError().text();
    }

    getCustomerVisitInfo(customerId);
    return data;
}


/**********
 * FUNCTION
 * Get visit info from database
***********/

void CustomerInfo::getCustomerVisitInfo(int customerId)
{
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
                      << query.value("plan").toString();

        }

        if (visitData.isEmpty()) {
            qDebug() << "No data found for customer_id:" << customerId;
        }
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    setCustomerVisitInfo(visitData);
}


/**********
 * FUNCTION
 * Set info from database to info tab
***********/

void CustomerInfo::setCustomerInfo(QStringList customerInfo)
{

    QString firstName = customerInfo[0];
    QString lastName = customerInfo[1];
    QString socialsecurity = customerInfo[2];
    QString address = customerInfo[3];
    QString phone = customerInfo[4];
    QString email = customerInfo[5];
    QString profession = customerInfo[6];
    QString hobbies = customerInfo[7];
    QString diseases = customerInfo[8];
    QString medication = customerInfo[9];

    mainWindow->ui->lineEditFirstname->setText(firstName);
    mainWindow->ui->lineEditLastname->setText(lastName);
    mainWindow->ui->lineEditSocialsecurity->setText(socialsecurity);
    mainWindow->ui->lineEditAddress->setText(address);
    mainWindow->ui->lineEditPhone->setText(phone);
    mainWindow->ui->lineEditEmail->setText(email);
    mainWindow->ui->lineEditProfession->setText(profession);
    mainWindow->ui->lineEditHobbies->setText(hobbies);
    mainWindow->ui->lineEditDiseases->setText(diseases);
    mainWindow->ui->lineEditMedication->setText(medication);
}


/**********
 * FUNCTION
 * Set info from database to visits tab
***********/

void CustomerInfo::setCustomerVisitInfo(QStringList customerVisitInfo)
{
    QString formattedText;

    for (int i = 0; i < customerVisitInfo.size(); ++i)
    {
        if (i % 4 == 0) // Make the first row (date row) bold
        {
            QString dateString = customerVisitInfo[i];
            QDate isoDate = QDate::fromString(dateString, "yyyy-MM-dd");
            QString displayDate = isoDate.toString("dd.MM.yyyy");

            formattedText += "<br><b>" + displayDate + "</b><br>";
        }
        else
        {
            formattedText += customerVisitInfo[i] + "<br><br>";
        }
    }

    mainWindow->ui->textBrowser->setHtml(formattedText);
}


/**********
 * FUNCTION
 * Pressing enter is same as clicking search
***********/

void MainWindow::on_lineEditSearch_editingFinished()
{
    on_btnSearch_clicked();
}

void MainWindow::onCustomerAdded()
{
    on_btnGet_clicked();
}


/**********
 * FUNCTION
 * Make save buttons visible if text is edited
***********/

void MainWindow::onLineEditTextChanged(const QString &arg1)
{
    Q_UNUSED(arg1); // Unused parameter to avoid compiler warnings
    ui->saveButton->setEnabled(true);
}

void MainWindow::onLineEditTextChanged2(const QString &arg1)
{
    Q_UNUSED(arg1); // Unused parameter to avoid compiler warnings
    ui->btnSave->setEnabled(true);
}

void MainWindow::on_cancelButton_clicked()
{
    ui->saveButton->setEnabled(false);
}

/**********
 * FUNCTION
 * Save modified customer infos
***********/

void MainWindow::on_saveButton_clicked()
{
    QSqlQuery query;
    query.prepare("UPDATE Customers SET Firstname = :firstname, Lastname = :lastname, SocialSecurity = :socialsecurity, Address = :address, Phone = :phone, Email = :email, Profession = :profession, Hobbies = :hobbies, Diseases = :diseases, Medication = :medication WHERE id = :customerid");

    query.bindValue(":customerid", ptrCustomerInfo->publicId);
    query.bindValue(":firstname", ui->lineEditFirstname->text());
    query.bindValue(":lastname", ui->lineEditLastname->text());
    query.bindValue(":socialsecurity", ui->lineEditSocialsecurity->text());
    query.bindValue(":address", ui->lineEditAddress->text());
    query.bindValue(":phone", ui->lineEditPhone->text());
    query.bindValue(":email", ui->lineEditEmail->text());
    query.bindValue(":profession", ui->lineEditProfession->text());
    query.bindValue(":hobbies", ui->lineEditHobbies->text());
    query.bindValue(":diseases", ui->lineEditDiseases->text());
    query.bindValue(":medication", ui->lineEditMedication->text());

    if (query.exec()) {
        qDebug() << "Row updated successfully!";
    } else {
        qDebug() << "Error updating row:" << query.lastError().text();
    }

    on_btnGet_clicked();
    ui->saveButton->setEnabled(false);
}


/**********
 * FUNCTION
 * set customerId and open AddVisit page
***********/

void MainWindow::on_addVisitButton_clicked()
{
    ptrAddVisit->setCustomerId(ptrCustomerInfo->getCustomerId());
    ptrAddVisit->show();
}



