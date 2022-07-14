#ifndef ADDGROUP_H
#define ADDGROUP_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class AddGroup;
}

class AddGroup : public QWidget
{
    Q_OBJECT

public:
    explicit AddGroup(QTcpSocket *, QString , QWidget *parent = nullptr);
    ~AddGroup();

private slots:
    void on_cancelButton_clicked();

    void on_addButton_clicked();

private:
    Ui::AddGroup *ui;
    QTcpSocket *socket;
    QString m_name;
};

#endif // ADDGROUP_H
