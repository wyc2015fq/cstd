# 图像局部特征（十八）--BOW - 工作笔记 - CSDN博客





2016年07月19日 10:11:27[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7994
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









**原文：**

**http://blog.sina.com.cn/s/blog_4cb0b54301014hxu.html**

**Bag of Word**， 顾名思义，即将某些Word打包，就像我们经常会把类似的物品装到一个柜子，或者即使是随意打包一些物品，也是为了我们能够方便的携带，在对大数据作处理的时候，为了能够方便的携带这些数据中的信息，与其一个一个的处理，还不如打包来的容易一点。

**Bag of Word** 因其理论简单，易懂，在**vision**界也得到了广泛的应用，有人将**Bag of Word**改成**Bag of Visual Word**来提出，充其量只是炒炒概念罢了，其基本的思想还是**BOW(Bag of Word)**。

      Anyway, 简单谈一些关于Bag of Word的东西：

**Bag of word实现步骤：**

**step 1:** 大数据聚类，找到适当的聚类中心点----**Vocabulary**。

**step 2:** 训练数据像聚类中心映射，得到每一个训练数据在该聚类中心空间的一个低维表示。

**step 3:** 得到每一个训练数据的低维表示后，选择适当的分类器训练。

**step 4:** 对新来的样本先映射到聚类中心空间，然后利用得到的分类器进行预测。



     在视觉领域，很希望能够利用已知的目标或者图像去得到相关的信息，这在机器人......等领域有着很广泛的应用，所以利用视觉领域中图像检索这一块解释一些基本问题。



**Vocabulary建立(数据聚类)：**

      以**SIFT 128**维特征作为例子。例如现在有1000张训练图片，对每一张训练图片都提取SIFT的128维特征，那么最终可以得到 N(i) * 128的特征，N(i)代表第几张图特征点的个数，因为图片并非完全相同，所以特征点的个数当然是不一样的。接下来就是建立词典的过程，利用现在常用的一些聚类算法，**kmeans**就OK，选择聚类中心点的个数，1000个也好， 10000个也罢，聚类完成后，得到的就是一个1000(10000) * 128聚类中心点的空间，称之为**词典**。

      这个**词典**就好比一个容器，通俗一点就是一个**直方图的基**，利用这个**基**去统计这些训练样本的个信息。



**训练样本的映射:**

      此时已经得到了一个**直方图的基**，如下图：

![Bag <wbr>of <wbr>Word闲谈](http://s6.sinaimg.cn/middle/4cb0b543gc6b90abdb895&690)

图1： 图中，n表示聚类中心点的个数，用BOW中的文字表示即使字典的个数。

      这些直方图的基在像是在空间的一些三维空间的**基向量i, j, k**， 利用这些**基向量**去构造别的向量，只需要知道特定的系数就行。所以接下来的步骤就是将原始的图像特征（SIFT 128维）向这些基向量作映射，得到相关的系数，如图:

![Bag <wbr>of <wbr>Word闲谈](http://s1.sinaimg.cn/middle/4cb0b543gc6bbd92a5f00&690)

图2：上图中给出了两个类别的直方图

      通过相关的映射，得到不同类别的一个类别的直方图的统计，这样整个BOW特征提取过程就算是完全实现了。接下来如何进行匹配，就是选择分类器的问题了。



**分类器的选择：**

      其实**BOW**之所以有bag的意思也是将相似的特征进行打包，得到统计的一个范围，这个范围就是直方图的某一个bin。在进行图像检索的时候，接下来就是进行分类器的训练与识别了，例如朴素贝叶斯分类器，支持向量机之类。一般利用BOW的时候，大多数人还是选择支持向量机这玩意，有实验证明**BOW**结合**SVM**效果要好于其他的分类器。不过，我没做过对比实验，这里也算提一下。



**新来样本的识别：**

      在训练好分类器后，对于新来的样本，同样先提取SIFT特征，然后将SIFT特征映射到上面图1中的词典中去，然后得到的直方图就可以通过分类器进行分类了。如：

![Bag <wbr>of <wbr>Word闲谈](http://s6.sinaimg.cn/bmiddle/4cb0b543gc6bc027bdb45&690)

图3 新图片的BOW直方图特征



      上图是一张新图映射到词典时得到的直方图，可以看出，这张图片相对于图2的情况而言，更接近类别1，所以通过分类器，理想的装填应该是判断为1。 但是我们都知道，理想状态的出现可能性太小，所以**BOW**难免会有出错的时候，通过阅读几篇论文，发现现在**BOW**的识别率大概在**60%-80%**之间，当然了一方面是数据量巨大的问题，另外一方面也是因为图像之间的相似度高。所以整体来讲，**BOW**的识别率还是再可以接受的范围。



**心得：**

      其实**BOW**没有什么特别的理论推导，我觉得仅仅只是将类似SIFT、HOG这些局部特征的统计方法从微观扩展到宏观的过程，利用直方图的统计的特性，构造多个词典，利用简单的距离映射，得到每一副图片的BOW的特征，但是这样一个简单的扩展确实最重要的创新点，同时也构造了一个广泛应用的框架。



**参考文献：**

**[1] C.Dance, J.Willamowski, L.Fan, C.Bray, G.Csurka, Visual categorization with Bags of Keypoints, in: ECCV International Workshop on Statistical Learning in Computer Vision. Prague, 2004**

**[2] **[**http://en.wikipedia.org/wiki/Bag_of_words_model**](http://en.wikipedia.org/wiki/Bag_of_words_model)



one Demo:

```cpp
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"

#include <iostream>

using namespace cv;
using namespace std;

#define ClusterNum 10

void DrawAndMatchKeypoints(const Mat& Img1, const Mat& Img2, const vector<KeyPoint>& Keypoints1,
	const vector<KeyPoint>& Keypoints2, const Mat& Descriptors1, const Mat& Descriptors2)
{
	Mat keyP1, keyP2;
	drawKeypoints(Img1, Keypoints1, keyP1, Scalar::all(-1), 0);
	drawKeypoints(Img2, Keypoints2, keyP2, Scalar::all(-1), 0);
	putText(keyP1, "drawKeyPoints", cvPoint(10, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar::all(-1));
	putText(keyP2, "drawKeyPoints", cvPoint(10, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar::all(-1));
	imshow("img1 keyPoints", keyP1);
	imshow("img2 keyPoints", keyP2);

	Ptr<DescriptorMatcher> descriptorMatcher = DescriptorMatcher::create("BruteForce");
	vector<DMatch> matches;
	descriptorMatcher->match(Descriptors1, Descriptors2, matches);
	Mat show;
	drawMatches(Img1, Keypoints1, Img2, Keypoints2, matches, show, Scalar::all(-1), CV_RGB(255, 255, 255), Mat(), 4);
	putText(show, "drawMatchKeyPoints", cvPoint(10, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar::all(-1));
	imshow("match", show);
}

//测试OpenCV：class BOWTrainer
void BOWKeams(const Mat& img, const vector<KeyPoint>& Keypoints,
	const Mat& Descriptors, Mat& centers)
{
	//BOW的kmeans算法聚类;
	BOWKMeansTrainer bowK(ClusterNum,
		cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 0.1), 3, 2);
	centers = bowK.cluster(Descriptors);
	cout << endl << "< cluster num: " << centers.rows << " >" << endl;

	Ptr<DescriptorMatcher> descriptorMatcher = DescriptorMatcher::create("BruteForce");
	vector<DMatch> matches;
	descriptorMatcher->match(Descriptors, centers, matches);//const Mat& queryDescriptors, const Mat& trainDescriptors第一个参数是待分类节点，第二个参数是聚类中心;
	Mat demoCluster;
	img.copyTo(demoCluster);

	//为每一类keyPoint定义一种颜色
	Scalar color[] = { CV_RGB(255,255,255),
		CV_RGB(255,0,0),CV_RGB(0,255,0),CV_RGB(0,0,255),
		CV_RGB(255,255,0),CV_RGB(255,0,255),CV_RGB(0,255,255),
		CV_RGB(123,123,0),CV_RGB(0,123,123),CV_RGB(123,0,123) };


	for (vector<DMatch>::iterator iter = matches.begin(); iter != matches.end(); iter++)
	{
		cout << "< descriptorsIdx:" << iter->queryIdx << "  centersIdx:" << iter->trainIdx
			<< " distincs:" << iter->distance << " >" << endl;
		Point center = Keypoints[iter->queryIdx].pt;
		circle(demoCluster, center, 2, color[iter->trainIdx], -1);
	}
	putText(demoCluster, "KeyPoints Clustering: 一种颜色代表一种类型",
		cvPoint(10, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar::all(-1));
	imshow("KeyPoints Clusrtering", demoCluster);

}




int main()
{
	cv::initModule_nonfree();//使用SIFT/SURF create之前，必须先initModule_<modulename>(); 

	cout << "< Creating detector, descriptor extractor and descriptor matcher ...";
	Ptr<FeatureDetector> detector = FeatureDetector::create("SIFT");

	Ptr<DescriptorExtractor> descriptorExtractor = DescriptorExtractor::create("SIFT");

	Ptr<DescriptorMatcher> descriptorMatcher = DescriptorMatcher::create("BruteForce");



	cout << ">" << endl;

	if (detector.empty() || descriptorExtractor.empty())
	{
		cout << "Can not create detector or descriptor exstractor or descriptor matcher of given types" << endl;
		return -1;
	}
	cout << endl << "< Reading images..." << endl;
	Mat img1 = imread("box.png");
	Mat img2 = imread("box_in_scene.png");
	cout << endl << ">" << endl;


	//detect keypoints;
	cout << endl << "< Extracting keypoints from images..." << endl;
	vector<KeyPoint> keypoints1, keypoints2;
	detector->detect(img1, keypoints1);
	detector->detect(img2, keypoints2);
	cout << "img1:" << keypoints1.size() << " points  img2:" << keypoints2.size()
		<< " points" << endl << ">" << endl;

	//compute descriptors for keypoints;
	cout << "< Computing descriptors for keypoints from images..." << endl;
	Mat descriptors1, descriptors2;
	descriptorExtractor->compute(img1, keypoints1, descriptors1);
	descriptorExtractor->compute(img2, keypoints2, descriptors2);

	cout << endl << "< Descriptoers Size: " << descriptors2.size() << " >" << endl;
	cout << endl << "descriptor's col: " << descriptors2.cols << endl
		<< "descriptor's row: " << descriptors2.rows << endl;
	cout << ">" << endl;

	//Draw And Match img1,img2 keypoints
	//匹配的过程是对特征点的descriptors进行match;
	DrawAndMatchKeypoints(img1, img2, keypoints1, keypoints2, descriptors1, descriptors2);

	Mat center;
	//对img1提取特征点，并聚类
	//测试OpenCV：class BOWTrainer
	BOWKeams(img1, keypoints1, descriptors1, center);


	waitKey();

}
```







