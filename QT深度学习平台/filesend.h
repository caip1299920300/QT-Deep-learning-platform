#ifndef FILESEND_H
#define FILESEND_H

#include <QDialog>
#include <QFile>
#include <QTcpSocket>


QT_BEGIN_NAMESPACE
namespace Ui { class FileSend; }
QT_END_NAMESPACE

class FileSend : public QDialog
{
    Q_OBJECT


protected slots:
    void on_selectBt_clicked();
    void on_sendBt_clicked();
    void send_file_head();
    void send_file_text();

private slots:
    void on_fanhui_clicked();
    void on_sendallBt_clicked();

public:
    explicit FileSend(QWidget *parent = nullptr);
    ~FileSend();

private:
    Ui::FileSend *ui;
    QFile file;
    QString filename;
    quint64 filesize;
    quint64 sendsize;
    QTcpSocket msocket;
};

#endif // FILESEND_H
