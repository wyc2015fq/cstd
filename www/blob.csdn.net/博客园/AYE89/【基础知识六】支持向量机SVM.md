
# 【基础知识六】支持向量机SVM - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[【基础知识六】支持向量机SVM](https://www.cnblogs.com/eniac1946/p/7347059.html)
|Posted on|2017-08-11 17:27|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7347059)|[收藏](#)
|开发库：
|[libsvm, liblinear](https://www.csie.ntu.edu.tw/~cjlin/libsvm/)|[GitHub地址](https://github.com/cjlin1/libsvm)
|SVM难点：核函数选择
|一、基本问题
|找到约束参数|ω|和b，支持向量到（分隔）超平面的距离最大；此时的分隔超平面称为“最优超平面”
|距离表示为，
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170811163844773-1779878802.png)
|问题表示为，
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170811164038257-2081696954.png)
|\#支持向量机名字的由来：由支持向量得到的分类器
|二、问题的求解
|上述问题为一个凸二次优化问题，可以由现成的优化计算包求解
|高效方法：用|拉格朗日乘子法|求解其对偶问题，得到问题的解——
|SMO算法|：在参数初始化后，
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170811171941429-1916697319.png)
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170811172015632-1587094465.png)
|SMO算法之所以高效，由于在固定其他参数后，仅优化两个参数（αi和αj）能做到非常高效。
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170811173053382-2008577209.png)
|b使用所有支持向量求解的平均值
|三、核函数
|问题：现实中，样本空间也许并不存在一个能正确划分两类样本的超平面。
|如果原始空间是有限维（即属性有限），那么一定存在一个高维特征空间使样本可分。
|\#核函数|：用来等效原始空间到高维空间的映射，为了实现两类样本线性可分
|要求样本在特征空间线性可分，则特征空间的好坏对支持向量机的性能至关重要。
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170811210127101-506893860.png)
|假设这样一个函数：
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170811210007382-1120186618.png)
|求解后可以得到
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170811210340898-1918861567.png)
|上式称为“支持向量展式”，κ(.,.)就是核函数
|通常|Φ|(.)的具体形式是未知的
|在不知道特征映射的形式时，我们并不知道什么样的核函数是合适的，而核函数也仅是隐式地定义了这个特征空间。
|于是，|“核函数选择”称为支持向量机的最大变数|。
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170811212058132-1662033187.png)
|此外，还可以通过函数组合得到“核函数”。
|四、软间隔与正则化
|样本空间或者特征空间中一定线性可分？
|1. 很难确定某个核函数使得训练样本在特征空间中线性可分
|2. 线性可分的结果是不是由于过拟合造成的
|问题的解决——
|“软间隔”：允许支持向量机在一些样本上出错
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170811212951992-1315958515.png)
|优化目标改写为：
![](https://images2017.cnblogs.com/blog/1181483/201708/1181483-20170814214516912-1464340820.png)
|C称为|正则化常数（惩罚因子）|，|l|0/1|表示损失函数，C为有限值时上式允许一些样本不满足约束
|正则化可理解为一种“罚函数法”，即对不希望得到的结果施以惩罚，从而使得优化过程趋向于希望目标。
|“正则化”regularization问题：
|推导发现“软间隔”问题，一方面与用户希望获得何种性质的模型有关；另一方面降低了最小化训练误差的过拟合风险；
|五、支持向量回归SVR
|SV Regression
|f(x)与y之间差的绝对值小于等于ε，则认为被预测正确。
|六、核方法
|给定训练样本，学得的模型总能表示成核函数的线性组合。
|那么，核方法指——引入核函数来将线性学习器拓展为非线性学习器。
|“核线性判别分析”
|补充阅读：
|1. 支持向量机的求解通常借助于凸优化技术
|2. 非线性核SVM的时间复杂度理论上不低于O(m^2)，研究重点是设计快速近似算法
|3. 线性核SVM常用于分析大规模数据
|4. 多核学习使用多个核函数并通过学习获得最优凸组合作为最终的核函数，实际是一种集成学习机制
|补充++
|SVM各个参数的含义？
|sigma: rbf核函数的参数，用于生成高维的特征，常用的有几种核函数，如径向核函数，线性核函数，这个也需要凭经验来选择。
|C：惩罚因子。在最优化函数中，对离群点的惩罚因子，也是对离群点的重视程度体现。这个也是凭经验和实验来选择。
|SVM种类：
|C-SV|M： 分类型SVM，需要调优的参数有惩罚因子C，核函数参数。 C的取值 10^-4, 10^-3, 10^-2,... 到 1, 5... 依次变大
|nu-SV|M： 分类型SVM, 在一定程度上与C-SVM相同，将惩罚因子C换成了因子nu。其最优化的函数略有不同。nu的取值是0-1，一般取值从0.1到0.8. 0代表样本落入间隔内的数目最小的情况，1代表样本可以落入间隔可以很多的情况。
|wiki上的原话：
|The main motivation for the nu versions of SVM is that it has a has a more meaningful interpretation. This is because nu represents an upper bound on the fraction of training samples which are errors (badly predicted) and a lower bound on the fraction of samples which are support vectors. Some users feel nu is more intuitive to use than C or epsilon.







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
