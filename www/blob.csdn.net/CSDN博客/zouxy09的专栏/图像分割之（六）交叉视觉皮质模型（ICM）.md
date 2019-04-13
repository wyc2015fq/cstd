
# 图像分割之（六）交叉视觉皮质模型（ICM） - zouxy09的专栏 - CSDN博客


2013年10月28日 20:24:10[zouxy09](https://me.csdn.net/zouxy09)阅读数：14137


**图像分割之（六）交叉视觉皮质模型（ICM）**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
我以前是不知道这个图像分割方法的。之前有个朋友看到我之前图像分割系列的博文，然后就和我说有这么一个东西。所以当时我就稍微看了下。主要还是参考下面这篇论文的，然后按照论文所说的算法自己实现了一部分的代码（没有实现熵的那部分）。
牛建伟等，《基于修正交叉视觉皮质模型的图像分割方法》，北京邮电大学学报，2010
上述论文中提到，交叉视觉皮质模型(ICM)主要基于Eckhorn模型和Rybak模型演化而来，同时吸收了其他视觉模型的优点，是多种脑皮层模型交叉综合的产物。ICM是一种简化的脉冲耦合神经网络（PCNN）模型。PCNN称为第3代人工神经网络。ICM由于保留了PCNN脉冲耦合、变阈值、同步脉冲发放等特性，十分适合图像处理。哈哈，看不太懂，看似很先进的样子。
**一、ICM模型**
ICM是单层神经网络，其单个神经元模型由接收、调制和脉冲产生三大部分组成，如图所示：
![](https://img-blog.csdn.net/20131028202154078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
直接拿图像分割和公式来说明会好一点。下图的公式（1~3）就构成了ICM模型。图像中的每一个像素就是ICM模型的一个神经元，所以每个神经元的标号是两维的，例如第(i, j)个神经元。这里有点意思的是，平时俺们的神经网络都是把输入图像拉成一列当成神经网络的输入的，可以说是一维的，但ICM模型却是2-D的神经元排布，这点当时好像可以给我点什么启发的，但到现在，这点启发对我来说还没办法让我脑袋的某个神经元兴奋。呵呵，扯远了。
![](https://img-blog.csdn.net/20131028202212359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**（1）接收部分：**
包括线性连接输入和反馈输入2部分，线性连接输入接收局部相邻神经元的输入；反馈输入接收来自外部的刺激输入。对于一张图像来说，每个像素就是一个神经元，表示为对应图像上的位置(i,j)，它的反馈输入Sij实际上就是每个像素的像素值。像素的灰度值越大，那么这个神经元的反馈输入越大。而对于图像分割，我们还得考虑到邻域像素的影响，一般来说，属于同一个物体，它的颜色就很像，不同物体相接的地方，就是边缘，相邻像素的颜色一般差别蛮大的，所以这个信息对图像分割来说异常重要。在这里，我们怎么用到这个信息呢？就是线性连接输入W{Y}ij了。它反映的是这个神经元与相邻神经元的连接强度，他们的颜色相似性越小，那么他们是同一个物体的可能性就越大，那么他们的连接强度就越大，线性连接输入也就越大。如公式（6）所示，这里考虑的是3x3的邻域。两个神经元或者像素(i, j)和(k, l)距离越近，相似度越大，属于同一个物体的可能性就越大，那么这个值越大。所以神经元(i, j)和(k, l)的连接权值Wij-kl=负灰度差异/两点欧式距离。
**（2）调制部分：**
调制部分将神经元的线性连接输入和反馈输入进行调制得到神经元的内部活动状态。如公式（1）所示，Fij[n]表示第n个时间（也就是第n次迭代）的时候ICM模型的内部活动状态值，这个值随着迭代的进行不断更新，它有个遗忘因子f。
**（3）脉冲产生部分**：
脉冲产生部分比较内部活动状态和动态阈值，若内部活动状态超过动态阈值Eij[n]，则产生脉冲。大则输出1，小的输出0，这样输入一幅图像，就会输出一幅同样大小的二值图，这个二值图就是每一次迭代的分割结果，最好的情况，当然是目标全是1，背景像素全是0了。如公式（2）所示。每个神经元（每个像素）都会保存各自的内部活动状态和各自的动态阈值。而且动态阈值是以公式（3）随着迭代的进行而更新的。当神经元内部状态值超过动态阈值时，输出1个脉冲，此时神经元的阈值因反馈迅速提高，阈值高于内部状态值，输出为0。之后阈值开始衰减下降，当阈值低于内部状态值，神经元再次点火而输出脉冲，如此反复循环。显然，点火的神经元又通过连接输入而影响其他神经元的点火状态。
上面说到，应用ICM进行图像分割时，每个像素点对应1个神经元，每个神经元与邻域神经元连接（一般领域取3×3），构成单层2维局部连接网络。归一化的像素值作为神经元的外部输入，因而亮度大的像素点对应的神经元首先点火。有些首先点火神经元的邻域神经元由于接收连接输入，提高内部状态值而提前点火，这种脉冲传递使亮度相似空间相邻的神经元发出同步脉冲。同步发放脉冲的神经元称为神经元集群，该神经元集群对应图像上不同的区域。放脉冲输出包含图像的区域、纹理和边缘等特征信息，可实现图像分割。
**二、代码实现**
我的代码是基于VS2010+ OpenCV 2.4.2的。代码实现主要参考论文中的算法描述。但步骤5后面（通过计算原图像与分割后图像间的互信息量客观评价图像的分割效果）就没有实现。具体代码如下：
ICM4Segmentation.cpp

```cpp
/******************************************************************/
// Image segmentation by ICM
// Author : zouxy
// Date   : 2013-8-23
// HomePage : http://blog.csdn.net/zouxy09
// Email  : zouxy09@qq.com
// Reference: 牛建伟等，基于修正交叉视觉皮质模型的图像分割方法
/******************************************************************/
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace std;
using namespace cv;
int main( int argc, char** argv )
{
	// read image
	Mat img = imread("hand.jpg");
	if (img.empty())
	{
		cout<<"Read image fail!"<<endl;
		return 1;
	}
	imshow("input", img);
	// step 1: initialize parameters
	const int MAX_ITERATION = 20;
	float h = 20;
	float delta = 1 / MAX_ITERATION;
	Mat net = Mat::zeros(img.rows, img.cols, CV_32FC1);		// F
	Mat output= Mat::zeros(img.rows, img.cols, CV_32FC1);	// Y
	Mat threshold = Mat::ones(img.rows, img.cols, CV_32FC1);// E
	Mat image = Mat::zeros(img.rows, img.cols, CV_32FC1);	// S
	vector<Mat> iteraOutput;
	// step 2: normalize image to [0, 1.0]
	Mat temp;
	cvtColor(img, temp, CV_RGB2GRAY);
	double maxP, minP;
	minMaxLoc(temp, &minP, &maxP);
	temp.convertTo(image, CV_32FC1, 1.0 / (maxP - minP), - minP / (maxP - minP));
	// step 3: start iteration
	for (int k = 1; k <= MAX_ITERATION; k++)
	{
		cout<<" Iteration: "<< k <<endl;
		// formula (7)
		float lamda = 1.0 / (MAX_ITERATION + k);
		for (int i = 1; i < image.rows - 1; i++)
		{
			for (int j = 1; j < image.cols - 1; j++)
			{
				// formula (6)
				float weight = (1 - abs(image.at<float>(i, j) - image.at<float>(i - 1, j))) +
								(1 - abs(image.at<float>(i, j) - image.at<float>(i + 1, j))) +
								(1 - abs(image.at<float>(i, j) - image.at<float>(i, j - 1))) +
								(1 - abs(image.at<float>(i, j) - image.at<float>(i, j + 1))) +
								0.5 * (1 - abs(image.at<float>(i, j) - image.at<float>(i - 1, j - 1))) +
								0.5 * (1 - abs(image.at<float>(i, j) - image.at<float>(i - 1, j + 1))) +
								0.5 * (1 - abs(image.at<float>(i, j) - image.at<float>(i + 1, j - 1))) +
								0.5 * (1 - abs(image.at<float>(i, j) - image.at<float>(i + 1, j + 1)));
				// formula (4)
				net.at<float>(i, j) = net.at<float>(i, j) - lamda + image.at<float>(i, j) + weight;
				// formula (5)
				threshold.at<float>(i, j) = threshold.at<float>(i, j) - delta + h * output.at<float>(i, j);
				// formula (2)
				output.at<float>(i, j) = net.at<float>(i, j) > threshold.at<float>(i, j) ? 1.0 : 0.0;
			}
		}
		iteraOutput.push_back(255 * output);
	}
	// step 4: create a big picture to show all iteration process result
	Mat showImg = Mat::zeros(img.rows * 4, img.cols * 5, CV_32FC1);	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			Rect roi(img.cols * j, img.rows * i, img.cols, img.rows);
			resize(iteraOutput[i * 4 + j], showImg(roi), roi.size());
		}
	}
	imshow("output", showImg);
	waitKey(-1);
	return 0;
}
```

**三、结果**
输入的待分割图像：
![](https://img-blog.csdn.net/20131028202310515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
迭代20次，每次的迭代结果：
![](https://img-blog.csdn.net/20131028202333390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**四、分析**
要分割的图像挺简单的，但分割出来的效果感觉不咋地（当然了，如果我的代码有错，还望大家指出，谢谢）。我在想，这个迭代其实是不是一个能量最小化的过程呢？如果不是能量最小化的过程，那么迭代的意义在哪呢？难道就像是在穷举先验分割结果，然后挑出一个好的吗？那可不可以将其纳入到某种能量最小化的框架里面，然后进行迭代优化呢？每次的迭代，能量都会变小，能量最小的时候，对应的就是最有的分割结果。就像一些图割算法一样。
了解有限，学识有限，还望大家不吝指点。谢谢。

**五、后话**
有位朋友发现了上面代码的一个错误（感谢这位朋友），也就是论文的公式（4），计算weight的时候，我少乘了Y(n-1)，然后修改后感觉分割效果更不咋的，哈哈。修改后代码如下：

```cpp
/******************************************************************/
// Image segmentation by ICM
// Author : zouxy
// Date   : 2013-11-28
// HomePage : http://blog.csdn.net/zouxy09
// Email  : zouxy09@qq.com
// Reference: 牛建伟等，基于修正交叉视觉皮质模型的图像分割方法
/******************************************************************/
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace std;
using namespace cv;
int main( int argc, char** argv )
{
	// read image
	Mat img = imread("hand.jpg");
	if (img.empty())
	{
		cout<<"Read image fail!"<<endl;
		return 1;
	}
	imshow("input", img);
	// step 1: initialize parameters
	const int MAX_ITERATION = 20;
	float h = 20;
	float delta = 1 / MAX_ITERATION;
	Mat net = Mat::zeros(img.rows, img.cols, CV_32FC1);		// F
	Mat output= Mat::zeros(img.rows, img.cols, CV_32FC1);	// Y
	Mat threshold = Mat::ones(img.rows, img.cols, CV_32FC1);// E
	Mat image = Mat::zeros(img.rows, img.cols, CV_32FC1);	// S
	vector<Mat> iteraOutput;
	// step 2: normalize image to [0, 1.0]
	Mat temp;
	cvtColor(img, temp, CV_RGB2GRAY);
	double maxP, minP;
	minMaxLoc(temp, &minP, &maxP);
	temp.convertTo(image, CV_32FC1, 1.0 / (maxP - minP), - minP / (maxP - minP));
	// step 3: start iteration
	for (int k = 1; k <= MAX_ITERATION; k++)
	{
		cout<<" Iteration: "<< k <<endl;
		// formula (7)
		float lamda = 1.0 / (MAX_ITERATION + k);
		Mat preOutput;
		if (k == 1)
			output.copyTo(preOutput);
		else
			preOutput = iteraOutput[iteraOutput.size()-1];
		for (int i = 1; i < image.rows - 1; i++)
		{
			for (int j = 1; j < image.cols - 1; j++)
			{
				// formula (6)
				float weight = (1 - abs(image.at<float>(i, j) - image.at<float>(i - 1, j))) * preOutput.at<float>(i - 1, j) +
								(1 - abs(image.at<float>(i, j) - image.at<float>(i + 1, j))) * preOutput.at<float>(i + 1, j) +
								(1 - abs(image.at<float>(i, j) - image.at<float>(i, j - 1))) * preOutput.at<float>(i, j - 1) +
								(1 - abs(image.at<float>(i, j) - image.at<float>(i, j + 1))) * preOutput.at<float>(i, j + 1) +
								0.5 * (1 - abs(image.at<float>(i, j) - image.at<float>(i - 1, j - 1))) * preOutput.at<float>(i - 1, j - 1) +
								0.5 * (1 - abs(image.at<float>(i, j) - image.at<float>(i - 1, j + 1))) * preOutput.at<float>(i - 1, j + 1) +
								0.5 * (1 - abs(image.at<float>(i, j) - image.at<float>(i + 1, j - 1))) * preOutput.at<float>(i + 1, j - 1) +
								0.5 * (1 - abs(image.at<float>(i, j) - image.at<float>(i + 1, j + 1))) * preOutput.at<float>(i + 1, j + 1);
				// formula (4)
				net.at<float>(i, j) = net.at<float>(i, j) - lamda + image.at<float>(i, j) + weight;
				// formula (5)
				threshold.at<float>(i, j) = threshold.at<float>(i, j) - delta + h * output.at<float>(i, j);
				// formula (2)
				output.at<float>(i, j) = net.at<float>(i, j) > threshold.at<float>(i, j) ? 1.0 : 0.0;
			}
		}
		iteraOutput.push_back(255 * output);
	}
	// step 4: create a big picture to show all iteration process result
	Mat showImg = Mat::zeros(img.rows * 4, img.cols * 5, CV_32FC1);	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			Rect roi(img.cols * j, img.rows * i, img.cols, img.rows);
			resize(iteraOutput[i * 4 + j], showImg(roi), roi.size());
		}
	}
	imshow("output", showImg);
	waitKey(-1);
	return 0;
}
```
效果图如下：
![](https://img-blog.csdn.net/20131128163944078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



