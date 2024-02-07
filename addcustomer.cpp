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
 * TODO: Close window after saving and refresh tableview on mainwindow
***********/

void AddCustomer::on_btnSave_clicked()
{
    QStringList data = getData();

    // TODO: Check that empty areas are not allowed
    QSqlQuery query;
    query.prepare("INSERT INTO Customers (FirstName, LastName, SocialSecurity, Address, Phone, Email, Preinformation, Comments) "
                  "VALUES (:firstname, :lastname, :socialsecurity, :address, :phone, :email, :preinfo, :comments)");
    query.bindValue(":firstname", data[0]);
    query.bindValue(":lastname", data[1]);
    query.bindValue(":socialsecurity", data[2]);
    query.bindValue(":address", data[3]);
    query.bindValue(":phone", data[4]);
    query.bindValue(":email", data[5]);
    query.bindValue(":preinfo", data[6]);
    query.bindValue(":comments", data[7]);


    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }
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
         << ui->textEditPreinfo->toPlainText()
         << ui->textEditComments->toPlainText();

    return data;
}

