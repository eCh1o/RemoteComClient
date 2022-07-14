#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class addFriend;
}

class addFriend : public QWidget
{
    Q_OBJECT

public:
    explicit addFriend(QTcpSocket *, QString, QWidget *parent = nullptr);
    ~addFriend();

private slots:
    void on_cancelButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::addFriend *ui;
    QTcpSocket *socket;
    QString m_username;
};

#endif // ADDFRIEND_H
