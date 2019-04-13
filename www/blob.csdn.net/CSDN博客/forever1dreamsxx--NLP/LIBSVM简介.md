
# LIBSVM简介 - forever1dreamsxx--NLP - CSDN博客


2012年12月04日 14:44:11[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：423个人分类：[算法																](https://blog.csdn.net/forever1dreamsxx/article/category/1226549)[SVM																](https://blog.csdn.net/forever1dreamsxx/article/category/1295874)[
							](https://blog.csdn.net/forever1dreamsxx/article/category/1226549)


转载地址：[http://blog.csdn.net/caohao2008/article/details/1646388](http://blog.csdn.net/caohao2008/article/details/1646388)

LIBSVM简介
支持向量机所涉及到的数学知识对一般的化学研究者来说是比较难的，自己编程实现该算法难度就更大了。但是现在的网络资源非常发达，而且国际上的科学研究者把他们的研究成果已经放在网络上，免费提供给用于研究目的，这样方便大多数的研究者，不必要花费大量的时间理解SVM算法的深奥数学原理和计算机程序设计。目前有关SVM计算的相关软件有很多，如LIBSVM、mySVM、SVMLight等，这些软件大部分的免费下载地址和简单介绍都可以在[http://www.kernel-machines.org/](http://www.kernel-machines.org/)上获得。
LIBSVM是台湾大学林智仁(Lin Chih-Jen)副教授等开发设计的一个简单、易于使用和快速有效的SVM模式识别与回归的软件包，他不但提供了编译好的可在Windows系列系统的执行文件，还提供了源代码，方便改进、修改以及在其它操作系统上应用；该软件还有一个特点，就是对SVM所涉及的参数调节相对比较少，提供了很多的默认参数，利用这些默认参数就可以解决很多问题；并且提供了交互检验(Cross Validation)的功能。该软件包可以在[http://www.csie.ntu.edu.tw/~cjlin/](http://www.csie.ntu.edu.tw/~cjlin/)免费获得。该软件可以解决C-SVM分类、&O1550;-SVM分类、&O1541;-SVM回归和&O1550;-SVM回归等问题，包括基于一对一算法的多类模式识别问题。在第2章中我们也介绍了该软件的一些优点，因此通过综合考虑，我们决定采用该软件作为工作软件。SVM用于模式识别或回归时，SVM方法及其参数、核函数及其参数的选择，目前国际上还没有形成一个统一的模式，也就是说最优SVM算法参数选择还只能是凭借经验、实验对比、大范围的搜寻或者利用软件包提供的交互检验功能进行寻优。
LIBSVM使用方法
LibSVM是以源代码和可执行文件两种方式给出的。如果是Windows系列操作系统，可以直接使用软件包提供的程序，也可以进行修改编译；如果是Unix类系统，必须自己编译，软件包中提供了编译格式文件，我们在SGI工作站(操作系统IRIX6.5)上，使用免费编译器GNU C++3.3编译通过。
LIBSVM使用的数据格式
该软件使用的训练数据和检验数据文件格式如下：
<label> <index1>:<value1> <index2>:<value2> ...
其中<label> 是训练数据集的目标值，对于分类，它是标识某类的整数(支持多个类)；对于回归，是任意实数。<index> 是以1开始的整数，可以是不连续的；<value>为实数，也就是我们常说的自变量。检验数据文件中的label只用于计算准确度或误差，如果它是未知的，只需用一个数填写这一栏，也可以空着不填。在程序包中，还包括有一个训练数据实例：heart_scale，方便参考数据文件格式以及练习使用软件。
可以编写小程序，将自己常用的数据格式转换成这种格式
Svmtrain和Svmpredict的用法
LIBSVM软件提供的各种功能都是DOS命令执行方式。我们主要用到两个程序，svmtrain(训练建模)和svmpredict(使用已有的模型进行预测)，下面分别对这两个程序的使用方法、各参数的意义以及设置方法做一个简单介绍：
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
-g r(gama)：核函数中的&O1543;函数设置(默认1/ k)
-r coef0：核函数中的coef0设置(默认0)
-c cost：设置C-SVC，&O1541; -SVR和&O1550;-SVR的参数(默认1)
-n nu：设置&O1550;-SVC，一类SVM和&O1550;- SVR的参数(默认0.5)
-p e：设置&O1541; -SVR 中损失函数&O1541;的值(默认0.1)
-m cachesize：设置cache内存大小，以MB为单位(默认40)
-e &O1541;：设置允许的终止判据(默认0.001)
-h shrinking：是否使用启发式，0或1(默认1)
-wi weight：设置第几类的参数C为weight&O1620;C(C-SVC中的C)(默认1)
-v n: n-fold交互检验模式
其中-g选项中的k是指输入数据中的属性数。option -v 随机地将数据剖分为n部分并计算交互检验准确度和均方根误差。以上这些参数设置可以按照SVM的类型和核函数所支持的参数进行任意组合，如果设置的参数在函数或SVM类型中没有也不会产生影响，程序不会接受该参数；如果应有的参数设置不正确，参数将采用默认值。training_set_file是要进行训练的数据集；model_file是训练结束后产生的模型文件，文件中包括支持向量样本数、支持向量样本以及lagrange系数等必须的参数；该参数如果不设置将采用默认的文件名，也可以设置成自己惯用的文件名。
Svmpredict的用法：svmpredict test_file model_file output_file
model_file是由svmtrain产生的模型文件；test_file是要进行预测的数据文件；Output_file是svmpredict的输出文件。svm-predict没有其它的选项。
svmtrain -s 0 -c 1000 -t 1 -g 1 -r 1 -d 3 data_file
训练一个由多项式核(u'v+1)^3和C=1000组成的分类器。
svmtrain -s 1 -n 0.1 -t 2 -g 0.5 -e 0.00001 data_file
在RBF核函数exp(-0.5|u-v|^2)和终止允许限0.00001的条件下，训练一个&O1550;-SVM (&O1550; = 0.1)分类器。
svmtrain -s 3 -p 0.1 -t 0 -c 10 data_file
以线性核函数u'v和C=10及损失函数&O1541;= 0.1求解SVM回归。

源代码下载网址：
Libsvm[http://www.csie.ntu.edu.tw/~cjlin/libsvm/](http://www.csie.ntu.edu.tw/~cjlin/libsvm/)
Svmlight[http://svmlight.joachims.org/](http://svmlight.joachims.org/)
Mysvm
[http://www-ai.cs.uni-dortmund.de/SOFTWARE/MYSVM/index.html](http://www-ai.cs.uni-dortmund.de/SOFTWARE/MYSVM/index.html)
SVMTorch[http://www.idiap.ch/machine-learning.php](http://www.idiap.ch/machine-learning.php)
http://www.kernel-machines.org/software.html
重要参考文献：
[1] N. Cristianini, and T.J. Shawe, An Introduction to Support Vector Machines and other Kernel-based Learning Methods, Cambridge University Press, New York, 2000.
[2] R.E. Fan, P.H. Chen, and C.J. Lin, Working Set Selection using Second Order Information for Training Support Vector Machine, Journal of Machine Learning Research, 2005, 6:1889-1918.

