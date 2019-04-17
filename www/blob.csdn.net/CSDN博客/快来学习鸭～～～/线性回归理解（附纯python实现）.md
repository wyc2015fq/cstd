# 线性回归理解（附纯python实现） - 快来学习鸭～～～ - CSDN博客





置顶2017年03月27日 20:44:40[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：37497








**author：DivinerShi**

**本文地址：**[http://blog.csdn.net/sxf1061926959/article/details/66976356](http://blog.csdn.net/sxf1061926959/article/details/66976356)

线性回归是机器学习中最基本的一个算法，但是那些所谓的效果很好的算法也无非是从这些基础算法慢慢演变而来。高中时候的数学老师给我讲过一个乔峰的故事，我今天再添油加醋的给你们说下。天龙八部中，乔峰在聚贤庄大战江湖群雄这个算是经典了，当时各路武林豪杰纷纷使出自家的看门绝学，什么易筋经啊，九阴真经啊，葵花点穴手啊等等，但统统都被乔峰一拳KO，直接秒杀，竟无一人是其敌手，那乔峰用的是什么高深武学呢？其实他用的是拳法当作最为基础的一套拳法，名为长拳，有点拳法方面知识的人都知道，长拳其实一套最最基础的拳法，大家可以把它想成军训时候的匕首操。但就是这么个匕首操把这么多绝世武功都KO了。为啥？因为乔峰小时候在少林寺山脚住的时候，就开始苦练基本功，苦练长拳，并没用刻意的去最求一些更高水平的武学，这才将一套基础拳法发挥得如此淋漓尽致。

（**这个故事我也只听了个大概，上面我很大部分都自己瞎写的，我就是要说一个道理。**）
**第一**：基础很重要
**第二**：一些简单的东西，学好了不比很多复杂的高深的东西差。

说了这么多，其实就是要引出今天的主题-------**线性回归**。线性回归我觉得可以当成是机器学习中的长拳。
## 线性回归

线性回归包括一元线性回归和多元线性回归，一元的是只有一个x和一个y。多元的是指有多个x和一个y。

下面我只讲下一元的，多元只是将![这里写图片描述](https://img-blog.csdn.net/20170327202644514?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 变成了![这里写图片描述](https://img-blog.csdn.net/20170327202709514?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
一元线性回归其实就是去找到一条直线，这条直线能以最小的误差（Loss）来拟合数据。
![这里写图片描述](https://img-blog.csdn.net/20170327202753445?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 怎么来表示误差呢？

![这里写图片描述](https://img-blog.csdn.net/20170327202816063?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如上图所示，横坐标表示x，纵坐标表示y。我们要找的就是图中的这条直线。我们要去找到这条直线，大家可以想象，我们肯定希望找到的那条线，距离每个点都很近，最好所有的点上都在这条线上，但是一条直线去拟合所有的点都在这条直线上肯定不现实，所以我们希望这些点尽量离这条直线近一点。即去找每个点和直线的距离 ![这里写图片描述](https://img-blog.csdn.net/20170327202916283?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)最小的那条线，为了简单起见，将绝对值转化为平方，那么误差可以表示为![这里写图片描述](https://img-blog.csdn.net/20170327202943940?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，这里i表示第i个数据，N表示总的样本个数。一般我们还会把Loss求和平均，来当作最终的损失，

即![这里写图片描述](https://img-blog.csdn.net/20170327203007550?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)。
## 怎么去最小化误差？

我们要怎么去找到最能拟合数据的直线？即最小化误差呢？
**一般有两个方法：**

## 最小二乘法

上面我们讲了我们定义的损失![这里写图片描述](https://img-blog.csdn.net/20170327203105181?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，其中的x,y,i,N都是已知的，那么我们就可以把这个方程看作是m和b的方程。作为一个m和b的二次方程。那么求Loss最小值的问题就转变成了求极值问题，这个高数学过的都应该知道点。

## 怎么求极值呢？

令每个变量的偏导数为零，求方程组的解呗，这个是很基础的高数问题了。

我们可以得到下面的方程组
![这里写图片描述](https://img-blog.csdn.net/20170327203149213?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170327203208147?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后就是巴拉巴拉巴拉把m和b求出来，这样就得到我们要的线性方程了。
## 梯度下降法

没有梯度下降就没有现在的深度学习，这是一个神奇的算法。

最小二乘法可以一步到位，直接算出m和b，但他是有前提的，具体我有点记不清了，好像是需要满秩什么的。梯度下降法和最小二乘不一样，它通过一步一步的迭代，慢慢的去靠近到那条最优直线。

最小二乘法里面我们提到了两个偏导数，分别为
![这里写图片描述](https://img-blog.csdn.net/20170327203250617?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170327203307070?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我们要去找Loss这个方程的最小值，最小值怎么求？按数学的求法就是最小二乘法呗，但是大家可以直观的想一下，很多地方都会用一个碗来形容，那我也找个碗来解释吧。
![这里写图片描述](https://img-blog.csdn.net/20170327203353977?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

大家把这个Loss函数想象成这个碗，而我们要求的最小值就是碗底。假设我们现在不能用最小二乘法求极小值，但是我们的计算机的计算能量很强，我们可以用计算量换结果，不管我们位于这个碗的什么位置，只要我们想去碗底，就要往下走。
**往下走？？？？？？？？**
**这个下不就是往梯度方向走吗**，那我们沿着梯度一点一点滑下去呗，反正计算机不嫌累。**梯度不就是上面那两个公式呗**。现在梯度有了，那每次滑多远呢，一滑划过头了不久白算半天了吗，所以还得定义**步长**，用来表示每次滑多长。这样我们就能每次向下走一点点，再定义一个**迭代值**用来表示滑多少次，这样我们就能慢慢的一点点的靠近最小值了，不出意外还是能距离最优值很近的。
## 顺便把上面这个梯度下降法实现下

每次向下滑要慢慢滑，就是要个步长，我们定义为learning_rate,往往很小的一个值。

向下滑动的次数，就是迭代的次数，我定义为num_iter,相对learning_rate往往很大。

定义好这两个，我们就可以一边求梯度，一边向下滑了。就是去更新m和b。
![这里写图片描述](https://img-blog.csdn.net/20170327203845499?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170327203903124?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**我这里用了加号，很多人会误以为梯度下降就要减，但是其实梯度本身是有方向的，所以这里直接加就可以。**
如下图所示，我们做的初始化

![这里写图片描述](https://img-blog.csdn.net/20170327203922703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后就是优化器，优化器就是去做梯度下降

![这里写图片描述](https://img-blog.csdn.net/20170327204019967?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

它里面的具体步骤，如下

![这里写图片描述](https://img-blog.csdn.net/20170327204039267?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

里面的compute_gradient方法就是去计算梯度做参数更新

![这里写图片描述](https://img-blog.csdn.net/20170327204059705?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

需要代码的可以到我github上下载。[https://github.com/Shicoder/DeepLearning_Demo/tree/master/linear_regression_use_gradient_decent](https://github.com/Shicoder/DeepLearning_Demo/tree/master/linear_regression_use_gradient_decent)

**Notes:**

线性回归适用于线性变换数据

线性回归受噪声影响较大










