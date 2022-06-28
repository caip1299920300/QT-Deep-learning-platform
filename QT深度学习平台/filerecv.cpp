#include "filerecv.h"
#include "ui_filerecv.h"

#include <QDebug>
#include <QMessageBox>
#include <QTcpSocket>
#include <sys/time.h>

filerecv::filerecv(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::filerecv)
{
    ui->setupUi(this);
    filesize = 0;
    recvsize = 0;
    //关联客户端连接信号
    connect(&mserver, &QTcpServer::newConnection, this, &filerecv::new_client);
    //启动服务器
    mserver.listen(QHostAddress::Any,50000);
}

filerecv::~filerecv()
{
    delete ui;
}

// 文件接受信号函数
void filerecv::new_client()
{
    filesize = 0; // 文件大小
    recvsize = 0; // 接收到的大小
    //创建与客户端通信的套接字
    QTcpSocket *msocket = mserver.nextPendingConnection();
    //关联读数据信号readyRead
    connect(msocket, &QTcpSocket::readyRead, this, &filerecv::read_data);
}
// 读取传过来文件的数据
void filerecv::read_data()
{
    QTcpSocket *msocket = dynamic_cast<QTcpSocket*>(sender());
    if(filesize == 0) //表达第一次读取数据--读文件信息
    {
        QByteArray array = msocket->readAll();
        QDataStream stream(&array, QIODevice::ReadOnly);//把套接字与数据流绑定
        stream>>filesize>>filename;//获取文件大小， 文件名

        //设置进度条最大值
        ui->progressBar->setMaximum(filesize);
        qDebug()<<filename;
        //打开文件
        file.setFileName(filename);
        file.open(QIODevice::WriteOnly);

        //显示进度条
        this->show();
    }

    //读文件内容
    if(recvsize <  filesize)
    {
        //读取一段写一段
        QByteArray array = msocket->readAll();
        file.write(array);
        recvsize += array.size();
        //更新进度条
        ui->progressBar->setValue(recvsize);
    }

    if(recvsize == filesize)
    {
        //读完，关闭文件
        file.close();
        //关闭套接字
        msocket->disconnectFromHost();
    }
}

// 确认是否接受到文件
void filerecv::on_yess_clicked()
{
//    QMessageBox::information(NULL, "Title", "Content");
//    this->hide();
    this->close(); // 关闭这个窗口
    this->parentWidget()->show(); //显示父窗口
}
