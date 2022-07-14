#include "creategroup.h"
#include "ui_creategroup.h"

createGroup::createGroup(QTcpSocket *s, QString name,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::createGroup)
{
    ui->setupUi(this);
    m_name = name;
    socket = s;
}

createGroup::~createGroup()
{
    delete ui;
}

void createGroup::on_cancelButton_clicked()
{
    this->close();
}

void createGroup::on_createButton_clicked()
{
    QString groupname = ui->lineEdit->text();
    QJsonObject obj;
    obj.insert("cmd","create_group");
    obj.insert("user",m_name);
    obj.insert("group",groupname);
    QByteArray ba = QJsonDocument(obj).toJson();
    socket->write(ba);
    this->close();
}
