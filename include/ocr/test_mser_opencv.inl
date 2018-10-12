
#include "wstd/filesystem.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;


// Mser车牌目标检测
std::vector<cv::Rect> mserGetPlate1(cv::Mat srcImage)
{
	// HSV空间转换
	cv::Mat gray, gray_neg;
	cv::Mat hsi;
	cv::cvtColor(srcImage, hsi, CV_BGR2HSV);
	// 通道分离
	std::vector<cv::Mat> channels;
	cv::split(hsi, channels);
	// 提取h通道
	gray = channels[1];
	// 灰度转换 
	cv::cvtColor(srcImage, gray, CV_BGR2GRAY);
	// 取反值灰度
	gray_neg = 255 - gray;
	std::vector<std::vector<cv::Point> > regContours;
	std::vector<std::vector<cv::Point> > charContours;

	// 创建MSER对象
	cv::Ptr<cv::MSER> mesr1 = cv::MSER::create(2, 10, 5000, 0.4, 0.3);
	cv::Ptr<cv::MSER> mesr2 = cv::MSER::create(2, 2, 400, 0.1, 0.3);

	std::vector<cv::Rect> bboxes1;
	std::vector<cv::Rect> bboxes2;
	// MSER+ 检测
	mesr1->detectRegions(gray, regContours, bboxes1);
	// MSER-操作
	mesr2->detectRegions(gray_neg, charContours, bboxes2);

	cv::Mat mserMapMat = cv::Mat::zeros(srcImage.size(), CV_8UC1);
	cv::Mat mserNegMapMat = cv::Mat::zeros(srcImage.size(), CV_8UC1);

	for (int i = (int)regContours.size() - 1; i >= 0; i--)
	{
		// 根据检测区域点生成mser+结果
		const std::vector<cv::Point>& r = regContours[i];
		for (int j = 0; j < (int)r.size(); j++)
		{
			cv::Point pt = r[j];
			mserMapMat.at<unsigned char>(pt) = 255;
		}
	}
	// MSER- 检测
	for (int i = (int)charContours.size() - 1; i >= 0; i--)
	{
		// 根据检测区域点生成mser-结果
		const std::vector<cv::Point>& r = charContours[i];
		for (int j = 0; j < (int)r.size(); j++)
		{
			cv::Point pt = r[j];
			mserNegMapMat.at<unsigned char>(pt) = 255;
		}
	}
	// mser结果输出
	cv::Mat mserResMat;
	// mser+与mser-位与操作
	mserResMat = mserMapMat & mserNegMapMat;
	cv::imshow("mserMapMat", mserMapMat);
	cv::imshow("mserNegMapMat", mserNegMapMat);
	cv::imshow("mserResMat", mserResMat);
	// 闭操作连接缝隙
	cv::Mat mserClosedMat;
	cv::morphologyEx(mserResMat, mserClosedMat,
		cv::MORPH_CLOSE, cv::Mat::ones(1, 20, CV_8UC1));
	cv::imshow("mserClosedMat", mserClosedMat);
	// 寻找外部轮廓
	std::vector<std::vector<cv::Point> > plate_contours;
	cv::findContours(mserClosedMat, plate_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	// 候选车牌区域判断输出
	std::vector<cv::Rect> candidates;
	for (size_t i = 0; i != plate_contours.size(); ++i)
	{
		// 求解最小外界矩形
		cv::Rect rect = cv::boundingRect(plate_contours[i]);
		// 宽高比例
		double wh_ratio = rect.width / double(rect.height);
		// 不符合尺寸条件判断
		if (rect.height > 20 && wh_ratio > 4 && wh_ratio < 7)
			candidates.push_back(rect);
	}
	return  candidates;
}

#include "BOCR.inl"
#include "opencv2/objdetect/objdetect.hpp"


struct SimilarRect {
	double eps;
	SimilarRect(double eps_ = 0.5) {
		this->eps = eps_;
	}
	inline bool operator()(const cv::Rect& r1, const cv::Rect& r2) const
	{
		// delta为最小长宽的eps倍
		double delta = eps*(std::min(r1.width, r2.width) + std::min(r1.height, r2.height))*0.5;
		// 如果矩形的四个顶点的位置差别都小于delta，则表示相似的矩形
		return std::abs(r1.x - r2.x) <= delta &&
			std::abs(r1.y - r2.y) <= delta &&
			std::abs(r1.x + r1.width - r2.x - r2.width) <= delta &&
			std::abs(r1.y + r1.height - r2.y - r2.height) <= delta;
	}
};

struct SimilarLine1 {
	double eps;
	SimilarLine1(double eps_ = 0.5) {
		this->eps = eps_;
	}
	inline bool operator()(const cv::Rect& r1, const cv::Rect& r2) const
	{
		// delta为最小长宽的eps倍
		double delta = eps*(std::min(r1.height, r2.height))*0.5;
		// 如果矩形的四个顶点的位置差别都小于delta，则表示相似的矩形
		int t = std::max(r1.y, r2.y);
		int b = std::min(r1.y + r1.height, r2.y + r2.height);
		int d = b - t;
		return d>delta;
	}
};

struct SimilarChar {
	double eps;
	double max_width;
	SimilarChar(double eps_, int max_width_) {
		this->eps = eps_;
		this->max_width = max_width_;
	}
	inline bool operator()(const cv::Rect& r1, const cv::Rect& r2) const
	{
		int l = std::min(r1.x, r2.x);
		int r = std::max(r1.x + r1.width, r2.x + r2.width);
		int w = r - l;
		printf("%d\n", w);
		return w < max_width;
	}
};

bool isdebug = false;

// Mser车牌目标检测
std::vector<std::vector<cv::Rect>> mserGetPlate(cv::Mat srcImage)
{
	// HSV空间转换
	cv::Mat gray, gray_neg;
	cv::Mat hsi;
	cv::cvtColor(srcImage, hsi, CV_BGR2HSV);
	// 通道分离
	std::vector<cv::Mat> channels;
	cv::split(hsi, channels);
	// 提取h通道
	gray = channels[1];
	// 灰度转换 
	cv::cvtColor(srcImage, gray, CV_BGR2GRAY);
	// 取反值灰度
	gray_neg = 255 - gray;
	std::vector<std::vector<cv::Point> > regContours;

	// 创建MSER对象
	cv::Ptr<cv::MSER> mesr1 = cv::MSER::create(2, 100, 800, 0.15, 0.3);

	std::vector<cv::Rect> bboxes1;
	// MSER+ 检测
	mesr1->detectRegions(gray, regContours, bboxes1);

	cv::Mat mserMapMat = cv::Mat::zeros(srcImage.size(), CV_8UC1);

	for (int i = (int)regContours.size() - 1; i >= 0; i--)
	{
		// 根据检测区域点生成mser+结果
		const std::vector<cv::Point>& r = regContours[i];
		for (int j = 0; j < (int)r.size(); j++)
		{
			cv::Point pt = r[j];
			mserMapMat.at<unsigned char>(pt) = 255;
		}
	}
  imshow("mserMapMat", mserMapMat); waitKey(0);
	cv::groupRectangles(bboxes1, 2);
	std::vector<cv::Rect> lines;
	std::vector<int> labels;
	std::vector<int> cnt;
	int nclasses = cv::partition(bboxes1, labels, SimilarLine1(0.3));
	lines = BOCR::rect_combine_by_index(bboxes1, labels, cnt);
	if (isdebug) {
		for (size_t i = 0; i != lines.size(); ++i) {
			cv::Rect rect = (lines[i]);
			if (cnt[i] > 2 && isdebug) {
				cv::rectangle(srcImage, rect, cv::Scalar(0, 0, 255), 1);
			}
		}
	}
	std::vector<std::vector<cv::Rect>> blocks;
	for (int i = 0; i < nclasses; ++i) {
		if (cnt[i] > 2) {
			std::vector<cv::Rect> line = BOCR::getline(bboxes1, labels, i);
			std::vector<int> index;
			std::vector<int> cnt;
			int max_width = lines[i].height * 7;
			int n = BOCR::rect_combine_in_line(line, index, max_width);
			//int n = cv::partition(line, index, SimilarChar(0.3, max_width));
			std::vector<cv::Rect> block = BOCR::rect_combine_by_index(line, index, cnt);
			if (isdebug) {
				for (size_t j = 0; j != block.size(); ++j) {
					cv::Rect rect = (block[j]);
					//candidates2.push_back(rect);
					cv::rectangle(srcImage, rect, cv::Scalar(0, 255, 0), 1);
				}
			}
			blocks.push_back(block);
		}
	}
	//BOCR::rect_combine_leftright(mserMapMat, bboxes1, bboxes1_out, 100, 10, 100, 100, 2, 2, 300);
	//bboxes1 = bboxes1_out;
	// mser+与mser-位与操作
	// 闭操作连接缝隙
	//cv::Mat mserClosedMat;
	//cv::Mat mserResMat = mserMapMat;
	//cv::morphologyEx(mserResMat, mserClosedMat, cv::MORPH_CLOSE, cv::Mat::ones(1, 20, CV_8UC1));
	//cv::imshow("mserClosedMat", mserClosedMat);
	// 候选车牌区域判断输出

	if (isdebug) {
		std::vector<cv::Rect> candidates;
		for (size_t i = 0; i != bboxes1.size(); ++i)
		{
			// 求解最小外界矩形
			cv::Rect rect = (bboxes1[i]);
			// 宽高比例
			double wh_ratio = rect.width / double(rect.height);
			if (rect.height > 10 && wh_ratio > 0. && wh_ratio < 2) {
				candidates.push_back(rect);
				cv::rectangle(srcImage, rect, cv::Scalar(255, 0, 0));
			}
		}
		cv::imshow("mserMapMat", mserMapMat);
		cv::imshow("srcImage", srcImage);
	}
	return  blocks;
}

cv::Mat MycopyMakeBorder(cv::Mat img, int dstw, int dsth) {
	cv::Mat img2;
	int d = 2;
	int w = img.cols, h = img.rows;
	int w1 = (dsth-2*d)*w / h;
	cv::Scalar color = mean(img.row(0));
	cv::resize(img, img, cv::Size(w1, dsth-2*d));
	cv::copyMakeBorder(img, img2, d, d, d, dstw - img.cols-d, cv::BORDER_CONSTANT, color);
	return img2;
}

#include "utils.h"

using namespace cv;

int myfindContours(cv::Mat srcImage) {
	cv::Mat _grayimage;
	cv::Mat _cannyimage;
	cv::cvtColor(srcImage, _grayimage, cv::COLOR_RGB2GRAY);
	int lowThreshold = 20;
	Canny(_grayimage, _cannyimage, lowThreshold, lowThreshold*3, 3);
	imshow("_cannyimage", _cannyimage);
	return 0;
}

int test_myfindContours() {
	std::vector<string> filenames;
	LoadTextFileList("E:/OCR_Line/demo_images/list.txt", filenames);
	int k;
	isdebug = true;
	for (k = 0; k < filenames.size(); ++k) {
		string fn = filenames[k];
		printf("%s\n", fn.c_str());
		cv::Mat srcImage = cv::imread(fn);
		if (srcImage.empty()) {
			continue;
		}
		if (srcImage.cols>600) {
			double t = 600.*1. / srcImage.cols;
			cv::resize(srcImage, srcImage, cv::Size(), t, t, cv::INTER_LANCZOS4);
		}
		myfindContours(srcImage);
		imshow("srcImage", srcImage);
		cv::waitKey(0);
	}
	return 0;
}

int test_mser_opencv2()
{
  //return test_hsv_bin();
	//return test_myfindContours();
	std::vector<string> filenames;
	LoadTextFileList("E:/OCR_Line/demo_images/list.txt", filenames);
	int k;
	isdebug = true;
	for (k = 0; k < filenames.size(); ++k) {
		string fn = filenames[k];
		printf("%s\n", fn.c_str());
		cv::Mat srcImage = cv::imread(fn);
		if (srcImage.empty()) {
			continue;
		}
		if (srcImage.cols>600) {
			double t = 600.*1. / srcImage.cols;
			cv::resize(srcImage, srcImage, cv::Size(), t,t, cv::INTER_LANCZOS4);
		}
		//cv::imshow("src Image", srcImage);
		std::vector<cv::Rect> candidates;
		std::vector<std::vector<cv::Rect>> blocks;
		std::string out;
		blocks = mserGetPlate(srcImage);
		// 车牌区域显示
		for (int i = 0; i < blocks.size(); ++i) {
			const std::vector<cv::Rect>& block = (blocks[i]);
			for (int j = 0; j < block.size(); ++j) {
			}
		}
		printf("%s\n", out.c_str());
		if (1) {
			cv::imshow("srcImage", srcImage);
			cv::waitKey(0);
		}
	}
	return 0;
}

int test_mser_opencv11()
{
	std::vector<string> filenames;
	LoadTextFileList("E:/OCR_Line/demo_images/list.txt", filenames);
	int k;
	for (k = 0; k < filenames.size(); ++k) {
		string fn = filenames[k];
		cv::Mat srcImage = cv::imread(fn);
		if (srcImage.empty()) {
			continue;
		}
		cv::resize(srcImage, srcImage, cv::Size(), 0.5, 0.5, cv::INTER_LANCZOS4);
		//cv::imshow("src Image", srcImage);
		std::vector<cv::Rect> candidates;
		std::vector<std::vector<cv::Rect>> blocks;
		std::string out;
		blocks = mserGetPlate(srcImage);
		// 车牌区域显示
		for (int i = 0; i < blocks.size(); ++i) {
			const std::vector<cv::Rect>& block = (blocks[i]);
			for (int j = 0; j < block.size(); ++j) {
				cv::Rect rect = (block[j]);
				cv::Mat aa = srcImage(rect);
				aa = MycopyMakeBorder(aa, 280, 32);
				char s[256];
				snprintf(s, 256, "rect%d", 1);
				string ss;
#ifdef _CAFFE_INL_
				ss = ocr_chinese(aa);
				out += ss;
#else
#endif
				if (0) {
					cv::imshow(s, aa);
					printf("%s", ss.c_str());
					//printf("%3d %3d %3d %3d\n", rect.x, rect.y, rect.width, rect.height);
					cv::waitKey(0);
				}
			}
			//printf("\n");
			out += "\n";
		}
		printf("%s\n", out.c_str());
		if (1) {
			cv::imshow("srcImage", srcImage);
			cv::waitKey(0);
		}
	}
	return 0;
}


int test_mser_opencv()
{
  //return test_hsv_bin();
	//return test_myfindContours();
	int k;
	isdebug = true;
  /// 加载源图像
  string im_dir = "E:/data/ew_id/we-loan.oss-cn-shanghai.aliyuncs.com/";
  string fn = im_dir + "upload/backend/20160927235243069-2951.jpg";
  vector<string> list;
  wstd::readlines("E:/data/ew_id/list1.txt", list, 100);
  for (int j = 0; j < list.size(); ++j) {
    vector<string> strs;
    wstd::split(strs, list[j], " ");
    string fn = im_dir + strs[0];
		printf("%s\n", fn.c_str());
		cv::Mat srcImage = cv::imread(fn, 1);
		if (srcImage.empty()) {
			continue;
		}
		if (srcImage.cols>600) {
			double t = 600.*1. / srcImage.cols;
			cv::resize(srcImage, srcImage, cv::Size(), t,t, cv::INTER_LANCZOS4);
		}
		//cv::imshow("src Image", srcImage);
		std::vector<cv::Rect> candidates;
		std::vector<std::vector<cv::Rect>> blocks;
		std::string out;
		blocks = mserGetPlate(srcImage);
		// 车牌区域显示
		for (int i = 0; i < blocks.size(); ++i) {
			const std::vector<cv::Rect>& block = (blocks[i]);
			for (int j = 0; j < block.size(); ++j) {
			}
		}
		printf("%s\n", out.c_str());
		if (1) {
			cv::imshow("srcImage", srcImage);
			cv::waitKey(0);
		}
	}
	return 0;
}