
# libsvm使用 - 追求卓越,做到专业 - CSDN博客


2016年08月29日 15:55:10[Waldenz](https://me.csdn.net/enter89)阅读数：449标签：[libsvm																](https://so.csdn.net/so/search/s.do?q=libsvm&t=blog)[分类																](https://so.csdn.net/so/search/s.do?q=分类&t=blog)[预测																](https://so.csdn.net/so/search/s.do?q=预测&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=分类&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=libsvm&t=blog)个人分类：[NLP																](https://blog.csdn.net/enter89/article/category/6390484)
[
																								](https://so.csdn.net/so/search/s.do?q=libsvm&t=blog)


原文地址： http://endual.iteye.com/blog/1262010
首先下载Libsvm、Python和Gnuplot：
l         libsvm的主页[http://www.csie.ntu.edu.tw/~cjlin/libsvm/](http://www.csie.ntu.edu.tw/~cjlin/libsvm/)上下载libsvm （我自己用2.86版本）
l         python的主页[http://www.python.org](http://www.python.org/)下载 python （我自己用2.5版本）
l         gnuplot的主页[http://www.gnuplot.info/](http://www.gnuplot.info/)下载gnuplot  （我用4.0版本）
LIBSVM 使用的一般步骤是：
1）按照LIBSVM软件包所要求的格式准备数据集；
2）对数据进行简单的缩放操作；
3）首要考虑选用RBF 核函数；
4）采用交叉验证选择最佳参数C与g ；
5）采用最佳参数C与g 对整个训练集进行训练获取支持向量机模型；
6）利用获取的模型进行测试与预测。
1）LIBSVM使用的数据格式
该软件使用的训练数据和检验数据文件格式如下：
[label] [index1]:[value1] [index2]:[value2] ...
[label] [index1]:[value1] [index2]:[value2] ...
一行一条记录数据，如:
+1 1:0.708 2:1 3:1 4:-0.320 5:-0.105 6:-1
这里(x,y)à((0.708,1,1, -0.320, -0.105, -1), +1)
label 或说是class, 就是你要分类的种类，通常是一些整数。
index 是有順序的索引，通常是连续的整数。
value 就是用来 train 的数据，通常是一堆实数。
2）对数据进行简单的缩放操作
扫描数据. 因为原始数据可能范围过大或过小, svmscale可以先将数据重新scale (縮放) 到适当范围使训练与预测速度更快。
svmscale.exe的用法：svmscale.exe feature.txt feature.scaled
默认的归一化范围是[-1,1]，可以用参数-l和-u分别调整上界和下届,feature.txt是输入特征文件名 输出的归一化特征名为feature.scaled
3） 考虑选用RBF 核函数
训练数据形成模型（model）,实质是算出了wx+b=0中的w,b.
Svmtrain的用法：svmtrain [options] training_set_file [model_file]
其中options涵义如下:
-s svm类型：设置SVM 类型，默认值为0，可选类型有：
0 -- C- SVC
1 -- nu - SVC
2 -- one-class-SVM
3 -- e - SVR
4 -- nu-SVR
-t 核函数类型：设置核函数类型，默认值为2，可选类型有：
0 -- 线性核：u'*v
1 -- 多项式核：(g*u'*v+ coef0)degree
2 -- RBF 核:exp(-||u-v||*||u-v||/g*g)
3 -- sigmoid 核：tanh(g*u'*v+ coef 0)
-d degree：核函数中的degree设置，默认值为3；
-g r(gama)：核函数中的函数设置(默认1/ k);
-r coef 0：设置核函数中的coef0，默认值为0；
-c cost：设置C- SVC、e - SVR、n - SVR中从惩罚系数C，默认值为1；
-n nu ：设置nu - SVC、one-class-SVM 与nu - SVR 中参数nu ，默认值0.5；
-p e ：核宽,设置e - SVR的损失函数中的e ，默认值为0.1；
-m cachesize：设置cache内存大小，以MB为单位(默认40)：
-e e ：设置终止准则中的可容忍偏差，默认值为0.001；
-h shrinking：是否使用启发式，可选值为0 或1，默认值为1；
-b 概率估计：是否计算SVC或SVR的概率估计，可选值0 或1，默认0；
-wi weight：对各类样本的惩罚系数C加权，默认值为1；
-v n：n折交叉验证模式。
其中-g选项中的k是指输入数据中的属性数。操作参数 -v 随机地将数据剖分为n 部分并计算交叉检验准确度和均方根误差。以上这些参数设置可以按照SVM 的类型和核函数所支持的参数进行任意组合，如果设置的参数在函数或SVM 类型中没有也不会产生影响，程序不会接受该参数；如果应有的参数设置不正确，参数将采用默认值。training_set_file是要进行训练的数据 集；model_file是训练结束后产生的模型文件，该参数如果不设置将采用默认的文件名，也可以设置成自己惯用的文件名。举个例子如下：
C:\libsvm-2.85\windows>svmtrain heart_scale
*
optimization finished, \#iter = 162
nu = 0.431029
obj = -100.877288, rho = 0.424462
nSV = 132, nBSV = 107
Total nSV = 132
现简单对屏幕回显信息进行说明：
\#iter为迭代次数，
nu 与前面的操作参数-n nu 相同，
obj为SVM文件转换为的二次规划求解得到的最小值，
rho 为判决函数的常数项b，
nSV 为支持向量个数，
nBSV为边界上的支持向量个数，
Total nSV为支持向量总个数。
训练后的模型保存为文件*.model，用记事本打开其内容如下：
svm_type c_svc % 训练所采用的svm类型，此处为C- SVC
kernel_type rbf %训练采用的核函数类型，此处为RBF核
gamma 0.0769231 %设置核函数中的g ，默认值为1/ k
nr_class 2 %分类时的类别数，此处为两分类问题
total_sv 132 %总共的支持向量个数
rho 0.424462 %决策函数中的常数项b
label 1 -1%类别标签
nr_sv 64 68 %各类别标签对应的支持向量个数
SV %以下为支持向量
1 1:0.166667 2:1 3:-0.333333 4:-0.433962 5:-0.383562 6:-1 7:-1 8:0.0687023 9:-1 10:-0.903226 11:-1 12:-1 13:1
0.5104832128985164 1:0.125 2:1 3:0.333333 4:-0.320755 5:-0.406393 6:1 7:1 8:0.0839695 9:1 10:-0.806452 12:-0.333333 13:0.5
1 1:0.333333 2:1 3:-1 4:-0.245283 5:-0.506849 6:-1 7:-1 8:0.129771 9:-1 10:-0.16129 12:0.333333 13:-1
1 1:0.208333 2:1 3:0.333333 4:-0.660377 5:-0.525114 6:-1 7:1 8:0.435115 9:-1 10:-0.193548 12:-0.333333 13:1
4）采用交叉验证选择最佳参数C与g
通常而言，比较重要的参数是 gamma (-g) 跟 cost (-c) 。而 cross validation (-v)
的参数常用5。那么如何去选取最优的参数c和g呢？libsvm 的 python 子目录下面的 grid.py 可以帮助我们。 此时。其中安装python2.5需要（一般默认安装到c:\python25
下），将gnuplot解压。安装解压完毕后，进入\libsvm\tools目录下，用文本编辑器（记事
本，edit都可以）修改grid.py文件，找到其中关于gnuplot路径的那项（其默认路径为
gnuplot_exe=r"c:\tmp\gnuplot\bin\pgnuplot.exe"），根据实际路径进行修改，并保存。然
后，将grid.py和C:\Python25目录下的python.exe文件拷贝到libsvm\windows目录下，键入以下命令：$ python grid.py train.1.scale 执行后，即可得到最优参数c和g。
另外，至于下libsvm和python的接口的问题，在libsvm2.86中林老师已经帮助我们解决，在\libsvm\windows\python目录下自带了svmc.pyd这个文件，将该文件文件复制到
libsvm\python目录下，同时，也将python.exe文件复制到该目录下，键入以下命令以检验效
果（注意：.Py文件中关于gnuplot路径的那项路径一定要根据实际路径修改）：
python svm_test.py
如果能看到程序执行结果，说明libsvm和python之间的接口已经配置完成，以后就可以直接在python程序里调用libsvm的函数了！
5） 采用最佳参数C与g 对整个训练集进行训练获取支持向量机模型
$ svmtrain –c x –g x –v x training_set_file [model_file]
x为上述得到的最优参数c和g的值，v的值一般取5。
6）利用获取的模型进行测试与预测
使用Svmtrain训练好的模型进行测试。输入新的X值，给出SVM预测出的Y值
$ Svmpredict  test_file  model_file  output_file
如：./svm-predict heart_scale heart_scale.model heart_scale.out
Accuracy = 86.6667% (234/270) (classification)
这里显示的是结果
一个具体使用的例子。
以libsvm中的heart_scale作为训练数据和测试数据，同时已经将python安装至c盘，并将grid.py文件中关于gnuplot路径的默认值修改为实际解压缩后的路径，将
heart_scale、grid.py和python.exe拷贝至\libsvm\windows文件夹下。
./svm-train heart_scale
optimization finished, \#iter = 162
nu = 0.431029
obj = -100.877288, rho = 0.424462
nSV = 132, nBSV = 107
Total nSV = 132
此时，已经得到heart_scale.model，进行预测：
./svm-predict heart_scale  heart_scale.model  heart_scale.out
Accuracy = 86.6667% (234/270) (classification)
正确率为Accuracy = 86.6667%。
./python grid.py heart_scale
得到最优参数c=2048，g=0.0001220703125.
./svm-train -c 2048 -g 0.0001220703125 heart_scale得到model后，由./svm-predict heart_scale  heart_scale.model heart_scale.out得到的正确
率为Accuracy = 85.1852%.这块还有点迷惑？为什么正确率降低了？
当然也可以结合subset.py 和 easy.py 实现自动化过程。
如果要训练多次，可以写个批处理程序省好多事。
这里举个例子：
::@ echo off
cls
:: split the data and output the results
for /L %%i in (1,1,1000) do python subset.py b59.txt 546 b59(%%i).in8 b59(%%i).out2
for /L %%i in (1,1,1000) do python easy.py b59(%%i).in8 b59(%%i).out2 >> result89.txt
这段批处理代码首先调用subset.py对文件b59.txt执行1000次分层随机抽样(对数据进行80-20%分割)然后调用easy.py 进行1000次参数寻优，把记录结果写到result89.txt中
（包括1000次训练的分类准确率和参数对）。
还可以调用fselect.py进行特征选择，调用plotroc.py进行roc曲线绘制。

