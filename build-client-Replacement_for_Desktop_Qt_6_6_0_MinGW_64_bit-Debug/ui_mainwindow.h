/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *stack_login;
    QLineEdit *input_login_id;
    QLineEdit *input_login_pw;
    QPushButton *btn_login;
    QPushButton *btn_login_to_signup;
    QWidget *stack_signup;
    QLineEdit *input_signup_id;
    QLineEdit *input_signup_pw;
    QLineEdit *input_signup_pwck;
    QLineEdit *input_signup_name;
    QLineEdit *input_signup_phone;
    QLabel *label_signup_id;
    QLabel *label_signup_pw;
    QLabel *label_signup_pwck;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *btn_signup_to_login;
    QPushButton *btn_signup;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(0, 0, 801, 601));
        stack_login = new QWidget();
        stack_login->setObjectName("stack_login");
        input_login_id = new QLineEdit(stack_login);
        input_login_id->setObjectName("input_login_id");
        input_login_id->setGeometry(QRect(160, 250, 161, 31));
        input_login_pw = new QLineEdit(stack_login);
        input_login_pw->setObjectName("input_login_pw");
        input_login_pw->setGeometry(QRect(160, 290, 161, 31));
        input_login_pw->setEchoMode(QLineEdit::Password);
        btn_login = new QPushButton(stack_login);
        btn_login->setObjectName("btn_login");
        btn_login->setGeometry(QRect(330, 250, 81, 71));
        btn_login_to_signup = new QPushButton(stack_login);
        btn_login_to_signup->setObjectName("btn_login_to_signup");
        btn_login_to_signup->setGeometry(QRect(480, 250, 101, 31));
        stackedWidget->addWidget(stack_login);
        stack_signup = new QWidget();
        stack_signup->setObjectName("stack_signup");
        input_signup_id = new QLineEdit(stack_signup);
        input_signup_id->setObjectName("input_signup_id");
        input_signup_id->setGeometry(QRect(370, 130, 161, 31));
        input_signup_pw = new QLineEdit(stack_signup);
        input_signup_pw->setObjectName("input_signup_pw");
        input_signup_pw->setGeometry(QRect(370, 190, 161, 31));
        input_signup_pw->setEchoMode(QLineEdit::Password);
        input_signup_pwck = new QLineEdit(stack_signup);
        input_signup_pwck->setObjectName("input_signup_pwck");
        input_signup_pwck->setGeometry(QRect(370, 250, 161, 31));
        input_signup_pwck->setEchoMode(QLineEdit::Password);
        input_signup_name = new QLineEdit(stack_signup);
        input_signup_name->setObjectName("input_signup_name");
        input_signup_name->setGeometry(QRect(370, 310, 161, 31));
        input_signup_phone = new QLineEdit(stack_signup);
        input_signup_phone->setObjectName("input_signup_phone");
        input_signup_phone->setGeometry(QRect(370, 370, 161, 31));
        label_signup_id = new QLabel(stack_signup);
        label_signup_id->setObjectName("label_signup_id");
        label_signup_id->setGeometry(QRect(370, 160, 161, 21));
        label_signup_id->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_signup_pw = new QLabel(stack_signup);
        label_signup_pw->setObjectName("label_signup_pw");
        label_signup_pw->setGeometry(QRect(370, 220, 161, 21));
        label_signup_pw->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_signup_pwck = new QLabel(stack_signup);
        label_signup_pwck->setObjectName("label_signup_pwck");
        label_signup_pwck->setGeometry(QRect(370, 280, 161, 21));
        label_signup_pwck->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label = new QLabel(stack_signup);
        label->setObjectName("label");
        label->setGeometry(QRect(200, 130, 161, 31));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_2 = new QLabel(stack_signup);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(200, 190, 161, 31));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3 = new QLabel(stack_signup);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(200, 250, 161, 31));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_4 = new QLabel(stack_signup);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(200, 310, 161, 31));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_5 = new QLabel(stack_signup);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(200, 370, 161, 31));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        btn_signup_to_login = new QPushButton(stack_signup);
        btn_signup_to_login->setObjectName("btn_signup_to_login");
        btn_signup_to_login->setGeometry(QRect(10, 10, 101, 31));
        btn_signup = new QPushButton(stack_signup);
        btn_signup->setObjectName("btn_signup");
        btn_signup->setGeometry(QRect(350, 480, 101, 31));
        stackedWidget->addWidget(stack_signup);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        input_login_id->setPlaceholderText(QCoreApplication::translate("MainWindow", "\353\241\234\352\267\270\354\235\270", nullptr));
        input_login_pw->setPlaceholderText(QCoreApplication::translate("MainWindow", "\353\271\204\353\260\200\353\262\210\355\230\270", nullptr));
        btn_login->setText(QCoreApplication::translate("MainWindow", "\353\241\234\352\267\270\354\235\270", nullptr));
        btn_login_to_signup->setText(QCoreApplication::translate("MainWindow", "\355\232\214\354\233\220\352\260\200\354\236\205", nullptr));
        input_signup_id->setPlaceholderText(QCoreApplication::translate("MainWindow", "\354\225\204\354\235\264\353\224\224", nullptr));
        input_signup_pw->setPlaceholderText(QCoreApplication::translate("MainWindow", "\353\271\204\353\260\200\353\262\210\355\230\270", nullptr));
        input_signup_pwck->setPlaceholderText(QCoreApplication::translate("MainWindow", "\353\271\204\353\260\200\353\262\210\355\230\270 \355\231\225\354\235\270", nullptr));
        input_signup_name->setPlaceholderText(QCoreApplication::translate("MainWindow", "\354\235\264\353\246\204", nullptr));
        input_signup_phone->setPlaceholderText(QCoreApplication::translate("MainWindow", "\354\240\204\355\231\224\353\262\210\355\230\270", nullptr));
        label_signup_id->setText(QCoreApplication::translate("MainWindow", "\354\244\221\353\263\265", nullptr));
        label_signup_pw->setText(QCoreApplication::translate("MainWindow", "\354\230\201\354\226\264+\354\210\253\354\236\220 8\354\236\220 \354\235\264\354\203\201", nullptr));
        label_signup_pwck->setText(QCoreApplication::translate("MainWindow", "\353\266\210\354\235\274\354\271\230", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\354\225\204\354\235\264\353\224\224", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\353\271\204\353\260\200\353\262\210\355\230\270", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\353\271\204\353\260\200\353\262\210\355\230\270 \355\231\225\354\235\270", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\354\235\264\353\246\204", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\354\240\204\355\231\224\353\262\210\355\230\270", nullptr));
        btn_signup_to_login->setText(QCoreApplication::translate("MainWindow", "\353\222\244\353\241\234\352\260\200\352\270\260", nullptr));
        btn_signup->setText(QCoreApplication::translate("MainWindow", "\355\232\214\354\233\220\352\260\200\354\236\205", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
