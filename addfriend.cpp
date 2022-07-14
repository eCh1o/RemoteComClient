#include "addfriend.h"
#include "ui_addfriend.h"

addFriend::addFriend(QTcpSocket *s, QString username, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addFriend)
{
    ui->setupUi(this);
    this->socket = s;
    m_username = username;
}

addFriend::~addFriend()
{
    delete ui;
}

void addFriend::on_cancelButton_clicked()
{
    this->close();
}

void addFriend::on_pushButton_2_clicked()
{
    QString friendname = ui->lineEdit->text();
    QJsonObject obj;
    obj.insert("cmd","add");
    obj.insert("user",m_username);
    obj.insert("friend",friendname);

    QByteArray ba = QJsonDocument(obj).toJson();
    socket->write(ba);

    this->close();
}
