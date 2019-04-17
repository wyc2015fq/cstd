# 没GPU也能玩梵高作画：Ubuntu tensorflow CPU版 - 结构之法 算法之道 - CSDN博客





2016年09月27日 19:39:04[v_JULY_v](https://me.csdn.net/v_JULY_v)阅读数：36671
所属专栏：[机器学习十大算法系列](https://blog.csdn.net/column/details/datamining.html)









> 
## **          没GPU也能玩梵高作画：Ubuntu tensorflow CPU版**








作者：七月在线开发/市场团队骁哲、李伟、July
时间：二零一六年九月二十七日
交流：TensorFlow实战交流Q群 472899334，有问题可以加此群共同交流。另探究实验背后原理，请参看：[深度学习在线班](https://www.julyedu.com/category/index/8/21)。








## 一、前言

    9月22号，我们开发/市场团队的两同事利用DL学梵高作画，安装cuda 8.0趟遍无数坑，很多朋友求避坑。因此，3天后的9月25日，便把教程《[教你从头到尾利用DL学梵高作画：GTX 1070 cuda 8.0 tensorflow gpu版](http://blog.csdn.net/v_july_v/article/details/52658965)》公布出去。但令人尴尬的是，不少同学没有GTX 1070、甚至没有一块像样的GPU。

    对于在北京的朋友，我们可以提供1070机器免费给大家实验，但对于更多不在北京的朋友呢？为了让**每一个人都能玩一把**，特此发布本tensorflow作画的CPU版教程。你会看到，搭建过程相比较GPU版本而言简单太多太多了，但缺点是没有了GPU，计算过程非常熬人，i7-6700大约1.5h（要知道，如果搭好GPU，最后计算过程就几分钟的事）。

    但不管咋样，之前你说没有GTX 1070所以没去动手，那现在有个CPU就能跑了，不受硬件条件限制，何不现在就动手试一把？希望更多朋友与我们一起玩更多有趣、好玩的实验。








## 二、安装

### 2.1、安装tensorflow

 Windows下打开浏览器，输入：[https://github.com/tensorflow/tensorflow](https://github.com/tensorflow/tensorflow)



下载Linux CPU-only:Python2 如下图，后面步骤3.1会用到此文件

![](https://img-blog.csdn.net/20160927194203447)


### 2.2 安装neural-style

	再在地址栏输入：[https://github.com/anishathalye/neural-style](https://github.com/anishathalye/neural-style)

> 


![](https://img-blog.csdn.net/20160927194218661)


> 



### 2.3 安装vgg19

再在地址栏输入：[http://www.vlfeat.org/matconvnet/models/beta16/imagenet-vgg-verydeep-19.mat](http://www.vlfeat.org/matconvnet/models/beta16/imagenet-vgg-verydeep-19.mat)

弹出一个下载对话框，直接下载。

下载后的文件放到刚刚的neural-style文件夹根目录下就行，如图就是我下载复制好的文件样子。

![](https://img-blog.csdn.net/20160927194240510)


PS：记得将以上所有文件夹放到自己U盘，以备Ubuntu下使用。

### 2.4、安装Ubuntu14.04

装Ubuntu16.04一样，CPU对Ubuntu版本无要求。

下载Ubuntu14.04：[http://www.ubuntu.com/download/alternative-downloads](http://www.ubuntu.com/download/alternative-downloads)（选择64位下载）

> 


> 
> 
> 
![](https://img-blog.csdn.net/20160925150305399)









UltraISO软碟通：

[http://cn.ultraiso.net/xiazai.html](http://cn.ultraiso.net/xiazai.html)（下载地址）

接下来，直接安装：[http://jingyan.baidu.com/article/eb9f7b6d8536a8869364e813.html](http://jingyan.baidu.com/article/eb9f7b6d8536a8869364e813.html)

如果遇到问题，详见GPU版本中关于安装Ubuntu的说明。^__^





## 三、实验

### 3.1 准备工作 

打开Ubuntu的命令窗口，输入：sudo –i

 *** （你的密码）

apt-get update（更新所有源）

sudo apt-get install python-dev python-pip Python-scipy git

（以上命令是下载一些tensorflow的必要库）

输入完以上命令后，将刚刚保存下载文件的U盘插入，然后将文件复制到Download目录下（其他目录也可以，熟悉Linux的同学自行搞定就OK！）

接着输入：cd  /home/***(你的用户名)/Download/            回车键



输入：pip install tensorflow-0.10.0-cp27-none-linux_x86_64.whl  //此文件为 2.1步骤中下载

### 3.2 开始实验

成功后输入：cd  neural-style- neural-style-master  进入neural文件夹目录下

输入：python neural_style.py --content ./examples/1-content.jpg --styles ./examples/1-style.jpg --output ./examples/JulyEdu.jpg回车键

然后等待

等待着下课
等待着放学
等待游戏的童年


…

![](https://img-blog.csdn.net/20160927195650165)


    等待就好了（你自会体会到我们之前所说的这句话的：等待的过程就像生孩子一样），计算过程可能一两个小时，可能3、4个小时，可能更久（看CPU配置和图片大小）。

    但直到“孩子”生出来，你会加倍热爱之。![](https://img-blog.csdn.net/20160927200813087)





## 其他教程

    咱们梵高作画的GPU和CPU教程出来后，许许多多的朋友开始尝试搭建、实验，大有全名DL、全名实验之感。以下是更多教程，供君参考，欢迎加入我们一起实验、一起玩！
- [梵高作画 MAC + tensorflow + CPU 版本](https://ask.julyedu.com/question/7403) （mac cpu）

- [i5-4590+VMware10+ubuntu16.04 谷歌tensorflow学习梵高作画](https://ask.julyedu.com/question/7404)

- [MAC MxNet CPU 版利用DL学梵高作画](https://ask.julyedu.com/question/7402)

- ..









## 后记

 没有别的想说的，就是我们会带着大家做更多实验。最后探究实验背后原理，请参看此课程：[深度学习在线班](https://www.julyedu.com/category/index/8/21)。
 七月在线开发/市场团队骁哲、李伟、July，二零一六年九月二十七日。



