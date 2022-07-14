#include "addgroup.h"
#include "ui_addgroup.h"

AddGroup::AddGroup(QTcpSocket *s, QString username,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddGroup)
{
    ui->setupUi(this);
    socket = s;
    m_name = username;
}

AddGroup::~AddGroup()
{
    delete ui;
}

void AddGroup::on_cancelButton_clicked()
{
    this->close();
}

void AddGroup::on_addButton_clicked()
{
    QString groupName = ui->lineEdit->text();
    QJsonObject obj;
    obj.insert("cmd","add_group");
    obj.insert("user",m_name);
    obj.insert("group",groupName);
    QByteArray ba = QJsonDocument(obj).toJson();
    socket->write(ba);
    this->close();
}
