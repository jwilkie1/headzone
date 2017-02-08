#include "mainwindow.h"
#include "socketclient.h"
#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QThread socketThread;
    SocketClient sc;
    sc.moveToThread(&socketThread);
    QObject::connect(&socketThread, SIGNAL(started()), &sc, SLOT(init()));
    QObject::connect(&sc, SIGNAL(finished()), &socketThread, SLOT(quit()));
    QObject::connect(&socketThread, SIGNAL(finished()), &socketThread, SLOT(deleteLater()));
    socketThread.start();

    MainWindow w;
    QObject::connect(&w, SIGNAL(finished()), &socketThread, SLOT(quit()));
    w.show();

    return a.exec();
}
