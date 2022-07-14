#include "recvthread.h"

recvthread::recvthread(QJsonObject obj)
{
    m_totle = 0;
    m_port = obj.value("port").toInt();
    m_filelength = obj.value("length").toInt();

    QString pathname = obj.value("filename").toString();
    QStringList strlist = pathname.split('/');
    m_filename = strlist.at(strlist.size() - 1);
}

void recvthread::run()
{
    m_file = new QFile(m_filename);
    m_file->open(QIODevice::WriteOnly);
    recvSocket = new QTcpSocket;
    connect(recvSocket, &QTcpSocket::readyRead, this, &recvthread::recv_file, Qt::DirectConnection);
    recvSocket->connectToHost(QHostAddress("192.168.229.129"), m_port);
    if(!recvSocket->waitForConnected(10000))
    {
        this->quit();
    }
    else
    {

    }
    exec();
}

void recvthread::recv_file()
{
    QByteArray ba = recvSocket->readAll();
    m_totle += ba.size();
    m_file->write(ba);
    if(m_totle >= m_filelength)
    {
        m_file->close();
        recvSocket->close();
        delete m_file;
        delete recvSocket;
        this->quit();
    }
}
