#pragma once
#include<opencv2/opencv.hpp>
#include<opencv2/dnn.hpp>
#include<fstream>
#include<iostream>
using namespace std;
using namespace cv;
using namespace cv::dnn;


int main()
{
	//---------------------------------------加载类别---------------------------------------
	ifstream classNamesFile("./model/coco.names");

	vector<string> classNamesVec;
	if (classNamesFile.is_open())
	{
		string className = "";
		while (std::getline(classNamesFile, className))
			classNamesVec.push_back(className);
	}
	for (int i = 0; i < classNamesVec.size(); i++) {
		cout << i + 1 << "\t" << classNamesVec[i].c_str() << endl;
	}

	//---------------------------------------模型设置---------------------------------------

	String cfg = "./model/yolov4.cfg";
	String weight = "./model/yolov4.weights";

	//模型读入
	dnn::Net net = readNetFromDarknet(cfg, weight);
	
	//预处理读取的图像，并将图像读入网络
	Mat frame = imread("./image/1.jpg");
	imshow("【输入图像】", frame);
	Mat inputBlob = blobFromImage(frame, 1.0 / 255, Size(608, 608), Scalar());
	net.setInput(inputBlob);
	
	//获取未连接输出层
	std::vector<String> outNames = net.getUnconnectedOutLayersNames();
	for (int i = 0; i < outNames.size(); i++) {
		cout << "output layer name : " << outNames[i].c_str() << endl;
	}

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
	String className;
	Size labelSize;

	for (size_t i = 0; i < indices.size(); ++i) {
		idx = indices[i];	    
		className = classNamesVec[classIds[idx]];

		labelSize = getTextSize(className, FONT_HERSHEY_SIMPLEX, 0.5, 1, 0);
		box = boxes[idx];
		textBox = Rect(Point(box.x - 1, box.y), 
					   Point(box.x + labelSize.width, box.y - labelSize.height));
		rectColor = Scalar(idx * 11 % 256, idx * 22 % 256, idx * 33 % 256);
		textColor = Scalar(255 - idx * 11 % 256, 255 - idx * 22 % 256, 255 - idx * 33 % 256);

		rectangle(frame, box, rectColor, 2, 8, 0);		
		rectangle(frame, textBox, rectColor, -1, 8, 0);
		putText(frame, className.c_str(), Point(box.x, box.y - 2), FONT_HERSHEY_SIMPLEX, 0.5, textColor, 1, 8);
	}
	
	imshow("【OpenCV - YOLO】", frame);
	waitKey(0);

	return 0;

}