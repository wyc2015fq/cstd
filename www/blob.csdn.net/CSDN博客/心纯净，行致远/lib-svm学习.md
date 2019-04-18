# lib-svm学习 - 心纯净，行致远 - CSDN博客





2018年08月04日 12:57:48[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：103








SVM可以在[http://www.csie.ntu.edu.tw/~cjlin/](http://www.csie.ntu.edu.tw/~cjlin/)免费获得(难道还交钱不成!)

首先跟大家说说分类流程简要描述:

训练：训练集——〉特征选取——〉训练——〉分类器

分类: 新样本——〉特征选取——〉分类——〉判决



LIBSVM是林智仁等开发设计的一个简单、易于使用和快速有效的SVM模式识别与回归的软件包，不但提供了编译好的可在Windows系列系统的执行文件，还提供了源代码，方便改进、修改以及在其它操作系统上应用,该软件还有一个特点，就是对SVM所涉及的参数调节相对比较少(因为提供了很多的默认参数)，利用这些默认参数就可以解决很多问题,并且提供了交互检验(Cross Validation)的功能。

该软件可以解决C-SVM分类、NU-SVM分类,SVM回归等问题，包括基于一对一算法的多类模式识别问题。SVM用于

模式识别或回归时，SVM方法及其参数、核函 数及其参数的选择，目前国际上还没有形成一个统一的模式

，也就是说最优SVM算法参数选择还只能是凭借经验、实验对比、大范围的搜寻或者利用软件包提供的交

互检验功能进行寻优。



LIBSVM使用方法

LibSVM是以源代码和可执行文件两种方式给出的。如果是Windows系列操作系统，可以直接使用软件包提

供的程序，也可以进行修改编译；如果是Unix类系统，必须自己编译，软件包中提供了编译格式文件，个人在SGI工作站(操作系统IRIX6.5)上，使用免费编译器GNU C++3.3编译通过。

LIBSVM 使用的一般步骤是：

1） 按照LIBSVM软件包所要求的格式准备数据集；

2） 对数据进行简单的缩放操作；

3） 考虑选用RBF 核函数；

4） 采用交叉验证选择最佳参数C与g ；

5） 采用最佳参数C与g 对整个训练集进行训练获取支持向量机模型；

6） 利用获取的模型进行测试与预测。



LIBSVM使用的数据格式

该软件使用的训练数据和检验数据文件格式如下：

<label> <index1>:<value1> <index2>:<value2> ...

1 1 :0.23 2 :0.34 ... 13:1

<label> 是训练数据集的目标值,对于分类，它是标识某类的整数(支持多个类)；对于回归，是任意

实数;

<index> 是以1开始的整数，可以是不连续的(某项可能为空)；

<value>为实数，就是常说的自变量。检验数据文件中的label只用于计算准确度或误差，如果它是未知的，只需用一个数填写这一栏，也可以空着不填。

在程序包中，还包括有一个训练数据实例：heart_scale，方便参考数据文件格式以及练习使用软件.

可以编写小程序，将自己常用的数据格式转换成这种格式

Svmtrain和Svmpredict的用法

LIBSVM软件提供的各种功能都是DOS命令执行方式。我们主要用到两个程序，svmtrain(训练建模)和

svmpredict(使用已有的模型进行预测)，下面分别对这两个程序的使用方法、各参数的意义以及设置方法

做一个简单介绍：

Svmtrain的用法：svmtrain [options] training_set_file [model_file]

Options：可用的选项即表示的涵义如下

-s svm类型：SVM设置类型(默认0)

0 -- C-SVC

1 --v-SVC

2 – 一类SVM

3 -- e -SVR

4 -- v-SVR

-t 核函数类型：核函数设置类型(默认2)

0 – 线性：u'v

1 – 多项式：(r*u'v + coef0)^degree

2 – RBF函数：exp(-r|u-v|^2)

3 –sigmoid：tanh(r*u'v + coef0)

-d degree：核函数中的degree设置(默认3)

-g r(gama)：核函数中的包含r的函数设置(默认1/ k)

-r coef0：核函数中的coef0设置(默认0)

-c cost：设置C-SVC，e-SVR和v-SVR的参数(默认1)



-n nu：设置c-SVC，一类SVM和v- SVR的参数(默认0.5)

-p e：设置e -SVR 中损失函数的值(默认0.1)

-m cachesize：设置cache内存大小，以MB为单位(默认40)

-e ?：设置允许的终止判据(默认0.001)

-h shrinking：是否使用启发式，0或1(默认1)

-wi weight：设置第几类的参数C为weight C(C-SVC中的C)(默认1)

-v n: n-fold交互检验模式

其中-g选项中的k是指输入数据中的属性数。option -v 随机地将数据剖分为n部分并计算交互检验准确度

和均方根误差。

以上这些参数设置可以按照SVM的类型和核函数所支持的参数进行任意组合，如果设置的

参数在函数或SVM类型中没有也不会产生影响，程序不会接受该参数；如果应有的参数设置不正确，参数

将采用默认值。

training_set_file是要进行训练的数据集(如heart_scle)；model_file是训练结束后产生的模型文件(train.model)，文件中包括支持向量样本数、支持向量样本以及lagrange系数等必须的参数；该参数如果不设置将采用默认的文件名，也可以设置成自己惯用的文件名。

Svmpredict的用法：svmpredict test_file model_file output_file

model_file是由svmtrain产生的模型文件(train.model)；test_file是要进行预测的数据文件；Output_file是

svmpredict的输出文件。svm-predict没有其它的选项。

svmtrain -s 0 -c 1000 -t 1 -g 1 -r 1 -d 3 data_file

训练一个由多项式核(u'v+1)^3和C=1000组成的分类器。

svmtrain -s 1 -n 0.1 -t 2 -g 0.5 -e 0.00001 data_file

在RBF核函数exp(-0.5|u-v|^2)和终止允许限0.00001的条件下，训练一个v-SVM (u= 0.1)分类器。

svmtrain -s 3 -p 0.1 -t 0 -c 10 data_file

以线性核函数u'v和C=10及损失函数是0.1求解SVM回归。



**原文地址：**[lib-svm学习（转）](http://blog.sina.com.cn/s/blog_4c70701801013xui.html)**作者：**[destiny_aqu](http://blog.sina.com.cn/u/1282437144)



