# OpenCV学习笔记（二十六）——小试SVM算法ml

2011年11月15日 15:07:54

yang_xian521

阅读数：51258

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

总感觉自己停留在码农的初级阶段，要想更上一层，就得静下心来，好好研究一下算法的东西。OpenCV作为一个计算机视觉的开源库，肯定不会只停留在数字图像处理的初级阶段，我也得加油，深入研究它的算法库。就从ml入手吧，最近做东西遇到随机森林，被搞的头大，深深感觉自己肚子里货太少，关键时刻调不出东西来。切勿浮躁，一点点研究吧。

这次就先介绍一下机器学习中的一个常用算法SVM算法，即支持向量机Support Vector Machine（SVM），是一种有监督学习方法，更多介绍请见维基百科<http://zh.wikipedia.org/wiki/SVM>。

OpenCV开发SVM算法是基于LibSVM软件包开发的，LibSVM是台湾大学林智仁(Lin Chih-Jen)等开发设计的一个简单、易于使用和快速有效的SVM模式识别与回归的软件包。用OpenCV使用SVM算法的大概流程是

### 1）设置训练样本集

需要两组数据，一组是数据的类别，一组是数据的向量信息。

### 2）设置SVM参数

利用CvSVMParams类实现类内的成员变量svm_type表示SVM类型：

CvSVM：：C_SVC  C-SVC

CvSVM：：NU_SVC v-SVC

CvSVM：：ONE_CLASS 一类SVM

CvSVM：：EPS_SVR e-SVR

CvSVM：：NU_SVR v-SVR

成员变量kernel_type表示核函数的类型：

CvSVM：：LINEAR 线性：u‘v

CvSVM：：POLY 多项式：(r*u'v + coef0)^degree

CvSVM：：RBF RBF函数：exp(-r|u-v|^2)

CvSVM：：SIGMOID sigmoid函数：tanh(r*u'v + coef0)

成员变量degree针对多项式核函数degree的设置，gamma针对多项式/rbf/sigmoid核函数的设置，coef0针对多项式/sigmoid核函数的设置，Cvalue为损失函数，在C-SVC、e-SVR、v-SVR中有效，nu设置v-SVC、一类SVM和v-SVR参数，p为设置e-SVR中损失函数的值，class_weightsC_SVC的权重，term_crit为SVM训练过程的终止条件。其中默认值degree  = 0，gamma = 1，coef0 = 0，Cvalue = 1，nu =  0，p = 0，class_weights = 0

### 3）训练SVM

调用CvSVM：：train函数建立SVM模型，第一个参数为训练数据，第二个参数为分类结果，最后一个参数即CvSVMParams

### 4）用这个SVM进行分类

调用函数CvSVM：：predict实现分类

### 5）获得支持向量

除了分类，也可以得到SVM的支持向量，调用函数CvSVM：：get_support_vector_count获得支持向量的个数，CvSVM：：get_support_vector获得对应的索引编号的支持向量。

实现代码如下：



```cpp
	// step 1:
	float labels[4] = {1.0, -1.0, -1.0, -1.0};
	Mat labelsMat(3, 1, CV_32FC1, labels);
	
	float trainingData[4][2] = { {501, 10}, {255, 10}, {501, 255}, {10, 501} };
	Mat trainingDataMat(3, 2, CV_32FC1, trainingData);
 
	// step 2:
	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
	
	// step 3:
	CvSVM SVM;
	SVM.train(trainingDataMat, labelsMat, Mat(), Mat(), params);
	
	// step 4:
	Vec3b green(0, 255, 0), blue(255, 0, 0);
	for (int i=0; i<image.rows; i++)
	{
		for (int j=0; j<image.cols; j++)
		{
			Mat sampleMat = (Mat_<float>(1,2) << i,j);
			float response = SVM.predict(sampleMat);
 
			if (fabs(response-1.0) < 0.0001)
			{
				image.at<Vec3b>(j, i) = green;
			}
			else if (fabs(response+1.0) < 0.001)
			{
				image.at<Vec3b>(j, i) = blue;
			}
		}
	}
 
	// step 5:
	int c = SVM.get_support_vector_count();
 
	for (int i=0; i<c; i++)
	{
		const float* v = SVM.get_support_vector(i);
	}
```