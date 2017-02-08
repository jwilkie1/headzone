#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QTcpServer>

class SocketClient : public QObject
{
    Q_OBJECT
public:
    explicit SocketClient(QObject *parent = 0);
   ~SocketClient();

signals:
   void finished();

public slots:
   void init();

private slots:
   void readPendingUdp();
   void tcpSocketConnected();
   void readServerSocket();

private:
   void processUdpMsg(QString msg, QHostAddress sender);
   QUdpSocket* udpSocket;
   QTcpSocket* tcpSocket;
   bool hostConnection;
   QString hpId;
};

#endif // SOCKETCLIENT_H
