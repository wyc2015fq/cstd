# 陆振波博士对四种SVM工具箱的使用要点 - 家家的专栏 - CSDN博客





2011年02月20日 15:04:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2851标签：[工具																[classification																[算法																[网络																[测试																[matlab](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)
个人分类：[数学 Matlab 基本知识](https://blog.csdn.net/yihaizhiyan/article/category/749753)





四种支持向量机(Support Vector Machine ,SVM)Matlab工具箱(Toolbox)的分类与回归算法的示例程序下载地址如下：[http://www.ilovematlab.cn/viewthread.php?tid=32735&highlight=%E5%9B%9B%E7%A7%8D%E6%94%AF%E6%8C%81%E5%90%91%E9%87%8F%E6%9C%BA](http://www.ilovematlab.cn/viewthread.php?tid=32735&highlight=%E5%9B%9B%E7%A7%8D%E6%94%AF%E6%8C%81%E5%90%91%E9%87%8F%E6%9C%BA)



关于针对初学者的<<四种支持向量机工具箱>>的详细使用说明。同时也不断有网友向我反映看不懂我的源代码，以及询问如何将该工具箱应用到实际数据分析等问题，其中有相当一部分网友并不了解模式识别的基本概念，就急于使用这个工具箱。本文从模式识别的基本概念谈起，过渡到神经网络模式识别，逐步引入到这四种支持向量机工具箱的使用。
本文适合没有模式识别基础，而又急于上手的初学者。作者水平有限，欢迎同行批评指正！

[1]模式识别基本概念
模式识别的方法有很多，常用有：贝叶斯决策、神经网络、支持向量机等等。特别说明的是，本文所谈及的模式识别是指“有老师分类”，即事先知道训练样本所属的类别，然后设计分类器，再用该分类器对测试样本进行识别，比较测试样本的实际所属类别与分类器输出的类别，进而统计正确识别率。正确识别率是反映分类器性能的主要指标。
分类器的设计虽然是模式识别重要一环，但是样本的特征提取才是模式识别最关键的环节。试想如果特征矢量不能有效地描述原样本，那么即使分类设计得再好也无法实现正确分类。工程中我们所遇到的样本一般是一维矢量，如：语音信号，或者是二维矩阵，如：图片等。特征提取就是将一维矢量或二维矩阵转化成一个维数比较低的特征矢量，该特征矢量用于分类器的输入。关于特征提取，在各专业领域中也是一个重要的研究方向，如语音信号的谐振峰特征提取，图片的PCA特征提取等等。

[2]神经网络模式识别
神经网络模式识别的基本原理是，神经网络可以任意逼近一个多维输入输出函数。以三类分类：I、II、III为例，神经网络输入是样本的特征矢量，三类样本的神经网络输出可以是[1;0;0]、[0;1;0]、[0;0;1]，也可以是[1;-1;-1]、[-1;1;-1]、[-1;-1;1]。将所有样本中一部分用来训练网络，另外一部分用于测试输出。通常情况下，正确分类的第I类样本的测试输出并不是[1;0;0]或是[1;-1;-1]，而是如[0.1;0;-0.2]的输出。也是就说，认为输出矢量中最大的一个分量是1，其它分量是0或是-1就可以了。

[3]支持向量机的多类分类
支持向量机的基本理论是从二类分类问题提出的。我想绝大部分网友仅着重于理解二类分类问题上了，我当初也是这样，认识事物都有一个过程。二类分类的基本原理固然重要，我在这里也不再赘述，很多文章和书籍都有提及。我觉得对于工具箱的使用而言，理解如何实现从二类分类到多类分类的过渡才是最核心的内容。下面我仅以1-a-r算法为例，解释如何由二类分类器构造多类分类器。
二类支持向量机分类器的输出为[1,-1]，当面对多类情况时，就需要把多类分类器分解成多个二类分类器。在第一种工具箱LS_SVMlab中，文件Classification_LS_SVMlab.m中实现了三类分类。训练与测试样本分别为n1、n2，它们是3 x 15的矩阵，即特征矢量是三维，训练与测试样本数目均是15；由于是三类分类，所以训练与测试目标x1、x2的每一分量可以是1、2或是3，分别对应三类，如下所示：
n1 = [rand(3,5),rand(3,5)+1,rand(3,5)+2];
x1 = [1*ones(1,5),2*ones(1,5),3*ones(1,5)]; 
n2 = [rand(3,5),rand(3,5)+1,rand(3,5)+2];
x2 = [1*ones(1,5),2*ones(1,5),3*ones(1,5)]; 
1-a-r算法定义：对于N类问题，构造N个两类分类器，第i个分类器用第i类训练样本作为正的训练样本，将其它类的训练样本作为负的训练样本，此时分类器的判决函数不取符号函数sign，最后的输出是N个两类分类器输出中最大的那一类。
在文件Classification_LS_SVMlab.m的第42行：codefct = 'code_MOC'，就是设置由二类到多类编码参数。当第42行改写成codefct ='code_OneVsAll'，再去掉第53行最后的引号，按F5运行该文件，命令窗口输出有：
codebook =
1 -1 -1
-1 1 -1
-1 -1 1
old_codebook =
1 2 3
比较上面的old_codebook与codebook输出，注意到对于第i类，将每i类训练样本做为正的训练样本，其它的训练样本作为负的训练样本，这就是1-a-r算法定义。这样通过设置codefct ='code_OneVsAll'就实现了支持向量机的1-a-r多类算法。其它多类算法也与之雷同，这里不再赘述。值得注意的是：对于同一组样本，不同的编码方案得到的训练效果不尽相同，实际中应结合实际数据，选择训练效果最好的编码方案。

[4]核函数及参数选择
常用的核函数有：多项式、径向基、Sigmoid型。对于同一组数据选择不同的核函数，基本上都可以得到相近的训练效果。所以核函数的选择应该具有任意性。对训练效果影响最大是相关参数的选择，如：控制对错分样本惩罚的程度的可调参数，以及核函数中的待定参数，这些参数在不同工具箱中的变量名称是不一样的。这里仍以Classification_LS_SVMlab.m为例，在第38、39行分别设定了gam、sig2的值，这两个参数是第63行trainlssvm函数的输入参数。在工具箱文件夹的trainlssvm.m文件的第96、97行有这两个参数的定义：
% gam : Regularization parameter
% sig2 : Kernel parameter (bandwidth in the case of the 'RBF_kernel')
这里gam是控制对错分样本惩罚的程度的可调参数，sig2是径向基核函数的参数。所以在充分理解基本概念的基础上，将这些概念与工具箱中的函数说明相结合，就可以自如地运用这个工具箱了，因此所以最好的教科书是函数自带的函数说明。
最佳参数选择目前没有十分好的方法，在Regression_LS_SVMlab.m的第46至49行的代码是演示了交叉验证优化参数方法，可这种方法相当费时。实践中可以采用网格搜索的方法：如gam=0:0.2:1，sig2=0:0.2:1，那么gam与sig2的组合就有6x6=36种，对这36种组合训练支持向量机，然后选择正确识别率最大的一组参数作为最优的gam与sig2，如果结果均不理想，就需要重新考虑gam 与sig2的范围与采样间隔了。

[5]由分类向回归的过渡
LS_SVMlab、SVM_SteveGunn这两个工具箱实现了支持向量机的函数拟合功能。从工具箱的使用角度来看，分类与回归的最大区别是训练目标不同。回归的训练目标是实际需要拟合的函数值；而分类的训练目标是1,2,…N(分成N类)，再通过适当的编码方案将N类分类转换成多个二类分类。比较文件Regression_LS_SVMlab.m与Classification_LS_SVMlab.m的前几行就可以注意到这一点。另外，分类算法以正确分类率来作为性能指标，在回归算法中通常采用拟合的均方误差(mean square error, MSE)来作为性能指标。 

写得还挺好的，就拷过来共享一下。 

转自：[http://www.5iai.com/bbs/simple/index.php?t3123.html](http://www.5iai.com/bbs/simple/index.php?t3123.html)

相关资源下载：[http://www.ilovematlab.cn/viewthread.php?tid=32735&highlight=%E5%9B%9B%E7%A7%8D%E6%94%AF%E6%8C%81%E5%90%91%E9%87%8F%E6%9C%BA](http://www.ilovematlab.cn/viewthread.php?tid=32735&highlight=%E5%9B%9B%E7%A7%8D%E6%94%AF%E6%8C%81%E5%90%91%E9%87%8F%E6%9C%BA)



四种SVM工具箱的分类与回归算法 QUOTE:



1、工具箱：LS_SVMlab
Classification_LS_SVMlab.m - 多类分类         Regression_LS_SVMlab.m - 函数拟合

2、工具箱：OSU_SVM3.00
Classification_OSU_SVM.m - 多类分类

3、工具箱：stprtool/svm
Classification_stprtool.m - 多类分类

4、工具箱：SVM_SteveGunn
Classification_SVM_SteveGunn.m - 二类分类    Regression_SVM_SteveGunn.m - 函数拟合](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=classification&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)




