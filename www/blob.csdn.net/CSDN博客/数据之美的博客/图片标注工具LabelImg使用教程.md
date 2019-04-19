# 图片标注工具LabelImg使用教程 - 数据之美的博客 - CSDN博客
2017年04月23日 17:24:34[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：2488
个人分类：[深度学习/机器学习																[数据处理																[python](https://blog.csdn.net/oppo62258801/article/category/6738404)](https://blog.csdn.net/oppo62258801/article/category/6857888)](https://blog.csdn.net/oppo62258801/article/category/6739087)
项目地址：[LabelImg](https://github.com/tzutalin/labelImg)
## 前言
我们知道，图片标注主要是用来创建自己的数据集，方便进行[深度学习](http://lib.csdn.net/base/deeplearning)训练。本篇博客将推荐一款十分好用的图片标注工具LabelImg，重点介绍其安装以及使用的过程。
在此感谢原作者在[github](https://github.com/tzutalin/labelImg)所做的贡献，博主发现软件一直在更新，各位小伙伴可以关注其最新版本。这款工具是全图形界面，用[Python](http://lib.csdn.net/base/python)和Qt写的，最牛的是其标注信息可以直接转化成为XML文件，与PASCAL
 VOC以及ImageNet用的XML是一样的。
## Ubuntu源码安装
由于Ubuntu系统自带python，这款软件在Ubuntu环境下的安装是最方便的。软件要求python版本在2.6以上，同时需要PyQt和lxml的支持。
```bash
sudo apt-get install pyqt4-dev-tools #安装PyQt4
sudo pip install lxml #安装lxml，如果报错，可以试试下面语句
sudo apt-get install python-lxml
git clone https://github.com/tzutalin/labelImg.git
cd labelImg
make all
./labelImg.py
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
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
```
- 1
- 2
- 1
- 2
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
