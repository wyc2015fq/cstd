# opencv 求协方差矩阵的特征值和特征向量 - sinat_31425585的博客 - CSDN博客
2018年03月28日 21:34:04[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：505
Opencv小结：Mat用法
社会人，人狠话不多，直接上代码做一个笔记：
参考博客[denny的学习专栏](https://www.cnblogs.com/denny402/p/5011456.html)，代码如下：
```cpp
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
int main()
{
	Mat src = imread("1.jpg");
	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);
	double f_min = 0, f_max = 0;
	Point min_loc, max_loc;
	minMaxLoc(gray, &f_min, &f_max, &min_loc, &max_loc);
	Scalar tempVal = cv::mean(gray);
	cout << tempVal[0] << endl;
	//Mat means = Mat::create(src.rows, src.cols, CV_32FC1);
	Mat means = tempVal[0] * Mat::ones(src.rows, src.cols, CV_64FC1);
	gray.convertTo(gray, CV_64FC1);
	//Mat diff = Mat::zeros(src.rows, src.cols, CV_32FC1);
	gray = gray - means;
	Mat covar = gray.t() * gray;
	Mat eValuesMat;
	Mat eVectorsMat;
	eigen(covar, eValuesMat, eVectorsMat);
	std::cout << "Eigen Values : " << std::endl;
	for (auto i = 0; i < eValuesMat.rows; i++){
		for (auto j = 0; j < eValuesMat.cols; j++){
			std::cout << eValuesMat.at<double>(i, j) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "Eigen Vectors: " << std::endl;
	for (auto i = 0; i < eVectorsMat.rows; i++){
		for (auto j = 0; j < eVectorsMat.cols; j++){
			std::cout << eVectorsMat.at<double>(i, j) << " ";
		}
		std::cout << std::endl;
	}
	return 0;
}
```
