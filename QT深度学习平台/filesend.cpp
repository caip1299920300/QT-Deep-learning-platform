#include "filesend.h"
#include "ui_filesend.h"

#include "communication.h"

#include <QFileDialog>
#include <QMessageBox>
extern QList < QTcpSocket *> sockets;

FileSend::FileSend(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileSend)
{
    ui->setupUi(this);
    //当客户端连接成功会发送connected信号， 当客户端掉线会发送disconnected信号
    connect(&msocket, &QTcpSocket::connected, this, &FileSend::send_file_head);

    //当套接字发送完毕会发送一个信号bytesWr1itten
    connect(&msocket, &QTcpSocket::bytesWritten, this, &FileSend::send_file_text);
    foreach (QTcpSocket * s, sockets)
    {
        if(s->peerAddress().toString().remove("::ffff:")=="")continue;
        ui->ipBox->addItem(s->peerAddress().toString().remove("::ffff:"));
    }
}

FileSend::~FileSend()
{
    delete ui;
}

// 选择本地文件进行发送
void FileSend::on_selectBt_clicked()
{
    //通过文件对话框获取文件路径
    QString filepath = QFileDialog::getOpenFileName(this);
    ui->fileEdit->setText(filepath);
}

// 连接需要发送的用户
void FileSend::on_sendBt_clicked()
{
    //连接服务器
    msocket.connectToHost(ui->ipBox->currentText(), 50000);
    //发送文件信息
    qDebug()<<"connect";
    //初始化
    filesize = 0;
    sendsize = 0;
}

//发送文件头信息
void FileSend::send_file_head()
{
    //发送文件名，文件大小
    QFileInfo info(ui->fileEdit->text());//文件信息对象
    filename = info.fileName();
    filesize = info.size();

    QByteArray array; //空间
    //把array与数据流绑定
    QDataStream stream(&array, QIODevice::WriteOnly);
    stream<<filesize<<filename;
    //设置进度条最大值
    ui->progressBar->setMaximum(filesize);
    //打开文件准备读取数据发送
    file.setFileName(ui->fileEdit->text());
    file.open(QIODevice::ReadOnly);
    //发送
    msocket.write(array);
}

// 通过TCP协议发送文件
void FileSend::send_file_text()
{
    if(sendsize < filesize)
    {
        QByteArray array = file.read(1024*10);//读取一段内容
        msocket.write(array);//发送一段内容
        sendsize += array.size();
        //设置进度条
        ui->progressBar->setValue(sendsize);
    }
    if(sendsize == filesize)
    {
        file.close();//关闭文件
    }
    msocket.disconnectFromHost();
}

// 返回聊天界面
void FileSend::on_fanhui_clicked()
{
    this->parentWidget()->show();
    this->close();
}

// 批量发送
void FileSend::on_sendallBt_clicked()
{
    QMessageBox::information(NULL, "提醒", "功能还在开发中...");
//    foreach (QTcpSocket * s, sockets)
//    {

//        if(s->peerAddress().toString().remove("::ffff:")!="")
//        {
//            QMessageBox::information(NULL, "Title", s->peerAddress().toString().remove("::ffff:"));
//            //连接服务器
//            msocket.connectToHost(s->peerAddress().toString().remove("::ffff:"), 50000);
//            //发送文件信息
//            qDebug()<<"connect";
//            //初始化
//            filesize = 0;
//            sendsize = 0;
//        }

//    }
}
