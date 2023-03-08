#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDebug>
#include <QMessageBox>
#include <QtCore/QCoreApplication>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_btn_login_clicked();

    void on_btn_login_to_signup_clicked();

    void on_btn_signup_to_login_clicked();

    void on_btn_signup_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket socket;
};
#endif // MAINWINDOW_H
