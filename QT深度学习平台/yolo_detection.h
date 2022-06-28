#ifndef YOLO_DETECTION_H
#define YOLO_DETECTION_H

#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QDialog>
#include <QCamera>
#include <QDateTime>
#include <QMessageBox>

namespace Ui {
class yolo_detection;
}

class yolo_detection : public QDialog
{
    Q_OBJECT

public:
    explicit yolo_detection(QWidget *parent = nullptr);
    ~yolo_detection();

private slots:
    // 返回深度学习介绍界面
    void on_Return_clicked();
    // 识别按钮
    void on_yolo_detection_2_clicked();
    // 上传图片函数
    void on_load_image_clicked();
    // 上传视频
    void on_load_movie_clicked();
    // 摄像头捕抓图片
    void on_shexiangtou_clicked();
    void on_photographBt_clicked();
    // 拍照按钮
    void take_photo(int id, const QImage &image);

private:
    Ui::yolo_detection *ui;
    QImage* image; // 需要识别的图片

    //摄像头对象指针
    QCamera* Camera;
    //摄像头的取景器
    QCameraViewfinder* CameraViewFinder;
    //存储摄像机设备信息
    QList<QCameraInfo> CameraInfoList;
    //记录摄像头内容
    QCameraImageCapture* CameraImageCapture;
    //照片的名字
    QString ImageName;
    QString img_path; // 用于存储图片存放的位置
    // 判断是否开摄像头
    QString isopen_camera;

};

#endif // YOLO_DETECTION_H
