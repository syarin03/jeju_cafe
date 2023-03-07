#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    cout << "Program Execute" << endl << endl;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
