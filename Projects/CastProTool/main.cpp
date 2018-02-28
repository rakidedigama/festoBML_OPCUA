#include "mainwindow.h"
#include <QApplication>
#include "stdio.h"
#include "iostream"
#include "fstream"

using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Write to log

    ofstream logFile("log.txt");
    streambuf *coutbuf = cin.rdbuf();
    cout.rdbuf(logFile.rdbuf());
    cout<<"Start App" <<endl;

    MainWindow w;
    w.show();


    return a.exec();
}
