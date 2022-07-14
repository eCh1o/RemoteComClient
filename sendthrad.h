#ifndef SENDTHRAD_H
#define SENDTHRAD_H
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QThread>
#include <QHostAddress>
#include <QFile>

class sendthrad : public QThread
{
public:
    sendthrad(QJsonObject);
    void run();

private:
    QString m_filename;
    int m_filelength;
    int m_port;
};

#endif // SENDTHRAD_H
