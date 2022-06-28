#include "introduction.h"
#include "ui_introduction.h"
// 导入“聊天”和“识别”的头文件
#include "communication.h"
#include "yolo_detection.h"
#include "logindialog.h"
#include "tcpclient.h"
#include <QToolButton>

extern QString user;
Introduction::Introduction(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Introduction)
{
    ui->setupUi(this);

    // 正确
    //dateEdit->setDisplayFormat("yyyy/MM/dd");
    //timeEdit->setDisplayFormat("HH:mm:ss");

    // 设置日期时间
    ui->timeEdit->setDateTime(QDateTime::currentDateTime());
}

Introduction::~Introduction()
{
    delete ui;
}
// 退出建
void Introduction::on_Return_clicked()
{
    // 生成一个登陆窗口
    logindialog* log;
    log = new logindialog();
    // 关闭这个窗口
    this->close();
    // 运行登陆窗口
    log->exec();
}

// 识别
void Introduction::on_detection_clicked()
{
    // 生成一个识别窗口
    yolo_detection* yolo;
    yolo = new yolo_detection();
    // 关闭这个窗口
    this->close();
    // 运行识别窗口
    yolo->exec();
}
// 交流平台
void Introduction::on_communication_clicked()
{
    // 登陆的是普通用户
    if(user=="putongyonghu")
    {
        // 生成一个普通用户的交流窗口
        TcpClient* comm1;
        comm1 = new TcpClient();
        // 关闭本窗口
        this->close();
        // 运行普通用户交流窗口
        comm1->show();
    }
    else if(user=="manage")
    {
        QMessageBox::information(this, "提示：", "管理员，欢迎你");
        // 生成一个管理员交流窗口
        communication* comm;
        comm = new communication();
        // 关闭本窗口
        this->close();
        // 运行管理员交流窗口
        comm->exec();
    }
}
