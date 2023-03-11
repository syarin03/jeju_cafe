#include <QtCore/QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlTableModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    int n = 0;

    QCoreApplication a(argc, argv);
    QList<QTcpSocket*> clientSockets;

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("10.10.21.110");
    db.setDatabaseName("1team_db");
    db.setUserName("jeju_cafe");
    db.setPassword("xlavmfhwprxm1");

    if (!db.open())
    {
        qDebug() << "Database connection error";
        return 1;
    }
    else
    {
        qDebug() << "Database connection success";
    }

    QTcpServer server;
    if (!server.listen(QHostAddress::Any, 9001))
    {
        qCritical() << "Server could not start";
        return 1;
    }
    else
    {
        qDebug() << "Server started, listening on" << server.serverAddress().toString() << ":" << server.serverPort();
    }

    QObject::connect(&server, &QTcpServer::newConnection, [&server,&clientSockets,&n]()
    {
        QTcpSocket* clientSocket = server.nextPendingConnection();
        qDebug() << "New client connected from" << clientSocket->peerAddress().toString() << ":" << clientSocket->peerPort();
        clientSockets.append(clientSocket);

        QObject::connect(clientSocket, &QTcpSocket::readyRead, [clientSocket,&clientSockets,&n]()
        {
            QByteArray data = clientSocket->readAll();
            qDebug() << "Data received from client:" << data << clientSocket;
            QJsonDocument doc = QJsonDocument::fromJson(data);
            QJsonObject obj = doc.object();
            QString schemaName = "1team_db";

            string method = obj.value("method").toString().toStdString();
            cout << method << endl;

            if (method == "login")
            {
                QString input_id = obj.value("input_id").toString();
                QString input_pw = obj.value("input_pw").toString();

                QString tableName = "member";
                QSqlQuery query;
                QString sql = QString("SELECT * FROM %1.%2 WHERE uid = '%3' and upw = '%4';")
                        .arg(schemaName, tableName, input_id, input_pw);
                cout << sql.toStdString() << endl;

                obj["method"] = "login_result";

                if (query.exec(sql))
                {
                    if (query.size() == 0)
                    {
                        obj["result"] = false;
                    }
                    else
                    {
                        obj["result"] = true;
                        query.next();
                        obj["user_num"] = query.value(0).toInt();
                        obj["user_id"] = query.value(1).toString();
                        obj["user_pw"] = query.value(2).toString();
                        obj["user_name"] = query.value(3).toString();
                        obj["user_phone"] = query.value(4).toString();
                    }
                    cout << obj["result"].toBool() << endl;
                }
                else
                {
                    qDebug() << "Failed to execute query";
                }
            }
            else if (method == "check_id")
            {
                QString input_id = obj.value("input_id").toString();

                QString tableName = "member";
                QSqlQuery query;
                QString sql = QString("SELECT * FROM %1.%2 WHERE uid = '%3';")
                        .arg(schemaName, tableName, input_id);
                cout << sql.toStdString() << endl;

                obj["method"] = "check_id_result";
                if (query.exec(sql))
                {
                    if (query.size() == 0)
                    {
                        obj["result"] = true;
                    }
                    else
                    {
                        obj["result"] = false;
                    }
                    cout << obj["result"].toBool() << endl;
                }
                else
                {
                    qDebug() << "Failed to execute query";
                }
            }
            else if (method == "signup")
            {
                QString input_id = obj.value("input_id").toString();
                QString input_pw = obj.value("input_pw").toString();
                QString input_name = obj.value("input_name").toString();
                QString input_phone = obj.value("input_phone").toString();

                QString tableName = "member";
                QSqlQuery query;

                QString sql = QString("INSERT INTO %1.%2 (uid, upw, uname, phone) VALUES ('%3', '%4', '%5', '%6');")
                        .arg(schemaName, tableName, input_id, input_pw, input_name, input_phone);
                cout << sql.toStdString() << endl;

                obj["method"] = "signup_result";

                if (query.exec(sql))
                {
                    qDebug() << "insert success";
                    obj["result"] = true;
                }
                else
                {
                    qCritical() << "insert failed";
                    obj["result"] = false;
                }
                cout << obj["result"].toBool() << endl;
            }
            else if (method == "map_info")
            {
                QString user_num = QString::number(obj.value("user_num").toInt());
                QString centerX = obj.value("centerX").toString();
                QString centerY = obj.value("centerY").toString();
                QString datetime = obj.value("send_time").toString();


                QString tableName = "history";
                QSqlQuery query;

                QString sql = QString("INSERT INTO %1.%2 (user_num, center_x, center_y, datetime) VALUES ('%3', '%4', '%5', '%6');")
                        .arg(schemaName, tableName, user_num, centerX, centerY, datetime);
                cout << sql.toStdString() << endl;

                obj["method"] = "map_info_result";

                if (query.exec(sql))
                {
                    qDebug() << "insert success";
                    obj["result"] = true;
                }
                else
                {
                    qCritical() << "insert failed";
                    obj["result"] = false;
                }
            }

            while (1)
            {
                if (clientSockets.at(n) == clientSocket)
                {
                    qDebug() << "yes";
                    QJsonDocument jsonDoc(obj);
                    QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);
                    clientSockets.at(n)->write(jsonData);
                    n = 0;
                    break;
                }
                else
                    n++;
            }
        });

        QObject::connect(clientSocket, &QTcpSocket::disconnected, [clientSocket]()
        {
            qDebug() << "Client disconnected from" << clientSocket->peerAddress().toString() << ":" << clientSocket->peerPort();
            clientSocket->deleteLater();
        });
    });

    return a.exec();
}
