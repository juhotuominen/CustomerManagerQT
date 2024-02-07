#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QStandardPaths>
#include <QMessageBox>
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setupTableWidget();

    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName(path_to_db);
    QFileInfo checkFile(path_to_db);

    if(checkFile.isFile())
    {
        if(DB.open())
        {
            ui->label->setText("Connected...");
        }
        else
        {
            ui->label->setText("Error: " + DB.lastError().text());
        }
    }
    else
    {
        ui->label->setText("Failed to open the database!");
    }

    on_btnGet_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (DB.isOpen())
    {
        DB.close();
    }
}


void MainWindow::setupTableWidget()
{
    // Number of columns
    ui->tableWidget->setColumnCount(7);

    // Header labels for each column
    QStringList headers;
    headers << "ID" << "Etunimi" << "Sukunimi" << "Hetu" << "Osoite" << "Puhelinnumero" << "Email";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Adjust column widths if needed
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

QStringList MainWindow::getData() const
{
    // TODO: Unique ID and check for empty areas
    QStringList data;
    data << ui->lineEditFirstname->text()
         << ui->lineEditLastname->text()
         << ui->lineEditSocialsecurity->text()
         << ui->lineEditAddress->text()
         << ui->lineEditPhone->text()
         << ui->lineEditEmail->text();

    return data;
}


void MainWindow::on_btnAdd_clicked()
{

    QStringList data = getData();

    // Insert the data into the SQLite database
    // TODO: Check that empty areas are not allowed
    QSqlQuery query;
    query.prepare("INSERT INTO Customers (FirstName, LastName, SocialSecurity, Address, Phone, Email) "
                  "VALUES (:firstname, :lastname, :socialsecurity, :address, :phone, :email)");
    query.bindValue(":firstname", data[0]);
    query.bindValue(":lastname", data[1]);
    query.bindValue(":socialsecurity", data[2]);
    query.bindValue(":address", data[3]);
    query.bindValue(":phone", data[4]);
    query.bindValue(":email", data[5]);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return; // If there's an error, exit the function
    }

    // Refresh the tableWidget by re-reading data
    on_btnGet_clicked();
}




void MainWindow::on_btnRemove_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    QString id = ui->tableWidget->item(currentRow, 0)->text();

    if (currentRow >= 0)
    {
        QSqlQuery query;
        query.prepare("DELETE FROM Customers WHERE ID = :id");
        query.bindValue(":id", id);

        if (query.exec())
        {
            // Remove the row from the tableWidget
            ui->tableWidget->removeRow(currentRow);
            QMessageBox::information(this, "Remove Row", "Row removed from table and database.");
        }
        else
        {
            QMessageBox::critical(this, "Database Error", "Failed to remove row from the database:\n" + query.lastError().text());
        }
    }
    else
    {
        QMessageBox::information(this, "Remove Row", "No row selected to remove.");
    }
}

void MainWindow::on_btnGet_clicked()
{
    QSqlQuery query("SELECT id, Firstname, Lastname, SocialSecurity, Address, Phone, Email FROM Customers");

    // Clear existing data in the tableWidget
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    int row = 0;
    while (query.next()) {
        // Access data using column names or indices
        QString id = query.value("id").toString();
        QString firstName = query.value("Firstname").toString();
        QString lastName = query.value("Lastname").toString();
        QString socialsecurity = query.value("SocialSecurity").toString();
        QString address = query.value("Address").toString();
        QString phone = query.value("Phone").toString();
        QString email = query.value("Email").toString();

        // Add a new row to the tableWidget
        ui->tableWidget->insertRow(row);

        // Populate cells in the current row
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(firstName));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(lastName));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(socialsecurity));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(address));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(phone));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(email));

        row++;
    }
}




void MainWindow::on_btnSearch_clicked()
{
    QSqlQuery query;
    QString searchParam = ui->lineEditSearch -> text();
    query.prepare("SELECT * FROM Customers WHERE Firstname LIKE :parameter OR SocialSecurity LIKE :parameter");
    query.bindValue(":parameter", searchParam + "%");

    if (query.exec())
    {
        // Clear existing data in the tableWidget
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);

        // Assuming your tableWidget has columns FirstName, LastName, etc.
        int row = 0;
        while (query.next())
        {
            QString id = query.value("id").toString();
            QString firstName = query.value("Firstname").toString();
            QString lastName = query.value("Lastname").toString();
            QString socialsecurity = query.value("SocialSecurity").toString();
            QString address = query.value("Address").toString();
            QString phone = query.value("Phone").toString();
            QString email = query.value("Email").toString();

            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(firstName));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(lastName));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(socialsecurity));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(address));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(phone));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(email));

            row++;
        }
    }
    else
    {
        qDebug() << "Error executing query:" << query.lastError().text();
    }
}

