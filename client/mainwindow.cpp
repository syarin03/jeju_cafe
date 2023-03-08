#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <typeinfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->socket.connectToHost("127.0.0.1", 9001);

    if (this->socket.waitForConnected())
    {
//        qDebug() << "Connected to server!";
        cout << "connected to server" << endl;
    }
    else
    {
//        qCritical() << "Failed to connect to server";
        cout << "failed to connect to server" << endl;
        exit(1);
    }

    QJsonObject obj;
    obj["method"] = "client connect";
    QJsonDocument doc(obj);
    QByteArray arr = doc.toJson(QJsonDocument::Compact);
    this->socket.write(arr);

//    if (this->socket.waitForReadyRead())
    connect(&socket, &QTcpSocket::readyRead, [=]()
    {
        QByteArray response = this->socket.readAll();
//        qDebug() << "Response from server:" << response;
        cout << "response from server: " << response.toStdString() << endl;
    });

    QRegularExpression re_eng_num("[A-Za-z0-9]*");
    QRegularExpression re_phone("[0-9]{0,11}");
    QRegularExpression re_eng_kor("[가-힣ㄱ-ㅎㅏ-ㅣA-Za-z]*");

    QValidator *val_eng_num = new QRegularExpressionValidator(re_eng_num, this);
    QValidator *val_phone = new QRegularExpressionValidator(re_phone, this);
    QValidator *val_eng_kor = new QRegularExpressionValidator(re_eng_kor, this);

    ui->setupUi(this);

    ui->stackedWidget->setCurrentWidget(ui->stack_login);

    ui->input_login_id->setValidator(val_eng_num);
    ui->input_login_pw->setValidator(val_eng_num);
    ui->input_signup_id->setValidator(val_eng_num);
    ui->input_signup_pw->setValidator(val_eng_num);
    ui->input_signup_pwck->setValidator(val_eng_num);
    ui->input_signup_name->setValidator(val_eng_kor);
    ui->input_signup_phone->setValidator(val_phone);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_login_clicked()
{
    if (ui->input_login_id->text().toStdString() == "" || ui->input_login_pw->text().toStdString() == "")
    {
        QMessageBox::warning(this, "경고", "아이디 또는 비밀번호를 확인해주세요.");
        return;
    }
    QJsonObject obj;
    obj["method"] = "login";
    obj["input_id"] = ui->input_login_id->text();
    obj["input_pw"] = ui->input_login_pw->text();
    QJsonDocument doc(obj);
    QByteArray arr = doc.toJson(QJsonDocument::Compact);
    this->socket.write(arr);

//    cout << obj["method"].toString().toStdString() << endl;
}


void MainWindow::on_btn_login_to_signup_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->stack_signup);
}


void MainWindow::on_btn_signup_to_login_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->stack_login);
}


void MainWindow::on_btn_signup_clicked()
{
    QJsonObject obj;
    obj["method"] = "signup";
    obj["input_id"] = ui->input_signup_id->text();
    obj["input_pw"] = ui->input_signup_pw->text();
    obj["input_name"] = ui->input_signup_name->text();
    obj["input_phone"] = ui->input_signup_phone->text();
    QJsonDocument doc(obj);
    QByteArray arr = doc.toJson(QJsonDocument::Compact);
    this->socket.write(arr);

    cout << obj["method"].toString().toStdString() << endl;

//    QJsonDocument doc2 = QJsonDocument::fromJson(arr);
//    QJsonObject obj2 = doc2.object();

//    for (auto a:obj2)
//    {
//        cout << a.type() << endl;
//        if (a.type() == 3)
//        {
//            cout << a.toString().toStdString() << endl;
//        }
//        else if (a.type() == 2)
//        {
//            cout << a.toInt() << endl;
//        }
//    }
}


void MainWindow::on_stackedWidget_currentChanged(int arg1)
{
    ui->input_login_id->clear();
    ui->input_login_pw->clear();

    ui->input_signup_id->clear();
    ui->input_signup_pw->clear();
    ui->input_signup_pwck->clear();
    ui->input_signup_name->clear();
    ui->input_signup_phone->clear();
}

