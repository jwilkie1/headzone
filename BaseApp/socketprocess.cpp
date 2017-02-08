#include "socketprocess.h"
#include <QStringList>

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
    if (udpSocket->bind(8888))
        connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingUdp()));
    else
        qDebug() << "failed to bind udpsocket to port 8888";
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
        qDebug() << "new udp message from" << sender.toString() << senderPort << currentClientAddress;
        if (sender != currentClientAddress) {
            currentClientAddress = sender;
            processUdpMsg(QString(datagram),sender);
        }
        else {
            qDebug() << "ignoring udp message, client is already active";
        }
    }
}

void SocketProcess::processUdpMsg(QString msg, QHostAddress sender)
{
    qDebug() << "message content" << msg;
    // check if it from one of our clients
    if (msg != "HeadZone?") {
        qDebug() << "unknown UDP message:" << msg;
        return;
    }
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newTcpConnection()));
    tcpServer->listen(QHostAddress::Any,10000);
    QByteArray response = "HeadZoneClient:"+QByteArray::number(10000);
    udpSocket->writeDatagram(response, sender, 8889);


}

void SocketProcess::newTcpConnection()
{
    tcpSocket = tcpServer->nextPendingConnection();
    qDebug() << "established tcp connection for datasocket" << tcpSocket->peerAddress().toString();
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readClientSock()));
    //connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
      //      this, SLOT(errorDataSock(QAbstractSocket::SocketError)));
}

void SocketProcess::readClientSock()
{
    QString msg = tcpSocket->readAll();
    qDebug() << "got data from client socket" << tcpSocket->peerPort() << msg;

    if (msg == "HPChannels?") {
        // need to respond with list of avail channels
        tcpSocket->write("AvailHP:HP1:HP2:HP3:HP4");
    }
    else if (msg.contains("SetHP")) {
        QStringList shp = msg.split(":");
        hpId = shp[1];
        // send the mixer settings
        // TODO: need to fetch these settings from the DSP

    }
}
