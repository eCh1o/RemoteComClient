#include "privatechat.h"
#include "ui_privatechat.h"

privateChat::privateChat(QTcpSocket *s, QString username, QString friendname, chatlist *c, QList<ChatWidgetInfo> *l, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::privateChat)
{
    ui->setupUi(this);
    socket = s;
    m_username = username;
    m_friendname = friendname;
    mainWidget = c;
    chatWidgetList = l;
    connect(mainWidget, &chatlist::signal_to_sub_widget,this,&privateChat::show_text_slot);
}

privateChat::~privateChat()
{
    delete ui;
}

void privateChat::on_sendButton_clicked()
{
    QString text = ui->lineEdit->text();
    QJsonObject obj;
    obj.insert("cmd","private_chat");
    obj.insert("user_from",m_username);
    obj.insert("user_to",m_friendname);
    obj.insert("text",text);
    QByteArray ba = QJsonDocument(obj).toJson();
    socket->write(ba);
    ui->lineEdit->clear();
    ui->textEdit->append(text);
    ui->textEdit->append("\n");
}

void privateChat::show_text_slot(QJsonObject obj)
{
    if(obj.value("cmd").toString() == "private_chat")
    {
        if(obj.value("user_from").toString() == m_friendname)
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

void privateChat::closeEvent(QCloseEvent *event)
{
    for(int i =0;i< chatWidgetList->size();i++)
    {
        if(chatWidgetList->at(i).name == m_friendname)
        {
            chatWidgetList->removeAt(i);
            break;
        }
    }
    event->accept();
}

void privateChat::on_fileButton_2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "发送文件", QCoreApplication::applicationFilePath());
    if(fileName.isEmpty())
    {
        QMessageBox::warning(this,"发送文件提示","请选择一个文件");
    }
    else
    {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QJsonObject obj;
        obj.insert("cmd","send_file");
        obj.insert("from_user",m_username);
        obj.insert("to_user", m_friendname);
        obj.insert("length",file.size());
        obj.insert("filename",fileName);
        QByteArray ba = QJsonDocument(obj).toJson();
        socket->write(ba);
        file.size();
    }
}
