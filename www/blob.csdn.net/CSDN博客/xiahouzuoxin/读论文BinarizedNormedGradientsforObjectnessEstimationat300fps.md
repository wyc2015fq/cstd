# 读论文BinarizedNormedGradientsforObjectnessEstimationat300fps - xiahouzuoxin - CSDN博客





2014年12月03日 09:41:07[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：7832








## 关于论文

这两天翻了翻cvpr2014的论文，发现程明明老师关于Objectness Detecting的论文，于是拜读了一番。论文贡献了两个观点：
- 
目标有closed boundary，因此将窗口resize到8x8也能进行目标和背景的识别，这实际上降低了窗口的分辨率，resize到8x8目的是加速计算。这就相当于我们看路上走的人一样，在很远的地方即使我们没看清楚脸，只是看到一个轮廓也能识别出是不是我们认识的人，反而，如果脸贴着脸去看一个人可能会认不出来。作者还使用了最简单的梯度特征，运算量非常小。

- 
作者巧妙的将对窗口打分（分数越高，越可能是目标，否则越可能是背景）的计算转化为（或近似）通过位运算来实现，并以此为基础达到单幅图片的计算时间为0.003s。


窗口打分是通过一个线性模型来操作的（其实就是一个滤波器），

![](http://www.forkosh.com/mathtex.cgi?%20s_l=%5Clangle%5Cmathbf%7Bw%7D,%5Cmathbf%7Bg_l%7D%5Crangle......(1))

为获得权值w，则必须通过训练数据训练，作者采用了最简单的Linear SVM，大致过程应该是：对训练用数据，目标窗和背景窗分别给定不同的分数（从程序上看，目标窗是1，背景窗是-1），训练数据经过Linear SVM调整w使训练数据的误差最小，得到调整w向量就用于预测中的窗口打分，打分越接进1的窗口为目标窗，否则为背景窗。

使用Linear SVM训练打分窗口的参数倒也没什么，重点在：窗口的预处理中考虑目标一般情况下“不会太小”，选择了一些固定的sliding window，如，10x160,10x320等，并且采取了降低窗口分辨率的方式，将窗口都resize到8x8，之后再进行窗口打分或训练w的操作。我对resize这种操作的理解是：虽然resize会降低前景与背景的差异，可能使前背景难以区分，但这种操作同时也减小了背景和背景之间、前景和前景之间的差异，但只要“背景和背景之间、前景和前景之间的差异”减小得比“前景与背景的差异”更多一些，则还是对区分前景和背景有利的，只不过应该可以找个折中，作者貌似为了计算的效率，直接resize到8x8了。所以(1)中的w和gl都是64维的向量。

既然得到了w，就能直接根据(1)计算窗口的分数，确定预测目标了，但作者没有简单的按(1)式做，而是将(1)的操作转化为位运算，这也是为什么特征称为BING（B就是Binarized），直接采用硬件指令大幅度地提升速度。为使用二进制运算，必须将w和gl都转成二进制的模型。Algorithm1就是将w转成二进制模型的算法，我感觉原理大致就是：将w在投影到不同的正交向量上，如果还不理解Algorithm1，好好看看算法是怎么操作的，那不就是“Gram-Schmidt正交化”吗？只不过只取了包含大部分信息的前Nw个正交向量作为输出，目的也是为了降低计算量。NG特征gl转成二进制模型是

![](http://www.forkosh.com/mathtex.cgi?%20g_l=%5Csum_%7Bk=1%7D%5E%7BN_g%7D2%5E%7B8-k%7D%5Cmathbf%7Bb_%7Bk,l%7D%7D....(2))

我觉得大概的意思好像是，比如一个十进制的数121D，转成二进制就是0111 1001B，也可以直接将低位截断（这时Ng=3），用0111 1000近似代替121D。不过这里还是有些不明白，b_kl不是8x8维的特征吗？就不明白这里是什么意思了，矩阵求sum会得到标量的gl？感觉这一段下表用得有些混乱，没解释太清楚。而为了计算64维的BING特征，要扫描64个点，作者用Algorithm2也是通过二进制的移位运算降低计算量，就如作者原文所说的——有些类似于积分图像的计算一样（with the integral image representation）。

最后将算法1和2结合起来对窗口打分的操作由卷积运算变成了大部分是位运算操作，

![](http://www.forkosh.com/mathtex.cgi?%20s_l=%5Csum_%7Bj=1%7D%5E%7BN_w%7D%5Cbeta_j%5Csum_%7Bk=1%7D%5E%7BN_g%7DC_%7Bj,k%7D....(3))

其中C_j,k是

![](http://www.forkosh.com/mathtex.cgi?%20C_%7Bj,k%7D=2%5E%7B8-k%7D(2%5Clangle%7Ba_j%7D%5E+,%20%5Cmathbf%7Bb_%7Bk,l%7D%7D%5Crangle-%7C%5Cmathbf%7Bb_%7Bk,l%7D%7D%7C).....(4))

上面的计算很容易通过位运算和SSE指令（支持8x8=64bit）来完成快速运算。

## 关于程序

程老师的程序也跑了遍，大致看了看，太佩服大牛了，即使想得到，这程序也不一定能写得好，居然还用的C++。程序配置上需要安装OpenCV，貌似低的版本还不行，作者原先用的VS版本是VS2012，如果不想自己重新编译OpenCV，最好使用OpenCV 2.4.8以上（我用的是2.4.10）。准备好OpenCV环境后，还需要的准备工作包括：
- 
下载VOC数据库，作者的页面给了链接，但要注意Annotations和VOC官方的xml格式不同，作者转成了yml格式方便OpenCV读，下载后覆盖原VOC数据库中的xml文件即可。这些都可以在 [http://mmcheng.net/bing/](http://mmcheng.net/bing/) 的Downloads选项中找到链接

- 
配置VS2012：因为程序中使用了并行处理，所以要开启/openmp，在配置的“C/C++->语言”选项中，另外可能需要SSE指令，在“C/C++->代码生成”中启用/arch:SSE2，不过貌似我的启用了，但编译时提示`忽略未知选项“/arch:SSE2”`，在我的x64上用不了，但也索性先这样跑着


我电脑上跑出来的效果貌似和论文给的还是差了一大截，但运算速度相对其它以前的方法来说已经好了不知多少了，

![跑出来的结果](http://xiahouzuoxin.github.io/notes/images/%E8%AF%BB%E8%AE%BA%E6%96%87BinarizedNormedGradientsforObjectnessEstimationat300fps/Objectness.png)
跑出来的结果


StageI也就是训练w参数居然用了13s，StageII用了344s居然，单张图片0.1s。把程序最后一行`objNess.illuTestReults(boxesTests);`注释去了，在VOC2007/Local/下能看到图片预测目标窗口的结果

![目标窗结果](http://xiahouzuoxin.github.io/notes/images/%E8%AF%BB%E8%AE%BA%E6%96%87BinarizedNormedGradientsforObjectnessEstimationat300fps/%E7%9B%AE%E6%A0%87%E7%AA%97%E7%BB%93%E6%9E%9C.png)
目标窗结果


效果还是不错的，精度还没怎么做分析，程序运行结束后，`VOC2007/Results/`下生成有个`PerImgAll.m`的文件，直接在Matlab中就能跑出结果：在1000样本处DR约96%，2038以上DR就达到97%了。

![DRandMABO](http://xiahouzuoxin.github.io/notes/images/%E8%AF%BB%E8%AE%BA%E6%96%87BinarizedNormedGradientsforObjectnessEstimationat300fps/DRandMABO.png)
DRandMABO


上面的精度曲线称为DR-#WIN curves，源自TPAMI 2012的一篇论文：Measuring the objectness of image windows。原文还提出了将窗口数量比如[[0,5000]归一化到[0,1]之间，用曲线下的面积作为目标检测的度量结果，并称之为the area under the curve(AUC)，这样AUC的范围就在[0,1]之间了。

### 检测精度DR的计算

DR的计算是参考[The PASCAL Visual Object Classes (VOC) Challenge](http://pascallin.ecs.soton.ac.uk/challenges/VOC/pubs/everingham10.pdf)，目标检测任务中DR的计算的是true/false positive精度，将算法检测目标结果放到groud truth中，将“预测目标区域与groud truth区域的交集”除以“预测目标区域与groud truth区域的并集”作为DR：

![](http://www.forkosh.com/mathtex.cgi?%20DR%20=%20%5Cfrac%7Barea(B_p%20%5Cbigcap%20B_%7Bbg%7D)%7D%7Barea(B_p%20%5Cbigcup%20B_%7Bbg%7D)%7D)

DR自少在50%以上才算目标检测正确，其实，50%已经是很低的了，几乎不能做为检测结果，难怪那些个算法（BING这篇文章也是）随随便便都到95%以上了。



