#include "yolo_detection.h"
#include "ui_yolo_detection.h"

#include "introduction.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMediaPlaylist>

#include <QCoreApplication>
#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/dnn.hpp>
#include <QDebug>
#include<fstream>
using namespace std;
using namespace cv;
using namespace cv::dnn;


yolo_detection::yolo_detection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::yolo_detection)
{
    ui->setupUi(this);
    // 判断是否开摄像头
    isopen_camera="flase";
    // 隐藏拍照按钮
    ui->photographBt->hide();

    //若连接到多个摄像头设备，获取可使用的摄像头信息
    CameraInfoList = QCameraInfo::availableCameras();
    //遍历，将摄像头信息逐个添加到下拉列表当中
    QList<QCameraInfo>::const_iterator it;
    for(it=CameraInfoList.begin(); it!=CameraInfoList.end(); it++)
    {
        ui->deviceComboBox->addItem((*it).description());
    }
}

yolo_detection::~yolo_detection()
{
    delete ui;
}


// 返回
void yolo_detection::on_Return_clicked()
{
    // 生成一个聊天窗口
    Introduction* introduction;
    introduction = new Introduction();
    // 关闭这个窗口
    this->close();
    // 运行聊天窗口
    introduction->exec();
}

// 识别按钮
void yolo_detection::on_yolo_detection_2_clicked()
{

    // -----------------------------------加载图片--------------------------------------
    //预处理读取的图像，并将图像读入网络
//    Mat frame = imread("/opt/project/Login/1.jpg");
    // 把QString类型转换为imread可以读取的格式
    Mat frame = imread(img_path.toLocal8Bit().data());
//    imshow("dog", frame);


    //---------------------------------------加载类别---------------------------------------
    ifstream classNamesFile("/opt/project/Login/model/coco.names");

    vector<string> classNamesVec;
    if (classNamesFile.is_open())
    {
        string className = "";
        while (std::getline(classNamesFile, className))
            classNamesVec.push_back(className);
    }
//    for (int i = 0; i < classNamesVec.size(); i++) {
//        cout << i + 1 << "\t" << classNamesVec[i].c_str() << endl;
//    }
    //---------------------------------------模型设置---------------------------------------

    String cfg = "/opt/project/Login/model/yolov4.cfg";
    String weight = "/opt/project/Login/model/yolov4.weights";
//    cout<<cfg<<weight<<endl;
    //模型读入
    Net net = readNetFromDarknet(cfg, weight);

    Mat inputBlob = blobFromImage(frame, 1.0 / 255, Size(608, 608), Scalar());
    net.setInput(inputBlob);

    //获取未连接输出层
    std::vector<String> outNames = net.getUnconnectedOutLayersNames();
//    for (int i = 0; i < outNames.size(); i++) {
//        cout << "output layer name : " << outNames[i].c_str() << endl;
//    }
    std::vector<Mat> outs;
    net.forward(outs, outNames);
    //---------------------------------------目标检测---------------------------------------
    //需要的变量
    float* data;
    Mat scores;

    vector<Rect> boxes;
    vector<int> classIds;
    vector<float> confidences;
    int centerX, centerY, width, height, left, top;
    float confidenceThreshold = 0.2;
    double confidence;
    Point classIdPoint;

    //找出所有的目标及其位置
    for (size_t i = 0; i<outs.size(); ++i){
        data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols){
            scores = outs[i].row(j).colRange(5, outs[i].cols);
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > confidenceThreshold){
                centerX = (int)(data[0] * frame.cols);
                centerY = (int)(data[1] * frame.rows);
                width = (int)(data[2] * frame.cols);
                height = (int)(data[3] * frame.rows);
                left = centerX - width / 2;
                top = centerY - height / 2;

                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(Rect(left, top, width, height));
            }
        }
    }

    vector<int> indices;
    NMSBoxes(boxes, confidences, 0.3, 0.2, indices);
    //---------------------------------------效果展示---------------------------------------

    Scalar rectColor, textColor; //box 和 text 的颜色
    Rect box, textBox;
    int idx;
    String className="";
//    String classNames="图片中有：\n";
    Size labelSize;

    int nums[81]={0}; // 用于统计图片物体的个数
    for (size_t i = 0; i < indices.size(); ++i) {
        idx = indices[i];

        // 累计每个类的个数
        nums[classIds[idx]]+=1; // classIds[idx]表示每个种类的下标
//        if(classNames.find(classNamesVec[classIds[idx]]) == string::npos) // 如果名字不存在，则加入进去
//            classNames += classNamesVec[classIds[idx]]+"；";
//        ui->classes->setText(QString::fromStdString(classNames));

        className=classNamesVec[classIds[idx]]; // 返回识别到类型
        labelSize = getTextSize(className, FONT_HERSHEY_SIMPLEX, 0.5, 1, 0);
        box = boxes[idx];
        // 绘制预测框
        textBox = Rect(Point(box.x - 1, box.y),
                       Point(box.x + labelSize.width, box.y - labelSize.height));
        rectColor = Scalar(idx * 11 % 256, idx * 22 % 256, idx * 33 % 256);
        textColor = Scalar(255 - idx * 11 % 256, 255 - idx * 22 % 256, 255 - idx * 33 % 256);

        rectangle(frame, box, rectColor, 2, 8, 0);
        rectangle(frame, textBox, rectColor, -1, 8, 0);
        // 绘制每个预测框的种类名
        putText(frame, className.c_str(), Point(box.x, box.y - 2), FONT_HERSHEY_SIMPLEX, 0.5, textColor, 1, 8);
    }

//    imshow("[OpenCV-YOLO]", frame);
//    waitKey(0);

    // 将图片转为rgb格式，用下下面格式转化
    cvtColor(frame,frame,cv::COLOR_BGR2RGB);
    QImage image(frame.data,
                frame.cols,
                frame.rows,
                frame.step,
                QImage::Format_RGB888);
    ui->new_image->setPixmap(QPixmap::fromImage(image.rgbSwapped()));
    //图像缩放
    QImage imgScaled =image.scaled(ui->new_image->width(),
                           ui->new_image->height(),
                           Qt::KeepAspectRatio);
    //显示图片
    ui->new_image->setPixmap(QPixmap::fromImage(imgScaled));
    this->show();

    String classesName="图片中有：\n";
    for (int j=0;j<81;j++) {
        if(nums[j]!=0){
            classesName+=classNamesVec[j]+std::to_string(nums[j])+"个；";
        }
    }
    // 显示图片中的物体以及个数
    ui->classes->setText(QString::fromStdString(classesName));

}
// 上传相片
void yolo_detection::on_load_image_clicked()
{
    ui->load_image->setText("重新上传图片");
    img_path = QFileDialog::getOpenFileName(this->parentWidget(),tr("文件对话框"),
                                                    "D:",tr("图片文件(*png *jpg)"));
    qDebug()<<"img_path:"<<img_path; // 显示图片的位置
    image=new QImage;
    if(!(image->load(img_path))) //加载图像
    {
        QMessageBox::information(this,
                                 tr("打开图像失败"),
                                 tr("打开图像失败!"));
        delete image;
        return;
    }
    //图像缩放
    QImage imgScaled =image->scaled(ui->old_image->width(),
                           ui->old_image->height(),
                           Qt::KeepAspectRatio);
    //显示图片
    ui->old_image->setPixmap(QPixmap::fromImage(imgScaled));
    this->show();
}

// 上传视频显示
void yolo_detection::on_load_movie_clicked()
{
    QMessageBox::information(this, "提示：", "此功能还在开发中...");
//    ui->load_image->setText("上传图片");
//    ui->load_image->setText("重新上传视频");
//    QString fileName = QFileDialog::getOpenFileName(this->parentWidget(),tr("文件对话框"),
//                                                    "D:",tr("视频文件(*mp4 *avi)"));
//    qDebug()<<"fileName:"<<fileName;
//    QWidget *widget = new QWidget;
//    QVBoxLayout *layout = new QVBoxLayout;

//    QMediaPlayer* player = new QMediaPlayer;
//    QVideoWidget* videoWidget = new QVideoWidget;
//    QMediaPlaylist* playlist=new QMediaPlaylist;

//    videoWidget->setAspectRatioMode(Qt::IgnoreAspectRatio);
//    videoWidget->setFullScreen(true);

//    layout->addWidget(videoWidget);
//    widget->setLayout(layout);

//    playlist->clear();
//    playlist->addMedia(QUrl::fromLocalFile(fileName));
//    player->setPlaylist(playlist);

//    QFile file(fileName);
//    if(!file.open(QIODevice::ReadOnly))
//        qDebug() << "Could not open file";

//    player->setVideoOutput(videoWidget);
//    player->setPlaylist(playlist);

//    widget->showFullScreen();
//    player->play();


}

//打开摄像头设备
void yolo_detection::on_shexiangtou_clicked()
{
    if(isopen_camera=="flase")
    {
        ui->shexiangtou->setText("关闭摄像头");
        // 显示拍照按钮
        ui->photographBt->show();
        // 隐藏上传图片和上传视频的按钮
        ui->load_image->hide();
        ui->load_movie->hide();
        ui->yolo_detection_2->hide();
        // ====================================================
        //获取到要打开的设备的名称
        QCameraInfo CameraInfo = CameraInfoList.at(ui->deviceComboBox->currentIndex());
        //创建摄像头对象
        Camera = new QCamera(CameraInfo);
        //创建取景器并将其附在要显示的区域
        CameraViewFinder = new QCameraViewfinder(ui->new_image);
        Camera->setViewfinder(CameraViewFinder);
        //调整摄像头数据显示的大小
        CameraViewFinder->resize(ui->new_image->width(), ui->new_image->height());
        //显示摄像头取景器
        CameraViewFinder->show();
        //开启摄像头
        Camera->start();

        //创建获取一帧数据对象
        CameraImageCapture = new  QCameraImageCapture(Camera);
        //关联图像获取信号
        connect(CameraImageCapture, &QCameraImageCapture::imageCaptured, this, &yolo_detection::take_photo);

        // 注释已调用摄像头
        isopen_camera="true";
    }
    else
    {
        ui->shexiangtou->setText("调用摄像头");
        // 隐藏拍照按钮
        ui->photographBt->hide();
        // 打开上传图片和上传视频的按钮
        ui->load_image->show();
        ui->load_movie->show();
        ui->yolo_detection_2->show();
        // 关闭摄像头
        Camera->stop();
        CameraViewFinder->hide(); // 隐藏取景器
        ui->new_image->setPixmap(QPixmap());  //释放内存，关闭画面
        ui->new_image->clear();
        // 注释未调用摄像头
        isopen_camera="flase";
    }

}

//点击了拍照按键
void yolo_detection::on_photographBt_clicked()
{
    //发送获取一帧数据的信号
    CameraImageCapture->capture();
}

// 拍照按钮
void yolo_detection::take_photo(int id, const QImage &image)
{
    //使用系统时间来命名图片的名称，时间是唯一的，图片名也是唯一的
    QDateTime dateTime(QDateTime::currentDateTime());
    QString time = dateTime.toString("yyyy-MM-dd-hh-mm-ss");
    //创建图片保存路径名
    //QString img_path = QString("./%1.jpg").arg(time);
    img_path = QString("./%1.jpg").arg(time);
    //保存一帧数据
    image.save(img_path);
    //提示获取到了一帧数据
    QMessageBox::information(this, "提示：", "获取到一帧图片");
    //图像缩放
    QImage imgScaled =image.scaled(ui->old_image->width(),
                           ui->old_image->height(),
                           Qt::KeepAspectRatio);
    //显示图片
    ui->old_image->setPixmap(QPixmap::fromImage(imgScaled));
}

