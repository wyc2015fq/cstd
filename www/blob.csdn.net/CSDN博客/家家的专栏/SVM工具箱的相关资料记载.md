# SVM工具箱的相关资料记载 - 家家的专栏 - CSDN博客





2011年02月21日 21:26:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2750标签：[工具																[matlab																[classification																[file																[windows																[optimization](https://so.csdn.net/so/search/s.do?q=optimization&t=blog)
个人分类：[数学 Matlab 基本知识](https://blog.csdn.net/yihaizhiyan/article/category/749753)





QUOTE:

> 
1、工具箱：LS_SVMlab
Classification_LS_SVMlab.m - 多类分类
Regression_LS_SVMlab.m - 函数拟合
2、工具箱：OSU_SVM3.00
Classification_OSU_SVM.m - 多类分类
3、工具箱：stprtool/svm
Classification_stprtool.m - 多类分类
4、工具箱：SVM_SteveGunn
Classification_SVM_SteveGunn.m - 二类分类
Regression_SVM_SteveGunn.m - 函数拟合

QUOTE:

> LIBSVM 是台湾大学林智仁(Chih-Jen Lin)博士等开发设计的一个操作简单、易于使用、快速有效的通用SVM 软件包，可以解决分类问题（包括C- SVC、n - SVC ）、回归问题（包括e - SVR、n - SVR ）以及分布估计（one-class-SVM ）等问题，提供了线性、多项式、径向基和S形函数四种常用的核函数供选择，可以有效地解决多类问题、交叉验证选择参数、对不平衡样本加权、多类问题的概率估计等。LIBSVM 是一个开源的软件包，需要者都可以免费的从作者的个人主页[http://www.csie.ntu.edu.tw/~cjlin/](http://www.csie.ntu.edu.tw/~cjlin/)处获得。他不仅提供了LIBSVM的C++语言的算法源代码，还提供了Python、Java、R、MATLAB、Perl、Ruby、LabVIEW以及C#.net 等各种语言的接口，可以方便的在Windows 或UNIX 平台下使用，也便于科研工作者根据自己的需要进行改进（譬如设计使用符合自己特定问题需要的核函数等）。另外还提供了WINDOWS 平台下的可视化操作工具SVM-toy，并且在进行模型参数选择时可以绘制出交叉验证精度的等高线图。

更加详细的介绍你可以查看[http://www.blog.sh/user3/warisa/archives/2006/75791.html](http://www.blog.sh/user3/warisa/archives/2006/75791.html)
或者查看该工具包作者的个人主页[http://www.csie.ntu.edu.tw/~cjlin/](http://www.csie.ntu.edu.tw/~cjlin/)



MySVM 程序作者：stefan ruping
程序介绍： 本程序实现支持向量机。支持向量机是一种基于统计学习理论的机器学习算法，是被认为比人工神经网络更好的可以针对小样本进行学习预测的有效算法。目前，该支持向量机程序可以用来进行模式识别和序列预测。
备注：无需编译，可以自动运行，打包的程序中有C++的源代码。

详细资料请查看[http://www.dmresearch.net/html/c ... ithm/1000000928.php](http://www.dmresearch.net/html/content/classfication-algorithm/1000000928.php)



matlab的SVM工具箱详细资料查看[http://asi.insa-rouen.fr/~arakotom/toolbox/index.html](http://asi.insa-rouen.fr/~arakotom/toolbox/index.html)



转自：[http://hi.baidu.com/%CB%AE%BE%A7%D2%BB%D1%A9/blog/item/e936090a0c9e2f32b0351d6e.html](http://hi.baidu.com/%CB%AE%BE%A7%D2%BB%D1%A9/blog/item/e936090a0c9e2f32b0351d6e.html)



SVM and Kernel Methods Matlab Toolbox：[http://asi.insa-rouen.fr/enseignants/~arakotom/toolbox/index.html](http://asi.insa-rouen.fr/enseignants/~arakotom/toolbox/index.html)


> 
转自：[http://i.eol.cn/blog_read.php?topicid=64388](http://i.eol.cn/blog_read.php?topicid=64388)


SVM工具箱及其相关介绍

在Dinga's Blog 上发现这篇关于SVM工具箱的介绍的文章，转载一下，共同学习之！



1、工具箱：LS_SVMlab
Classification_LS_SVMlab.m - 多类分类
Regression_LS_SVMlab.m - 函数拟合#此前在首页部分显示#

2、工具箱：OSU_SVM3.00
Classification_OSU_SVM.m - 多类分类

3、工具箱：stprtoolsvm
Classification_stprtool.m - 多类分类

4、工具箱：SVM_SteveGunn
Classification_SVM_SteveGunn.m - 二类分类
Regression_SVM_SteveGunn.m - 函数拟合

可以参考：[http://luzhenbo.88uu.com.cn/](http://luzhenbo.88uu.com.cn/)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

更加详细的介绍你可以查看[http://www.blog.sh/user3/warisa/archives/2006/75791.html](http://www.blog.sh/user3/warisa/archives/2006/75791.html)
或者查看该工具包作者的个人主页[http://www.csie.ntu.edu.tw/~cjlin/](http://www.csie.ntu.edu.tw/~cjlin/)

附录1：LIBSVM的简单介绍

1. LIBSVM 软件包简介

LIBSVM 是台湾大学林智仁(Chih-Jen Lin)博士等开发设计的一个操作简单、

易于使用、快速有效的通用SVM 软件包，可以解决分类问题（包括C- SVC、

n - SVC ）、回归问题（包括e - SVR、n - SVR ）以及分布估计（one-class-SVM ）

等问题，提供了线性、多项式、径向基和S形函数四种常用的核函数供选择，可

以有效地解决多类问题、交叉验证选择参数、对不平衡样本加权、多类问题的概

率估计等。LIBSVM 是一个开源的软件包，需要者都可以免费的从作者的个人主页[http://www.csie.ntu.edu.tw/~cjlin/](http://www.csie.ntu.edu.tw/~cjlin/) 处获得。他不仅提供了LIBSVM的C++语言的算法源代码，还提供了Python、Java、R、MATLAB、Perl、Ruby、LabVIEW

以及C#.net 等各种语言的接口，可以方便的在Windows 或UNIX 平台下使用，

也便于科研工作者根据自己的需要进行改进（譬如设计使用符合自己特定问题需

要的核函数等）。另外还提供了WINDOWS 平台下的可视化操作工具SVM-toy，

并且在进行模型参数选择时可以绘制出交叉验证精度的等高线图。

2. LIBSVM 使用方法简介

LIBSVM 在给出源代码的同时还提供了Windows操作系统下的可执行文件，包括：进行支持向量机训练的svmtrain.exe；根据已获得的支持向量机模型对数据集进行预测的svmpredict.exe；以及对训练数据与测试数据进行简单缩操作的svmscale.exe。它们都可以直接在DOS 环境中使用。如果下载的包中只有C++
的源代码，则也可以自己在VC等软件上编译生成可执行文件。

LIBSVM 使用的一般步骤是：

1） 按照LIBSVM软件包所要求的格式准备数据集；

2） 对数据进行简单的缩放操作；

3） 考虑选用RBF 核函数2 K(x,y) e x y = -g - ；

4） 采用交叉验证选择最佳参数C与g ；

5） 采用最佳参数C与g 对整个训练集进行训练获取支持向量机模型；

6） 利用获取的模型进行测试与预测。

一. LIBSVM 使用的数据格式

LIBSVM使用的训练数据和测试数据文件格式如下：

： ：< 2> …

其中 是训练数据集的目标值，对于分类，它是标识某类的整数(支持

多个类)；对于回归，是任意实数。

是以1 开始的整数，表示特征的序号；

为实数，也就是我们常说的特征值或自变量。当特征值为0 时，特征序号与特征值都可以同时省略，即index可以是不连续的自然数。

与第一个特征序号、前一个特征值与后一个特征序号之间用空格隔开。测试数据文件中的label 只用于计算准确度或误差，如果它是未知的，只需用任意一个数填写这一栏，也可以空着不填。例如：

+1 1:0.708 2:1 3:1 4:-0.320 5:-0.105 6:-1 8:1.21

二. svmscale 的用法

对数据集进行缩放的目的在于：

1）避免一些特征值范围过大而另一些特征值范围过小；

2）避免在训练时为了计算核函数而计算内积的时候引起数值计算的困难。因此，通常将数据缩放到[ -1,1]或者是[0,1]之间。

用法：svmscale [-l lower] [-u upper] [-y y_lower y_upper]

[-s save_filename] [-r restore_filename] filename

（缺省值： lower = -1，upper = 1，没有对y进行缩放）

其中，

-l：数据下限标记；lower：缩放后数据下限；

-u：数据上限标记；upper：缩放后数据上限；

-y：是否对目标值同时进行缩放；y_lower为下限值，y_upper为上限值；

-s save_filename：表示将缩放的规则保存为文件save_filename；

-r restore_filename：表示将缩放规则文件restore_filename载入后按此缩放；

filename：待缩放的数据文件（要求满足前面所述的格式）。

缩放规则文件可以用文本浏览器打开，看到其格式为：

lower upper

lval1 uval1

lval2 uval2

其中的lower 与upper 与使用时所设置的lower 与upper 含义相同；index 表

示特征序号；lval 为该特征对应转换后下限lower 的特征值；uval 为对应于转换后上限upper 的特征值。

数据集的缩放结果在此情况下通过DOS窗口输出，当然也可以通过DOS的

文件重定向符号“>”将结果另存为指定的文件。

使用实例：

1) svmscale –s train3.range train3>train3.scale

表示采用缺省值（即对属性值缩放到[ -1,1]的范围，对目标值不进行缩放）

对数据集train3 进行缩放操作，其结果缩放规则文件保存为train3.range，缩放集的缩放结果保存为train3.scale。

2） svmscale –r train3.range test3>test3.scale

表示载入缩放规则train3.range 后按照其上下限对应的特征值和上下限值线

性的地对数据集test3 进行缩放，结果保存为test3.scale。

三. svmtrain 的用法

svmtrain实现对训练数据集的训练，获得SVM模型。

用法： svmtrain [options] training_set_file [model_file]

其中，

options（操作参数）：可用的选项即表示的涵义如下所示

-s svm类型：设置SVM 类型，默认值为0，可选类型有：

0 -- C- SVC

1 -- n - SVC

2 -- one-class-SVM

3 -- e - SVR

4 -- n - SVR

-t 核函数类型：设置核函数类型，默认值为2，可选类型有：

0 -- 线性核：u'*v

1 -- 多项式核： (g*u'*v+ coef 0)deg ree

2 -- RBF 核：e( u v 2) g -

3 -- sigmoid 核：tanh(g*u'*v+ coef 0)

-d degree：核函数中的degree设置，默认值为3； -g g ：设置核函数中的g ，默认值为1/ k ；

-r coef 0：设置核函数中的coef 0，默认值为0；

-c cost：设置C- SVC、e - SVR、n - SVR中从惩罚系数C，默认值为1；

-n n ：设置n - SVC、one-class-SVM 与n - SVR 中参数n ，默认值0.5；

-p e ：设置n - SVR的损失函数中的e ，默认值为0.1；

-m cachesize：设置cache内存大小，以MB为单位，默认值为40；

-e e ：设置终止准则中的可容忍偏差，默认值为0.001；

-h shrinking：是否使用启发式，可选值为0 或1，默认值为1；

-b 概率估计：是否计算SVC或SVR的概率估计，可选值0 或1，默认0；

-wi weight：对各类样本的惩罚系数C加权，默认值为1；

-v n：n折交叉验证模式。

其中-g选项中的k是指输入数据中的属性数。操作参数 -v 随机地将数据剖分为n 部分并计算交叉检验准确度和均方根误差。以上这些参数设置可以按照SVM 的类型和核函数所支持的参数进行任意组合，如果设置的参数在函数或SVM 类型中没有也不会产生影响，程序不会接受该参数；如果应有的参数设置不正确，参数将采用默认值。training_set_file是要进行训练的数据集；model_file是训练结束后产生的模型文件，该参数如果不设置将采用默认的文件名，也可以设置成自己惯用的文件名。

使用实例：

1）svmtrain train3.scale train3.model

训练train3.scale，将模型保存于文件train3.model，并在dos窗口中输出如下

结果：

optimization finished, #iter = 1756

nu = 0.464223

obj = -551.002342, rho = -0.337784

nSV = 604, nBSV = 557

Total nSV = 604

其中，#iter为迭代次数，nu 与前面的操作参数-n n 相同，obj为SVM文件

转换为的二次规划求解得到的最小值，rho 为判决函数的常数项b，nSV 为支持

向量个数，nBSV为边界上的支持向量个数，Total nSV为支持向量总个数。

训练后的模型保存为文件train3.model，用记事本等文本浏览器打开可以看到其内容如下（其后“%”后内容为笔者所加注释）：

svm_type c_svc % 训练所采用的svm类型，此处为C- SVC

kernel_type rbf % 训练采用的核函数类型，此处为RBF核

gamma 0.047619 % 与操作参数设置中的g 含义相同

nr_class 2 % 分类时的类别数，此处为两分类问题

total_sv 604 % 总共的支持向量个数

rho -0.337784 % 决策函数中的常数项b

label 0 1 % 类别标签

nr_sv 314 290 % 各类别标签对应的支持向量个数

SV % 以下为支持向量

1 1:-0.963808 2:0.906788 ... 19:-0.197706 20:-0.928853 21:-1

1 1:-0.885128 2:0.768219 ... 19:-0.452573 20:-0.980591 21:-1

... ... ...

1 1:-0.847359 2:0.485921 ... 19:-0.541457 20:-0.989077 21:-1

% 对于分类问题，上面的支持向量的各列含义与训练数据集相同；对于回归问题，略有不同，与训练数据中的标签label（即y值）所对应的位置在模型文件的支持向量中现在存放的是Lagrange 系数a 值，即为下面决策函数公式中的a 值：

* *

1

() ( )( ( ) ( )) ( ) ( , )

( , )

k

i i i i i i

i i sv

i i

i sv

fx a a x x b a a k x x b

ak x x b

=   = - F F + = - +

= + g

四. svmpredict 的用法

svmpredict 是根据训练获得的模型，对数据集合进行预测。

用法：svmpredict [options] test_file model_file output_file

options（操作参数）：

-b probability_estimates：是否需要进行概率估计预测，可选值为0 或者1，

默认值为0。

model_file 是由svmtrain 产生的模型文件；test_file 是要进行预测的数据文

件；output_file 是svmpredict 的输出文件，表示预测的结果值。svmpredict 没有

其它的选项。

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

MySVM 程序作者：stefan ruping 
程序介绍： 本程序实现支持向量机。支持向量机是一种基于统计学习理论的机器学习算法，是被认为比人工神经网络更好的可以针对小样本进行学习预测的有效算法。目前，该支持向量机程序可以用来进行模式识别和序列预测。 
备注：无需编译，可以自动运行，打包的程序中有C++的源代码。
详细资料请查看[http://www.dmresearch.net/html/c](http://www.dmresearch.net/html/c) ... ithm/1000000928.php

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
SVMlight
作者：Thorsten Joachims <[thorsten@joachims.org](http://writeblog.csdn.net/thorsten@joachims.org)>

详细的介绍查看[http://www.360doc.com/showWeb/0/0/117221.aspx](http://www.360doc.com/showWeb/0/0/117221.aspx)

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
matlab的SVM工具箱详细资料查看[http://asi.insa-rouen.fr/~arakotom/toolbox/index.html](http://asi.insa-rouen.fr/~arakotom/toolbox/index.html)

+++++++本文转载于 Dinga's Blog　[http://www.dinga.cn](http://www.dinga.cn)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=classification&t=blog)](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)




