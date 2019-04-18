# 目标检测任务中图片标注工具LabelImg等 - YZXnuaa的博客 - CSDN博客
2018年02月05日 11:16:37[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：587
原文地址：http://blog.csdn.net/jesse_mx/article/details/53606897                                                                                                                                                          

目录[(?)](http://blog.csdn.net/jesse_mx/article/details/53606897#)[[-]](http://blog.csdn.net/jesse_mx/article/details/53606897#)
- [前言](http://blog.csdn.net/jesse_mx/article/details/53606897#t0)
- [Ubuntu源码安装](http://blog.csdn.net/jesse_mx/article/details/53606897#t1)
- [Ubuntu pip安装](http://blog.csdn.net/jesse_mx/article/details/53606897#t2)
- [Windows环境下安装](http://blog.csdn.net/jesse_mx/article/details/53606897#t3)- [安装Anaconda 420](http://blog.csdn.net/jesse_mx/article/details/53606897#t4)
- [安装PyQt包](http://blog.csdn.net/jesse_mx/article/details/53606897#t5)
- [下载源码并运行](http://blog.csdn.net/jesse_mx/article/details/53606897#t6)
- [使用方法](http://blog.csdn.net/jesse_mx/article/details/53606897#t7)
- [后续说明](http://blog.csdn.net/jesse_mx/article/details/53606897#t8)
项目地址：[LabelImg](https://github.com/tzutalin/labelImg)
## 前言
我们知道，图片标注主要是用来创建自己的数据集，方便进行[深度学习](http://lib.csdn.net/base/deeplearning)训练。本篇博客将推荐一款十分好用的图片标注工具LabelImg，重点介绍其安装以及使用的过程。
在此感谢原作者在Github所做的贡献，博主发现软件一直在更新，各位小伙伴可以关注其最新版本。这款工具是全图形界面，用[Python](http://lib.csdn.net/base/python)和Qt写的，最牛的是其标注信息可以直接转化成为XML文件，与PASCAL
 VOC以及ImageNet用的XML是一样的。
PS.作者在5月更新了代码，现在最新的版本号是1.3.0，博主亲测，源代码在Windows 10和Ubuntu 16.04上运行正常。
## Ubuntu源码安装
由于Ubuntu系统自带[python](http://lib.csdn.net/base/python)，这款软件在Ubuntu环境下的安装是最方便的。软件要求python版本在2.6以上，同时需要PyQt和lxml的支持。
```bash
sudo apt-get install pyqt4-dev-tools # 安装PyQt4
sudo pip install lxml # 安装lxml，如果报错，可以试试下面语句
sudo apt-get install python-lxml
git clone https://github.com/tzutalin/labelImg.git
cd labelImg
make all
./labelImg.py # 或者 python labelImg.py
# 作者新加的命令模式（应该只适用于最新github代码）
./labelImg.py [IMAGE_PATH] [PRE-DEFINED CLASS FILE]
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
## Ubuntu pip安装
作者最近（2017.05）增添的一种安装方式，使用Python的pip工具安装，感觉更方便了，博主暂未[测试](http://lib.csdn.net/base/softwaretest)。
```
pip install labelImg
labelImg
labelImg [IMAGE_PATH] [PRE-DEFINED CLASS FILE]
```
- 1
- 2
- 3
- 1
- 2
- 3
## Windows环境下安装
由于Windows不像[Linux](http://lib.csdn.net/base/linux)那样自带Python，所以安装过程要稍微麻烦一些，重点是解决Python的安装问题。
### 安装Anaconda 4.2.0
这里选择安装Anaconda，它是一款十分好用的python集成安装环境，主要是方便扩展包管理。具体介绍可参考这篇[博客](http://blog.csdn.net/dq_dm/article/details/47065323) 。首先从[官网](https://www.continuum.io/downloads)下载最新的版本，这里我下载的是64位的python2.7版本。然后双击安装，一般点选“for
 all person”，然后安装到系统默认位置即可。安装成功后建议在环境变量path里面增加一条“C:\Program Files\Anaconda2” 
![这里写图片描述](https://img-blog.csdn.net/20161213101941211?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmVzc2VfTXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20161213102000821?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmVzc2VfTXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 安装PyQt包
打开命令行窗口，输入`conda list` ，会列出所有预装的python扩展包，可以看到里面已经有了lxml，但是缺少PyQt。命令行输入
```
conda
 install pyqt=4
```
 ,等待一会即可完成PyQt4的安装。 
![这里写图片描述](https://img-blog.csdn.net/20161213102032962?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmVzc2VfTXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 下载源码并运行
在原作者的[github](https://github.com/tzutalin/labelImg)下载源码压缩包，解压可得到名为labelImg-master的文件夹，进入该文件夹，在空白处使用“Shift+鼠标右键”，进入当前目录的命令行窗口，依次输入下面语句即可打开软件。
```python
pyrcc4 -o resources.py resources.qrc
python labelImg.py
python labelImg.py [IMAGE_PATH] [PRE-DEFINED CLASS FILE]
```
- 1
- 2
- 3
- 1
- 2
- 3
![这里写图片描述](https://img-blog.csdn.net/20161213102115572?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmVzc2VfTXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 使用方法
- 
修改默认的XML文件保存位置，使用快捷键“Ctrl+R”，改为自定义位置，这里的路径一定不能包含中文，否则无法保存。
- 
源码文件夹中使用notepad++打开data/predefined_classes.txt，修改默认类别，比如改成person、car、motorcycle三个类别。
- 
“Open Dir”打开图片文件夹，选择第一张图片开始进行标注，使用“Create RectBox”或者“Ctrl+N”开始画框，单击结束画框，再双击选择类别。完成一张图片后点击“Save”保存，此时XML文件已经保存到本地了。点击“Next Image”转到下一张图片。
- 
标注过程中可随时返回进行修改，后保存的文件会覆盖之前的。
- 
完成标注后打开XML文件，发现确实和PASCAL VOC所用格式一样。
![这里写图片描述](https://img-blog.csdn.net/20161213102203042?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmVzc2VfTXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20161213102222417?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmVzc2VfTXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 后续说明
- 
原作者也发布了打包好的软件，包括Windows和[linux](http://lib.csdn.net/base/linux)平台，无需编译，可以直接运行，不愿意折腾的朋友可以使用这个版本，直接下载最新的版本即可（目前是1.3.2）。[LabelImg打包版本](http://tzutalin.github.io/labelImg/) （PS.Linux版本如果提示权限问题，可加sudo）
- 
最近有人运行Github的源码版本时，总是遇到各种错误（参见评论区），个人感觉是作者的更新出了问题，这里提供一份之前版本的源码，亲测可用，[LabelImg可运行版（源码）](http://download.csdn.net/detail/jesse_mx/9826531)。使用方法：解压后，执行`./labelimg.py` 命令。
- 
官方下载地址可能被墙，提供1.3.0打包版本（之后的版本暂未测试）：[百度云](https://pan.baidu.com/s/1kU8ReuF)
其他同类标注工具（Github）： 
[Yolo_mark](https://github.com/AlexeyAB/Yolo_mark)
[BBox-Label-Tool](https://github.com/puzzledqs/BBox-Label-Tool)
[ImageLabel](https://github.com/lanbing510/ImageLabel)
