#ifndef INTRODUCTION_H
#define INTRODUCTION_H

#include <QDialog>

namespace Ui {
class Introduction;
}

class Introduction : public QDialog
{
    Q_OBJECT

public:
    explicit Introduction(QWidget *parent = nullptr);
    ~Introduction();

private slots:
    void on_Return_clicked();
    void on_detection_clicked();
    void on_communication_clicked();

private:
    Ui::Introduction *ui;
};

#endif // INTRODUCTION_H
