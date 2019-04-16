# 7行Python代码的人脸识别 - 我相信...... - CSDN博客





2017年03月20日 08:41:57[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：60987标签：[python																[脸部识别																[opencv](https://so.csdn.net/so/search/s.do?q=opencv&t=blog)
个人分类：[python																[大数据](https://blog.csdn.net/wireless_com/article/category/6231064)](https://blog.csdn.net/wireless_com/article/category/5586333)





![AI](https://img-blog.csdn.net/20170320083644125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

随着去年alphago 的震撼表现，AI 再次成为科技公司的宠儿。AI涉及的领域众多，图像识别中的人脸识别是其中一个有趣的分支。百度的BFR，Face++的开放平台，汉王，讯飞等等都提供了人脸识别的API，对于老码农而言，自己写一小段代码，来看看一张图片中有几个人，没有高大上，只是觉得好玩，而且只需要7行代码。

```
import cv2

face_patterns = cv2.CascadeClassifier('/usr/local/opt/opencv3/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml')

sample_image = cv2.imread('/Users/abel/201612.jpg')

faces = face_patterns.detectMultiScale(sample_image,scaleFactor=1.1,minNeighbors=5,minSize=(100, 100))

for (x, y, w, h) in faces:
    cv2.rectangle(sample_image, (x, y), (x+w, y+h), (0, 255, 0), 2)

cv2.imwrite('/Users/abel/201612_detected.png', sample_image);
```

### 第1行 引入 OpenCV

开源是伟大的，使我们视野更开阔，而且不用重复造轮子。这里没有用PIL，再结合特定算法，而是直接使用了OpenCV([http://opencv.org](http://opencv.org))。OpenCV是一个基于BSD许可发行的跨平台计算机视觉库，可以运行在Linux、Windows和Mac OS操作系统上，轻量而且高效，用C／C++编写，同时提供了Python、Ruby、MATLAB等接口，实现了图像处理和计算机视觉方面的很多通用算法。

### 第2行  加载分类器 cv2.CascadeClassifier

CascadeClassifier是Opencv中做人脸检测时候的一个级联分类器，该类中封装的是目标检测机制即滑动窗口机制+级联分类器的方式。数据结构包括Data和FeatureEvaluator两个主要部分。Data中存储的是从训练获得的xml文件中载入的分类器数据；而FeatureEvaluator中是关于特征的载入、存储和计算。这里采用的训练文件是OpenCV中默认提供的haarcascade_frontalface_default.xml。至于Haar，LBP的具体原理，可以参考opencv的相关文档，简单地，可以理解为人脸的特征数据。

### 第3行  加载目标图片 imread

人脸识别系统一般分为：人脸图像采集、人脸图像预处理、人脸图像特征提取以及匹配与识别。 简化起见，之间读入图片，这是一张去年中生代北京闭门会的集体照。 
![集体照](https://img-blog.csdn.net/20170320083842432?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 第4行  多尺度检测 detectMultiScale

调用 CascadeClassifier 中的调detectMultiScale函数进行多尺度检测，多尺度检测中会调用单尺度的方法detectSingleScale。 

参数说明：
- scaleFactor 是 图像的缩放因子
- minNeighbors 为每一个级联矩形应该保留的邻近个数，可以理解为一个人周边有几个人脸
- minSize 是检测窗口的大小

这些参数都是可以针对图片进行调整的，处理结果返回一个人脸的矩形对象列表。

### 第5行 和 第6行 为每个人脸画一个框

循环读取人脸的矩形对象列表，获得人脸矩形的坐标和宽高， 然后在原图片中画出该矩形框，调用的是OpenCV的rectangle 方法，其中矩形框的颜色等是可调整的。

### 第7行 保存检测后的结果

万事具备了，调用imwrite，将检测后的结果保存到指定的位置。结果图如下： 
![检测结果](https://img-blog.csdn.net/20170320083800111?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
神秘感不是这7行代码，而是OpenCV中的相关实现，OpenCV的中文网也是一个学习体会的好场所。

因此，7行代码只是个噱头，真正的核心是OpenCV。然后，安装OpenCV环境的时候就是有一些坑，特别记录一下。

### 基于Mac的OpenCV环境

建议使用Brew 安装，如果没有安装brew，先执行下面命令：

`$/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)”`

然后，指定目标仓库 `$brew tap homebrew/science`

安装OpenCV3  `$brew install opencv3`

安装速度取决于网络，安装完毕需要绑定Python开发环境，有多种土方法：

1）增加环境变量，将opencv 的site-packages 添加到PYTHONPATH中

2）使用ln 软连接，将cv2.so 链接到python 环境的 site-packages中

3）直接cp cv2.so 到 python 环境的 site-packages 目录下

更简单地是执行如下命令：

```
echo /usr/local/opt/opencv3/lib/python2.7/site-packages >> /usr/local/lib/python2.7/site-packages/opencv3.pth
  mkdir -p /Users/hecom/.local/lib/python2.7/site-packages
  echo 'import site; site.addsitedir("/usr/local/lib/python2.7/site-packages")' >> /Users/hecom/.local/lib/python2.7/site-packages/homebrew.pth
```

### 对于强迫症患者

可以全面安装opencv的各种插件，如3方插件，视频插件等：


```
$brew install opencv3 --with-contrib --with-cuda --with-ffmpeg --with-tbb --with-qt5
```


可能会遇到很多坑，如 
`Error: You must ‘brew link texi2html’ before homebrew/science/opencv3 can be installed`

当你执行 `$brew link texi2html`时又会遇错： 

```
Error: Cowardly refusing to 'sudo brew link' 

You can use brew with sudo, but only if the brew executable is owned by root. 

However, this is both not recommended and completely unsupported so do so atyour own risk


说明brew 的权限有问题， 需要解决： 

`$sudo chown root:wheel ‘which brew’`

### 在Ubuntu上OpenCV3的源码安装

在Ubantu上安装先要做一下准备活动

```
$ aptitude -f install
$ sudo apt-get update
```

然后安装编译环境： 

    $sudo apt-get install build-essential

安装依赖库 
`$sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev`

还要安装python 依赖及其它依赖库 
`$sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev`
获得opencv源代码 
`git clone https://github.com/Itseez/opencv.git`

终于可以编译安装了：

```
$cd opencv
$cmake -D CMAKE_INSTALL_PREFIX=/home/opencv/build -D CMAKE_BUILD_TYPE=RELEASE   ..
```

最后，安装python－opencv：

$sudo apt-get install python-opencv

### OpenCV的Python环境简单验证

一行代码验证OpenCV的Python环境是否成功：

$python -c “import cv2; print dir(cv2)”

如果输出了很多cv2 的属性和函数名的字典，就表示成功；否则的话，还需要继续填坑。

![这里写图片描述](https://img-blog.csdn.net/20170320084042005?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```](https://so.csdn.net/so/search/s.do?q=脸部识别&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)




