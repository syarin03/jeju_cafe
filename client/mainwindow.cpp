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


QString MainWindow::make_html(QString &coordx, QString &coordy, QString &maplevel)
{
    QString html =
        R"(<!DOCTYPE html>
            <html>
            <head>
            <meta charset="utf-8"/>
            <title></title>
            </head>
            <body style="margin:0;">
            <div id="map" style="width:500px;height:500px; margin:0;"></div>
            <div style="display: none;">
                <p id="centerX"></p>
                <p id="centerY"></p>
                <p id="swX"></p>
                <p id="swY"></p>
                <p id="neX"></p>
                <p id="neY"></p>
                <p id="level"></p>
            </div>
            <script type="text/javascript" src="//dapi.kakao.com/v2/maps/sdk.js?appkey=26e625ac275a309101517f4bb67157e2&libraries=services"></script>
            <script>
                var container = document.getElementById('map');
                var options = {center: new kakao.maps.LatLng)";
    html += "(" + coordx + "," + coordy + "),";
    html += "level: " + maplevel + "};";
    html +=
            R"(
                var map = new kakao.maps.Map(container, options);
                var level = map.getLevel();
                var center = map.getCenter();
                var bounds = map.getBounds();
                var swLatLng = bounds.getSouthWest();
                var neLatLng = bounds.getNorthEast();
                var boundsStr = bounds.toString();
                var info = [center.getLat(), center.getLng(),
                            swLatLng.getLat(), swLatLng.getLng(),
                            neLatLng.getLat(), neLatLng.getLng(),
                            level];

                function getInfo(level, center, bounds, swLatLng, neLatLng, boundsStr) {
                level = map.getLevel();
                center = map.getCenter();
                bounds = map.getBounds();
                swLatLng = bounds.getSouthWest();
                neLatLng = bounds.getNorthEast();
                boundsStr = bounds.toString();
                info = [center.getLat(), center.getLng(),
                            swLatLng.getLat(), swLatLng.getLng(),
                            neLatLng.getLat(), neLatLng.getLng(),
                            level];
                return info;
                }
                kakao.maps.event.addListener(map, 'center_changed', function () {
                    var info = getInfo(level, center, bounds, swLatLng, neLatLng, boundsStr)
                    document.getElementById("centerX").textContent = info[0];
                    document.getElementById("centerY").textContent = info[1];
                    document.getElementById("swX").textContent = info[2];
                    document.getElementById("swY").textContent = info[3];
                    document.getElementById("neX").textContent = info[4];
                    document.getElementById("neY").textContent = info[5];
                    document.getElementById("level").textContent = info[6];
                });
            </script>
        </body>
        </html>)";
    return html;
}


void MainWindow::write(QString filename)
{
    QFile file(filename);
    // Trying to open in WriteOnly and Text mode
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << " Could not open file for writing";
        return;
    }

    // To write text, we use operator<<(),
    // which is overloaded to take
    // a QTextStream on the left
    // and data types (including QString) on the right
    QTextStream out(&file);
    out << make_html(coordx, coordy, level);
    file.flush();
    file.close();
}


void MainWindow::read(QString filename)
{
    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "@ open failed";
        return;
    }
    QTextStream in(&file);
    QString myText = in.readAll();
    qDebug() << myText;
    file.close();
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

    jejuCity.insert("건입동", "126.5445896,33.5170777");
    jejuCity.insert("구좌읍", "126.8520445,33.5225711");
    jejuCity.insert("노형동", "126.8520445,33.5225711");
    jejuCity.insert("도두동", "126.4772312,33.483098");
    jejuCity.insert("봉개동", "126.5949431,33.4914049");
    jejuCity.insert("삼도1동", "126.5182829,33.5036792");
    jejuCity.insert("삼도2동", "126.5221714,33.5118173");
    jejuCity.insert("삼양동", "126.5859702,33.5216232");
    jejuCity.insert("아라동", "126.5450031,33.4763498");
    jejuCity.insert("애월읍", "126.3295244,33.4619478");
    jejuCity.insert("연동", "126.4968963,33.4881257");
    jejuCity.insert("오라동", "126.5116727,33.4951531");
    jejuCity.insert("외도동", "126.4322912,33.4931521");
    jejuCity.insert("용담1동", "126.5137552,33.5095955");
    jejuCity.insert("용담2동", "126.5117554,33.5114842");
    jejuCity.insert("우도면", "126.953361,33.5064911");
    jejuCity.insert("이도1동", "126.5255881,33.5084288");
    jejuCity.insert("이도2동", "126.5353375,33.4969863");
    jejuCity.insert("이호동", "126.4580386,33.4996238");
    jejuCity.insert("일도1동", "126.5265039,33.515012");
    jejuCity.insert("일도2동", "126.5383639,33.5115126");
    jejuCity.insert("조천읍", "126.6342455,33.5344076");
    jejuCity.insert("추자면", "126.2961287,33.9636825");
    jejuCity.insert("한경면", "126.184217,33.3501173");
    jejuCity.insert("한림읍", "126.2671535,33.4103985");
    jejuCity.insert("화북동", "126.5655282,33.520234");

    seoguipoCity.insert("남원읍", "126.7207056,33.2799141");
    seoguipoCity.insert("대륜동", "126.5115113,33.2481539");
    seoguipoCity.insert("대정읍", "126.2522254,33.2268151");
    seoguipoCity.insert("대천동", "126.4775984,33.2507361");
    seoguipoCity.insert("동홍동", "126.5671727,33.2663197");
    seoguipoCity.insert("서홍동", "126.5507293,33.2639577");
    seoguipoCity.insert("성산읍", "126.8767558,33.3810625");
    seoguipoCity.insert("송산동", "126.5660903,33.2450722");
    seoguipoCity.insert("안덕면", "126.3371482,33.2496801");
    seoguipoCity.insert("영천동", "126.5869758,33.2687917");
    seoguipoCity.insert("예래동", "126.3979667,33.2542904");
    seoguipoCity.insert("정방동", "126.5655063,33.24646");
    seoguipoCity.insert("중문동", "126.4348243,33.2514303");
    seoguipoCity.insert("중앙동", "126.5651453,33.2507375");
    seoguipoCity.insert("천지동", "126.5612847,33.2479047");
    seoguipoCity.insert("표선면", "126.831638,33.326494");
    seoguipoCity.insert("효돈동", "126.6155853,33.2629589");

    // 콤보박스 초기화
    now = ui->location1->currentText();
    set_combobox();

    // 중심좌표, 지도레벨 초기화 (제주시 건입동)
    coordx = "33.5170777";
    coordy = "126.5445896";
    level = "3";

    // 맵 데이터 초기화
    mapData.insert("centerX", "-");
    mapData.insert("centerY", "-");
    mapData.insert("swX", "-");
    mapData.insert("swY", "-");
    mapData.insert("neX", "-");
    mapData.insert("neY", "-");
    mapData.insert("level", "-");

    // 창을 열자마자 웹 엔진이 보이는 구 코드
    // QString filename = "C:/web_110/kakaomap.html";
    // write(filename);

    // QwebEngineView에 setUrl(QUrl), 로컬의 html파일을 가져온다.
    // ui->map->setUrl(QUrl("http://localhost/myweb/kakaomap.html"));

    // 지도 숨기기
    ui->mapbox->hide();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::set_combobox()
{
    if (now == "제주시") {
        foreach(QString key, jejuCity.keys()){
            ui->location2->addItem(key);
        }
    }
    else if (now == "서귀포시") {
        foreach(QString key, seoguipoCity.keys()){
            ui->location2->addItem(key);
        }
    }
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


void MainWindow::on_map_view_btn_clicked()
{
    QString select = ui->location2->currentText();
    if (now == "제주시") {
        QString point = jejuCity.value(select);
        // 지도 재생성
        coordx = point.split(',')[1];
        coordy = point.split(',')[0];
        qDebug() << coordx << 'and' << coordy;

    }
    else if (now == "서귀포시") {
        QString point = seoguipoCity.value(select);

        // 좌표 설정
        coordx = point.split(',')[1];
        coordy = point.split(',')[0];
        qDebug() << coordx << 'and' << coordy;

    }

    QString filename = "C:/web_110/kakaomap.html";
    write(filename);

    // QwebEngineView에 setUrl(QUrl), 로컬의 html파일을 가져온다.
    ui->map->setUrl(QUrl("http://localhost/myweb/kakaomap.html"));
    ui->mapbox->show();
}


void MainWindow::on_btn_info_to_main_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->stack_main);
}


void MainWindow::on_btn_map_to_main_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->stack_main);
}


void MainWindow::on_btn_main_to_map_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->stack_map);
}


void MainWindow::on_btn_main_to_info_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->stack_info);
}


void MainWindow::on_send_btn_clicked()
{
    // .pro에 QT+= webenginecore 추가, #include <QWebEnginePage>
    QWebEnginePage *page = ui->map->page();
    QString html;

    // 페이지의 HTML 코드 수집
    page->toHtml([&](const QString& html){
        // 코드를 파싱하여 원하는 값을 추출
        for(int i = 0; i < 7; i++){
                        // 정규식과 match를 이용해 문자열을 추출하여 mapData에 키와 값을 넣는 코드
            QRegularExpressionMatch match = regexes[i].match(html);
            if(match.hasMatch()){
                QString key = regexes[i].pattern().split("\"")[1];
                QString value = match.captured(1);
                mapData.insert(key, value);
            }
        }
    // ui에 표시
    ui->centerX->setText(mapData["centerX"]);
    ui->centerY->setText(mapData["centerY"]);
    ui->swX->setText(mapData["swX"]);
    ui->swY->setText(mapData["swY"]);
    ui->neX->setText(mapData["neX"]);
    ui->neY->setText(mapData["neY"]);
    ui->Level->setText(mapData["level"]);

    });

    qDebug() << mapData;

    // toHtml() 함수가 비동기 함수이므로, 위 코드가 바로 실행되지 않는다.
    // 대신, toHtml() 함수가 완료될 때까지 기다린 후에 다음 코드를 실행
    // qApp->processEvents() 현재 대기 중인 이벤트를 즉시 처리
    // 일부 이벤트를 처리하지 않은 채로 GUI가 응답하지 않게 되는 문제를 해결
    qApp->processEvents();
}


void MainWindow::on_location1_currentTextChanged(const QString &arg1)
{
    now = ui->location1->currentText();
    qDebug() << now;
    ui->location2->clear();
    set_combobox();
}

