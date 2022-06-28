#include "tcpclient.h"
#include "ui_tcpclient.h"

#include <QHostAddress>
#include <QDebug>
#include <QMessageBox>
#include "filerecv.h"
#include "introduction.h"

TcpClient::TcpClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    //如果对方(服务器)发来数据，mysocket对象会发出readyRead信号，关联槽函数
    connect(&mysocket,&QTcpSocket::readyRead,this,&TcpClient::read_data);

    ////辅助功能////
    //字体
    connect(ui->fontCbx,&QFontComboBox::currentFontChanged,[=](const QFont &font){
        ui->textEdit->setCurrentFont(font);
        ui->textEdit->setFocus();
    });
    //字号
    void(QComboBox::*cbxsingal)(const QString &text) = &QComboBox::currentIndexChanged;
    connect(ui->sizeCbx,cbxsingal,[=](const QString &text){

        ui->textEdit->setFontPointSize(text.toDouble());
        ui->textEdit->setFocus();
    });
}

TcpClient::~TcpClient()
{
    delete ui;
}

// 读取传输过来的文字
void TcpClient::read_data()
{
//    qDebug()<<"666666666666";
    QString msg= mysocket.readAll(); // 读写信息
    QString ip = mysocket.peerAddress().toString(); // 获取发送方的ip
    //ui->textBrowser->append(ip+"-> "+msg); // 打印ip+内容
    ui->textBrowser->append(msg); // 打印ip+内容
    if(msg=="welcome..."){
        QMessageBox::information(NULL, "提醒", "连接成功！");
    }
    if(msg=="uploading image.......") // 如果接受到的语句是loading.......则表示接受文件
    {
//        mysocket.disconnect();
        filerecv *filer; // 定义一个filerecv窗口
        filer = new filerecv(this); // 分配空间
        this->hide(); // 隐藏本窗口
        filer->show(); // 显示filerecv窗口
    }
}



// 连接
void TcpClient::on_connectbt_clicked()
{
    mysocket.connectToHost(ui->iple->text(),ui->portle->text().toUShort());
}


// 发送信息
void TcpClient::on_sendbt_clicked()
{
    QString str = ui->textEdit->toPlainText();
    mysocket.write(str.toUtf8());
}


// 清空输入的信息
void TcpClient::on_clearbt_clicked()
{
    ui->textEdit->clear();
}

// 返回按钮
void TcpClient::on_fanhui_clicked()
{
    Introduction* com;
    com = new Introduction();
    this->close();
    com->exec();
}
