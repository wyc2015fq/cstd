#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <WINSOCK2.H>
#include<iostream>
#include<thread>
#include <winsock2.h>
#include <stdio.h>
#include<string>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
#include<vector>
 
using namespace cv;
 
 
					//////////////////////////////////////////////////////////////////
					//函数功能：用向量来做COSα=两向量之积/两向量模的乘积求两条线段夹角
					//输入：   线段3个点坐标pt1,pt2,pt0,最后一个参数为公共点
					//输出：   线段夹角，单位为角度
					//////////////////////////////////////////////////////////////////
double angle(CvPoint* pt1, CvPoint* pt2, CvPoint* pt0)
{
	double dx1 = pt1->x - pt0->x;
	double dy1 = pt1->y - pt0->y;
	double dx2 = pt2->x - pt0->x;
	double dy2 = pt2->y - pt0->y;
	double angle_line = (dx1*dx2 + dy1 * dy2) / sqrt((dx1*dx1 + dy1 * dy1)*(dx2*dx2 + dy2 * dy2) + 1e-10);//余弦值
	return acos(angle_line) * 180 / 3.141592653;
}
//////////////////////////////////////////////////////////////////
//函数功能：采用多边形检测，通过约束条件寻找矩形
//输入：   img 原图像
//          storage 存储
//          minarea，maxarea 检测矩形的最小/最大面积
//          minangle,maxangle 检测矩形边夹角范围，单位为角度
//输出：   矩形序列
//////////////////////////////////////////////////////////////////
CvSeq* findSquares4(IplImage* img, CvMemStorage* storage, int minarea, int maxarea, int minangle, int maxangle, int(&temp)[30])
{
	CvSeq* contours;//边缘
	int N = 6;  //阈值分级
	CvSize sz = cvSize(img->width & -2, img->height & -2);
	IplImage* timg = cvCloneImage(img);//拷贝一次img
	IplImage* gray = cvCreateImage(sz, 8, 1); //img灰度图
	IplImage* pyr = cvCreateImage(cvSize(sz.width / 2, sz.height / 2), 8, 3);  //金字塔滤波3通道图像中间变量
	IplImage* tgray = cvCreateImage(sz, 8, 1); ;
	CvSeq* result;
	double s, t;
	int sk = 0;
	CvSeq* squares = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint), storage);
 
	cvSetImageROI(timg, cvRect(0, 0, sz.width, sz.height));
	//金字塔滤波 
	cvPyrDown(timg, pyr, 7);
	cvPyrUp(pyr, timg, 7);
	//在3个通道中寻找矩形 
	for (int c = 0; c < 3; c++) //对3个通道分别进行处理 
	{
		cvSetImageCOI(timg, c + 1);
		cvCopy(timg, tgray, 0);  //依次将BGR通道送入tgray         
		for (int l = 0; l < N; l++)
		{
			//不同阈值下二值化
			cvThreshold(tgray, gray, 75, 250, CV_THRESH_BINARY);
			cvShowImage("111", gray);
			cvFindContours(gray, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
			while (contours)
			{ //多边形逼近             
				result = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0);
 
				//如果是凸四边形并且面积在范围内 
				if (result->total == 4 && fabs(cvContourArea(result, CV_WHOLE_SEQ)) > minarea  && fabs(cvContourArea(result, CV_WHOLE_SEQ)) < maxarea &&  cvCheckContourConvexity(result))
				{
 
					s = 0;
					//判断每一条边
					for (int i = 0; i < 5; i++)
					{
						if (i >= 2)
						{   //角度            
							t = fabs(angle((CvPoint*)cvGetSeqElem(result, i), (CvPoint*)cvGetSeqElem(result, i - 2), (CvPoint*)cvGetSeqElem(result, i - 1)));
							s = s > t ? s : t;
						}
					}
					//这里的S为直角判定条件 单位为角度
					if (s > minangle && s < maxangle)
					{
						for (int i = 0; i < 4; i++)
							cvSeqPush(squares, (CvPoint*)cvGetSeqElem(result, i));
						CvRect rect = cvBoundingRect(contours, 1);       // 获取矩形边界框 
						CvPoint p1;
						p1 = cvPoint(rect.x + rect.width / 2, rect.y + rect.height / 2);   //矩形中心坐标  
						std::cout << "X:" << p1.x << "Y：" << p1.y << std::endl;
					}
				}
				contours = contours->h_next;
			}
		}
		std::cout << "圆的数量是"<<sk << std::endl;
		temp[26] = sk;
 
	
		sk = 0;
	}
	cvReleaseImage(&gray);
	cvReleaseImage(&pyr);
	cvReleaseImage(&tgray);
	cvReleaseImage(&timg);
 
	return squares;
}
//////////////////////////////////////////////////////////////////
//函数功能：画出所有矩形
//输入：   img 原图像
//          squares 矩形序列
//          wndname 窗口名称
//输出：   图像中标记矩形
//////////////////////////////////////////////////////////////////
void drawSquares(IplImage* img, CvSeq* squares, const char* wndname)
{
	CvSeqReader reader;
	IplImage* cpy = cvCloneImage(img);
	CvPoint pt[4];
	int i;
	cvStartReadSeq(squares, &reader, 0);
	for (i = 0; i < squares->total; i += 4)
	{
		CvPoint* rect = pt;
		int count = 4;
		memcpy(pt, reader.ptr, squares->elem_size);
		CV_NEXT_SEQ_ELEM(squares->elem_size, reader);
		memcpy(pt + 1, reader.ptr, squares->elem_size);
		CV_NEXT_SEQ_ELEM(squares->elem_size, reader);
		memcpy(pt + 2, reader.ptr, squares->elem_size);
		CV_NEXT_SEQ_ELEM(squares->elem_size, reader);
		memcpy(pt + 3, reader.ptr, squares->elem_size);
		CV_NEXT_SEQ_ELEM(squares->elem_size, reader);
		//cvPolyLine( cpy, &rect, &count, 1, 1, CV_RGB(0,255,0), 3, CV_AA, 0 );
		cvPolyLine(cpy, &rect, &count, 1, 1, CV_RGB(rand() & 255, rand() & 255, rand() & 255), 1, CV_AA, 0);//彩色绘制
	}
	cvShowImage("22", cpy);
	cvReleaseImage(&cpy);
}

using namespace std;

int LoadTextFileList(const string& testfile, std::vector<string>& imgs) {
	FILE* input = NULL;
	input = fopen(testfile.c_str(), "rb");
	if (input) {
		char buf[1024];
		char fname[256];
		for (; fgets(buf, 1024, input) > 0; ) {
			sscanf(buf, "%s", fname);
			imgs.push_back(fname);
		}
		fclose(input);
	}
	return imgs.size();
}

int test_findSquares4()
{
	Mat edges;  //定义转化的灰度图
	const char* winn = "1111";

	std::vector<string> flist;
	LoadTextFileList("E:/OCR_Line/demo_images/list.txt", flist);
	CvMemStorage* storage = 0;
	storage = cvCreateMemStorage(0);
	for (int j = 0; j < flist.size(); ++j) {
		int Y = 0, J = 0;
		Mat frame;
		frame = imread(flist[j], IMREAD_COLOR);
		resize(frame, frame, Size(), 0.5, 0.5, INTER_LANCZOS4);
		IplImage img0 = frame;
		int a[30];
		drawSquares(&img0, findSquares4(&img0, storage, 100, 2000, 80, 100, a), winn);

		Mat E = frame(Range(1, 320), Range(1, 240));
		cvtColor(frame, edges, CV_BGR2GRAY);

		imshow("【效果图】", frame);
		waitKey(-1);
	}
	return 0;
}

int test_findSquares4_cam()
{
	//开起摄像头
	VideoCapture capture;
	capture.open(0);
	Mat edges;  //定义转化的灰度图
	if (!capture.isOpened())
		namedWindow("【效果图】", CV_WINDOW_NORMAL);
	const char* winn = "1111";
	if (!capture.isOpened())
		//namedWindow(winn, CV_WINDOW_NORMAL);
		CvMemStorage* storage = 0;
	CvMemStorage* storage = 0;
	storage = cvCreateMemStorage(0);
	while (1)
	{
		int Y = 0, J = 0;
		Mat frame;
		capture >> frame;
		IplImage img0 = frame;
		//drawSquares(&img0, findSquares4(&img0, storage, 100, 2000, 80, 100, a), winn);
		//cvClearMemStorage(storage);  //清空存储
		Mat E = frame(Range(1, 320), Range(1, 240));
		cvtColor(frame, edges, CV_BGR2GRAY);
		//高斯滤波
		GaussianBlur(edges, edges, Size(7, 7), 2, 2);
		std::vector<Vec3f> circles;//存储每个圆的位置信息
								   //霍夫圆
		HoughCircles(edges, circles, CV_HOUGH_GRADIENT, 1.5, 10, 100, 100, 0, 50);
		for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			//std::cout << "圆的X是" << circles[i][0] << "圆的Y是" << circles[i][1] << std:: endl;
			//绘制圆轮廓  
			circle(frame, center, radius, Scalar(155, 50, 255), 3, 8, 0);
			int R = frame.at<Vec3b>(cvRound(circles[i][1]), cvRound(circles[i][0]))[2];//R
			int G = frame.at<Vec3b>(cvRound(circles[i][1]), cvRound(circles[i][0]))[1];//G
			int B = frame.at<Vec3b>(cvRound(circles[i][1]), cvRound(circles[i][0]))[0];//B
			int num = R + G + B;
			std::cout << "圆心颜色是" << num << std::endl;
		}

		imshow("【效果图】", frame);
		waitKey(30);
	}
	return 0;
}
 
