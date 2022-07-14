#ifndef PRIVATECHAT_H
#define PRIVATECHAT_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QFileDialog>
#include <QCoreApplication>
#include <QFile>

class chatlist;
struct ChatWidgetInfo;
#include <chatlist.h>

namespace Ui {
class privateChat;
}

class privateChat : public QWidget
{
    Q_OBJECT

public:
    explicit privateChat(QTcpSocket *, QString, QString, chatlist *, QList<ChatWidgetInfo> *, QWidget *parent = nullptr);
    ~privateChat();
    void closeEvent(QCloseEvent *event);
private slots:
    void on_sendButton_clicked();
    void show_text_slot(QJsonObject);

    void on_fileButton_2_clicked();

private:
    Ui::privateChat *ui;
    QTcpSocket *socket;
    QString m_username;
    QString m_friendname;
    chatlist *mainWidget;
    QList<ChatWidgetInfo> *chatWidgetList;
};

#endif // PRIVATECHAT_H
