
# opencv3.3 svm的使用 - 郭云飞的专栏 - CSDN博客


2017年09月26日 18:43:40[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：1877


在OpenCV 3.3中取消了CvSVM类的定义，结构变成了这样的了：
![](http://docs.opencv.org/3.3.0/d1/d2d/classcv_1_1ml_1_1SVM.png)
具体可参考文档：

```python
http://docs.opencv.org/3.3.0/d1/d2d/classcv_1_1ml_1_1SVM.html#a77d9a35898cae44ac9071c4b35bc96a8
```
下边将OpenCV老版本的例子用OpenCV3.3的重新写了一下，亲测通过：
```python
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <sys/io.h>
#include <sys/times.h>
#include <iomanip> // setw()
using namespace std;
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp> // svm 
using namespace cv;
int main(int argc, char** argv)
{
	cout << ">> ----" << "\n" << endl;
	int wid = 512;
	int hei = 512;
	Mat image = Mat::zeros(hei, wid, CV_8UC3);
	// Set up training data
    int labels[4] = {1, -1, -1, -1};
    Mat labelsMat(4, 1, CV_32SC1, labels);
    float trainingData[4][2] = { {501, 10}, {255, 10}, {501, 255}, {10, 501} };
    Mat trainingDataMat(4, 2, CV_32FC1, trainingData);
	
	Ptr<ml::SVM> svm = ml::SVM::create();
	// Type of a SVM formulation. See SVM::Types. Default value is SVM::C_SVC
	svm->setType(ml::SVM::C_SVC); 
	// Initialize with one of predefined kernels. See SVM::KernelTypes
	// Linear kernel. No mapping is done, linear discrimination (or regression) 
	// is done in the original feature space. It is the fastest option. 
	if (0)
		svm->setKernel(ml::SVM::LINEAR);
	else{
		svm->setKernel(ml::SVM::POLY);
		svm->setDegree(1.0); 
	}
	// Termination criteria of the iterative SVM training procedure which 
	// solves a partial case of constrained quadratic optimization problem. 
	// You can specify tolerance and/or the maximum number of iterations. 
	// Default value is TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 1000, FLT_EPSILON );
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 1000, FLT_EPSILON));
	
	// train
	// The second parameter: int lyout. See:
	// cv::ml::SampleTypes has two values: ROW_SAMPLE, COL_SAMPLE
	svm->train(trainingDataMat, ml::ROW_SAMPLE, labelsMat);
	
	Vec3b green(0,255,0), blue (255,0,0);
    // Show the decision regions given by the SVM
    for (int i = 0; i < image.rows; ++i)
        for (int j = 0; j < image.cols; ++j) {
            Mat sampleMat = (Mat_<float>(1,2) << i,j);
            // predict
            float response = svm->predict(sampleMat);
            if (response == 1)
                image.at<Vec3b>(j, i)  = green;
            else if (response == -1) 
                 image.at<Vec3b>(j, i)  = blue;
        }
	// Show the training data
    int thickness = -1;
    int lineType  =  8;
    circle(image, Point(501,  10), 6, Scalar(  0,   0,   0), thickness, lineType);
    circle(image, Point(255,  10), 6, Scalar(255, 255, 255), thickness, lineType);
    circle(image, Point(501, 255), 6, Scalar(255, 255, 255), thickness, lineType);
    circle(image, Point( 10, 501), 6, Scalar(255, 255, 255), thickness, lineType);
	// Show support vectors
    thickness = 2;
    lineType  = 8;
    // The method returns all the support vectors as a floating-point matrix, 
    // where support vectors are stored as matrix rows.
    Mat SupportVectorsMat = svm->getSupportVectors();
    for (int r = 0; r < SupportVectorsMat.rows; r++) {
        float* data = SupportVectorsMat.ptr<float>(r);
        cout << r << "	" << data[0] << "	" << data[1] << endl;
        circle(image, Point((int)data[0], (int)data[1]), 6, Scalar(255, 0, 255), thickness, lineType);
    }
	imshow("SVM Simple Example", image); // show it to the user
    waitKey(0);
	
	cout << "\n" << ">> ----" << endl;
	return 0;
}
```
注意代码中这几行：

```python
if (0)
		svm->setKernel(ml::SVM::LINEAR);
	else{
		svm->setKernel(ml::SVM::POLY);
		svm->setDegree(1.0); 
	}
```
如果核函数用ml::SVM::LINEAR，下边Mat SupportVectorsMat = svm->getSupportVectors();将无法正常输出支持向量。这并非BUG，官方解释：

```python
https://github.com/Itseez/opencv/blob/2.4/modules/ml/src/svm.cpp#L1531
```
将所有支持向量压缩为了一个，这样做可以极大地优化线性核时的运行效率。





