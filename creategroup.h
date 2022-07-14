#ifndef CREATEGROUP_H
#define CREATEGROUP_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class createGroup;
}

class createGroup : public QWidget
{
    Q_OBJECT

public:
    explicit createGroup(QTcpSocket *, QString, QWidget *parent = nullptr);
    ~createGroup();

private slots:
    void on_cancelButton_clicked();

    void on_createButton_clicked();

private:
    Ui::createGroup *ui;
    QString m_name;
    QTcpSocket *socket;
};

#endif // CREATEGROUP_H
