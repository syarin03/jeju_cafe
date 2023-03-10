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

class User
{
private:
    int num;
    QString uid;
    QString upw;
    QString uname;
    QString phone;
public:
    User();
    User(int num, QString uid, QString upw, QString uname, QString phone);
    void printInfo();
};

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

    void on_stackedWidget_currentChanged(int arg1);

    void on_input_signup_id_editingFinished();

    void on_input_signup_pw_editingFinished();

    void on_input_signup_pwck_textChanged(const QString &arg1);

    void on_btn_logout_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket socket;
    User login_user;
    bool isIDCheck = false;
    bool isPWRule = false;
    bool isPWSame = false;
};
#endif // MAINWINDOW_H
