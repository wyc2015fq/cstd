# libsvm笔记系列（1）——编译使用LIBSVM - xiahouzuoxin - CSDN博客





2013年07月18日 16:39:49[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：10267








**LIBSVM **is an integrated software for support vector classification, (C-SVC,[nu-SVC](http://www.csie.ntu.edu.tw/~cjlin/libsvm/index.html#nuandone)),regression (epsilon-SVR,[nu-SVR](http://www.csie.ntu.edu.tw/~cjlin/libsvm/index.html#nuandone))
 and distribution estimation ([one-class SVM](http://www.csie.ntu.edu.tw/~cjlin/libsvm/index.html#nuandone)). It supports multi-class classification.

LIBSVM是一个集成软件包，提供支持向量机分类（C-SVC,nu-SVC），回归（epsilon-SVR,nu-SVR）以及分布估计（one-class SVM）.工具包支持多类分类问题。




LIBSVM是台湾大学林智仁(LinChih-Jen)副教授等开发设计的一个简单、易于使用和快速有效的SVM模式识别与回归的软件包。

以上介绍来自LIBSVM官网：http://www.csie.ntu.edu.tw/~cjlin/libsvm/index.html

官网上提供了软件包及各种其它工具的下载。




***1. 编译***

拿到软件包的第一件事就是阅读README，面对“读我……读我”这么热情的呼唤你难道无动于衷？

*On Unix systems, type `make' to build the `svm-train' and `svm-predict'programs. Run them without arguments to show the usages of them.*


只要在libsvm目录下使用make命令编译就OK了。




*2.使用——训练*

如果你对SVM还没有任何了解，请你先参考机器学习及模式识别相关书籍。如果你用过matlab，http://www.matlabsky.com/thread-10966-1-1.html以matlab版本的libsvm为基础提供了许多清晰易懂的讲解。




在入门阶段，我们还是死抓着README不放，接下来写了什么？数据格式，

*The format of training and testing data file is:<label> <index1>:<value1> <index2>:<value2> ...*


label，需要分几类就有几个不同的标签值，对于训练数据，标签一定是已知的，对于测试数据，标签用来衡量精度，对于新的要预测的数据，标签是未知的，目的就是为了预测标签，此时数据文件的第一列可以设为任意值；

index值是递增的，若要使用自定义的核函数，index从0开始，否则从1开始；

value表示分类目标的特征值，一行特征值[value1
 value2 value3 ... valuen]形成一个特征向量。




LIBSVM中给了一个测试用的数据文件heart_scale，其中部分数据如下：

*+1 1:0.708333 2:1 3:1 4:-0.320755 5:-0.105023 6:-1 7:1 8:-0.419847 9:-1 10:-0.225806 12:1 13:-1-1 1:0.583333 2:-1 3:0.333333 4:-0.603774 5:1 6:-1 7:1 8:0.358779 9:-1 10:-0.483871 12:-1 13:1+1 1:0.166667 2:1 3:-0.333333 4:-0.433962 5:-0.383562 6:-1 7:-1 8:0.0687023 9:-1 10:-0.903226 11:-1 12:-1 13:1-1 1:0.458333 2:1 3:1 4:-0.358491 5:-0.374429 6:-1 7:-1 8:-0.480916 9:1 10:-0.935484 12:-0.333333 13:1-1 1:0.875 2:-1 3:-0.333333 4:-0.509434 5:-0.347032 6:-1 7:1 8:-0.236641 9:1 10:-0.935484 11:-1 12:-0.333333 13:-1-1 1:0.5 2:1 3:1 4:-0.509434 5:-0.767123 6:-1 7:-1 8:0.0534351 9:-1 10:-0.870968 11:-1 12:-1 13:1+1 1:0.125 2:1 3:0.333333 4:-0.320755 5:-0.406393 6:1 7:1 8:0.0839695 9:1 10:-0.806452 12:-0.333333 13:0.5*

标签只有+1和-1两类，索引值从1~13，因此特征向量的长度为13。




下面使用编译得到的svm-train对heart_scale数据进行训练。



```
[monkeyzx@CentOS libsvm-3.14]$ ./svm-train
Usage: svm-train [options] training_set_file [model_file]
options:
-s svm_type : set type of SVM (default 0)
	0 -- C-SVC		(multi-class classification)
	1 -- nu-SVC		(multi-class classification)
	2 -- one-class SVM
	3 -- epsilon-SVR	(regression)
	4 -- nu-SVR		(regression)
-t kernel_type : set type of kernel function (default 2)
	0 -- linear: u'*v
	1 -- polynomial: (gamma*u'*v + coef0)^degree
	2 -- radial basis function: exp(-gamma*|u-v|^2)
	3 -- sigmoid: tanh(gamma*u'*v + coef0)
	4 -- precomputed kernel (kernel values in training_set_file)
-d degree : set degree in kernel function (default 3)
-g gamma : set gamma in kernel function (default 1/num_features)
-r coef0 : set coef0 in kernel function (default 0)
-c cost : set the parameter C of C-SVC, epsilon-SVR, and nu-SVR (default 1)
-n nu : set the parameter nu of nu-SVC, one-class SVM, and nu-SVR (default 0.5)
-p epsilon : set the epsilon in loss function of epsilon-SVR (default 0.1)
-m cachesize : set cache memory size in MB (default 100)
-e epsilon : set tolerance of termination criterion (default 0.001)
-h shrinking : whether to use the shrinking heuristics, 0 or 1 (default 1)
-b probability_estimates : whether to train a SVC or SVR model for probability estimates, 0 or 1 (default 0)
-wi weight : set the parameter C of class i to weight*C, for C-SVC (default 1)
-v n: n-fold cross validation mode
-q : quiet mode (no outputs)
```
当没有输入数据时，svm-train将打印出其格式和用法。







```
[monkeyzx@CentOS libsvm-3.14]$ ./svm-train heart_scale svm_model
*
optimization finished, #iter = 162
nu = 0.431029
obj = -100.877288, rho = 0.424462
nSV = 132, nBSV = 107
Total nSV = 132
```
从svm-train对heart_scale的训练结果可以看出，总的优化迭代次数为162，支持向量的数量为132，其它几个值都是SVM相关的参数。训练结果生成model，该model将用于对新样本的预测（因为新样本的标签是未知的，预测就是根据新样本的特征预测其标签）。我们可以看看model_file里面有什么，


*svm_type c_svc                      // 支持向量机类型kernel_type rbf                       //   核函数类型为RBFgamma 0.0769231                //   核函数的gamma值nr_class 2                               //  分类类数total_sv 132                           //   总的支持向量个数rho 0.424462                         //   支持向量机的判决函数的常量label 1 -1                                //   标签nr_sv 64 68                           //   与标签对应的支持向量个数，加起来为132SV                                          //   下面都是支持向量1 1:0.166667 2:1 3:-0.333333 4:-0.433962 5:-0.383562 6:-1 7:-1 8:0.0687023 9:-1 10:-0.903226 11:-1 12:-1 13:10.5104832128985153 1:0.125 2:1 3:0.333333 4:-0.320755 5:-0.406393 6:1 7:1 8:0.0839695 9:1 10:-0.806452 12:-0.333333 13:0.51 1:0.333333 2:1 3:-1 4:-0.245283 5:-0.506849 6:-1 7:-1 8:0.129771 9:-1 10:-0.16129 12:0.333333 13:-11 1:0.208333 2:1 3:0.333333 4:-0.660377 5:-0.525114 6:-1 7:1 8:0.435115 9:-1 10:-0.193548 12:-0.333333 13:11 1:0.166667 2:1 3:0.333333 4:-0.358491 5:-0.52968 6:-1 7:1 8:0.206107 9:-1 10:-0.870968 12:-0.333333 13:11 1:0.25 2:1 3:-1 4:0.245283 5:-0.328767 6:-1 7:1 8:-0.175573 9:-1 10:-1 11:-1 12:-1 13:-11 1:-0.541667 2:1 3:1 4:0.0943396 5:-0.557078 6:-1 7:-1 8:0.679389 9:-1 10:-1 11:-1 12:-1 13:1*

...






*2.使用——测试*



```
[monkeyzx@CentOS libsvm-3.14]$ ./svm-predict
Usage: svm-predict [options] test_file model_file output_file
options:
-b probability_estimates: whether to predict probability estimates, 0 or 1 (default 0); for one-class SVM only 0 is supported
-q : quiet mode (no outputs)
```




test_file就是测试文件，格式与训练时数据文件格式一样。


model_file就是svm-train的输出model文件。

output_file为svm_predict的输出结果文件，我们可以看一下输出结果是什么，

1

-1

-1

1

-1

-1

1

1

1

1

1

1

-1
...


在测试时就是预测的标签结果，通过该标签结果与给定的包含基准事实的label比较，得到分类精度，从而衡量分类器的性能。





最终，我在资源中心上传了一份[林智仁的讲义](http://download.csdn.net/detail/xiahouzuoxin/5778927)

SVM理论基础看这个基本上就够了，需要深入则可以参考其中给出的参考文献。







