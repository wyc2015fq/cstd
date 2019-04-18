# 《学习OpenCV》课后习题解答(第三章)(仅供参考) - 江南烟雨 - CSDN博客
2011年08月23日 19:26:40[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：3416
**代码在VS2008下通过，要在附加依赖项中添加：opencv_core220d.lib opencv_highgui220d.lib opencv_imgproc220d.lib。**
**也可以在代码里面添加以下内容：**
- #pragma comment(lib,"opencv_core220d.lib")
- #pragma comment(lib,"opencv_highgui220d.lib")
- #pragma comment(lib,"opencv_imgproc220d.lib")
```cpp
#pragma comment(lib,"opencv_core220d.lib")  
#pragma comment(lib,"opencv_highgui220d.lib")  
#pragma comment(lib,"opencv_imgproc220d.lib")
```
**具体可见这篇文章：[配置环境](http://blog.csdn.net/xiajun07061225/article/details/6694195)**
**Chapter 3:**
2、This exercise will accustom you to the idea of many functions taking matrix types.Create a two-dimensional matrix with three channels of type byte with data size 100-by-100. Set all the values to 0.
a. Draw a circle in the matrix using void cvCircle( CvArr* img, CvPoint center,
intradius, CvScalar color, int thickness=1, int line_type=8, int shift=0 ).
b. Display this image using methods described in Chapter 2.
Solution：
```cpp
#include "opencv\cv.h"
#include "opencv\highgui.h"
int main(){
	CvMat* mat = cvCreateMat(100,100,CV_8UC3);
	cvZero(mat);
	cvCircle(mat,
		cvPoint(50,50),
		30,
		CV_RGB(255,0,0),
		2,
		8);
	cvNamedWindow("xiti_ex3_2",CV_WINDOW_AUTOSIZE);
	cvShowImage("xiti_ex3_2",mat);
	cvWaitKey(0);
	//cvReleaseImage(&mat);
	cvDestroyWindow("xiti_ex3_2");
}
```
Test：
![](http://hi.csdn.net/attachment/201108/23/0_1314098793ibjK.gif)
3、Create a two-dimensional matrix with three channels of type byte with data size 100-by-100, and set all the values to 0. Use the pointer element access function cvPtr2D to point to the middle (“green”) channel. Draw a green rectangle between (20, 5) and
 (40, 20).
Solution:
```cpp
#include "opencv\cv.h"
#include "opencv\highgui.h"
int main(){
	CvMat* mat = cvCreateMat(100,100,CV_8UC3);
	cvZero(mat);
	int i,j;
	for(i = 20;i < 40; ++i){
		for(j = 5;j < 20; ++j){
			uchar* p = cvPtr2D(mat,i,j);
			p[1] = 255;
		}
	}
	cvNamedWindow("drawRectangle",CV_WINDOW_AUTOSIZE);
	cvShowImage("drawRectangle",mat);
	cvWaitKey(0);
	cvDestroyWindow("drawRectangle");
}
```
Test：
![](http://hi.csdn.net/attachment/201108/23/0_13141010520s6L.gif)
4、Create a three-channel RGB image of size 100-by-100. Clear it. Use pointer arithmetic to draw a green square between (20, 5) and (40, 20).
Solution:
```cpp
#include "opencv\cv.h"
#include "opencv\highgui.h"
int main(){
	IplImage* img = cvCreateImage(
		cvSize(100,100),
		8,
		3);
	cvZero(img);
	for(int y = 5;y<= 20;++y){
		uchar* ptr = (uchar*)(
			img ->imageData + y * img ->widthStep);
		for(int x = 20;x <= 40;++x){
			ptr[3 * x + 1] = 255;
		}
	}
	cvNamedWindow("drawRectangle",CV_WINDOW_AUTOSIZE);
	cvShowImage("drawRectangle",img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("drawRectangle");
}
```
Test：
![](http://hi.csdn.net/attachment/201108/23/0_131410079878zS.gif)
5、Practice using region of interest (ROI). Create a 210-by-210 single-channel byte image and zero it. Within the image, build a pyramid of increasing values using ROI and cvSet(). Th at is: the outer border should be 0, the next inner border should be 20,
 the next inner border should be 40, and so on until the fi nal innermost square is set to value 200; all borders should be 10 pixels wide. Display the image.
Solution：
```cpp
#include "opencv\cv.h"
#include "opencv\highgui.h"
int main(){
	IplImage* img = cvCreateImage(
		cvSize(210,210),
		IPL_DEPTH_8U,
		1);
	cvZero(img);
	int x = 0;
	int y = 0;
	int scalar = 0;
	int add = 0;
	while(add < 200){
		cvSetImageROI(
			img,
			cvRect(x,y,210 - add,10));
		cvSet(img,cvScalar(scalar));//赋值
		x += 10;
		y += 10;
		scalar += 20;
		add += 20;
		cvResetImageROI(img);
	}
	cvNamedWindow("Example",CV_WINDOW_AUTOSIZE);
	cvShowImage("Example",img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("Example");
}
```
Test：
![](http://hi.csdn.net/attachment/201108/23/0_13141056381V5O.gif)
6、Use multiple image headers for one image. Load an image that is at least 100-by-100. Create two additional image headers and set their origin, depth, number of channels,and widthstep to be the same as the loaded image. In the new image headers,set the
 width at 20 and the height at 30. Finally, set their imageData pointers to point to the pixel at (5, 10) and (50, 60), respectively. Pass these new image subheaders to cvNot(). Display the loaded image, which should have two inverted rectangles
within the larger image.、
Solution：
```cpp
#include "opencv\cv.h"
#include "opencv\highgui.h"
int main(){
	IplImage* img = cvLoadImage("HUST.jpg");
	IplImage* img1;
	IplImage* img2;
	img1 = cvCreateImageHeader(
		cvSize(20,30),
		img ->depth,
		img ->nChannels);
	img1 ->origin = img ->origin;
	img1 ->widthStep = img ->widthStep;
	img2 = cvCreateImageHeader(
		cvSize(20,30),
		img ->depth,
		img ->nChannels);
	img2 ->origin = img ->origin;
	img2 ->widthStep = img ->widthStep;
	img1 ->imageData = img ->imageData + 10 * img ->widthStep +
		5 *  img ->nChannels;
	img2 ->imageData = img ->imageData + 60 * img ->widthStep +
		50 *  img ->nChannels;
	cvNot(img1,img1);
	cvNot(img2,img2);
	cvNamedWindow("Example",CV_WINDOW_AUTOSIZE);
	cvShowImage("Example",img);
	cvWaitKey(0);
	cvReleaseImage(&img1);
	cvReleaseImage(&img2);
	cvReleaseImage(&img);
	cvDestroyWindow("Example");
}
```
Test：
![](http://hi.csdn.net/attachment/201108/23/0_1314102902hoHf.gif)
7、Create a mask using cvCmp(). Load a real image. Use cvSplit() to split the image into red, green, and blue images.
a. Find and display the green image.
b. Clone this green plane image twice (call these clone1 and clone2).
c. Find the green plane’s minimum and maximum value.
d. Set clone1’s values to thresh = (unsigned char)((maximum - minimum)/2.0).
e. Set clone2 to 0 and use cvCmp(green_image, clone1, clone2, CV_CMP_GE). Now clone2 will have a mask of where the value exceeds thresh in the green image.
f. Finally, use cvSubS(green_image,thresh/2, green_image, clone2) and display the results.
Solution：
```cpp
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include <iostream>
using namespace std;
int main(){
	IplImage* img = cvLoadImage("HUST.jpg");
	//显示原始图像
	cvNamedWindow("img_pre",CV_WINDOW_AUTOSIZE);
	cvShowImage("img_pre",img);
	
	//创建三通道图像
	IplImage* imgRed = cvCreateImage(
		cvSize(img ->width,img ->height),
		img ->depth,
		1);
	IplImage* imgGreen = cvCreateImage(
		cvSize(img ->width,img ->height),
		img ->depth,
		1);
	IplImage* imgBlue = cvCreateImage(
		cvSize(img ->width,img ->height),
		img ->depth,
		1);
	//分离出R、G、B三通道图像
	cvSplit(img,imgGreen,imgBlue,imgRed,NULL);
	//显示绿色图像
	cvNamedWindow("img_Green",CV_WINDOW_AUTOSIZE);
	cvShowImage("img_Green",imgGreen);
	
	//克隆绿色图像两次
	IplImage* imgGreenClone1 = cvCloneImage(imgGreen);
	IplImage* imgGreenClone2 = cvCloneImage(imgGreen);
	double MaxNum,MinNum;
	cvMinMaxLoc(imgGreen,&MinNum,&MaxNum);
	cout<<"MinNum in imgGreen："<<MinNum<<endl;
	cout<<"MaxNum in imgGreen："<<MaxNum<<endl;
	double scalar = (MaxNum - MinNum)/2;
	cvSet(imgGreenClone1,cvScalar(scalar));
	cvZero(imgGreenClone2);
	cvCmp(imgGreen,imgGreenClone1,imgGreenClone2,CV_CMP_GE);
	cvSubS(imgGreen,cvScalar(scalar / 2),imgGreen,imgGreenClone2);
	cvNamedWindow("imgGreen_after",CV_WINDOW_AUTOSIZE);
	cvShowImage("imgGreen_after",imgGreen);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvReleaseImage(&imgGreen);
	cvDestroyWindow("img_pre");
	cvDestroyWindow("img_Green");
	cvReleaseImage(&imgGreenClone2);
	cvDestroyWindow("imgGreen_after");
}
```
Test：
![](http://hi.csdn.net/attachment/201108/24/0_13141490739oOh.gif)
8、Create a structure of an integer, a CvPoint and a CvRect; call it “my_struct”.
Write two functions: void write_my_struct( CvFileStorage * fs, const char * name, my_struct *ms) and void read_my_struct( CvFileStorage* fs, CvFileNode* ms_node, my_struct* ms ). Use them to write and read my_struct.
Solution：
```cpp
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include <iostream>
using namespace std;
typedef struct{
	int i;
	CvPoint pt;
	CvRect rect;
}my_struct;
void write_my_struct(
					CvFileStorage *fs,
					const char *name,
					my_struct *ms){
	assert(fs != 0);
	cvWriteInt(fs,"i",ms ->i);
	cvStartWriteStruct(fs,"pt",CV_NODE_SEQ);
	cvWriteInt(fs,0,ms ->pt.x);
	cvWriteInt(fs,0,ms ->pt.y);
	cvEndWriteStruct(fs);
	cvStartWriteStruct(fs,"rect",CV_NODE_SEQ);
	cvWriteInt(fs,0,ms ->rect.x);
	cvWriteInt(fs,0,ms ->rect.y);
	cvWriteInt(fs,0,ms ->rect.width);
	cvWriteInt(fs,0,ms ->rect.height);
	cvEndWriteStruct(fs);
}
void read_my_struct(
					CvFileStorage *fs,
					CvFileNode *ms_node,
					my_struct *ms){
	assert(fs != 0);
	ms ->i = cvReadIntByName(fs,0,"i",0);
	CvSeq* s = cvGetFileNodeByName(fs,0,"pt") ->data.seq;
	ms ->pt.x = cvReadInt((CvFileNode*)cvGetSeqElem(s,0)); 
	ms ->pt.y = cvReadInt((CvFileNode*)cvGetSeqElem(s,1)); 
	
	CvSeq* s1 = cvGetFileNodeByName(fs,0,"rect") ->data.seq;
	ms ->rect.x = cvReadInt((CvFileNode*)cvGetSeqElem(s1,0)); 
	ms ->rect.y = cvReadInt((CvFileNode*)cvGetSeqElem(s1,1)); 
	ms ->rect.width = cvReadInt((CvFileNode*)cvGetSeqElem(s1,2));
	ms ->rect.height = cvReadInt((CvFileNode*)cvGetSeqElem(s1,3));
	//输出读取的结果
	cout<<"my_struct："<<endl;
	cout<<"i:"<<ms ->i<<endl;
	cout<<"pt:("<<ms ->pt.x<<","<<ms ->pt.y<<")"<<endl;
	cout<<"rect:("<<ms ->rect.x<<","<<ms ->rect.y<<","<<
		ms ->rect.width<<","<<ms ->rect.height<<endl;
}
int main(){
	//往xml文件中写数据
	CvFileStorage* fs = cvOpenFileStorage(
		"cfg.xml",
		0,
		CV_STORAGE_WRITE);
	my_struct ms;
	ms.i = 100;
	ms.pt.x = 10;
	ms.pt.y = 20;
	ms.rect.x = 30;
	ms.rect.y = 40;
	ms.rect.width = 50;
	ms.rect.height = 60;
	write_my_struct(fs,NULL,&ms);
	cvReleaseFileStorage(&fs);
	//往xml文件中读数据
	CvFileStorage* fs1 = cvOpenFileStorage(
		"cfg.xml",
		0,
		CV_STORAGE_READ);
	my_struct ms1;
	read_my_struct(fs1,NULL,&ms1);
	cvReleaseFileStorage(&fs1);
}
```
Test：
![](http://hi.csdn.net/attachment/201108/24/0_1314153471PPn9.gif)![](http://hi.csdn.net/attachment/201108/24/0_13141534763P9Z.gif)
