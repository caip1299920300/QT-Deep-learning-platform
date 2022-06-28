#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include "yolo_detection.h"

namespace Ui {
class communication;
}

class communication : public QDialog
{
    Q_OBJECT

public:
    explicit communication(QWidget *parent = nullptr);
    ~communication();

private slots:
    void on_listenbt_clicked();
    void new_client();
    void read_data();

    void on_sendbt_clicked();
    void on_sendimages_clicked();
    void on_Return_clicked();

    void on_sendallbt_clicked();

private:
    Ui::communication *ui;
    QTcpServer myserver;
//    QList < QTcpSocket *> sockets;
    QString flag;// 判断是否需要发送“loading image.......”

};

#endif // COMMUNICATION_H
