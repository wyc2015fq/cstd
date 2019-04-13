
# Adaboost人脸检测介绍（都是大白话） - 郭云飞的专栏 - CSDN博客


2017年09月22日 15:14:20[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：1463


**前言**
人脸检测属于计算机视觉的范畴，早期人们的主要研究方向是人脸识别，即根据人脸来识别人物的身份，后来在复杂背景下的人脸检测需求越来越大，人脸检测也逐渐作为一个单独的研究方向发展起来。目前的人脸检测方法主要有两大类：**基于知识和基于统计**。基于知识的方法主要利用先验知识将人脸看作器官特征的组合，根据眼睛、眉毛、嘴巴、鼻子等器官的特征以及相互之间的几何位置关系来检测人脸。基于统计的方法则将人脸看作一个整体的模式——二维像素矩阵，从统计的观点通过大量人脸图像样本构造人脸模式空间，根据相似度量来判断人脸是否存在。
**基于知识的人脸检测方法：**
Ø 模板匹配
Ø 人脸特征
Ø 形状与边缘
Ø 纹理特性
Ø 颜色特征
**基于统计的人脸检测方法：**
Ø 主成分分析与特征脸
Ø 神经网络方法
Ø 支持向量机
Ø 隐马尔可夫模型
Ø Adaboost算法
本文介绍的Haar分类器方法，包含了Adaboost算法，言归正传。
**Haar特征**
>> 最早的Haar特征由Papageorgiou C.等提出（《A general framework for object detection》）
>> 之后Paul Viola和Michal Jones提出利用**积分图像法**快速计算Haar特征的方法（《Rapid object detection using a boosted cascade of simple features》）
>> 之后Rainer Lienhart 和 Jochen Maydt用对角特征对Haar特征库进行了扩展（《An extended set of Haar-like features for rapid object detection》）
>> 目前OpenCV的Haar分类器就是基于扩展后的特征库实现的
Haar特征本身并不复杂，就是用图中白色矩形所有像素值的和减去黑色矩形所有像素值的和。目前Haar特征共有如下种：
![](https://img-blog.csdn.net/20170922151937625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**Haar特征个数的计算**
看过Rainer Lienhart文章的人知道，Rainer Lienhart在文章中给出了计算特定图像面积内Haar特征个数公式。
0°矩形Haar特征个数的计算公式：
![](https://img-my.csdn.net/uploads/201211/23/1353646336_9902.png)
其中*W*和*H*分别为图像的宽和高；**w**和*h*为矩形特征的尺寸（宽和高）；**X = W / w，Y = H / h**表示矩形特征在水平和垂直方向的能放大的最大比例系数。
45°Haar特征个数的计算公式：
![](https://img-my.csdn.net/uploads/201211/23/1353646944_2705.png)
其中**z = w+h, X = W / z, Y = H / h**（见下图）
![](https://img-blog.csdn.net/20170922152757982?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**举例来计算Haar特征的个数**
以1（a）特征为例，特征大小为2×1（*w×h*）；假设图像大小为24×24。则X = W / w = 24 / 2 = 12, Y = H / h = 24 / 1 = 24；套入0°Haar特征个数的公式计算可得：43200.0（浮点数运算）。那么，如何理解呢：
>> 缩放：
特征1（a）在水平方向放大的尺寸为：4×1、6×1、8×1、...24×1，共12个；垂直方向放大的尺寸为：2×2、2×3、2×4、...2×24，共24个；
>> 窗口在图像内滑动：
水平可滑动23步，垂直可滑动24步
用代码实现就是：

```python
int getHaarCount(int W,int H,int w,int h){
	int X=W/w;
	int Y=H/h;
	int count=0;
	// 缩放
	for (int i=1;i<=X;i++)
		for(int j=1;j<=Y;j++)
			//滑动
			for(int x=1;x<=W-i*w+1;x++)
				for(int y=1;y<=H-j*h+1;y++)
					count++;
	return count;
}
```
对于45°的Haar特征，原理类似。
**利用积分图像法快速计算Haar特征**
上边在计算1（a）Haar特征时，在24×24的图像上，该特征的个数为43200个；但Haar特征形状有14个；所有形状都用上的话，在24×24的图像上的Haar特征个数将达到115984个之多，远远大于图像的像素个数。前面提到Haar特征的值是矩形中白色区域所有像素值的和减去黑色区域所有像素值的和。如果要计算每个特征的像素和，计算量会非常大，而且很多次运算是重复的。
于是积分图（Integral image），也叫Summed Area Table，就派送用场了。简单说来，就是先构造一张“积分图”，之后任何一个Haar矩形特征都可以通过查表的方法（Look Up Table）和有限次简单运算得到，大大减少了运算次数。
将Haar特征的矩形表示为：
![](https://img-my.csdn.net/uploads/201211/24/1353727655_2637.png)
矩形内像素值之和表示为：
![](https://img-my.csdn.net/uploads/201211/24/1353727732_1166.png)
对于0°Haar特征来说，构造积分图：
![](https://img-my.csdn.net/uploads/201211/24/1353727992_9083.png)
积分图像中，每个点存储是其左上方所有像素之和：
![](https://img-my.csdn.net/uploads/201211/24/1353727809_7246.png)
其中I(x,y)表示图像(x,y)位置的像素值。
积分图像可以采用增量的方式计算：
![](https://img-my.csdn.net/uploads/201211/24/1353727870_2532.png)
初始边界：*SAT(-1,y)=SAT(x,-1)=SAT(-1,-1)=0*。所以，只需要对整张图像遍历一次就可以求得这张图的积分图像。
这样以来，利用积分图计算可计算矩形区域内像素和：
![](https://img-my.csdn.net/uploads/201211/24/1353728156_9326.png)

所以，无论矩形r的尺寸大小，只需查找积分图像4次就可以求得任意矩形内像素值的和。
![](https://img-blog.csdn.net/20170922160609538?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```python
void integral(unsigned char* inputMatrix, unsigned long* outputMatrix, int width, int height)
{  
    // calculate integral of the first line  
    for(int i=0;i<width;i++){  
        outputMatrix[i] = inputMatrix[i];  
        if(i>0){  
            outputMatrix[i] += outputMatrix[i-1];  
        }  
    }  
    for (int i=1;i<height;i++){  
        int offset = i*width;  
        // first column of each line  
        outputMatrix[offset] = outputMatrix[offset-width] + 
                               inputMatrix[offset];  
        // other columns   
        for(int j=1;j<width;j++){ 
            outputMatrix[offset + j] = outputMatrix[offset + j - 1] + 
                                       outputMatrix[offset - width] - 
                                       outputMatrix[offset - width - 1] + 
                                       inputMatrix[offset];   
        }  
    }  
    return ;  
}
```
对于45°的积分图像类似。
**特征值的标准化**
在这115984个Haar特征中，窗口较小的特征的值的取值范围可能很小；但窗口较大（还记得缩放吗？）的特征的值的取值范围可能就很大了。这种跨度较大的特性不利于量化评定特征值，所以要标准化。OpenCV采用如下方式“标准化”：
1. 计算检测窗口中间部分(w-2)*(h-2)的图像的灰度值和灰度值平方和：
![](https://img-blog.csdn.net/20151101125231931)
2. 计算均值
![](https://img-blog.csdn.net/20151101125238620)
3. 计算标准化因子：
![](https://img-blog.csdn.net/20151101125249228)
4. 标准化特征值：
![](https://img-blog.csdn.net/20151101125258015)
上边第一步中，为什么一定要缩进去一个边呢？作者猜测这可能是为了避免边缘像素不稳定，干扰标准化因子的计算。其实如何标准化并不重要，重要的是检测和训练时的方法一定要一致！否则可能会由于标准化不同带来的误差导致模型无法工作！
![](https://img-blog.csdn.net/20170224002224812)

**训练弱分类器**
为便于理解，假设现在有人脸样本2000张，非人脸样本4000张。这些样本都经过了归一化，大小都是24X24的图像。这样就有：
-- 总Haar特征数量：115984个
-- 每一个Haar特征计算出了6000个特征值
说明：
一个弱分类器，可能是单个Haar特征，此时有多少Haar特征就能训练出多少弱分类器；
一个弱分类器，也可能是若干个特征的级联，此时弱分类器的结构是二叉树结构（关于二叉决策CART自行查相关资料）。
为简单起见，此文的一个弱分类器就1个Haar特征。
所谓**训练一个弱分类器**就是：为该Haar特征确定一个分类阈值*T*；使得对所以样本（6000个）的分类错误率最低。
每个Haar特征都能训练出一个弱分类器（本文有115984个Haar特征，就有115984个弱分类器，每个弱分类器对于一个分类阈值**T**，以及其错误率**e**）。
最后，在所有115984个弱分类器中选出错误率**e**最低的那个，做为本轮训练的**最优分类器**（该分类器仍然是弱分类器，只不过分类效果要比随机猜测的好一丢丢罢了）。
++ 额，等等！刚才为了思维流畅，一下说到了最优分类器。回过头看有个问题还没解决：针对某个Haar特征，如何确定分类阈值*T*呢？
++ 为了更清楚的说明T的确定过程，我们不用6000个样本了。这里该用个2正样本，3个负样本，共5个样本。此时，针对某个Haar特征，就会有5个特征值（有多少个样本就有多少个特征值，之前6000个样本时，每个Haar特征有6000个特征值）。这5个特征值假设为——2正样本 -0.5、0.1；3个负样本： -0.4、 -0.3、0；
++ 接下来，我们把这5个特征值**按大小排序**（升序）：
++ 正样本1、负样本1、负样本2、负样本3、正样本2
++ 最好的分类器（decision stump）应该是正样本1和负样本1之间的一条线（此时线左边为正样本，右边为负样本）， 或者负样本3和正样本2之间的一条线（此时线左边为负样本，右边为正样本），对应错了一个样本。
++ 那么分类阈值T就是在-0.5 ~ -0.4之间取一个值，或者在0 ~ 0.1之间取一个值。在实际过程中由于样本权重不同，特征值每次排序都相同，但是相同的分类线对应的错误率就不同了。我们选择错误率小的其中一个值就行。
为便于理解，写了段伪代码来说明如何选出一个最优分类器：

```python
// 这是一段伪代码，仅用来说明逻辑
{
...
	std::vector<double> vHarrsError; // 记录每一个Haar特征的分类错误率
	for (int i = 0; i < HaarNum; i++) {	// HaarNum是总的Haar特征数量，本例为115984个
		// 样本结构体
		struct aSample {
		long nIdx;// 样本序号
		int nClass; // 样本类别，1人脸；-1为非人脸
		double dHaarValue; // 当前Haar特征，当前样本的值
		};
		std::vector<aSample> vSamp;
		for (int j = 0; j < SampNum; j++) { // SampNum是样本数量，本例为6000个
			aSample curSamp;
			curSamp.nIdx = j; // 当前样本的序号
			curSamp.nClass = ... // 当前样本的类别
			curSamp.dHaarValue = ... // 利用积分图计算出此值
			vSamp.pushback(curSamp);
		}
		dTotalFaceValue = 0.f; // 所以正样本的值综合
		for (int j = 0; j < SampNum; j++) {
			if (vSamp[j].nClass == 1) {
				dTotalFaceValue += vSamp[j].dHaarValue;
			}
		}
		dNoneTotalFaceValue = 0.f; // 所以负样本的值综合
		for (int j = 0; j < SampNum; j++) {
			if (vSamp[j].nClass == -1) {
				dNoneTotalFaceValue += vSamp[j].dHaarValue;
			}
		}
		std::sort(vSamp.begin(), vSamp.end()) // 所有样本升序排序
		std::vector<double> vError; // 
		for (int j = 0; j < SampNum; j++) {
			// 在此元素之前的人脸样本的权重的和
			LeftFaceValue = 0.f;
			for (int k = 0; k < j; k++) {
				if (vSamp[k].nClass == 1) 
					LeftFaceValue += vSamp[k].dHaarValue;
			}
			// 在此元素之前的非人脸样本的权重的和
			LeftNoneFaceValue = 0.f;
			for (int k = 0; k < j; k++) {
				if (vSamp[k].nClass == -1) 
					LeftNoneFaceValue += vSamp[k].dHaarValue;
			}
			// 以当前值为阈值T时的错误率
			curError = min(LeftFaceValue + (dNoneTotalFaceValue - LeftNoneFaceValue), 
						   LeftNoneFaceValue + (dTotalFaceValue - LeftFaceValue));
			vError.pushback(curError);
		}
		minError = ... // 在vError中找到最小值作为当前Haar特征的分类阈值
	}
	vHarrsError.pushback(minError);
	// 最后，在所有Haar特征中找到错误率最小的那个Haar特征，即最优弱分类器
	// vHarrsError中最小值所对应的那个Haar特征
...
}
```
**强分类器（用ababoost迭代算法实现）**
AdaBoost（Adaptive Boosting）是由Yoav Freund和Robert Schapire在1995年提出。它的自适应在于：前一个基本分类器分错的样本会得到加强，加权后的全体样本再次被用来训练下一个基本分类器。AdaBoost是一种迭代算法，在每一轮中加入一个新的弱分类器，直到达到某个预定的足够小的错误率。
每一个训练样本都被赋予一个权重。如果某个样本已经被准确地分类，那么在构造下一个训练集中，它被选中的概率就被降低；相反，如果某个样本没有被准确地分类，那么它的权重就得到提高。
下边结合本文说明用ababoost经过多轮迭代产生强分类器的过程：
给定训练集**{(x1,y1), x2,y2), ..., (xN,yN)}**；本例中，*xi*就是指一幅图像；而yi指这幅图像的类别（人脸or非人脸）；本例中有6000个样本，*N = 6000*。
1. 初始化训练集的权值分布。每一个训练样本最开始时都被赋予相同的权重：*1/N*。
![](https://img-blog.csdn.net/20141102234630160)
上式中**W**下标中，第一个数字表明了是第几轮迭代，第二个数字为样本的索引。
2. 假设我们要进行*M*轮迭代，也就是选出*M*个**最优弱分类器**，接下来开始迭代：
for (int m = 1; m <= M; m++) {
2.1 使用具有权值分布**Dm**的训练数据集学习，得到一个最优弱分类器：
![](https://img-blog.csdn.net/20141102234909561)
2.2 计算**Gm(x)**在训练数据集上的分类误差率：
![](https://img-blog.csdn.net/20141102235141318)
2.3 计算**α****m**，表示**Gm(x)**在最终分类器中的重要程度：
![](https://img-blog.csdn.net/20141102235307399)
由上述式子可知，*e**m <= 1/2*时，*am >= 0*，且**α****m**随着*e**m*的减小而增大，意味着分类误差率越小的基本分类器在最终分类器中的作用越大。
2.4 更新训练数据集的权值分布
![](https://img-blog.csdn.net/20141103000618960)
其中，*Z**m*是规范化因子，使得*D**m+1*成为一个概率分布：
![](https://img-blog.csdn.net/20141103000759596)
权值更新后，便使得被基本分类器*G**m(x)*误分类样本的权值增大，而被正确分类样本的权值减小。就这样，通过这样的方式，AdaBoost方法能**聚焦于**那些较难分的样本上。
}
3. 经过上边M轮迭代后，便可以组合强分类器：

![](https://img-blog.csdn.net/20141103001155359)
上式中，可以看到最终的强分类器是个轮选出的弱分类器的线性组合。

**检测**
检测过程中，就是通过在一幅图片中不断的调整检测窗口的位置、比例，来找到人脸。



















