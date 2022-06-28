#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
class TcpClient;
}

class TcpClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit TcpClient(QWidget *parent = nullptr);
    ~TcpClient();

private slots:
    void on_connectbt_clicked();

    void on_sendbt_clicked();

    void on_clearbt_clicked();

    void read_data();

    void on_fanhui_clicked();

private:
    Ui::TcpClient *ui;
    QTcpSocket mysocket;

};

#endif // TCPCLIENT_H
