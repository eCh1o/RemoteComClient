#include "sendthrad.h"

sendthrad::sendthrad(QJsonObject obj)
{
    m_port = obj.value("port").toInt();
    m_filename = obj.value("filename").toString();
    m_filelength = obj.value("length").toInt();
}

void sendthrad::run()
{
    QTcpSocket sendsocket;
    sendsocket.connectToHost(QHostAddress("192.168.229.129"), m_port);
    if(!sendsocket.waitForConnected(10000))
    {
        this->quit();
    }
    else
    {
        QFile file(m_filename);
        file.open(QIODevice::ReadOnly);
        while(1)
        {
            QByteArray ba = file.read(1024 * 1024);
            if(ba.size() == 0)
            {
                break;
            }
            sendsocket.write(ba);
            sendsocket.flush();
            usleep(2000000);
        }
        file.close();
        sendsocket.close();
        this->quit();
    }
}
