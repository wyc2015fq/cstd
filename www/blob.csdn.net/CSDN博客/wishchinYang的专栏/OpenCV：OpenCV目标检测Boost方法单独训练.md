# OpenCV：OpenCV目标检测Boost方法单独训练 - wishchinYang的专栏 - CSDN博客
2017年03月22日 13:05:19[wishchin](https://me.csdn.net/wishchin)阅读数：1173
        在古老的CNN方法出现以后，并不能适用于图像中目标检测。20世纪60年代，Hubel和Wiesel( [百度百科](http://baike.baidu.com/link?url=to_m1AOYgTtPrdFgaov-3sZ7GkB-0_Ahdqxla3DwRqgcIghTMjWAIyjNCIVQddJkq7QS_sTpxoiR1RVUqwKE2-aBCIjqbcmsHZ9pswd0G1aB5d13eDd5YOXTGwW2IrzNGXef9hzBRmXvc-GZAZU7EK) )在研究猫脑皮层中用于局部敏感和方向选择的神经元时发现其独特的网络结构可以有效地降低反馈神经网络的复杂性，继而提出了卷积神经网络（Convolutional Neural Networks-简称CNN）。
        参考：[DNN的演进结构——CNN](http://blog.csdn.net/wishchin/article/details/45286805)
        AdaBoost，是英文"Adaptive Boosting"（自适应增强）的缩写，由Yoav Freund和Robert Schapire在1995年（[Adaboost原理与推导](http://blog.csdn.net/v_july_v/article/details/40718799)）提出。它的自适应在于：前一个基本分类器分错的样本会得到加强，加权后的全体样本再次被用来训练下一个基本分类器。同时，在 每一轮中加入一个新的弱分类器，直到达到某个预定的足够小的错误率或达到预先指定的最大迭代次数。
       不同于同属于集成算法的随机森林算法，随机森林使用了**模型平均**，而boost后一层是学习前一层的残差，因此产生了“boost”功能。
       boost算法的功能致力于降低模型**偏差**，增强学习器的**准确性**；随机森林使用模型平均，致力于降低模型的**方差**，增强模型的**泛化性能**。
       使用OpenCV作图像检测， Adaboost+haar训练过程，工程已经生成了opencv_haartraining.exe文件。
       打开OpenCV的源码工程，发现函数调用栈是这样的。
**1、对OpenCV源代码载入工程**
      源代码apps里面opencv_haartraining 工程的sources\apps\haartraining\haartraining.cpp
      main函数使用了cvCreateTreeCascadeClassifier(）函数，函数体在\sources\apps\haartraining\cvhaartraining.cpp文件
在cvhaartraining.h 前面添加 #define cvBoost_API __declspec (dllexport) 
函数名
```cpp
void cvCreateTreeCascadeClassifier(const char* dirname,
		const char* vecfilename,
		const char* bgfilename,
		int npos, int nneg, int nstages,
		int numprecalculated,
		int numsplits,
		float minhitrate, float maxfalsealarm,
		float weightfraction,
		int mode, int symmetric,
		int equalweights,
		int winwidth, int winheight,
		int boosttype, int stumperror,
		int maxtreesplits, int minpos, bool bg_vecfile = false);
```
修改为：
```cpp
#ifdef __cplusplus
extern "C"
{
#endif
	cvBoost_API void __stdcall cvCreateTreeCascadeClassifier(const char* dirname,
		const char* vecfilename,
		const char* bgfilename,
		int npos, int nneg, int nstages,
		int numprecalculated,
		int numsplits,
		float minhitrate, float maxfalsealarm,
		float weightfraction,
		int mode, int symmetric,
		int equalweights,
		int winwidth, int winheight,
		int boosttype, int stumperror,
		int maxtreesplits, int minpos, bool bg_vecfile = false);
}
```
对应函数体的头修改为：
```cpp
cvBoost_API void cvCreateTreeCascadeClassifier(const char* dirname,
//__declspec (dllimport) void __stdcall cvCreateTreeCascadeClassifier(const char* dirname,
                                    const char* vecfilename,
                                    const char* bgfilename,
                                    int npos, int nneg, int nstages,
                                    int numprecalculated,
                                    int numsplits,
                                    float minhitrate, float maxfalsealarm,
                                    float weightfraction,
                                    int mode, int symmetric,
                                    int equalweights,
                                    int winwidth, int winheight,
                                    int boosttype, int stumperror,
                                    int maxtreesplits, int minpos, bool bg_vecfile )
{
}
```
去掉opencv_haartraining 工程的main函数，
修改opencv_haartraining 工程exe 替换为 dll工程，则可以生成lib和dll文件，可添加引用。
（若不生成lib文件，则表示在opencv_haartraining_engine.lib  中导出了函数声明，需要引用此库）
函数体为：
```cpp
```
CNN 维基百科：[https://en.wikipedia.org/wiki/Convolutional_neural_network](https://en.wikipedia.org/wiki/Convolutional_neural_network)
Adaboost维基百科: [https//en.wikipedia.org/wiki/AdaBoost](https//en.wikipedia.org/wiki/AdaBoost)
