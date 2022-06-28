#include "logindialog.h"
#include "tcpclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    logindialog w;
    w.show();
    return a.exec();
}
