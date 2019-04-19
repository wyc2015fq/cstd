# opencv 3.1 中adaboost接口使用 - sinat_31425585的博客 - CSDN博客
2018年03月20日 23:22:31[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：422
第一步：准备数据集（face和nonface）
![](https://img-blog.csdn.net/20180320225357556)
第二步：获取文件路径及图片名称：
![](https://img-blog.csdn.net/20180320225611269)![](https://img-blog.csdn.net/20180320225620775)
这里我自己写了个代码，得到这两个txt文件。下载链接[数据集及程序](https://download.csdn.net/download/sinat_31425585/10298799)
第三步：将人脸数据保存为.vec文件
![](https://img-blog.csdn.net/20180320230711688)
将生成的pos.txt，neg.txt，negdata文件夹、posdata文件夹及两个exe文件拷贝到一个文件夹中，然后在当前文件夹里面按住shift并点击右键，在当前文件夹中打开cmd，运行：
```cpp
opencv_createsamples  -img pos.txt -vec pos.vec -num 7000 -w 20 -h 20
```
会文件夹下会生成一个pos.vec文件。
第四步：训练
首先在当前文件夹中新建一个mode文件夹，保存训练模型，然后在CMD中继续输入如下命令：
```cpp
opencv_traincascade -data mode -vec pos.vec -bg neg.txt -numStages 20 -numThreads 4 -w 20 -h 20
```
就会开始训练。训练结束后，会在mode文件夹下生成一个cascade.xml文件。
第五步：测试
新建一个工程，将下面测试代码及生成的模型文件cascade.xml文件拷贝到一个新建工程中去。
```cpp
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;
using namespace cv;
/* Function Headers */
void detectAndDisplay(Mat frame);
/* Global variables */
String face_cascade_name = "cascade.xml";
CascadeClassifier face_cascade;
String window_name = "Capture - Face detection";
/* @function main */
int main(void)
{
	VideoCapture capture;
	Mat frame;
	//-- 1. Load the cascades
	if (!face_cascade.load(face_cascade_name)){ printf("--(!)Error loading face cascade\n"); return -1; };
	capture.open(0);
	if (!capture.isOpened()) { printf("--(!)Error opening video capture\n"); return -1; }
	while (capture.read(frame))
	{
		if (frame.empty())
		{
			printf(" --(!) No captured frame -- Break!");
			break;
		}
		//-- 3. Apply the classifier to the frame
		detectAndDisplay(frame);
		int c = waitKey(10);
		if ((char)c == 27) { break; } // escape
	}
	return 0;
}
/* @function detectAndDisplay */
void detectAndDisplay(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
		Mat faceROI = frame_gray(faces[i]);
	}
	//-- Show what you got
	imshow(window_name, frame);
}
```
运行效果如下：
![](https://img-blog.csdn.net/20180320232226417)

