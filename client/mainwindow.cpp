#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <typeinfo>

User::User(int num, QString uid, QString upw, QString uname, QString phone)
    : num(num), uid(uid), upw(upw), uname(uname), phone(phone) {}

User::User() {}

void User::printInfo()
{
    cout << "num: " << this->num << endl;
    cout << "uid: " << this->uid.toStdString() << endl;
    cout << "upw: " << this->upw.toStdString() << endl;
    cout << "name: " << this->uname.toStdString() << endl;
    cout << "phone: " << this->phone.toStdString() << endl;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->socket.connectToHost("127.0.0.1", 9001);

    if (this->socket.waitForConnected())
    {
        qDebug() << "Connected to server!";
    }
    else
    {
        qCritical() << "Failed to connect to server";
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
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();

        string method = obj.value("method").toString().toStdString();

        if (method == "login_result")
        {
            if (!obj["result"].toBool())
            {
                QMessageBox::warning(this, "경고", "아이디 또는 비밀번호가 일치하지 않습니다.");
            }
            else
            {
                QMessageBox::information(this, "알림", "로그인 성공");
                int num = obj.value("user_num").toInt();
                QString uid = obj.value("user_id").toString();
                QString upw = obj.value("user_pw").toString();
                QString uname = obj.value("user_name").toString();
                QString phone = obj.value("user_phone").toString();
                this->login_user = User(num, uid, upw, uname, phone);
                this->login_user.printInfo();
                ui->stackedWidget->setCurrentWidget(ui->stack_main);
            }
        }
        else if (method == "check_id_result")
        {
            if (!obj["result"].toBool())
            {
                ui->label_signup_id->setText("이미 사용 중인 아이디");
                this->isIDCheck = false;
            }
            else
            {
                ui->label_signup_id->setText("사용 가능한 아이디");
                this->isIDCheck = true;
            }
        }
        else if (method == "signup_result")
        {
            if (!obj["result"].toBool())
            {
                QMessageBox::warning(this, "경고", "오류가 발생하여 회원가입에 실패하였습니다.");
            }
            else
            {
                QMessageBox::information(this, "알림", "회원가입 성공");
                ui->stackedWidget->setCurrentWidget(ui->stack_login);
            }
        }
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
    if (!this->isIDCheck)
    {
        QMessageBox::warning(this, "경고", "이미 사용 중인 아이디입니다.");
        return;
    }
    else if (!this->isPWRule)
    {
        QMessageBox::warning(this, "경고", "영문 숫자 혼용 8자 이상 비밀번호를 입력해주세요.");
        return;
    }
    else if (!this->isPWSame)
    {
        QMessageBox::warning(this, "경고", "비밀번호가 일치하지 않습니다.");
        return;
    }
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
}


void MainWindow::on_stackedWidget_currentChanged(int arg1)
{
    cout << "current stack: " << arg1 << endl;
    ui->input_login_id->clear();
    ui->input_login_pw->clear();

    ui->input_signup_id->clear();
    ui->input_signup_pw->clear();
    ui->input_signup_pwck->clear();
    ui->input_signup_name->clear();
    ui->input_signup_phone->clear();
}


void MainWindow::on_input_signup_id_editingFinished()
{
    QJsonObject obj;
    obj["method"] = "check_id";
    obj["input_id"] = ui->input_signup_id->text();
    QJsonDocument doc(obj);
    QByteArray arr = doc.toJson(QJsonDocument::Compact);
    this->socket.write(arr);

    cout << obj["method"].toString().toStdString() << endl;
}


void MainWindow::on_input_signup_pw_editingFinished()
{
    int len = ui->input_signup_pw->text().length();
    int cnt_eng = 0, cnt_num = 0;

    for (auto a:ui->input_signup_pw->text().toStdString())
    {
        if (isdigit(a) == 0)
        {
            cnt_eng++;
        }
        else
        {
            cnt_num++;
        }
    }

    if (len < 8 || cnt_eng == len || cnt_num == len)
    {
        ui->label_signup_pw->setText("영문 숫자 혼용 8자 이상");
        this->isPWRule = false;
    }
    else
    {
        ui->label_signup_pw->clear();
        this->isPWRule = true;
    }

    if (ui->input_signup_pw->text() != ui->input_signup_pwck->text())
    {
        ui->label_signup_pwck->setText("비밀번호가 일치하지 않습니다");
        this->isPWSame = false;
    }
    else
    {
        ui->label_signup_pwck->clear();
        this->isPWSame = true;
    }
}


void MainWindow::on_input_signup_pwck_textChanged(const QString &arg1)
{
    if (ui->input_signup_pw->text() != ui->input_signup_pwck->text())
    {
        ui->label_signup_pwck->setText("비밀번호가 일치하지 않습니다");
        this->isPWSame = false;
    }
    else
    {
        ui->label_signup_pwck->clear();
        this->isPWSame = true;
    }
}


void MainWindow::on_btn_logout_clicked()
{
    QMessageBox::information(this, "알림", "로그아웃 되었습니다.");
    this->login_user = User();
    ui->stackedWidget->setCurrentWidget(ui->stack_login);
}

