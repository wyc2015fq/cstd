# 如何用OpenCV训练自己的分类器 - 深之JohnChen的专栏 - CSDN博客

2009年11月30日 17:35:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：22806


[http://hi.baidu.com/andyzcj/blog/item/3b9575fc63c3201f09244d9a.html](http://hi.baidu.com/andyzcj/blog/item/3b9575fc63c3201f09244d9a.html)

最近要做一个性别识别的项目，在人脸检测与五官定位上我采用OPENCV的haartraining进行定位，这里介绍下这两天我学习的如何用opencv训练自己的分类器。在这两天的学习里，我遇到了不少问题，不过我遇到了几个好心的大侠帮我解决了不少问题，特别是无忌，在这里我再次感谢他的帮助。

一、简介
目标检测方法最初由Paul Viola [Viola01]提出，并由Rainer Lienhart [Lienhart02]对这一方法进行了改善。该方法的基本步骤为： 首先，利用样本（大约几百幅样本图片）的 harr 特征进行分类器训练，得到一个级联的boosted分类器。
分类器中的"级联"是指最终的分类器是由几个简单分类器级联组成。在图像检测中，被检窗口依次通过每一级分类器， 这样在前面几层的检测中大部分的候选区域就被排除了，全部通过每一级分类器检测的区域即为目标区域。
分类器训练完以后，就可以应用于输入图像中的感兴趣区域的检测。检测到目标区域分类器输出为1，否则输出为0。为了检测整副图像，可以在图像中移动搜索窗口，检测每一个位置来确定可能的目标。 为了搜索不同大小的目标物体，分类器被设计为可以进行尺寸改变，这样比改变待检图像的尺寸大小更为有效。所以，为了在图像中检测未知大小的目标物体，扫描程序通常需要用不同比例大小的搜索窗口对图片进行几次扫描。
目前支持这种分类器的boosting技术有四种： Discrete Adaboost, Real Adaboost, Gentle Adaboost and Logitboost。
"boosted" 即指级联分类器的每一层都可以从中选取一个boosting算法(权重投票)，并利用基础分类器的自我训练得到。
根据上面的分析，目标检测分为三个步骤：
1、 样本的创建
2、 训练分类器
3、 利用训练好的分类器进行目标检测。
二、样本创建
训练样本分为正例样本和反例样本，其中正例样本是指待检目标样本，反例样本指其它任意图片。
负样本
负样本可以来自于任意的图片，但这些图片不能包含目标特征。负样本由背景描述文件来描述。背景描述文件是一个文本文件，每一行包含了一个负样本图片的文件名（基于描述文件的相对路径）。该文件创建方法如下：

**采用Dos命令生成样本描述文件。具体方法是在Dos下的进入你的图片目录，比如我的图片放在D:/face/posdata下，则：**

**按Ctrl+R打开Windows运行程序，输入cmd打开DOS命令窗口，输入d:回车，再输入cd D:/face/negdata进入图片路径，再次输入dir /b > negdata.dat，则会图片路径下生成一个negdata.dat文件，打开该文件将最后一行的negdata.dat删除，这样就生成了负样本描述文件。dos命令窗口结果如下图：**

![](http://hiphotos.baidu.com/andyzcj/abpic/item/446dcaf6bb0fcf09720eecbc.jpg)

   e.g: 负样本描述文件的一个例子：
      假定目录结构如下：
      /img
        img1.jpg
        img2.jpg
        bg.txt
      则背景描述文件bg.txt的内容为：
         img/img1.jpg
         img/img2.jpg

正样本

对于正样本，通常的做法是先把所有正样本裁切好，并对尺寸做规整（即缩放至指定大小），如下图所示：

![](http://hiphotos.baidu.com/andyzcj/abpic/item/5ba0fe6c769ce5d043169486.jpg)

由于HaarTraining训练时输入的正样本是vec文件，所以需要使用OpenCV自带的CreateSamples程序（在你所按照的opencv/bin下，如果没有需要编译opencv/apps/HaarTraining/make下的.dsw文件，注意要编译release版的）将准备好的正样本转换为vec文件。转换的步骤如下：

1) 制作一个正样本描述文件，用于描述正样本文件名（包括绝对路径或相对路径），正样本数目以及各正样本在图片中的位置和大小。典型的正样本描述文件如下：

posdata/1(10).bmp 1 1 1 23 23
posdata/1(11).bmp 1 1 1 23 23
posdata/1(12).bmp 1 1 1 23 23

不过你可以把描述文件放在你的posdata路径（即正样本路径）下，这样你就不需要加前面的相对路径了。同样它的生成方式可以用负样本描述文件的生成方法，最后用txt的替换工具将“bmp”全部替换成“bmp 1 1 1 23 23
”就可以了，如果你的样本图片多，用txt替换会导致程序未响应，你可以将内容拷到word下替换，然后再拷回来。bmp后面那五个数字分别表示图片个数，目标的起始位置及其宽高。这样就生成了正样本描述文件posdata.dat。

2) 运行CreateSamples程序。如果直接在VC环境下运行，可以在Project/Settings/Debug属性页的Program arguments栏设置运行参数。下面是一个运行参数示例：
-info D:/face/posdata/posdata.dat -vec D:/face/pos.vec -num 50 -w 20 -h 20
表示有50个样本，样本宽20，高20，正样本描述文件为posdata.dat，结果输出到pos.vec。

或者在dos下输入：

"D:/Program Files/OpenCV/bin/createsamples.exe" -info "posdata/posdata.dat" -vec data/pos.vec -num 50 -w 20 -h 20 
运行完了会d:/face/data下生成一个*.vec的文件。该文件包含正样本数目，宽高以及所有样本图像数据。结果入下图：

![](http://hiphotos.baidu.com/andyzcj/pic/item/0835c1096a475dfa63d9864a.jpg)

Createsamples程序的命令行参数：
命令行参数：
－vec <vec_file_name>
训练好的正样本的输出文件名。
－img<image_file_name>
源目标图片（例如：一个公司图标）
－bg<background_file_name>
背景描述文件。
－num<number_of_samples>
要产生的正样本的数量，和正样本图片数目相同。
－bgcolor<background_color>
背景色（假定当前图片为灰度图）。背景色制定了透明色。对于压缩图片，颜色方差量由bgthresh参数来指定。则在bgcolor－bgthresh和bgcolor＋bgthresh中间的像素被认为是透明的。
－bgthresh<background_color_threshold>
－inv
如果指定，颜色会反色
－randinv
如果指定，颜色会任意反色
－maxidev<max_intensity_deviation>
背景色最大的偏离度。
－maxangel<max_x_rotation_angle>
－maxangle<max_y_rotation_angle>，
－maxzangle<max_x_rotation_angle>
最大旋转角度，以弧度为单位。
－show
如果指定，每个样本会被显示出来，按下"esc"会关闭这一开关，即不显示样本图片，而创建过程继续。这是个有用的debug选项。
－w<sample_width>
输出样本的宽度（以像素为单位）
－h《sample_height》
输出样本的高度，以像素为单位。
到此第一步样本训练就完成了。恭喜你，你已经学会训练分类器的五成功力了，我自己学这个的时候花了我一天的时间，估计你几分钟就学会了吧。
三、训练分类器
样本创建之后，接下来要训练分类器，这个过程是由haartraining程序来实现的。该程序源码由OpenCV自带，且可执行程序在OpenCV安装目录的bin目录下。
Haartraining的命令行参数如下：
－data<dir_name>
存放训练好的分类器的路径名。
－vec<vec_file_name>
正样本文件名（由trainingssamples程序或者由其他的方法创建的）
－bg<background_file_name>
背景描述文件。
－npos<number_of_positive_samples>，
－nneg<number_of_negative_samples>
用来训练每一个分类器阶段的正/负样本。合理的值是：nPos = 7000;nNeg = 3000
－nstages<number_of_stages>
训练的阶段数。
－nsplits<number_of_splits>
决定用于阶段分类器的弱分类器。如果1，则一个简单的stump classifier被使用。如果是2或者更多，则带有number_of_splits个内部节点的CART分类器被使用。
－mem<memory_in_MB>
预先计算的以MB为单位的可用内存。内存越大则训练的速度越快。
－sym（default）
－nonsym
指定训练的目标对象是否垂直对称。垂直对称提高目标的训练速度。例如，正面部是垂直对称的。
－minhitrate《min_hit_rate》
每个阶段分类器需要的最小的命中率。总的命中率为min_hit_rate的number_of_stages次方。
－maxfalsealarm<max_false_alarm_rate>
没有阶段分类器的最大错误报警率。总的错误警告率为max_false_alarm_rate的number_of_stages次方。
－weighttrimming<weight_trimming>
指定是否使用权修正和使用多大的权修正。一个基本的选择是0.9
－eqw
－mode<basic(default)|core|all>
选择用来训练的haar特征集的种类。basic仅仅使用垂直特征。all使用垂直和45度角旋转特征。
－w《sample_width》
－h《sample_height》
训练样本的尺寸，（以像素为单位）。必须和训练样本创建的尺寸相同。
一个训练分类器的例子：
"D:/Program Files/OpenCV/bin/haartraining.exe"   -data data/cascade -vec data/pos.vec -bg negdata/negdata.dat -npos 49 -nneg 49 -mem 200 -mode ALL -w 20 -h 20

训练结束后，会在目录data下生成一些子目录，即为训练好的分类器。

训练结果如下：

![](http://hiphotos.baidu.com/andyzcj/pic/item/7e974e1168376e2a203f2e3d.jpg)

恭喜你，你已经学会训练分类器的九成功力了。

四：利用训练好的分类器进行目标检测。

这一步需要用到performance.exe，该程序源码由OpenCV自带，且可执行程序在OpenCV安装目录的bin目录下。

performance.exe -data data/cascade -info posdata/test.dat -w 20 -h 20 -rs 30

performance的命令行参数如下：

Usage: ./performance
-data <classifier_directory_name>
-info <collection_file_name>
[-maxSizeDiff <max_size_difference = 1.500000>]
[-maxPosDiff <max_position_difference = 0.300000>]
[-sf <scale_factor = 1.200000>]
[-ni]
[-nos <number_of_stages = -1>]
[-rs <roc_size = 40>]
[-w <sample_width = 24>]
[-h <sample_height = 24>]

也可以用opencv的cvHaarDetectObjects函数进行检测：

CvSeq* faces = cvHaarDetectObjects( img, cascade, storage,
                                            1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(40, 40) ); //3. 检测人脸
注：OpenCv的某些版本可以将这些目录中的分类器直接转换成xml文件。但在实际的操作中，haartraining程序却好像永远不会停止，而且没有生成xml文件，后来在OpenCV的yahoo论坛上找到一个haarconv的程序，才将分类器转换为xml文件，其中的原因尚待研究。

