#include "logindialog.h"
#include "ui_logindialog.h"

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QMessageBox>

QString user="putongyonghu";

logindialog::logindialog(QWidget *parent)
     : QDialog(parent)
    , ui(new Ui::logindialog)
{
    ui->setupUi(this);
    // 建立一个数据库
    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE","loaded");
   // 给数据库设置名字
    mydb.setDatabaseName("load.db");
    // 判断数据库是否建立
    if(!mydb.open())
    {
        qDebug()<<"open fail";
    }
    else
    {
        QString  sql = QString("create table load(username char(32) PRIMARY KEY,password char(64))");
        QSqlQuery query(QSqlDatabase::database("loaded"));
        query.exec(sql);
    }
}

logindialog::~logindialog()
{
    delete ui;
}


// 登陆界面
void logindialog::on_denglu_clicked()
{
    // 获取组件的用户名和密码
    QString username = ui->m_usernameEdit->text();
    QString password = ui->m_passwordEdit->text();
    if(username=="")
    {
        //账号不能为空！
        QPushButton *okbtn=new QPushButton(QObject::tr("确定"));
        QMessageBox *mymsgbox=new QMessageBox;
        mymsgbox->addButton(okbtn,QMessageBox::AcceptRole);
        mymsgbox->setWindowTitle("提示框！");
        mymsgbox->setIcon(QMessageBox::Critical);
        mymsgbox->setText("账号不能为空！");
        mymsgbox->show();
    }
    else if(password=="")
    {
        // 密码不能为空!
        QPushButton *okbtn=new QPushButton(QObject::tr("确定"));
        QMessageBox *mymsgbox=new QMessageBox;
        mymsgbox->addButton(okbtn,QMessageBox::AcceptRole);
        mymsgbox->setWindowTitle("提示框！");
        mymsgbox->setIcon(QMessageBox::Warning);
        mymsgbox->setText("密码不能为空！");
        mymsgbox->show();
    }
    else
    {
        // 连接数据库
        QSqlQuery query(QSqlDatabase::database("loaded"));
        //构建创建数据库的sql语句字符串
        QString sql = QString("select password from load where username='%1'").arg(username);
        //  执行sql语句
        if(!query.exec(sql))
            qDebug()<<"查询失败！";
        if(!query.next())
        {
//            qDebug()<<"不存在该用户！";
            QPushButton *okbtn=new QPushButton(QObject::tr("确定"));
            QMessageBox *mymsgbox=new QMessageBox;
            mymsgbox->addButton(okbtn,QMessageBox::AcceptRole);
            mymsgbox->setWindowTitle("提示框！");
            mymsgbox->setIcon(QMessageBox::Critical);
            mymsgbox->setText("不存在该用户！");
            mymsgbox->show();
        }
        else
        {
            QString old_password = query.value(0).toString();
        //  qDebug()<<query.value(0).toString();
            // 对密码进行加密
            QCryptographicHash hash(QCryptographicHash::Md5);
            hash.addData(password.toUtf8());
            QString hash_password=hash.result().toHex();
            // 对比是否是所需要的密码
            if(old_password==hash_password)
            {
                // 判断是否为管理者登陆
                if(ui->m_usernameEdit->text()=="wcpeng"){
                    user="manage";
                }
                qDebug()<<"登陆中...";
//                QPushButton *okbtn=new QPushButton(QObject::tr("确定"));
//                QMessageBox *mymsgbox=new QMessageBox;
//                mymsgbox->addButton(okbtn,QMessageBox::AcceptRole);
//                mymsgbox->setWindowTitle("提示框！");
//                mymsgbox->setIcon(QMessageBox::Information);
//                mymsgbox->setText("登陆中...");
//                mymsgbox->show();
//                delete mymsgbox;
                // 构建“介绍”的那个窗口
                Introduction* newmin;
                // 进入另外一个窗口
                newmin = new Introduction();
                // 关闭本窗口
                this->close();
                newmin->exec();
            }
            else
            {
//                qDebug()<<"密码输入错误，请重新输入！";
                QPushButton *okbtn=new QPushButton(QObject::tr("确定"));
                QMessageBox *mymsgbox=new QMessageBox;
                mymsgbox->addButton(okbtn,QMessageBox::AcceptRole);
                mymsgbox->setWindowTitle("提示框！");
                mymsgbox->setIcon(QMessageBox::Warning);
                mymsgbox->setText("密码输入错误，请重新输入！");
                mymsgbox->show();
            }
        }
    }

}

// 注册界面
void logindialog::on_zuce_clicked()
{
    if(iszuce=="no")// 用于显示输入管理员登陆
    {
        // 申请注册
        QPushButton *okbtn=new QPushButton(QObject::tr("确定"));
        QMessageBox *mymsgbox=new QMessageBox;
        mymsgbox->addButton(okbtn,QMessageBox::AcceptRole);
        mymsgbox->setWindowTitle("提示框！");
        mymsgbox->setIcon(QMessageBox::Information);
        mymsgbox->setText("请输入管理者账号密码，按确定");
        mymsgbox->show();
        // 按钮部件名字改为确定
        ui->zuce->setText("确定");
        iszuce="panduan"; // 切换判断管理员账号是否正确
    }
    else if(iszuce=="panduan")
    {
        // 管理员账号密码输入匹配
        if(ui->m_usernameEdit->text()=="wcpeng" && ui->m_passwordEdit->text()=="wcpeng123")
        {
            // 管理员账号登陆成功
            iszuce="chenggong";
            ui->zuce->setText("注册");
            // 清空输入信息
            ui->m_usernameEdit->clear();
            ui->m_passwordEdit->clear();
            // 提醒可以注册了
            QPushButton *okbtn=new QPushButton(QObject::tr("确定"));
            QMessageBox *mymsgbox=new QMessageBox;
            mymsgbox->addButton(okbtn,QMessageBox::AcceptRole);
            mymsgbox->setWindowTitle("提示框！");
            mymsgbox->setIcon(QMessageBox::Information);
            mymsgbox->setText("管理账号密码输入正确，请输入信息，按注册");
            mymsgbox->show();
        }
        else
        {// 提醒管理员账号或密码输入错误
            QPushButton *okbtn=new QPushButton(QObject::tr("确定"));
            QMessageBox *mymsgbox=new QMessageBox;
            mymsgbox->addButton(okbtn,QMessageBox::AcceptRole);
            mymsgbox->setWindowTitle("提示框！");
            mymsgbox->setIcon(QMessageBox::Information);
            mymsgbox->setText("输入有误，请重新输入");
            mymsgbox->show();
        }
    }
    else
    {// 如果走过了前面两个流程，则开始注册
        // 获取组件的用户名和密码
        QString username = ui->m_usernameEdit->text();
        QString password = ui->m_passwordEdit->text();
        if(username=="")
        {
            // 账号不能为空
            QPushButton *okbtn=new QPushButton(QObject::tr("确定"));
            QMessageBox *mymsgbox=new QMessageBox;
            mymsgbox->addButton(okbtn,QMessageBox::AcceptRole);
            mymsgbox->setWindowTitle("提示框！");
            mymsgbox->setIcon(QMessageBox::Critical);
            mymsgbox->setText("账号不能为空！");
            mymsgbox->show();
        }
        else if(password=="")
        {
            // 密码不能为空
            QPushButton *okbtn=new QPushButton(QObject::tr("确定"));
            QMessageBox *mymsgbox=new QMessageBox;
            mymsgbox->addButton(okbtn,QMessageBox::AcceptRole);
            mymsgbox->setWindowTitle("提示框！");
            mymsgbox->setIcon(QMessageBox::Critical);
            mymsgbox->setText("密码不能为空！");
            mymsgbox->show();
        }
        else
        {
            QSqlQuery query(QSqlDatabase::database("loaded"));
            //构建创建数据库的sql语句字符串
            QString sql = QString("select password from load where username='%1'").arg(username);
            //  执行sql语句
            if(!query.exec(sql))
                qDebug()<<"查询失败！";
            if(query.next())
            {// 读取数据库，通过返回值是否为空，判断是否已存在数据库
                QPushButton *okbtn=new QPushButton(QObject::tr("确定"));
                QMessageBox *mymsgbox=new QMessageBox;
                mymsgbox->addButton(okbtn,QMessageBox::AcceptRole);
                mymsgbox->setWindowTitle("提示框！");
                mymsgbox->setIcon(QMessageBox::Critical);
                mymsgbox->setText("用户已存在，请重新输入！");
                mymsgbox->show();
            }
            else
            {
                // 对密码进行加密
                QCryptographicHash hash(QCryptographicHash::Md5);
                hash.addData(password.toUtf8());
                QString hash_password=hash.result().toHex();
                // 插入数据库
                QString  sql = QString("insert into load values('%1','%2')")
                        .arg(username).arg(hash_password);
                // 执行插入语句
                QSqlQuery query(QSqlDatabase::database("loaded"));
                if(!query.exec(sql))
                {
                    //注册失败
                    QPushButton *okbtn=new QPushButton(QObject::tr("确定"));
                    QMessageBox *mymsgbox=new QMessageBox;
                    mymsgbox->addButton(okbtn,QMessageBox::AcceptRole);
                    mymsgbox->setWindowTitle("提示框！");
                    mymsgbox->setIcon(QMessageBox::Warning);
                    mymsgbox->setText("注册失败");
                    mymsgbox->show();
                }
                else
                {
                    // 注册成功
                    QPushButton *okbtn=new QPushButton(QObject::tr("确定"));
                    QMessageBox *mymsgbox=new QMessageBox;
                    mymsgbox->addButton(okbtn,QMessageBox::AcceptRole);
                    mymsgbox->setWindowTitle("提示框！");
                    mymsgbox->setIcon(QMessageBox::Information);
                    mymsgbox->setText("注册成功！");
                    mymsgbox->show();
                }

            }

        }
    }

}

