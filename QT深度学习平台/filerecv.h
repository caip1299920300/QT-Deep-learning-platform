#ifndef FILERECV_H
#define FILERECV_H

#include <QDialog>
#include <QFile>
#include <QTcpServer>

namespace Ui {
class filerecv;
}

class filerecv : public QDialog
{
    Q_OBJECT

public:
    explicit filerecv(QWidget *parent = nullptr);
    ~filerecv();

protected slots:
    void read_data();
    void new_client();

private slots:
    void on_yess_clicked();

private:
    Ui::filerecv *ui;
    QTcpServer mserver;
    QFile file;
    QString filename;
    quint64 filesize;
    quint64 recvsize;
};

#endif // FILERECV_H
