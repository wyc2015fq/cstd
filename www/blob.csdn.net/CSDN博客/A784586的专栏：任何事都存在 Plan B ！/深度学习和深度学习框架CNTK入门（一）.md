# 深度学习和深度学习框架CNTK入门（一） - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年04月06日 23:14:12[QuJack](https://me.csdn.net/A784586)阅读数：4292








> 
**深度学习和深度学习框架CNTK入门（一）**


**深度学习介绍**

是基于机器学习延伸出来的一个新的领域，由以人大脑结构为启发的神经网络算法为起源加之模型结构深度的增加发展，并伴随大数据和计算能力的提高而产生的一系列新的算法。



**学习能用来干什么？为什么近年来引起如此广泛的关注？**

          深度学习，作为机器学习中延伸出来的一个领域，被应用在图像处理与计算机视觉，自然语言处理以及语音识别等领域。自2006年至今，学术界和工业界合作在深度学习方面的研究与应用在以上领域取得了突破性的进展。以ImageNet为数据库的经典图像中的物体识别竞赛为例，击败了所有传统算法，取得了前所未有的精确度。


**深度学习如今和未来将对我们生活造成怎样的影响？**

          目前我们使用的Android手机中google的语音识别，百度识图，google的图片搜索，都已经使用到了深度学习技术。Facebook在去年名为DeepFace的项目中对人脸识别的准备率第一次接近人类肉眼（97.25% vs 97.5%)。大数据时代，结合深度学习的发展在未来对我们生活的影响无法估量。保守而言，很多目前人类从事的活动都将因为深度学习和相关技术的发展被机器取代，如自动汽车驾驶，无人飞机，以及更加职能的机器人等。深度学习的发展让我们第一次看到并接近人工智能的终极目标。



一.CNTK前奏：
1. cntk简单介绍：微软开源**人工智能工具**包CNTK（Computational
 Network Toolkit）;
开始叫Computational Network Toolkit，改名后叫The Microsoft Cognitive Toolkit.
CNTK只是一个框架或者说是一套简单的工具帮助我们实现我们所涉及的深度学习或者是神经网络。其中已经集成好很多经典的算法。当然大家也可以根据实际情况去自己定义具体的算法或者输入输出的方式。
它是一种深度学习框架，类似的框架还有TensorFlow,MxNet,Caffe等；
入门介绍：https://github.com/Microsoft/CNTK/wiki
官方入门教程 https://github.com/Microsoft/CNTK/wiki/Tutorial 教程
官方论坛 https://github.com/Microsoft/CNTK/issues
官方论文 http://research.microsoft.com/pubs/226641/CNTKBook-20160217..pdf 这个有150页，当作字典来用；
CNTK可以解决什么问题？
- 类别分析 ，语音识别， 图像识别 等等；
**CNTK有什么特点？**
- 速度快，微软一直在强调这个 ， 训练简单使用方便
CNTKBook官方英文知道里面说了**有5大特性**：
1.支持各种神经网络模型；
2.一个简单的配置文件配置特定网络；
3.CNTK 可以用GPU，支持CUDA编程；
4.自动计算所需的导数；
5.可扩展；
**License问题**：CNTK的License是MIT的，它默认带的其他东西也一样，在使用上没有什么大的限制，但是有一样东西大家在使用上需要注意就是1bitSGD（SGD是Stochastic
 Gradient Descent, 随机梯度下降的意思），这个的License特殊，只可以非商业性质使用，当然，CNTK提供了其他SGD供大家使用。1bitSGD只有在DNN（深度深度神经网络）的训练以及使用上有所帮助。
**下面给出CNTK的总体架构图**：


![](https://img-blog.csdn.net/20170420165234233?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


IDataReader这一层可以理解为基础的数据；

CN这一层是学习层；

IExecutionEngine这一层是学习后的预测层；

CNTK的架构图中给出的是一个学习的过程。

简单介绍下**什么是神经网络**：
**人工神经网络**（artificial neural network，缩写ANN），简称**神经网络**（neural network，缩写NN），是一种模仿生物神经网络(动物的中樞神經系統，特别是大脑)的结构和功能的数学模型或计算模型。神经网络由大量的人工神经元联结进行计算。大多数情况下人工神经网络能在外界信息的基础上改变内部结构，是一种自适应系统。现代神经网络是一种非线性统计性数据建模工具，常用来对输入和输出间复杂的关系进行建模，或用来探索数据的模式。

神经网络是一种运算模型，由大量的節點（或稱“神經元”，或“單元”）和之間相互聯接構成。每个节点代表一种特定的输出函数，称为激励函数（activation function）。每两个节点间的连接都代表一个对于通过该连接信号的加权值，称之为权重（weight），這相當於人工神經網路的記憶。网络的输出则依网络的连接方式，权重值和激励函数的不同而不同。而网络自身通常都是对自然界某种算法或者函数的逼近，也可能是对一种逻辑策略的表达。
**单个神经元结构图：**


![](https://img-blog.csdn.net/20170420165240780?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**大概思想**简单理解就是：多个输入通过一个神经元，某个函数规则计算，得到输出是什么。这里用到了数据概率论的知识，先给个值，后面不断调整里面的参数，调整参数的过程就是学习的过程，是的最终输入什么，就能测试预测输出什么相对更准确。神经网络就是多个神经元之间相互连接，呈现一种有向图，数据作为数据流在有向图中流动。
**所谓的****神经元网络**，就是把神经元链接起来，彼此连接输入输出，这里的连接方式不同，也就出现了目前所谓的不同的神经网络算法，也应对不同的功能和适合的情况。

神经网络可以分层次，也就是**输入层，隐含层以及输出层**。

**三种常见的神经网络模型**：

![](https://img-blog.csdn.net/20170420165620286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


------------------------------------------------------------------

二. CNTK环境搭建：[点击访问：window上轻松安装CNTK并运行demo](http://blog.csdn.net/a784586/article/details/70313965)

这里可以下载源码自己编译安装，也可以下载编译好的二进制源码安装：

此处为了简单，下载编译好的二进制源码：

二进制安装网址：https://github.com/Microsoft/CNTK/wiki/Setup-CNTK-on-your-machine

CNTK的release的页面：https://github.com/Microsoft/CNTK/releases

我们可以选择安装only—CPU，有windows版本 ，Linux版本；

由于个人PC（64bit）上的虚拟机上的ubutun是32位的，因此选择window版本的安装。
**步骤：**

1. Anaconda3 Python 3.5 as your active Python environment

在安装CNTK之前要有相关的Python环境：https://github.com/Microsoft/CNTK/wiki/Setup-Windows-Python

备注：一个开源的 [Python](http://baike.baidu.com/item/Python) 发行版本，包含了
 conda、Python 等 180 多个科学包及其依赖项。

2.pip install https://cntk.ai/PythonWheel/CPU-Only/cntk-2.0rc1-cp35-cp35m-win_amd64.whl

3.测试： C:\> python -c "import cntk; print(cntk.__version__)"

4.可能的环境依赖问题：
4.1 报错什么dll找不到啦之类的，你可以考虑去安装下“Visual C++ Redistributable for Visual Studio 2013”
http://www.microsoft.com/en-us/download/details.aspx?id=40784
4.2 检查下MS-MPI SDK有没有安装。MPI是c++的多线程编程用的东西，微软将其列入HPC
https://msdn.microsoft.com/en-us/library/bb524831(v=vs.85).aspx
4.3 GPU：更新NVIDIA的驱动到最新的版，并确保CUDA功能是使能的。同时，CNTK说不需要用户自己安装CUDA的SDK，建议安装。

5.设置一个环境变量ACML_FMA为0（ACML_FMA=0）

6.跑一个分类的例子：训练的样本是若干个分布在平面上的二维坐标，包括X坐标和Y坐标，并且每一个点归结为一种类型

首先，打开熟悉的CMD（Win+R，cmd，回车）。用cd命令导航到 Config目录。

cd C:\cntk\Examples\Other\Simple2d\Config

然后，运行如下的命令。

cntk configFile=Sample.cntk

这里会执行两个事情，第一件事是训练，第二件事是测试。 

备注声明：【如果随着之后学习，发现不准确的地方，我会及时修改，博客也是作为自我学习的笔记记录。若发现描述理解错误的地方及时联系我，及时纠正，惠及他人。非常感谢。】

----------------------------------------------------------------------------------------------------------------

三. 一些术语缩写：

computational network (CN)：计算网络，有人工智能的含义，通过计算实现人工智能；

deep neural networks (DNNs)：深度神经网络

convolutional neural networks (CNNs)：卷积神经网络 convolutionaladj. 卷积的；回旋的；脑回的

recurrent neural networks (RNNs)：递归神经网络

long short term memory(LSTM)：长短时记忆

logistic regression：逻辑回归

maximum entropy model：最大熵模型

forward computation：前向计算

gradient calculation：梯度计算；

CUDA(Compute Unified Device Architecture)：是显卡厂商NVIDIA推出的运算平台。 CUDA™是一种由NVIDIA推出的通用并行计算架构，该架构使GPU能够解决复杂的计算问题。

-----------------------------------------------------

附录：Linux下的命令：

sudo uname --m 查看Linux是32还是64位；//显示i686,2位操作系统;显示 x86_64，你安装了64位操作系统;

sudo uname --s  显示内核名字s

sudo uname --r 显示内核版本

sudo uname --n 显示网络主机名

sudo uname --p 显示cpu 





