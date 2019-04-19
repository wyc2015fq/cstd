# libfacedetection（一）：最简单粗暴的配置方法 - sinat_31425585的博客 - CSDN博客
2017年09月08日 10:55:32[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：3899
之前玩了一下Opencv中文网上![@ShiqiYu](https://avatars3.githubusercontent.com/u/7151133?v=4&s=40)[ShiqiYu](https://github.com/ShiqiYu)老师提供的人脸检测库[点击打开链接](http://www.opencv.org.cn/portal.php?mod=view&aid=2)，本着我分享我快乐的心态，现在将配置过程总结一波，顺便分享一下，libfacedetection库在github上的下载链接为：[点击打开链接](https://github.com/ShiqiYu/libfacedetection)。
具体配置过程如下：
1、下载libfacedetection库，然后解压，可以得到如下：
![](https://img-blog.csdn.net/20170908104011623)
2、将bin目录下的文件复制到opencv对应的bin目录下，注意区分32位还是64位，我用的是32位编译器，所以需要将libfacedetect.dll复制到D:\opencv\build\x86\vc10\bin目录下：
![](https://img-blog.csdn.net/20170908104409251)
3、将lib目录下的文件复制到opencv对应的lib目录下，同样的要区分32位还是64位，将libfacedetect.lib复制到D:\opencv\build\x86\vc10\lib目录下：
![](https://img-blog.csdn.net/20170908104645032)
4、将include目录下的文件复制到opencv对应的include文件夹下面，即，将facedetect-dll.h文件复制到D:\opencv\build\include下面
5、将libfacedetect.lib添加到附加依赖项中
6、测试，这里我写了一个摄像头中的人脸检测程序，摄像头启动程序参考的是opencv2计算机识别编程手册216页的程序，下面是具体demo：
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
int main(int argc, char* argv[])
{
	Mat frame;  
	Mat gray;  
	bool stop = true;  
	VideoCapture capture(0);//打开摄像头  
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
		//pResults = facedetect_frontal_surveillance(pBuffer, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
		//	1.2f, 2, 48, 0, doLandmark);
		//pResults = facedetect_multiview(pBuffer, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
		//	1.2f, 2, 48, 0, doLandmark);
		pResults = facedetect_multiview_reinforce(pBuffer, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
			1.2f, 3, 48, 0, doLandmark);
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
		}
		imshow("libface", frame);  
		if (waitKey(30) >= 0)  
			stop =false;  
	}  
	return 0;  
}
```
好了，打完收工！！！
