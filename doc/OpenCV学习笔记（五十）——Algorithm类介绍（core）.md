# OpenCV学习笔记（五十）——Algorithm类介绍（core）

2012年05月04日 13:25:03

yang_xian521

阅读数：27499

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

刚开始学习新的2.4.0，大概看了一遍使用手册，还是有些失望的，很多章节还是2.3.1的内容，文档上的代码也是漏洞很多。这里就简单介绍一下Algorithm这个新的基类。本来以为这部分是一个新模块的，看了referenceManual后才知道它只是一个基类，集成了一些相对复杂的算法，比如BM算法等立体匹配算法，前景背景分离的算法，光流法等模块都集成在其中。类内成员函数主要有get、set、writhe、read、getList、create这些函数。前4个是用来从字符串和XML文件中读取算法的参数用的，使用get，set要注意参数的名字对应的类型要和数据的类型对应，后果大家肯定懂的。有了Algorithm::write，以后SITF::wirte啊SURF::write啊，神马的都可以再见了。getList可以得到Algorithm支持的算法类型。我测试了一下getList目前getList的算法有：BRIEF、Dense、FAST、CFIT、HARRIS、MSER、ORB、SIFT、STAR、SURF，日后应该都会实现，在手册上只有SURF和SIFT的介绍，至少目前，我看来只支持这两个算法。create就是创建算法的函数。使用呢，也相对简单。这里还是要说明一下，新版的features2d模块有较大改动，反正我看文档中该部分大多剩下的都是一些公共接口，比如特征描述子、描述子的提取、特征的匹配等。只有FAST、MSER、ORB还在文档中坚挺的出现（估计日后也要移植走），以前的Star、randomTree、rTreeClassifer神马该被遗弃的一起，该放ml的去ml，SIFT、SURF啥的也被弄到nonfree这个新模块里去了，不过这个新模块好弱啊，目前只有SIFT和SURF算法，估计以后还要把其他的算法整合进去。

接下来我就根据文档中的一个示例实践一下新版本的SIFT特征点检测算法，并且对比下之前版本的SITF的写法，大家从中感受新结构的不同吧。



```cpp
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
 
using namespace cv;
 
void main()
{
	Mat image = imread("church01.jpg");
	Mat imageGray = imread("church01.jpg", 0);
	Mat descriptors;
	vector<KeyPoint> keypoints;
	// 新版本2.4.0方法
	initModule_nonfree();
 
	Ptr<Feature2D> sift1 = Algorithm::create<Feature2D>("Feature2D.SIFT");
	sift1->set("contrastThreshold", 0.01f);
	(*sift1)(imageGray, noArray(), keypoints, descriptors);
 
	// 2.3.1方法
// 	SiftFeatureDetector sift2(0.06f, 10.0);
// 	sift2.detect(imageGray, keypoints);
	
	drawKeypoints(image, keypoints, image, Scalar(255,0,0));
 
	imshow("test", image);
	waitKey();
}
```

 注意initModule_<modulename>()函数，这个函数要在create前使用，才可以动态的创建算法，不然那个create的指针很野哦。大家都懂的。如果要使用SURF、SIFT算法，就要调用

inintModule_nonfree()

,要使用EM算法，就要先调用

initModule_ml

()。 





其实在我看来，这个Algorithm类更重要的是为开发者的算法提供一个公共的接口，以后开发者开发的算法，想要增加到OpenCV中去，就按照Algorithm的接口做就OK了，以免以后算法越来越多，我们的OpenCV也要变得臃肿了。

创建自己的算法类现在也很容易了，用Algorithm类做基类，算法的参数作为成员变量，用get函数调用，添加AlgorithmInfo*  info()  const到自己的算法类中去，添加构造函数，AlgorithmInfo变量，可以参考EM算法的初始化看如何操作，添加其他函数。做好后就用create调用你自己的函数就ok啦，这次先简单介绍，以后有机会自己做个函数了，体会深了再详细写出来跟大家分享。