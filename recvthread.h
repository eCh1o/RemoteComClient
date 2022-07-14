#ifndef RECVTHREAD_H
#define RECVTHREAD_H
#include <QThread>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostAddress>
#include <QFile>

class recvthread : public QThread
{
    Q_OBJECT
public:
    recvthread(QJsonObject);
    void run();

private slots:
    void recv_file();

private:
    QString m_filename;
    int m_filelength;
    int m_totle;
    int m_port;
    QTcpSocket *recvSocket;
    QFile *m_file;
};

#endif // RECVTHREAD_H
