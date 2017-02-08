#include "socketclient.h"
#include <QDebug>

SocketClient::SocketClient(QObject *parent) : QObject(parent)
    ,hostConnection(false)
{

}

SocketClient::~SocketClient()
{
    emit finished();
}

void SocketClient::init()
{
   qDebug() << "starting socket client";
   udpSocket = new QUdpSocket(this);
   udpSocket->bind(8889);
   connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingUdp()));
   // broadcast the discovery message to find the server
   // TODO: will need to do this on a timed loop
   udpSocket->writeDatagram("HeadZone?",QHostAddress::Broadcast, 8888);

}

void SocketClient::readPendingUdp()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                              &sender, &senderPort);
        qDebug() << "new server udp message from" << sender.toString() << senderPort;
        processUdpMsg(QString(datagram),sender);
    }
}

void SocketClient::processUdpMsg(QString msg, QHostAddress sender)
{
    qint16 port;
    qDebug() << "message content" << msg;
    // check if it from one of our clients
    if (!msg.contains("HeadZoneClient")) {
        qDebug() << "unknown UDP message:" << msg;
        return;
    }
    QStringList m = msg.split(":");
    if (m.size() != 2) {
        qDebug() << "malformed udp response from server:" << msg;
        return;
    }
    port = m[1].toInt();
    qDebug() << "Got server response, my port is" << port;
    // now connect to the host at given port
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(tcpSocketConnected()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readServerSocket()));
    tcpSocket->connectToHost(sender,port);

}

void SocketClient::tcpSocketConnected()
{
    qDebug() << "tcpSocket now connected to HeadZone server";
    hostConnection = true;
    // ask what headphone channels are available
    tcpSocket->write("HPChannels?");
}

void SocketClient::readServerSocket()
{
    QString msg = tcpSocket->readAll();
    qDebug() << "got data from server socket" << msg;
    if (msg.contains("AvailHP")) {
        QStringList hps = msg.split(":");
        // TODO: will need to have user choose this
        hpId = hps[1];
        qDebug() << "choosing headphone channel:" << hpId;
        tcpSocket->write("SetHP:"+hpId.toLatin1());
    }
}
