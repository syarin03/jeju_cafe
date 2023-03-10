#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>
#include <QStringList>
#include <QRegularExpression>


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
    void on_send_btn_clicked();

    void on_map_view_btn_clicked();

    void on_location1_currentTextChanged(const QString &arg1);

private:
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
//    QList <QMap<QString, QString>> nowCity;
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

    // ui 초기화 함수들
    void set_combobox();

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
