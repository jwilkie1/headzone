#ifndef SOCKETPROCESS_H
#define SOCKETPROCESS_H

#include <QObject>
#include <QUdpSocket>

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

private:
    void processUdpMsg(QString msg);

    QUdpSocket* udpSocket;
};

#endif // SOCKETPROCESS_H
