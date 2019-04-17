# KNN（一）--简单KNN原理及实现 - 工作笔记 - CSDN博客





2016年07月21日 13:15:03[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6834








原文：

http://blog.csdn.net/damotiansheng/article/details/40628417

1. Knn算法介绍：

        百度百科：[http://baike.baidu.com/view/1485833.htm?from_id=3479559&type=syn&fromtitle=knn&fr=aladdin](http://baike.baidu.com/view/1485833.htm?from_id=3479559&type=syn&fromtitle=knn&fr=aladdin)，其内容如下：



邻近算法，或者说K最近邻(kNN，k-NearestNeighbor)分类算法可以说是整个数据挖掘分类技术中最简单的方法了。所谓K最近邻，就是k个最近的邻居的意思，说的是每个样本都可以用她最接近的k个邻居来代表。

kNN算法的核心思想是如果一个样本在特征空间中的k个最相邻的样本中的大多数属于某一个类别，则该样本也属于这个类别，并具有这个类别上样本的特性。该方法在确定分类决策上只依据最邻近的一个或者几个样本的类别来决定待分样本所属的类别。 


右图中，绿色圆要被决定赋予哪个类，是红色三角形还是蓝色四方形？如果K=3，由于红色三角形所占比例为2/3，绿色圆将被赋予红色三角形那个类，如果K=5，由于蓝色四方形比例为3/5，因此绿色圆被赋予蓝色四方形类。
[](http://baike.baidu.com/picture/1485833/1485833/0/b74124f3c4b8a34e352accfb?fr=lemma&ct=single)
KNN算法的决策过程



K最近邻(k-Nearest Neighbor，KNN)分类算法，是一个理论上比较成熟的方法，也是最简单的机器学习算法之一。该方法的思路是：如果一个样本在特征空间中的k个最相似(即特征空间中最邻近)的样本中的大多数属于某一个类别，则该样本也属于这个类别。KNN算法中，所选择的邻居都是已经正确分类的对象。该方法在定类决策上只依据最邻近的一个或者几个样本的类别来决定待分样本所属的类别。 KNN方法虽然从原理上也依赖于极限定理，但在类别决策时，只与极少量的相邻样本有关。由于KNN方法主要靠周围有限的邻近的样本，而不是靠判别类域的方法来确定所属类别的，因此对于类域的交叉或重叠较多的待分样本集来说，KNN方法较其他方法更为适合。

KNN算法不仅可以用于分类，还可以用于回归。通过找出一个样本的k个最近邻居，将这些邻居的属性的平均值赋给该样本，就可以得到该样本的属性。更有用的方法是将不同距离的邻居对该样本产生的影响给予不同的[权值](http://baike.baidu.com/view/2109980.htm)(weight)，如权值与距离成反比。


```cpp
#include "ml.h"
#include "highgui.h"

int main(int argc, char** argv)

{
	const int K = 10;

	int i, j, k, accuracy;

	float response;

	int train_sample_count = 100;

	CvRNG rng_state = cvRNG(-1);

	CvMat* trainData = cvCreateMat(train_sample_count, 2, CV_32FC1);

	CvMat* trainClasses = cvCreateMat(train_sample_count, 1, CV_32FC1);

	IplImage* img = cvCreateImage(cvSize(500, 500), 8, 3);

	float _sample[2];

	CvMat sample = cvMat(1, 2, CV_32FC1, _sample);

	cvZero(img);


	CvMat trainData1, trainData2, trainClasses1, trainClasses2;



	// form the training samples

	cvGetRows(trainData, &trainData1, 0, train_sample_count / 2);

	cvRandArr(&rng_state, &trainData1, CV_RAND_NORMAL, cvScalar(200, 200), cvScalar(50, 50));


	cvGetRows(trainData, &trainData2, train_sample_count / 2, train_sample_count);

	cvRandArr(&rng_state, &trainData2, CV_RAND_NORMAL, cvScalar(300, 300), cvScalar(50, 50));


	cvGetRows(trainClasses, &trainClasses1, 0, train_sample_count / 2);

	cvSet(&trainClasses1, cvScalar(1));


	cvGetRows(trainClasses, &trainClasses2, train_sample_count / 2, train_sample_count);

	cvSet(&trainClasses2, cvScalar(2));



	// learn classifier
	CvKNearest knn(trainData, trainClasses, 0, false, K);

	CvMat* nearests = cvCreateMat(1, K, CV_32FC1);

	for (i = 0; i < img->height; i++)

	{

		for (j = 0; j < img->width; j++)

		{

			sample.data.fl[0] = (float)j;

			sample.data.fl[1] = (float)i;



			// estimates the response and get the neighbors' labels

			response = knn.find_nearest(&sample, K, 0, 0, nearests, 0);



			// compute the number of neighbors representing the majority

			for (k = 0, accuracy = 0; k < K; k++)

			{

				if (nearests->data.fl[k] == response)

					accuracy++;

			}

			// highlight the pixel depending on the accuracy (or confidence)

			cvSet2D(img, i, j, response == 1 ?

				(accuracy > 5 ? CV_RGB(180, 0, 0) : CV_RGB(180, 120, 0)) :

				(accuracy > 5 ? CV_RGB(0, 180, 0) : CV_RGB(120, 120, 0)));

		}

	}



	// display the original training samples

	for (i = 0; i < train_sample_count / 2; i++)

	{

		CvPoint pt;

		pt.x = cvRound(trainData1.data.fl[i * 2]);

		pt.y = cvRound(trainData1.data.fl[i * 2 + 1]);

		cvCircle(img, pt, 2, CV_RGB(255, 0, 0), CV_FILLED);

		pt.x = cvRound(trainData2.data.fl[i * 2]);

		pt.y = cvRound(trainData2.data.fl[i * 2 + 1]);

		cvCircle(img, pt, 2, CV_RGB(0, 255, 0), CV_FILLED);

	}



	cvNamedWindow("classifier result", 1);

	cvShowImage("classifier result", img);

	cvWaitKey(0);



	cvReleaseMat(&trainClasses);

	cvReleaseMat(&trainData);

	return 0;

}
```






