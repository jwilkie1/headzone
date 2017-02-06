#include "socketprocess.h"

SocketProcess::SocketProcess(QObject *parent) : QObject(parent)
{

}

SocketProcess::~SocketProcess()
{
    emit finished();
}

void SocketProcess::init()
{
    udpSocket = new QUdpSocket(this);
    // listen for any udp messages on port 8888
    udpSocket->bind(8888);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingUdp()));

}

void SocketProcess::readPendingUdp()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                              &sender, &senderPort);
        qDebug() << "new udp message from" << sender.toString() << senderPort;
        processUdpMsg(QString(datagram));
    }
}

void SocketProcess::processUdpMsg(QString msg)
{
    qDebug() << "message content" << msg;
}
