#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardPaths>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //void ReadFile();

    //void UpdateFile();

    void setupTableWidget();

    void on_btnAdd_clicked();

    void on_btnRemove_clicked();

    void on_btnGet_clicked();

    void on_btnSearch_clicked();

private:
    Ui::MainWindow *ui;

    QSqlDatabase DB;
    QStringList getData() const;
    QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "\\customers.txt";
    QString path_to_db = "D:/SQLite/Customers.db";
};
#endif // MAINWINDOW_H
