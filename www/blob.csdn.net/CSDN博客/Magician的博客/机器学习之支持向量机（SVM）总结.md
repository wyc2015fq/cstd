
# 机器学习之支持向量机（SVM）总结 - Magician的博客 - CSDN博客


2018年09月12日 20:39:35[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：215所属专栏：[自学机器学习之路](https://blog.csdn.net/column/details/26597.html)




### 文章目录
[1、知道SVM相关的各种概念](#1SVM_2)
[2、知道目标函数、损失函数、正则项的意义](#2_14)
[2.1、目标函数](#21_20)
[2.2、损失函数](#22_29)
[2.3、正则化（添加噪音避免过拟合）](#23_45)
[3、了解SVM算法原理](#3SVM_56)
[4、知道SVM的目标函数](#4SVM_66)
[5、知道SVM的损失函数以及公式](#5SVM_73)
[6、知道SVM支持向量机的计算过程](#6SVM_79)
[6.1、推导过程：](#61_88)
[6.2、推导实例](#62_115)
[7、了解核函数相关知识](#7_125)
[8、了解SVM算法的调参技巧](#8SVM_135)

# 1、知道SVM相关的各种概念
线性可分：一条直线可以分割开两种类别
间隔：分割线两边直线之间的间隔
支持向量：分割线两边直线上的点
非线性支持向量机：在高维空间形成分割超平面映射到低维空间形成分割界限。
![这里写图片描述](https://img-blog.csdn.net/20180912195931630?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**分割超平面**：两个集合上的距离最近的两个点的垂直平分面。
![这里写图片描述](https://img-blog.csdn.net/20180912195923863?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 2、知道目标函数、损失函数、正则项的意义
弄清楚目标函数、损失函数、正则项这几个概念：
## 2.1、目标函数
目标函数：指所关心的目标与相关的因素的函数关系。举个例子，假如我们想要预测公司楼下手抓饼店明天能卖多少张手抓饼，已知过去10天每天卖多少，以及每天的天气情况，是否有节假日，和手抓饼店老板和老板娘的感情状况，这几项数据都统计好了。对应到目标函数的解释就是：
关心的目标是明天能卖多少手抓饼，未知量。
相关因素是每天数量，天气情况，节假日，老板老板娘感情状况，已知量。
目标函数：按照一定的思路把这些已知条件利用起来，去求解未知量的函数关系式。
目标函数f(x)就是用设计变量来表示的所追求的目标形式，所以目标函数就是设计变量的函数，是一个标量。（简单的说就是函数表达式）是指所关心的目标(某一变量)与相关的因素(某些变量)的函数关系。
## 2.2、损失函数
也叫代价函数，是同一个东西。**是用来评估模型的预测值f(x)与真实值Y的差距，它是一个非负实值函数**。稍等一下，为什么是非负的呢？举个例子：比如上面说的手抓饼，模型预测第一天是30，真实值是29，差距30-29是1；预测第二天是35，真实值是36，差距35-36是-1；如果把它们直接加起来就是1-1 = 0，难到没有差距？非常明显是错误的，差距是0人家还以为都预测对了呢，其实都预测错了，所以不能有负值，可以用绝对值，平方或者一些其它的数学运算。
常用的损失函数有：
LR逻辑回归：log对数损失函数 参考
最小二乘法：平方损失函数 参考：
Adaboost：指数损失函数 （后面会专门介绍这个算法）
好的，现在我们再来看一下目标函数通常的定义形式：
Obj(Θ)=L(Θ)+Ω(Θ)
中文翻译一下是 ：
目标函数 = 损失函数 + 正则项
目标函数和损失函数我们都理解了，那么正则项是干什么的呢？
## 2.3、正则化（添加噪音避免过拟合）
回到我们前面举的那个例子，如果第五天有个人中彩票了然后兴奋的来买了1百张手抓饼，比几天的量都多，很明显这个情况会影响我们的模型训练，这个情况就是过拟合。而加入正则项的目的就是使模型避免过拟合。
常用的有L1，L2正则化，很熟悉对不对，前面讲过的线性代数中的L1，L2范数有些类似。其实本质是一样的，在机器学习里叫正则化，线性代数叫范数，统计学里叫惩罚项。
机器学习里：L1使用的是绝对值距离，也叫曼哈顿距离，L2使用的是平方距离，也叫做欧式（Euclidean）距离
线性代数：L1 范数计算的是向量所有元素绝对值的和，L2 范数计算的是通常意义上的向量长度
**目标函数，损失函数和正则项的作用，优化损失函数使模型避免欠拟合使损失函数最小化，优化正则项使模型避免过拟合。**
# 3、了解SVM算法原理
即先求每个点到每条直线的最小值，这样每条直线对应一个最小距离值，再从这组距离值中找出最大值，这个最大值对应的直线也就是我们需要的分割超平面。
![这里写图片描述](https://img-blog.csdn.net/20180912201330336?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
w为对应系数矩阵（A,B），Φ（x）代表的是变量矩阵(x,y)
y(xi)=0：代表点在平面上。>0：代表点在平面的上方。
y(xi)>0：代表的是在分隔超平面的一侧，yi=1;
![这里写图片描述](https://img-blog.csdn.net/20180912201129890?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/2018091220142732?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 4、知道SVM的目标函数
知道目标函数（分割超平面）的表达式以及对应的含义：某点到某分隔超平面的距离（需要找到最优的分隔超平面：即先找到每个点到每条直线的最小距离，之后得到每条直线对应的最小距离组成一个数组，再找出这组数中的最大值，对应的分隔超平面即为所求的最优分隔超平面（所以求目标函数的过程才是一个先求最小值而后求最大值的步骤，知道最小和最大代表的含义））
![这里写图片描述](https://img-blog.csdn.net/20180912203204816?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 5、知道SVM的损失函数以及公式
SVM：分对了损失值为0，分错了损失值为距离支持向量所在的直线的距离。
![这里写图片描述](https://img-blog.csdn.net/20180912203332545?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 6、知道SVM支持向量机的计算过程
先求出点到直线距离的一般表示形式。
由于是求所有点到直线的距离所以需要除以一个w来做归一化处理。
由于一系列的约束条件，转化为了用拉格朗日乘子法优化的问题（求极值的问题）
利用拉格朗日的一些性质来转化，最终得到一个最简的式子。
之后将对应的点代入这个式子就能求出一般表达式中对应的一个参数，进而得到整个式子。
## 6.1、推导过程：
通过缩放w,b（即||w||）总是可以使得yi*y(xi)>=1得到满足。即此处多了一个限制条件，根据这个限制条件来进一步转化目标函数。*
![这里写图片描述](https://img-blog.csdn.net/2018091220164060?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*由约束条件yi**[y(xi)>=1，得到原目标函数可以转化为新目标函数](https://img-blog.csdn.net/2018091220164060?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)*
![这里写图片描述](https://img-blog.csdn.net/20180912201714754?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*线性可分SVM的目标函数*
![这里写图片描述](https://img-blog.csdn.net/20180912201836935?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*将目标函数转化为拉格朗日乘子法来求。*
*一般问题的优化的解法（即求极值的问题）：拉格朗日乘子法*
*拉格朗日乘子法是一种经典的求解条件极值的解析方法，可将所有约束的优化模型问题转化为无约束极值问题的求解。*
![这里写图片描述](https://img-blog.csdn.net/20180912201934872?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180912201958348?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*求极大极小问题，先求极小值即先对w,b求导数。*
![这里写图片描述](https://img-blog.csdn.net/20180912202049781?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*得到上面两个条件后代入拉格朗日函数得到下面的式子*
![这里写图片描述](https://img-blog.csdn.net/20180912202108172?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*由前面得到得条件接着求解*
![这里写图片描述](https://img-blog.csdn.net/20180912202128300?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*原函数求的是极小极大值而拉格朗日对偶函数求的是极大极小值（参考上图），所以需要先求该式子的最小值，这里通过添加负号做出对应的转换。*
![这里写图片描述](https://img-blog.csdn.net/20180912202138734?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*求的最小值对应的α值*
![这里写图片描述](https://img-blog.csdn.net/20180912202157237?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*根据α值得到下面的值*
![这里写图片描述](https://img-blog.csdn.net/20180912202211616?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*6.2、推导实例*
![这里写图片描述](https://img-blog.csdn.net/20180912202400219?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*注意计算这个最小值的过程（求一个一元二次方程的最小值的过程）*
![这里写图片描述](https://img-blog.csdn.net/20180912202449160?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*注意这个结论*
![这里写图片描述](https://img-blog.csdn.net/20180912202515219?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*7、了解核函数相关知识*
*核函数思想：将不可分的输入数据映射到核函数空间中来进行分割。*
![这里写图片描述](https://img-blog.csdn.net/20180912202634175?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*知道核函数为什么被使用：减小运算的复杂度。*
*知道这几个核函数：多项核函数、高斯核函数RBF、Sigmoid核函数。*
*知道为什么需要核函数：将原始输入空间映射到新的特征空间从而使得原本线性不可分的样本在核空间中可分。*
![这里写图片描述](https://img-blog.csdn.net/2018091220261266?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*8、了解SVM算法的调参技巧*
*知道调参中常用的参数以及对应的参数的含义。*
*详情参考：**[https://blog.csdn.net/qq_16633405/article/details/70243030](https://blog.csdn.net/qq_16633405/article/details/70243030)*

