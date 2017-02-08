#ifndef SOCKETPROCESS_H
#define SOCKETPROCESS_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>

class SocketProcess : public QObject
{
    Q_OBJECT
public:
    explicit SocketProcess(QObject *parent = 0);
    ~SocketProcess();

signals:
    void finished();

public slots:
    void init();

private slots:
    void readPendingUdp();
    void newTcpConnection();
    void readClientSock();

private:
    void processUdpMsg(QString msg, QHostAddress sender);

    QUdpSocket* udpSocket;
    QTcpServer* tcpServer;
    QTcpSocket* tcpSocket;
    QString hpId;
    QHostAddress currentClientAddress;
};

#endif // SOCKETPROCESS_H
