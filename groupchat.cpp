#include "groupchat.h"
#include "ui_groupchat.h"

groupChat::groupChat(QTcpSocket *s, QString groupname, QString username, chatlist *c, QList<groupWidgetInfo> *l, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::groupChat)
{
    ui->setupUi(this);
    socket = s;
    m_groupname = groupname;
    m_username = username;
    mainWidget = c;
    groupWidgetList = l;

    QJsonObject obj;
    obj.insert("cmd","get_group_member");
    obj.insert("group",m_groupname);
    QByteArray ba = QJsonDocument(obj).toJson();
    socket->write(ba);

    connect(mainWidget, &chatlist::signal_to_sub_widget_member, this, &groupChat::show_group_member);
    connect(mainWidget, &chatlist::signal_to_sub_widget_group, this, &groupChat::show_group_text);
}

groupChat::~groupChat()
{
    delete ui;
}

void groupChat::show_group_member(QJsonObject obj)
{
    if (obj.value("cmd").toString() == "get_group_member_reply")
    {
        if(obj.value("group").toString() == m_groupname)
        {
            QStringList strList = obj.value("member").toString().split("|");
            for(int i =0; i< strList.size();i++)
            {
                ui->listWidget->addItem(strList.at(i));
            }
        }
    }
}

void groupChat::on_sendButton_clicked()
{
    QString text = ui->lineEdit->text();
    QJsonObject obj;
    obj.insert("cmd","group_chat");
    obj.insert("user",m_username);
    obj.insert("group",m_groupname);
    obj.insert("text",text);
    QByteArray ba = QJsonDocument(obj).toJson();
    socket->write(ba);
    ui->lineEdit->clear();
    ui->textEdit->append(text);
    ui->textEdit->append("\n");
}

void groupChat::show_group_text(QJsonObject obj)
{
    if(obj.value("cmd").toString() == "group_chat")
    {
        if(obj.value("group").toString() == m_groupname)
        {
            if(this->isMinimized())
            {
                this->showNormal();
            }
            this->activateWindow();
            ui->textEdit->append(obj.value("text").toString());
            ui->textEdit->append("\n");
        }
    }
}

void groupChat::closeEvent(QCloseEvent *event)
{
    for(int i = 0; i<groupWidgetList->size(); i++)
    {
        if(groupWidgetList->at(i).name == m_groupname)
        {
            groupWidgetList->removeAt(i);
        }
    }
    event->accept();
}
