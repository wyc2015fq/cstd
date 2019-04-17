# OpenCV系统学习（基本了解完） - 工作笔记 - CSDN博客





2015年01月18日 22:16:29[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：8443
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









1.下载安装.[http://blog.csdn.net/poem_qianmo/article/details/19809337](http://blog.csdn.net/poem_qianmo/article/details/19809337)

2.框架架构[http://blog.csdn.net/poem_qianmo/article/details/19925819](http://blog.csdn.net/poem_qianmo/article/details/19925819)

3.图像显示[](http://blog.csdn.net/poem_qianmo/article/details/19925819)[http://blog.csdn.net/poem_qianmo/article/details/20537737](http://blog.csdn.net/poem_qianmo/article/details/20537737)

视频压缩编码 CV_FOURCC('M','J','P','G')

预定义类型 CV_<位深>（S|U|F）C<通道数>，例如CV_8UC3

4.ROI设置[](http://blog.csdn.net/poem_qianmo/article/details/19925819)[](http://blog.csdn.net/poem_qianmo/article/details/20537737)[http://blog.csdn.net/poem_qianmo/article/details/20911629](http://blog.csdn.net/poem_qianmo/article/details/20911629)

5.Blending[](http://blog.csdn.net/poem_qianmo/article/details/19925819)[](http://blog.csdn.net/poem_qianmo/article/details/20537737)[](http://blog.csdn.net/poem_qianmo/article/details/21176257)[http://blog.csdn.net/poem_qianmo/article/details/21176257](http://blog.csdn.net/poem_qianmo/article/details/21176257)

6.鼠标响应，轨迹条[](http://blog.csdn.net/poem_qianmo/article/details/19925819)[](http://blog.csdn.net/poem_qianmo/article/details/20537737)[](http://blog.csdn.net/poem_qianmo/article/details/21176257)[](http://blog.csdn.net/poem_qianmo/article/details/21176257)[http://blog.csdn.net/poem_qianmo/article/details/21479533](http://blog.csdn.net/poem_qianmo/article/details/21479533)

7.源码工程添加[](http://blog.csdn.net/poem_qianmo/article/details/19925819)[](http://blog.csdn.net/poem_qianmo/article/details/20537737)[](http://blog.csdn.net/poem_qianmo/article/details/21176257)[](http://blog.csdn.net/poem_qianmo/article/details/21176257)[](http://blog.csdn.net/poem_qianmo/article/details/21479533)[http://blog.csdn.net/poem_qianmo/article/details/21974023](http://blog.csdn.net/poem_qianmo/article/details/21974023)

8.线性滤波http://blog.csdn.net/poem_qianmo/article/details/22745559[](http://blog.csdn.net/poem_qianmo/article/details/21479533)

9.非线性滤波[http://blog.csdn.net/poem_qianmo/article/details/23184547](http://blog.csdn.net/poem_qianmo/article/details/23184547)

10.形态学[http://blog.csdn.net/poem_qianmo/article/details/23710721](http://blog.csdn.net/poem_qianmo/article/details/23710721)、[**http://blog.csdn.net/poem_qianmo/article/details/23184547**](http://blog.csdn.net/poem_qianmo/article/details/23184547)

11.边缘检测[**http://blog.csdn.net/poem_qianmo/article/details/25560901**](http://blog.csdn.net/poem_qianmo/article/details/25560901)

12.图像金字塔[](http://blog.csdn.net/poem_qianmo/article/details/22745559)[**http://blog.csdn.net/poem_qianmo/article/details/26157633**](http://blog.csdn.net/poem_qianmo/article/details/26157633)

13.霍夫变换[http://blog.csdn.net/poem_qianmo/article/details/26977557](http://blog.csdn.net/poem_qianmo/article/details/26977557)

14.漫水填充[http://blog.csdn.net/poem_qianmo/article/details/28261997](http://blog.csdn.net/poem_qianmo/article/details/28261997)

15.重映射[http://blog.csdn.net/poem_qianmo/article/details/30974513](http://blog.csdn.net/poem_qianmo/article/details/30974513)

16.仿射变换[http://blog.csdn.net/poem_qianmo/article/details/33320997](http://blog.csdn.net/poem_qianmo/article/details/33320997)

17.直方图匹配[http://blog.csdn.net/xiaowei_cqu/article/details/7606607](http://blog.csdn.net/xiaowei_cqu/article/details/7606607)

18.EMD及反向投影[http://www.cnblogs.com/slysky/archive/2011/10/13/2210745.html](http://www.cnblogs.com/slysky/archive/2011/10/13/2210745.html)

19.轮廓[http://blog.csdn.net/augusdi/article/details/9000829](http://blog.csdn.net/augusdi/article/details/9000829) 其实就是目标识别前的描绘子和表达。

20.codebook [http://my.oschina.net/JiamingMai/blog/191893](http://my.oschina.net/JiamingMai/blog/191893)

21.德劳内三角划分和泰森多边形[http://blog.csdn.net/oOCEAN1/article/details/41076719](http://blog.csdn.net/oOCEAN1/article/details/41076719)

# 跟踪与运动

理解物体的运动，包含识别和建模

识别：

经典的跟踪不明物体的方法，是跟踪关键点。这样就可以给出物体实际位置的初步计算，第二部分就是建模。

角点，独一无二，两个正交方向都有明显变化，例如Harris角点。

我们使用每个点周围小窗口的二阶导数图像的自相关矩阵有两个最大特征值的地方，即本质上，表示此点为中心周围存在至少两个不同方向的纹理（或者边缘）。这样的点，对移动和旋转有不变性。还有SIFT缩放，旋转不变。

光流，连续两帧之间的位移关联。

几种不同的跟踪方法

稀疏光流：Lucas-Kanade方法，不足之处在于不能跟踪运动过快的物体，较大的运动会移出设置的窗口，金字塔LK，则可以解决该问题。

稠密光流：Horn-Schunck，块匹配，比LK慢很多，不支持图像金字塔匹配，不能跟踪大幅度运动

Horn-Schunck，基于亮度恒定假设，用速度的平滑约束，求解方程组，其效果是惩罚光流变化的剧烈区域。

块匹配，将图像划分成小块，计算这些小块的运动，因此返回的速度图像分辨率较低，

mean-shift:,常用于计算机视觉，即选择一个窗口（设置窗口类型，大小，形状），计算其重心，将窗口中心设置在重心出，重复计算，最后收敛。

camshift会自动调整窗口大小，

运动模板，在姿态识别中应用广泛，需要知道物体轮廓或者轮廓的一部分。其实就是根据输入的轮廓，建立运动历史模板，然后计算出该模板的梯度，获取局部运动和全局运动方向，其中局部运动做删除，同时要消除特别大的梯度。

最后一种，就是预估器，首先根据之前的信息，预测出当前的位置，然后根据测量值进行矫正。常用的有卡尔曼滤波器，以及condensation算法。两者的区别在于状态概率密度如何描述。

卡尔曼[http://blog.csdn.net/ouyang_linux007/article/details/7563998](http://blog.csdn.net/ouyang_linux007/article/details/7563998)

# 摄像机模型与标定

[http://www.360doc.com/content/14/0402/08/10724725_365632946.shtml](http://www.360doc.com/content/14/0402/08/10724725_365632946.shtml)

# 投影与三维视觉

双目测距

[http://blog.csdn.net/chenyusiyuan/article/details/5961769](http://blog.csdn.net/chenyusiyuan/article/details/5961769)


# 机器学习


OpenCV实际上主要介绍的机器学习是统计性机器学习，倾向于鉴别算法，即通过给定的数据来判断类别，而不倾向产生式算法，即通过给定类别来生成数据的分包，鉴别式算法在根据给定的数据做出预测上有优势，后者在是在为你提供更强大的数据表达或者有条件的生成新数据上有优势。

OpenCV中的学习算法有：

马氏距离，多维空间中两点相似性的度量，本身不是聚类或者分类算法

K均值，尝试找到自然分类，最常用聚类技术，与高斯混合中的EM很相似，

朴素贝叶斯分类器，最简单的监督学习分类器，也叫正态贝叶斯分类器，简单是因为假设所有特征之间相互独立，虽然这在现实很少，但它有时能获得惊人的效果，也是贝叶斯网络的最简单情况。

决策树，执行简单，容易解释结果，适应不同的数据类型，能够处理数据丢失，通过分裂的顺序能够给数据特征赋予重要性，是Boosting,随即森林的基础。

Boosting，内部使用了决策树，通常是ML库中最好的监督算法。从弱分类器中加权赋值生成强分类器。有discret/real/logit/gentle AdaBoost。OpenCV执行时一个两类分类器。

随即森林，通过收集很多数的子节点对各个类别的投票，选择投票最多的类别作为判别结果。

Haar分类器

EM，聚类算法，OpenCV仅仅支持高斯混合模型的EM，通过迭代先找到给定模型时的最大可能性的猜想，然后调整模型使得猜想正确率最大。

K近邻，只存储所有训练样本数据，如果需要分类一个新的数据样本，只需要找到她的K个最相邻的点，然后统计哪个类在这K近邻点中频率最高，然后标记为出现频率最高的类，占用内存大，速度慢，使用这个算法前把训练集聚类来降低数据的大小。

神经网络，MLP，也叫反向传播，性能最好的分类器之一，尤其在文字识别方面，训练很慢，使用梯度下降来调整网络节点之间的连接权来最小化误差。测试模式时，速度很快

支持向量机，当数据集比较小时，SVM效果很好，它首先把数据集映射到高纬空间，然后再高维空间找到类别间最优的线性分类器，而这种线性分类器在原始数据空间中可能是非线性的，因此使用基于最大类间隔的线性分类技术，得到某种意义上较优的区分类别的非线性分类器。

如何在视觉中使用机器学习

1.怎样采集数据，采集数据，给数据标签，需要提取那些特征，数据分类（训练集，验证集，测试集）。

2.选择分类器，需要考虑计算速度，数据形式，内存。

需要快速训练，则最邻近算法，朴素贝叶斯分类器，决策树。

需要考虑内存，决策树和神经网络

不需要快速训练，需要快速判断，则神经网络，朴素贝叶斯，SVM

不需要训练很快，但精度要高，则可选择boosting，随即森林

如果选取特征较好，仅仅需要一个简单的特征就可以分类，就选择决策树和最邻近算法

同时，我们需要评估每个变量的重要性，即对分类器的贡献，二进制决策树可以解决该问题，即在每个节点选择最能够分裂出数据的变量。知道了变量重要性，可以减少特征个数。提高训练和测试速度、Breiman有个变量重要性算法，理论上该技术适用于任何分类器，但OpenCV只在决策树和随即森林中实现了。

机器学习的性能诊断有两种，一种是过拟合，一种是欠拟合。前者受噪声影响，后者模型假设太严格。

通常数据量越大越好，好的特征比好的算法更重要。同时要注意是否正在解决自己想要解决的问题。也可能出现模型无法学习数据，这时你要考虑特征是否选对。

评估机器学习的工具有，交叉验证（将数据分为K，取K-1训练，余下的测试，做K次，结果平均），自抽样（与交叉验证类似，但验证集从训练集随机选择，选择的点仅用于测试，不用于训练，做N次，最后的结果平均，一般好于交叉验证），还有就是画ROC曲线，填充混淆矩阵

另外，我们常常对错误识别赋予更大的权值，例如每个错误识别的权值设为错误拒绝权值的10倍，这在SVM和决策树中，通过指定类别的先验概率，自动平衡击中率和误报率。

还要注意，对于特征向量，如果特征的方差不一致，某些算法会对个数较小的特征进行忽略，因此，需要预处理特征向量，使得他们的方差一致。如果特征不相关，这步非常重要。如果相关，则可用协方差或者平方差归一化。决策树等一类算法不受方差不一致影响，如马氏距离，K均值算法等需要注意这一点。




简单人脸识别[http://www.cnblogs.com/mengdd/archive/2012/08/01/2619043.html](http://www.cnblogs.com/mengdd/archive/2012/08/01/2619043.html)﻿﻿


﻿﻿

﻿﻿

﻿﻿

﻿﻿

﻿﻿

﻿﻿

﻿﻿

﻿﻿

﻿﻿

﻿﻿

﻿﻿



