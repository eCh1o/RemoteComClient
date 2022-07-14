#ifndef CHATLIST_H
#define CHATLIST_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QList>
#include <addfriend.h>
#include <creategroup.h>
#include <addgroup.h>
#include <groupchat.h>
#include <sendthrad.h>
#include <recvthread.h>

class groupChat;
#include <groupchat.h>

class privateChat;
#include <privatechat.h>

namespace Ui {
class chatlist;
}

struct ChatWidgetInfo
{
    privateChat *w;
    QString name;
};

struct groupWidgetInfo
{
    groupChat *w;
    QString name;
};

class chatlist : public QWidget
{
    Q_OBJECT

public:
    explicit chatlist(QTcpSocket *, QString, QString, QString, QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event);
    ~chatlist();

private slots:
    void server_reply();
    void on_addButton_clicked();

    void on_creategroupButton_2_clicked();

    void on_addgroupButton_3_clicked();

    void on_friendList_doubleclicked();

    void on_groupList_doubleclicked();

signals:
    void signal_to_sub_widget(QJsonObject);

    void signal_to_sub_widget_member(QJsonObject);

    void signal_to_sub_widget_group(QJsonObject);

private:
    void client_login_reply(QString);
    void client_add_friend_reply(QJsonObject &);
    void client_create_group_reply(QJsonObject &);
    void client_add_group_reply(QJsonObject &);
    void client_private_chat_reply(QString);
    void client_chat_reply(QJsonObject &);
    void client_get_group_member_reply(QJsonObject);
    void client_group_chat_reply(QJsonObject);
    void client_send_file_reply(QString);
    void client_send_file_port_reply(QJsonObject);
    void client_recv_file_port_reply(QJsonObject);
    void client_friend_offline(QString);
    Ui::chatlist *ui;
    QTcpSocket *socket;
    QString m_username;
    QList<ChatWidgetInfo> chatWidgetList;
    QList<groupWidgetInfo> groupWidgetList;
};

#endif // CHATLIST_H
