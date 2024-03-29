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
#include <QList>
#include <QStringList>
#include <fstream>
#include <Qfile>
#include <QHash>
#include <QTimer>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QChartView>


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
    int get_num();
    QString get_uid();
    QString get_upw();
    QString get_uname();
    QString get_phone();
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

    void on_map_view_btn_clicked();

    void on_btn_info_to_main_clicked();

    void on_send_btn_clicked();

    void on_location1_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    QTcpSocket socket;

    User login_user;

    bool isIDCheck = false;
    bool isPWRule = false;
    bool isPWSame = false;

    // html 태그 멤버변수
    QString html;

    // html 생성을 위한 멤버변수
    QString coordx;
    QString coordy;
    QString level;

    // 지역-좌표 멤버변수
    QMap <QString, QString> jejuCity;
    QMap <QString, QString> seoguipoCity;

    // 선택 지역 멤버변수
    QString now;

    // Qt의 딕셔너리 자료형, key-value 쌍으로 이루어짐
    // 좌표와 지도 레벨을 저장하기 위한 맵
    QMap <QString, QString> mapData;

    // html에서 좌표를 가져올 정규식을 저장하기 위한 리스트
    const QRegularExpression regexes[7] = {
            QRegularExpression("<p\\s+id=\"centerX\">([^<]+)</p>"),
            QRegularExpression("<p\\s+id=\"centerY\">([^<]+)</p>"),
            QRegularExpression("<p\\s+id=\"swX\">([^<]+)</p>"),
            QRegularExpression("<p\\s+id=\"swY\">([^<]+)</p>"),
            QRegularExpression("<p\\s+id=\"neX\">([^<]+)</p>"),
            QRegularExpression("<p\\s+id=\"neY\">([^<]+)</p>"),
            QRegularExpression("<p\\s+id=\"level\">([^<]+)</p>")
        };

    // 지도 API html 함수들(생성)
    void write(QString filename);
    void read(QString filename);
    QString make_html(QString &coordx, QString &coordy, QString &level);

    QChartView chartView;

    // ui 초기화 함수들
    void set_combobox();
};
#endif // MAINWINDOW_H
