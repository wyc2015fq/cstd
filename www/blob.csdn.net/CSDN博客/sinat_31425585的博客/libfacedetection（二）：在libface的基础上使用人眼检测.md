# libfacedetection（二）：在libface的基础上使用人眼检测 - sinat_31425585的博客 - CSDN博客
2017年09月21日 00:23:07[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：714
在前面基于libface的基础上，增加opencv自带的人眼检测器，具体过程可以参考一下具体代码：
```cpp
/*
The MIT License (MIT)
Copyright (c) 2015-2017 Shiqi Yu
shiqi.yu@gmail.com
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "facedetect-dll.h"
#pragma comment(lib,"libfacedetect.lib")
//#pragma comment(lib,"libfacedetect-x64.lib")
//define the buffer size. Do not change the size!
#define DETECT_BUFFER_SIZE 0x20000
using namespace cv;
using namespace std;
String eyeCascadeName = "haarcascade_eye_tree_eyeglasses.xml";//人眼的训练数据  
int main(int argc, char* argv[])
{
	Mat frame;  
	Mat gray;  
	bool stop = true;  
	CascadeClassifier eyeCascade;
	if( !eyeCascade.load( eyeCascadeName ) )//从指定的文件目录中加载级联分类器  
	{  
		cerr << "ERROR: 载入人眼的xml文件时出错！！！" << endl;   
	}  
	vector<Rect> eyeRect;  
	VideoCapture capture(1);//打开摄像头  
	if (!capture.isOpened())  
	{  
		cout << "open camera error!!!" << endl;   
		return -1;  
	}  
	int * pResults = NULL; 
	//pBuffer is used in the detection functions.
	//If you call functions in multiple threads, please create one buffer for each thread!
	unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
	if(!pBuffer)
	{
		fprintf(stderr, "Can not alloc buffer.\n");
		return -1;
	}
	int doLandmark = 1;
	while (stop)  
	{  
		capture >> frame;//读取当前帧到frame矩阵中  
		cvtColor(frame, gray, CV_BGR2GRAY);//转为灰度图  
		
		//pResults = facedetect_frontal(pBuffer, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
		//	1.2f, 2, 48, 0, doLandmark);
		pResults = facedetect_frontal_surveillance(pBuffer, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
			1.2f, 2, 48, 0, doLandmark);
		//pResults = facedetect_multiview(pBuffer, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
		//	1.2f, 2, 48, 0, doLandmark);
		//pResults = facedetect_multiview_reinforce(pBuffer, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
		//	1.2f, 3, 48, 0, doLandmark);
		printf("%d faces detected.\n", (pResults ? *pResults : 0));
		for(int i = 0; i < (pResults ? *pResults : 0); i++)
		{
			short * p = ((short*)(pResults+1))+142*i;
			int x = p[0];
			int y = p[1];
			int w = p[2];
			int h = p[3];
			int neighbors = p[4];
			int angle = p[5];
			printf("face_rect=[%d, %d, %d, %d], neighbors=%d, angle=%d\n", x, y, w, h, neighbors, angle);
			rectangle(frame, Rect(x, y, w, h), Scalar(0, 255, 0), 2);
			if (doLandmark)
			{
				for (int j = 0; j < 68; j++)
					circle(frame, Point((int)p[6 + 2 * j], (int)p[6 + 2 * j + 1]), 1, Scalar(0, 255, 0));
			}
			// 截取人脸区域进行人眼检测
			Mat faceROI = gray(Rect(x, y, w, h));
			Mat face;
			faceROI.copyTo(face);
			equalizeHist(face, face);//直方图均衡
			// 人眼检测
			eyeCascade.detectMultiScale( face, eyeRect,  
				1.1, 2, 0  
				//|CV_HAAR_FIND_BIGGEST_OBJECT  
				//|CV_HAAR_DO_ROUGH_SEARCH  
				//|CV_HAAR_DO_CANNY_PRUNING  
				|CV_HAAR_SCALE_IMAGE  
				,  
				Size(30, 30) );  
			for( vector<Rect>::const_iterator nr = eyeRect.begin(); nr != eyeRect.end(); nr++ )  
			{  
				rectangle(frame, Point(nr->x + x, nr->y + y), 
					Point(nr->x + x + nr->width, nr->y + y + nr->height),
					Scalar(255, 0, 0));
				
			}  
		}
		imshow("libface", frame);  
		if (waitKey(30) >= 0)  
			stop =false;  
	}  
	return 0;  
}
```
