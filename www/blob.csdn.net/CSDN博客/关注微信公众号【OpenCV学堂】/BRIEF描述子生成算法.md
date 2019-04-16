# BRIEF描述子生成算法 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2017年02月03日 23:07:07[gloomyfish](https://me.csdn.net/jia20003)阅读数：3588








**学习OpenCV关注微信公众号【OpenCV学堂】**

**一：介绍**

我们知道SIFT算法通常通过对每个关键点生成128个特征向量作为描述子、SURF算法通常对关键点生成最少64个特征向量作为描述子。但是对于图像来说创建上千或者上万个这样的描述子内存开销比较大，运行速度受到严重影响。特别对嵌入式设备与一定设备来说，内存限制尤为明显，而且匹配的时候计算也比较耗时。



但是实际上这些特征数据OpenCV在匹配的时候并没有完全利用上，而是通过PCA、LDA等方法对它进行压缩，或者是LSH(局部敏感哈希)方法把这些特征描述子压缩从浮点数转换为二进制字符串，然后通过汉明距离(HammingDistance)进行比较。这样就可以通过简单的异或操作（OXR）与位值计算来加速实现对象特征匹配，对于SSE指令集的CPU这种方法可以大大加速。但是这种方法仍然需要首先计算描述子，然后使用LSH方法进行压缩，无法避免过度的内存开销。



而Brief方法可以直接通过关键点生成二进制字符串，跳过了中间描述子生成步骤，这样就大大减低了内存要求与计算开销。Brief方法主要思路是对每个关键点附件选择若干个像素点，将这些像素点的像素值组合成二进制字符串，然后使用该字符串作为该关键点的描述子。

此方法是在2010年提出来的。结果实验测试在选取256个点甚至128个点情况下对没有旋转对象识别率非常高而且速度比SURF还快。但是当对象有旋转时候由于Brief不能很好的支持旋转不变性识别，特别是当旋转角度超过30度以上，准确率会快速下降。



**二：Brief描述子生成步骤**

Brief描述子生成首先需要产生足够多的随机点对，然后根据随机点对坐标得到对应像素值，对所有点对进行二进制字符串拼接，拼接完成即生成了描述子。

第一步：选择关键点周围SxS大小正方形图像区块，进行高斯模糊。这样做的原因是需要降低图像随机噪声，OpenCV在完成Brief的时候考虑到效率问题并没有采用高斯模糊而是采用了基于积分图的盒子模糊方法。

第二步：选择n个像素点对，其中n的取值常见为256、此外还可以是128、512。每个点对比较像素值输出如下。

![](https://img-blog.csdn.net/20170203230153174)




对N个点对完成操作最终得到了二进制字符串，表达如下：



![](https://img-blog.csdn.net/20170203230205410)

**三：方法**

高斯模糊比较

通过实验对比高斯sigma参数在0~3之间准确率比较高，窗口大小取值在9x9取得比较好的模糊去噪效果。论文中实验结果图示如下：

![](https://img-blog.csdn.net/20170203230217956)




随机点生成方法比较

对于随机生成点对的方法，论文中给出了五种随机方法与实验结果比较，五种方法描述如下：

![](https://img-blog.csdn.net/20170203230249128)


图示如下：
![](https://img-blog.csdn.net/20170203230304582)



对图像五个几何采样完成测试结果如下：
![](https://img-blog.csdn.net/20170203230329676)



其中第二种方法比其他四种稍微有点优势，而最不好的方法则是第五种方法。

**四：OpenCV中Biref描述子演示**

```cpp
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

int main(int argc, char** argv) {
	Mat img1 = imread("D:/tree.png", IMREAD_GRAYSCALE);
	Mat img2 = imread("D:/tree_in_scene.png", IMREAD_GRAYSCALE);
	Mat src = imread("D:/gloomyfish/14.png", IMREAD_GRAYSCALE);
	Mat src1 = imread("D:/gloomyfish/14.png");
	if (!img1.data || !img2.data) {
		printf("could not load images...\n");
		return -1;
	}
	imshow("box", img1);
	imshow("scene", img2);

	auto detector = FastFeatureDetector::create();
	vector<KeyPoint> keypoints_obj;
	vector<KeyPoint> keypoints_scene;
	detector->detect(img1, keypoints_obj, Mat());
	detector->detect(img2, keypoints_scene, Mat());

	auto descriptor = BriefDescriptorExtractor::create();
	Mat descriptor_obj, descriptor_scene;
	descriptor->compute(img1, keypoints_obj, descriptor_obj);
	descriptor->compute(img2, keypoints_scene, descriptor_scene);

	BFMatcher matcher(NORM_L2);
	vector<DMatch> matches;
	matcher.match(descriptor_obj, descriptor_scene, matches);

	Mat resultImg;
	drawMatches(img1, keypoints_obj, img2, keypoints_scene, matches, resultImg);
	imshow("Brief Descriptor Match Result", resultImg);

	waitKey(0);
	return 0;
}
```
运行结果
![](https://img-blog.csdn.net/20170203230452537)



