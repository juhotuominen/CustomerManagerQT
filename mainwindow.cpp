#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QStandardPaths>
#include <QMessageBox>
#include <QTableWidget>
#include <QStyleFactory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qApp->setStyle(QStyleFactory::create("Fusion"));

    ui->setupUi(this);
    ptrAddCustomer = new AddCustomer(this);
    ptrCustomerInfo = new CustomerInfo(this);
    ptrAddVisit = new AddVisit();
    ptrEditVisit = new EditVisit();

    const QStringList lineEditObjectNames = {
        "lineEditFirstname",
        "lineEditLastname",
        "lineEditSocialsecurity",
        "lineEditAddress",
        "lineEditPhone",
        "lineEditEmail",
        "lineEditProfession",
        "lineEditHobbies",
        "lineEditDiseases",
        "lineEditMedication"
    };

    for (const QString& objectName : lineEditObjectNames) {
        QLineEdit* lineEdit = findChild<QLineEdit*>(objectName);
        if (lineEdit) {
            connect(lineEdit, &QLineEdit::textEdited, this, &MainWindow::onLineEditTextChanged);
        }
    }

    const QStringList lineEditObjectNames2 = {
        "lineEditFirstname_2",
        "lineEditLastname_2",
        "lineEditSocialsecurity_2",
        "lineEditAddress_2",
        "lineEditPhone_2",
        "lineEditEmail_2",
        "lineEditProfession_2",
        "lineEditHobbies_2",
        "lineEditDiseases_2",
        "lineEditMedication_2"
    };

    for (const QString& objectName : lineEditObjectNames2) {
        QLineEdit* lineEdit2 = findChild<QLineEdit*>(objectName);
        if (lineEdit2) {
            connect(lineEdit2, &QLineEdit::textEdited, this, &MainWindow::onLineEditTextChanged2);
        }
    }

    connect(ptrAddVisit, &AddVisit::visitAdded, this, &MainWindow::onVisitAdded);
    connect(ptrEditVisit, &EditVisit::visitEdited, this, &MainWindow::onVisitEdited);

    setupTableWidget();

    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName(path_to_db);
    QFileInfo checkFile(path_to_db);
    QSqlQuery query(DB);

    if (checkFile.isFile()) {
        if (DB.open()) {
            ui->labelStatus->setText("Connected...");

            // Enable foreign key support after opening the database
            query.exec("PRAGMA foreign_keys = ON;");
        } else {
            ui->labelStatus->setText("Error: " + DB.lastError().text());
        }
    } else {
        ui->labelStatus->setText("Failed to open the database!");
    }

    ui->saveButton->setEnabled(false);
    ui->btnSave->setEnabled(false);

    connect(ptrAddCustomer, &AddCustomer::customerAdded, this, &MainWindow::onCustomerAdded);
    on_btnGet_clicked();
}

MainWindow::~MainWindow()
{
    delete ptrCustomerInfo;
    delete ptrAddCustomer;
    delete ui;

    if (DB.isOpen())
    {
        DB.close();
    }
}

/**********
 * FUNCTION
 * Setup table at the start of program
 * Number of columns, Headers for each column and column width
**************/

void MainWindow::setupTableWidget()
{
    ui->tableWidget->setColumnCount(3);

    QStringList headers;
    headers << "ID" << "Etunimi" << "Sukunimi";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


/**********
 * FUNCTION
 * Remove customer from database and table view
 * First remove from database, if succes remove from view and infrom user
***********/

void MainWindow::on_btnRemove_clicked()
{
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow >= 0)
    {
        // Ask for confirmation
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm Deletion", "Haluatko varmasti poistaa asiakkaan?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            QString id = ui->tableWidget->item(currentRow, 0)->text();

            QSqlQuery query;
            query.prepare("DELETE FROM Customers WHERE ID = :id");
            query.bindValue(":id", id);

            if (query.exec())
            {
                ui->tableWidget->removeRow(currentRow);
                QMessageBox::information(this, "Remove Row", "Asiakas poistettu");
                ui->stackedWidget->setCurrentIndex(2);
            }
            else
            {
                QMessageBox::critical(this, "Database Error", "Failed to remove row from the database:\n" + query.lastError().text());
            }
        }
    }
    else
    {
        QMessageBox::information(this, "Remove Row", "Poistettavaa asiakasta ei valittu");
    }
}



/**********
 * FUNCTION
 * Get informations from database (used as refresh)
 * Clear existing data on tablewidget and then get data from database
***********/

void MainWindow::on_btnGet_clicked()
{
    QSqlQuery query("SELECT id, Firstname, Lastname FROM Customers");

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    int row = 0;
    while (query.next()) {
        QString id = query.value("id").toString();
        QString firstName = query.value("Firstname").toString();
        QString lastName = query.value("Lastname").toString();

        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(firstName));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(lastName));

        ui->tableWidget->setColumnHidden(0, true);
        row++;
    }
}


/**********
 * FUNCTION
 * Get informations from database with spesific customer,
 * search with letter or fullname/socialsecurity.
 * First clear existing data on tableview and then get data
 * from database with matching searchparameter.
***********/

void MainWindow::on_btnSearch_clicked()
{
    QSqlQuery query;
    QString searchParam = ui->lineEditSearch -> text();
    query.prepare("SELECT id, Firstname, Lastname FROM Customers WHERE Firstname LIKE :parameter OR Lastname LIKE :parameter OR SocialSecurity LIKE :parameter");
    query.bindValue(":parameter", searchParam + "%");

    if (query.exec())
    {
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);

        int row = 0;
        while (query.next())
        {
            QString id = query.value("id").toString();
            QString firstName = query.value("Firstname").toString();
            QString lastName = query.value("Lastname").toString();


            ui->tableWidget->insertRow(row);

            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(id));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(firstName));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(lastName));

            row++;
        }
    }
    else
    {
        qDebug() << "Error executing query:" << query.lastError().text();
    }
}


/**********
 * FUNCTION
 * Add new customer.
 * Opens a new window for customer adding.
***********/

void MainWindow::on_btnAddCustomer_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    on_btnGet_clicked();
}

/**********
 * FUNCTION
 * Edit customer visits
 * Opens a new window for customer visit editing.
***********/

void MainWindow::on_pushButton_clicked()
{
    ptrEditVisit->getCustomerVisitInfo(ptrCustomerInfo->publicId);
    ptrEditVisit->show();
}

/**********
 * FUNCTION
 * Update when signals are emitted
 *
***********/


void MainWindow::onVisitAdded()
{
    ptrCustomerInfo->getCustomerVisitInfo(ptrCustomerInfo->getCustomerId());
}

void MainWindow::onVisitEdited()
{
    ptrCustomerInfo->getCustomerVisitInfo(ptrCustomerInfo->getCustomerId());
}


/**********
 * FUNCTION
 * Click column to show customer
 * spesific info on a new window.
***********/

void MainWindow::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    int customerId = ui->tableWidget->item(item->row(), 0)->text().toInt();

    QStringList customerInfo = ptrCustomerInfo->getCustomerInfo(customerId);

    // Update CustomerInfoTab with the retrieved information
    ptrCustomerInfo->setCustomerInfo(customerInfo);

    // Clear the existing layout before adding the CustomerInfo widget

    ui->stackedWidget->setCurrentIndex(1);
}

