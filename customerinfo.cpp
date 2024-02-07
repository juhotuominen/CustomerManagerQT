#include "customerinfo.h"
#include "ui_customerinfo.h"

CustomerInfo::CustomerInfo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CustomerInfo)
{
    ui->setupUi(this);
}

CustomerInfo::~CustomerInfo()
{
    delete ui;
}


QStringList CustomerInfo::getCustomerInfo(int customerId)
{
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

    return data;
}


void CustomerInfo::setCustomerInfo(QStringList customerInfo)
{

    // Assuming customerInfo contains data for one customer
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


    ui->label->setText(firstName);
    ui->label_2->setText(lastName);
    ui->label_3->setText(socialsecurity);
    ui->label_4->setText(address);
    ui->label_5->setText(phone);
    ui->label_6->setText(email);
    ui->label_7->setText(profession);
    ui->label_8->setText(hobbies);
    ui->label_9->setText(diseases);
    ui->label_10->setText(medication);
}
