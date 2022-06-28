#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "introduction.h"

QT_BEGIN_NAMESPACE
namespace Ui { class logindialog; }
QT_END_NAMESPACE

class logindialog : public QDialog
{
    Q_OBJECT

public:
    logindialog(QWidget *parent = nullptr);
    ~logindialog();


private slots:

    void on_zuce_clicked();
    void on_denglu_clicked();

private:
    Ui::logindialog *ui;
    QString iszuce="no";

};
#endif // LOGINDIALOG_H
