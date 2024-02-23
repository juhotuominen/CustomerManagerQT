#include "addcustomer.h"
#include "ui_addcustomer.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"

AddCustomer::AddCustomer(MainWindow *mainWindow, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddCustomer)
    , mainWindow(mainWindow)
{
    ui->setupUi(this);
}

AddCustomer::~AddCustomer()
{
    delete ui;
}

/**********
 * FUNCTION
 * Add new customer to database and table view
 * First insert data into database
 * TODO: refresh tableview on mainwindow after closing
***********/

void MainWindow::on_btnSave_clicked()
{
    QStringList data = ptrAddCustomer->getData();

    QSqlQuery query;
    query.prepare("INSERT INTO Customers (FirstName, LastName, SocialSecurity, Address, Phone, Email, Profession, Hobbies, Diseases, Medication) "
                  "VALUES (:firstname, :lastname, :socialsecurity, :address, :phone, :email, :profession, :hobbies, :diseases, :medication)");
    query.bindValue(":firstname", data[0]);
    query.bindValue(":lastname", data[1]);
    query.bindValue(":socialsecurity", data[2]);
    query.bindValue(":address", data[3]);
    query.bindValue(":phone", data[4]);
    query.bindValue(":email", data[5]);
    query.bindValue(":profession", data[6]);
    query.bindValue(":hobbies", data[7]);
    query.bindValue(":diseases", data[8]);
    query.bindValue(":medication", data[9]);


    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    emit ptrAddCustomer->customerAdded();

    ui->lineEditAddress_2->clear();
    ui->lineEditDiseases_2->clear();
    ui->lineEditEmail_2->clear();
    ui->lineEditFirstname_2->clear();
    ui->lineEditLastname_2->clear();
    ui->lineEditHobbies_2->clear();
    ui->lineEditMedication_2->clear();
    ui->lineEditPhone_2->clear();
    ui->lineEditProfession_2->clear();
    ui->lineEditSocialsecurity_2->clear();

}

/**********
 * FUNCTION
 * Get data from lineEdits and textEdits to be added for new customer
***********/

QStringList AddCustomer::getData()
{
    QStringList data;
    data << mainWindow->ui->lineEditFirstname_2->text()
         << mainWindow->ui->lineEditLastname_2->text()
         << mainWindow->ui->lineEditSocialsecurity_2->text()
         << mainWindow->ui->lineEditAddress_2->text()
         << mainWindow->ui->lineEditPhone_2->text()
         << mainWindow->ui->lineEditEmail_2->text()
         << mainWindow->ui->lineEditProfession_2->text()
         << mainWindow->ui->lineEditHobbies_2->text()
         << mainWindow->ui->lineEditDiseases_2->text()
         << mainWindow->ui->lineEditMedication_2->text();

    return data;
}

/**********
 * FUNCTION
 * Close window without saving and clear data
***********/

void MainWindow::on_btnCancel_clicked()
{
    ui->lineEditAddress_2->clear();
    ui->lineEditDiseases_2->clear();
    ui->lineEditEmail_2->clear();
    ui->lineEditFirstname_2->clear();
    ui->lineEditLastname_2->clear();
    ui->lineEditHobbies_2->clear();
    ui->lineEditMedication_2->clear();
    ui->lineEditPhone_2->clear();
    ui->lineEditProfession_2->clear();
    ui->lineEditSocialsecurity_2->clear();
}

