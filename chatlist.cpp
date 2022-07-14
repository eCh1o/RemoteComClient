#include "chatlist.h"
#include "ui_chatlist.h"

chatlist::chatlist(QTcpSocket *fd, QString fri, QString group, QString username, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatlist)
{
    ui->setupUi(this);
    m_username = username;
    socket = fd;
    connect(socket,&QTcpSocket::readyRead, this, &chatlist::server_reply);

    QStringList frilist = fri.split('|');
    for(int i = 0; i<frilist.size();i++)
    {
        if(frilist.at(i) != "")
        {
            ui->friendlist->addItem(frilist.at(i));
        }
    }
    QStringList grolist = group.split('|');
    for(int i = 0;i<grolist.size();i++)
    {
        if(grolist.at(i) != "")
        {
            ui->grouplist->addItem(grolist.at(i));
        }
    }
    connect(ui->friendlist, &QListWidget::itemDoubleClicked,this, &chatlist::on_friendList_doubleclicked);
    connect(ui->grouplist, &QListWidget::itemDoubleClicked,this,&chatlist::on_groupList_doubleclicked);
}

chatlist::~chatlist()
{
    delete ui;
}

void chatlist::server_reply()
{
    QByteArray ba = socket->readAll();
    QJsonObject obj = QJsonDocument::fromJson(ba).object();
    QString cmd = obj.value("cmd").toString();
    if(cmd == "friend_login")
    {
        client_login_reply(obj.value("friend").toString());
    }
    else if(cmd == "add_reply")
    {
        client_add_friend_reply(obj);
    }
    else if(cmd == "add_friend_reply")
    {
        QString str = QString("%1把你添加为好友").arg(obj.value("result").toString());
        QMessageBox::information(this,"好友添加提醒",str);
        ui->friendlist->addItem(obj.value("result").toString());
    }
    else if(cmd == "create_group_reply")
    {
        client_create_group_reply(obj);
    }
    else if(cmd == "add_group_reply")
    {
        client_add_group_reply(obj);
    }
    else if(cmd == "private_chat_reply")
    {
        client_private_chat_reply(obj.value("result").toString());
    }
    else if(cmd == "private_chat")
    {
        client_chat_reply(obj);
    }
    else if(cmd == "get_group_member_reply")
    {
        client_get_group_member_reply(obj);
    }
    else if(cmd == "group_chat")
    {
        client_group_chat_reply(obj);
    }
    else if(cmd == "send_file_reply")
    {
        client_send_file_reply(obj.value("result").toString());
    }
    else if(cmd == "send_file_port_reply")
    {
        client_send_file_port_reply(obj);
    }
    else if(cmd == "recv_file_port_reply")
    {
        client_recv_file_port_reply(obj);
    }
    else if(cmd == "friend_offline")
    {
        client_friend_offline(obj.value("friend").toString());
    }
}

void chatlist::client_login_reply(QString fri)
{
    QString str = QString("%1好友上线").arg(fri);
    QMessageBox::information(this,"好友上线提醒", str);
}

void chatlist::client_create_group_reply(QJsonObject &obj)
{
    if(obj.value("result").toString() == "group_exist")
    {
        QMessageBox::warning(this,"创建群提示","该群已经存在");
    }
    else if(obj.value("result").toString() == "success")
    {
        ui->grouplist->addItem(obj.value("group").toString());
    }
}

void chatlist::on_addButton_clicked()
{
    addFriend *addFriWidget = new addFriend(socket,m_username);
    addFriWidget->show();
}

void chatlist::client_add_friend_reply(QJsonObject &obj)
{
    if(obj.value("result").toString() == "user_not_exist")
    {
        QMessageBox::warning(this,"添加好友提醒","好友不存在");
    }
    else if(obj.value("result").toString() == "already_friend")
    {
        QMessageBox::warning(this,"添加好友提醒","已经是好友");
    }
    else if(obj.value("result").toString() == "success")
    {
        QMessageBox::information(this,"添加好友提醒","好友添加成功");
        ui->friendlist->addItem(obj.value("friend").toString());
    }
}

void chatlist::client_add_group_reply(QJsonObject &obj)
{
    if(obj.value("result").toString() == "group_not_exist")
    {
        QMessageBox::warning(this,"添加群提示","群不存在");
    }
    else if(obj.value("result").toString() == "user_in_group")
    {
        QMessageBox::warning(this,"添加群提示","已在群里面");
    }
    else if(obj.value("result").toString() == "success")
    {
        ui->grouplist->addItem(obj.value("group").toString());
    }
}

void chatlist::client_private_chat_reply(QString result)
{
    if(result == "offline")
    {
        QMessageBox::warning(this,"发送提醒","对方不在线");
    }
}

void chatlist::client_chat_reply(QJsonObject &obj)
{
    int flag = 0;
    for(int i = 0; i<chatWidgetList.size(); i++)
    {
        if(chatWidgetList.at(i).name == obj.value("user_from").toString())
        {
            flag = 1;
            break;
        }
    }
    if(flag == 0) //聊天窗口没有打开
    {
        privateChat *privateChatWidget = new privateChat(socket, m_username, obj.value("user_from").toString(),this,&chatWidgetList);
        privateChatWidget->setWindowTitle(obj.value("user_from").toString());
        privateChatWidget->show();
        ChatWidgetInfo c = {privateChatWidget, obj.value("user_from").toString()};
        chatWidgetList.push_back(c);
    }
    emit signal_to_sub_widget(obj);
}

void chatlist::client_get_group_member_reply(QJsonObject obj)
{
    emit signal_to_sub_widget_member(obj);
}

void chatlist::client_group_chat_reply(QJsonObject obj)
{
    int flag = 0;
    for(int i = 0; i<groupWidgetList.size(); i++)
    {
        if(groupWidgetList.at(i).name == obj.value("group").toString())
        {
            flag = 1;
            break;
        }
    }
    if(flag == 0)
    {
        QString groupname = obj.value("group").toString();
        groupChat *groupChatWidget = new groupChat(socket, groupname, m_username, this, &groupWidgetList);
        groupChatWidget->setWindowTitle(groupname);
        groupChatWidget->show();
        groupWidgetInfo g = {groupChatWidget, groupname};
        groupWidgetList.push_back(g);
    }
    emit signal_to_sub_widget_group(obj);
}

void chatlist::client_send_file_reply(QString res)
{
    if(res == "offline")
    {
        QMessageBox::warning(this,"发送文件提醒","对方不在线");
    }
    else if(res == "timeout")
    {
        QMessageBox::warning(this,"发送文件提醒","连接超时");
    }
}

void chatlist::client_send_file_port_reply(QJsonObject obj)
{
    sendthrad *mysendThread = new sendthrad(obj);
    mysendThread->start();
}

void chatlist::client_recv_file_port_reply(QJsonObject obj)
{
    recvthread *myrecvthread = new recvthread(obj);
    myrecvthread->start();
}

void chatlist::client_friend_offline(QString fri)
{
    QString str = QString("%1下线").arg(fri);
    QMessageBox::information(this,"下线提醒",str);
}

void chatlist::on_creategroupButton_2_clicked()
{
    createGroup *createGroupWidgt = new createGroup(socket, m_username);
    createGroupWidgt->show();
}

void chatlist::on_addgroupButton_3_clicked()
{
    AddGroup *addGroupWidget = new AddGroup(socket,m_username);
    addGroupWidget->show();
}

void chatlist::on_friendList_doubleclicked()
{
    QString friendName = ui->friendlist->currentItem()->text();
    privateChat *privateChatWidget = new privateChat(socket, m_username, friendName, this,&chatWidgetList);
    privateChatWidget->setWindowTitle(friendName);
    privateChatWidget->show();
    ChatWidgetInfo c = {privateChatWidget, friendName};
    chatWidgetList.push_back(c);
}

void chatlist::on_groupList_doubleclicked()
{
    QString groupname = ui->grouplist->currentItem()->text();
    groupChat *groupChatWidget = new groupChat(socket, groupname, m_username, this, &groupWidgetList);
    groupChatWidget->setWindowTitle(groupname);
    groupChatWidget->show();
    groupWidgetInfo g = {groupChatWidget, groupname};
    groupWidgetList.push_back(g);
}

void chatlist::closeEvent(QCloseEvent *event)
{
    QJsonObject obj;
    obj.insert("cmd","offline");
    obj.insert("user",m_username);
    QByteArray ba = QJsonDocument(obj).toJson();
    socket->write(ba);
    socket->flush();

    event->accept();
}


