#include "addcustomer.h"
#include "ui_addcustomer.h"

AddCustomer::AddCustomer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddCustomer)
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

void AddCustomer::on_btnSave_clicked()
{
    QStringList data = getData();

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

    emit customerAdded();

    ui->lineEditAddress->clear();
    ui->lineEditDiseases->clear();
    ui->lineEditEmail->clear();
    ui->lineEditFirstname->clear();
    ui->lineEditLastname->clear();
    ui->lineEditHobbies->clear();
    ui->lineEditMedication->clear();
    ui->lineEditPhone->clear();
    ui->lineEditProfession->clear();
    ui->lineEditSocialsecurity->clear();

    this->close();
}

/**********
 * FUNCTION
 * Get data from lineEdits and textEdits to be added for new customer
***********/

QStringList AddCustomer::getData()
{
    QStringList data;
    data << ui->lineEditFirstname->text()
         << ui->lineEditLastname->text()
         << ui->lineEditSocialsecurity->text()
         << ui->lineEditAddress->text()
         << ui->lineEditPhone->text()
         << ui->lineEditEmail->text()
         << ui->lineEditProfession->text()
         << ui->lineEditHobbies->text()
         << ui->lineEditDiseases->text()
         << ui->lineEditMedication->text();

    return data;
}

/**********
 * FUNCTION
 * Close window without saving and clear data
***********/

void AddCustomer::on_btnCancel_clicked()
{
    ui->lineEditAddress->clear();
    ui->lineEditDiseases->clear();
    ui->lineEditEmail->clear();
    ui->lineEditFirstname->clear();
    ui->lineEditLastname->clear();
    ui->lineEditHobbies->clear();
    ui->lineEditMedication->clear();
    ui->lineEditPhone->clear();
    ui->lineEditProfession->clear();
    ui->lineEditSocialsecurity->clear();

    this->close();
}

