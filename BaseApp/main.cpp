#include "BaseWindow.h"
#include "socketprocess.h"
#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QThread socketThread;
    SocketProcess sp;
    sp.moveToThread(&socketThread);
    QObject::connect(&socketThread, SIGNAL(started()), &sp, SLOT(init()));
    QObject::connect(&sp, SIGNAL(finished()), &socketThread, SLOT(quit()));
    QObject::connect(&socketThread, SIGNAL(finished()), &socketThread, SLOT(deleteLater()));
    socketThread.start();

    MainWindow w;
    QObject::connect(&w, SIGNAL(finished()), &socketThread, SLOT(quit()));
    w.show();

    return a.exec();
}
