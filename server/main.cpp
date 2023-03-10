#include <QtCore/QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QMainWindow>
//#include <QApplication>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlTableModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    //점수매기기 test
    double x = 126.723442;              //임시xy값
    double y = 33.4534214;
    string command = "C:\\Users\\kiot\\AppData\\Local\\Programs\\Python\\Python39\\python C:\\Users\\kiot\\PycharmProjects\\pythonProject1\\1228.py " + std::to_string(x) + "," + std::to_string(y); //인터프리터경로,파일위치,인자1,인자2
    std::ofstream outfile("C:\\Users\\kiot\\Documents\\GitHub\\jeju_cafe\\server\\output.txt");                                     //해당 경로에 txt파일생성
    system(command.c_str());                                                                                                        //command 경로를 토대로 파일 작동
    std::ifstream infile("C:\\Users\\kiot\\Documents\\GitHub\\jeju_cafe\\server\\output.txt");                                      //해당경로의 파일 읽기시작
    std::string line;                                                                                                               //line 변수
    while (std::getline(infile, line)) {                                                                                            //한줄씩 읽어들임
        std::cout << line << std::endl;
    }

    int n = 0;
    int n2 = 0;

    QCoreApplication a(argc, argv);
    QList<QTcpSocket*> clientSockets; //클라이언트여러개 정보 받을 리스트
    // 1. QTcpServer 인스턴스 생C성
//    QApplication a(argc, argv);

    // 데이터베이스 연결 설정
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");                                                              //mysql db와 연결식
    db.setHostName("10.10.21.110");
    db.setDatabaseName("1team_db");
    db.setUserName("jeju_cafe");
    db.setPassword("xlavmfhwprxm1");


    // 데이터베이스 연결
    if (!db.open()) {
        qDebug() << "Database connection error";
        return 1;
    }
    else {
        qDebug() << "db no error!";
//        QString schemaName = "jeju_cafe";
//           QString tableName = "cafe";
//           QSqlQuery query;
//           QJsonArray list;
//           QJsonArray list2;
//           QString sql = QString("SELECT * FROM `%1`.`%2`").arg(schemaName).arg(tableName);
//           if (query.exec(sql)) {
//               while (query.next())
//               {
//                   int id = query.value(0).toInt();
//                   QString name = query.value(1).toString();
//                   int num = 0;
//                   while (num<8)
//                   {
//                       list.append(query.value(num).toString());
//                       num++;

//                   }
//                   QJsonDocument jsonDoc(list);
//                   list2.append(list);
//                   list2.append("@@@");
//                   QJsonDocument jsonDoc2(list2);
//                   list = QJsonArray();
//               }
//               qDebug() << list2;
//           }
//           else {
//               qDebug() << "Failed to execute query";
//           }
    }

    QTcpServer server;
    // 2. 서버가 클라이언트의 연결을 받을 수 있도록 포트 지정 및 리스닝 시작
    if (!server.listen(QHostAddress::Any, 9001)) {                                                                  //포트넘버를 1234로설정하고 리슨을시작, 리슨 실패시 오류메세지출력, 성공시 아래 else동작
        qCritical() << "Server could not start";                                                                     //오류메세지
        return 1;
    } else {
        qDebug() << "Server started, listening on" << server.serverAddress().toString() << ":" << server.serverPort(); //qDebug=콘솔창에 메세지출력, 서버스타트알림과 서버주소출력함
    }

    // 3. 새로운 연결이 있을 때마다 실행되는 코드
    QObject::connect(&server, &QTcpServer::newConnection, [&server,&clientSockets,&n,&n2](){                                    //클라이언트 연결시마다 작동,따로스레드나 반복문 안걸어도 자동루프함
        QTcpSocket* clientSocket = server.nextPendingConnection();                                                          //연결된 클라이언트 소켓 얻어옴
        qDebug() << "New client connected from" << clientSocket->peerAddress().toString() << ":" << clientSocket->peerPort(); //연결한 클라의 주소와 포트번호 출력
        clientSockets.append(clientSocket);                                                                                  //QList인 clientSockets에 클라소켓 append

        // 4. 연결된 클라이언트와 데이터 송수신
        QObject::connect(clientSocket, &QTcpSocket::readyRead, [clientSocket,&clientSockets,&n,&n2]() {                     //데이터 수신시마다 작동,따로 스레드나 반복문안걸어도 자동루프함
            QByteArray data = clientSocket->readAll();                                                                  //클라소켓으로부터 수신된 데이터 읽어옴
//            qDebug() << "Data received from client:" << data<<clientSocket;                                             //클라이언트로부터온 데이터 출력

            QJsonObject jsonResponse = QJsonDocument::fromJson(data).object();
//            QStringList keys = jsonResponse.keys();
            string method = jsonResponse.value("method").toString().toStdString();
            if ( method=="login")
            {
                QJsonValue mapValue = jsonResponse.operator []("input_id");
                QJsonValue mapValue2 = jsonResponse.operator []("input_pw");
                if (mapValue.isString()) {
                    // 문자열인 경우
                    QString strValue = mapValue.toString();
                    qDebug() << "Response from server id:" << strValue;
                    //여기에 이제 전달식 넣으면 됨
                } else {
                    // 그 외의 경우
                    qCritical() << "Unexpected value for key 'map'";
                }
                if (mapValue2.isString()) {
                    // 문자열인 경우
                    QString strValue = mapValue2.toString();
                    qDebug() << "Response from server pw:" << strValue;
                    //여기에 이제 전달식 넣으면 됨
                } else {
                    // 그 외의 경우
                    qCritical() << "Unexpected value for key 'map'";
                }
            }
            else if ( method=="signup")
            {
                QJsonValue mapValueid = jsonResponse.operator []("input_id");
                QJsonValue mapValuepw = jsonResponse.operator []("input_pw");
                QJsonValue mapValuename = jsonResponse.operator []("input_name");
                QJsonValue mapValuephone = jsonResponse.operator []("input_phone");
                if (mapValueid.isString()) {
                    // 문자열인 경우
                    QString strValue = mapValueid.toString();
                    qDebug() << "Response from server id:" << strValue;
                    //여기에 이제 전달식 넣으면 됨
                } else {
                    // 그 외의 경우
                    qCritical() << "Unexpected value for key 'map'";
                }
                if (mapValuepw.isString()) {
                    // 문자열인 경우
                    QString strValue = mapValuepw.toString();
                    qDebug() << "Response from server pw:" << strValue;
                    //여기에 이제 전달식 넣으면 됨
                } else {
                    // 그 외의 경우
                    qCritical() << "Unexpected value for key 'map'";
                }

                if (mapValuename.isString()) {
                    // 문자열인 경우
                    QString strValue = mapValuename.toString();
                    qDebug() << "Response from server name:" << strValue;
                    //여기에 이제 전달식 넣으면 됨
                } else {
                    // 그 외의 경우
                    qCritical() << "Unexpected value for key 'map'";
                }
                if (mapValuephone.isString()) {
                    // 문자열인 경우
                    QString strValue = mapValuephone.toString();
                    qDebug() << "Response from server phone:" << strValue;
                    //여기에 이제 전달식 넣으면 됨
                } else {
                    // 그 외의 경우
                    qCritical() << "Unexpected value for key 'map'";
                }
               QString schemaName = "1team_db";
               QString tableName = "member";
               QSqlQuery query;
               QJsonArray list;
               QString sql = QString("SELECT * FROM `%1`.`%2`").arg(schemaName).arg(tableName);
               if (query.exec(sql)) {
                    while (query.next())
                    {
                        int num = 0;
                        while (num<5)
                        {
                            list.append(query.value(num).toString());
                            num++;
                        }
                        QJsonDocument jsonDoc(list);
                    }
//                     list = QJsonArray();
                 }
                 else {
                     qDebug() << "Failed to execute query";
                 }
               qDebug() << list;

               //insert형식 테스트
               if(list == QJsonArray())
               {
                   qDebug() << "!!!!!!!!!!!!!!!!!";
                   QSqlQuery query;
                   QString queryString = "INSERT INTO " + tableName + " (uid, upw, uname, phone) "
                                          "VALUES (:val1, :val2, :val3, :val4)";
                   query.prepare(queryString);
                   query.bindValue(":val1", mapValueid);
                   query.bindValue(":val2", mapValuepw);
                   query.bindValue(":val3", mapValuename);
                   query.bindValue(":val4", mapValuephone);

                   if (!query.exec()) {
                       qDebug() << "Error: Failed to insert data into table.";
                   }
               }
            }

            QJsonObject jsonObject;                                                                                     //            QJsonObject jsonObject;      //json형 object생성
            while(1)                                                                                                    //무한반복문,n을 1씩증가시키면서 데이터를 보낸 클라이언트의 소켓을 만날때까지 반복함
            {
                if (clientSockets.at(n)==clientSocket){                                                                 //현재 데이터를 보낸 클라이언트와 n번소켓이같을때
                    qDebug()<<"yes";                                                                                    //if문 작동확인 메세지
                    jsonObject ["map"]= 1;
                    QJsonDocument jsonDoc(jsonObject);
                    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);
                    clientSockets.at(n)->write(jsonData);                                                              //n번 클라이언트에 메세지 전달
                    n=0;                                                                                                //n을 0으로
                    break;                                                                                              //반복종료
                }
                else
                    n++;                                                                                                //반복할때마다 n증가
            }
        });

        // 5. 클라이언트가 연결을 종료할 때마다 실행
        QObject::connect(clientSocket, &QTcpSocket::disconnected, [clientSocket]() {
            qDebug() << "Client disconnected from" << clientSocket->peerAddress().toString() << ":" << clientSocket->peerPort();
            clientSocket->deleteLater();
        });
    });

    return a.exec();
}
