#ifndef GROUPCHAT_H
#define GROUPCHAT_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>

class chatlist;
struct groupWidgetInfo;
#include <chatlist.h>

namespace Ui {
class groupChat;
}

class groupChat : public QWidget
{
    Q_OBJECT

public:
    explicit groupChat(QTcpSocket *, QString, QString, chatlist *, QList<groupWidgetInfo> *, QWidget *parent = nullptr);
    ~groupChat();

private slots:
    void show_group_member(QJsonObject);

    void on_sendButton_clicked();

    void show_group_text(QJsonObject);

    void closeEvent(QCloseEvent *);

private:
    Ui::groupChat *ui;
    QTcpSocket *socket;
    QString m_username;
    QString m_groupname;
    chatlist *mainWidget;
    QList<groupWidgetInfo> *groupWidgetList;
};

#endif // GROUPCHAT_H
