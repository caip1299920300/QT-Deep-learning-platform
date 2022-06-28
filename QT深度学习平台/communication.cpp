#include "communication.h"
#include "ui_communication.h"

#include "introduction.h"
#include "filesend.h"

#include <QMessageBox>
#include <QTcpSocket>

QList < QTcpSocket *> sockets;
communication::communication(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::communication)
{
    ui->setupUi(this);
    flag="true";// 判断是否需要发送“loading image.......”
    //关联服务器对象的newConnection信号与槽函数
    connect(&myserver,&QTcpServer::newConnection,this,&communication::new_client);

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

communication::~communication()
{
    delete ui;
}

// 返回
void communication::on_Return_clicked()
{
    // 生成一个介绍窗口
    Introduction* introduction;
    introduction = new Introduction();
    // 关闭这个窗口
    this->close();
    // 运行介绍窗口
    introduction->exec();
}

// 发出TCP信号监听
void communication::on_listenbt_clicked()
{
    myserver.listen(QHostAddress::Any,ui->portle->text().toUShort());
    QMessageBox::information(NULL, "提醒", "信号发送成功！");

}

// 当有连接信号来时，触发下面这个函数
void communication::new_client()
{
    //通过服务器对象QTcpServer获取QTcpSocket对象
    QTcpSocket *mysocket = myserver.nextPendingConnection();
    mysocket->write("welcome..."); // 发送“欢迎”这几个给连接的用户
    //关联QTcpSocket对象的readyRead信号
    connect(mysocket,&QTcpSocket::readyRead,this,&communication::read_data);
    // 读取连接用户的ip
    QString peerip=mysocket->peerAddress().toString();
    peerip.remove("::ffff:");
    qDebug()<<peerip;
    // 添加到下拉列表中显示
    ui->comboBox->addItem(peerip);
    sockets.append(mysocket);
}

// 读取信号发送者发来的信息
void communication::read_data()
{
    //获取信号的发送者
    QTcpSocket *mysocket = dynamic_cast<QTcpSocket *>(sender());
    //通过mysocket读取数据
    QString msg = mysocket->readAll();
    //通过mysocket获取对方的IP
    QString ip = mysocket->peerAddress().toString();
    ip.remove("::ffff:");
    //把msg显示在textBrowser上
    ui->textBrowser->append(ip +"-> "+ msg);
    msg=ip +"-> "+ msg;
    // 接受到的信息全部用户可以显示
    foreach (QTcpSocket * s, sockets)
    {
        // 发送一条数据
        s->write(msg.toUtf8());
    }
}

// 向指定ip的用户发送信息
void communication::on_sendbt_clicked()
{
    QTcpSocket * mysocket; //获取信号的发送者
    foreach (QTcpSocket * s, sockets)
    {
        // 寻找和下拉栏相同ip的信号
        if(s->peerAddress().toString().remove("::ffff:") == ui->comboBox->currentText())
        {
            mysocket = s;
            break;
        }
    }
    // 发送一条数据
    mysocket->write(ui->textEdit->toPlainText().toUtf8());
}


// 批量发信息，向连接本机的所有用户发送信息
void communication::on_sendallbt_clicked()
{
    QTcpSocket * mysocket; //获取信号的发送者
    foreach (QTcpSocket * s, sockets)
    {
        // 发送一条数据
        s->write(ui->textEdit->toPlainText().toUtf8());
    }
}


// 文件分发
void communication::on_sendimages_clicked()
{
//    qDebug()<<ui->comboBox->currentText();
//    emit sendData(ui->comboBox->currentText());  //获取lineEdit的输入并且传递出去
//    globaldata = ui->comboBox->currentText();
//    if(flag=="true") // 只在第一次发送信号
//    {
//        QTcpSocket * mysocket; // 建立信号
//        foreach (QTcpSocket * s, sockets)
//        {
//            if(s->peerAddress().toString().remove("::ffff:") == ui->comboBox->currentText())
//            {
//                mysocket = s;
//                break;
//            }
//            s->write("uploading image.......");
//            flag="flase";
//        }
//        mysocket->write("uploading image.......");
//        flag="flase";
//    }

    QTcpSocket * mysocket; // 建立信号
    foreach (QTcpSocket * s, sockets)
    {
        if(s->peerAddress().toString().remove("::ffff:") == ui->comboBox->currentText())
        {
            mysocket = s;
            break;
        }
    }
    mysocket->write("uploading image.......");

    FileSend *files; // 建立一个FileSend窗口
    files = new FileSend(this); // 分配内存
    this->hide(); // 隐藏本窗口
    files->show(); // 显示FileSend窗口

}
