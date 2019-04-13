
# Ubantu下使用vi时，方向键变字母输出、退格键无法删除字符的解决办法 - Magician的博客 - CSDN博客


2018年04月12日 09:04:05[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：739


[目录](#目录)[前言：](#前言)
[一、编辑/etc/vim/vimrc.tiny](#一编辑etcvimvimrctiny)
[二、安装vim full版本](#二安装vim-full版本)
[三、添加”.vimrc”文件](#三添加vimrc文件)


# 目录
## 前言：
最近由于要玩TensorFlow，所以把尘封已久的Ubantu给打开了，不过配置网络的时候，算是一团糟，出现了在插入模式下，输入方向键不能移动光标而是变成了乱码；退格键不能删除字符，只能移动光标的问题。真被的搞的“焦头烂额”。后来找到一篇不错的文章总算把问题解决了，这里记录下，权当是笔记吧。
问题的解决有以下几种方法：
## 一、编辑/etc/vim/vimrc.tiny
由于/etc/vim/vimrc.tiny的拥有者是root用户，所以要在root的权限下对这个文件进行修改。很简单，这个文件里面的倒数第二句话是“set compatible”，如下所示：
![这里写图片描述](https://img-blog.csdn.net/20180412085907276?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20180412085907276?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
将“compatible”改成“nocompatible”非兼容模式就可以解决方向键变ABCD的问题了。接下来要解决Backspace键的问题也很简单，在刚才那句话后面再加一句：
[
](https://img-blog.csdn.net/20180412085907276?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
set
```
```python
backspace=
```
```python
2
```
[
](https://img-blog.csdn.net/20180412085907276?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![这里写图片描述](https://img-blog.csdn.net/20180412090053098?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[变成下图所示：](https://img-blog.csdn.net/20180412090053098?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
就可以解决问题了。这个时候，无论对于哪个用户，使用vi都能很方便的按照我们的习惯去编辑文件了。
## 二、安装vim full版本
由于ubuntu预安装的是tiny版本，所以会导致我们在使用上的产生上述的不便。但是，我们安装了vim的full版本之后，键盘的所有键在vi下就很正常了。
首先，要先卸掉旧版的vi，输入以下命令：
```python
sudo apt
```
```python
-get
```
```python
remove vim
```
```python
-common
```
然后安装full版的vim，输入命令：
```python
sudo
```
```python
apt-get install vim
```
这样安装好了之后，我们在编辑文件的时候依然是使用“vi”命令来启动新装的vim，但是操作起来比tiny更加方便了。
## 三、添加”.vimrc”文件
这并不是一个聪明的方法。它是在用户的个人目录下，编辑.vimrc文件(注意文件名是以“.”开头的)，添加下面两句语句：
```python
set
```
```python
nocompatible
```
```python
//以非兼容模式工作
```
```python
set
```
```python
backspace=
```
```python
2
```
如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20180412085822194?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20180412085822194?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
保存退出之后，以后该用户使用vi就能够避免键盘方向键和Backspace键无效的问题了。
[
](https://img-blog.csdn.net/20180412085822194?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)之所以说它不是个聪明的办法是因为我们必须为每个用户都在其个人目录下添加“.vimrc”文件，才能使所有用户都正常使用vi的功能，这是个比较大的工作量。所以也不推荐，尽管我自己初始也是用这种办法解决的(因为我系统上的用户只有2个，比较少)。
[
](https://img-blog.csdn.net/20180412085822194?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)以上三种方法，总有一个适合你，选最好的，不如选最适合你的！！博主采用第一种方法就把问题解决了。喜欢的点个赞，不喜勿喷！！
[            ](https://img-blog.csdn.net/20180412085822194?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

