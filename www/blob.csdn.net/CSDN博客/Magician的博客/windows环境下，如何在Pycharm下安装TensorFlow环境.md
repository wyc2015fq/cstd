
# windows环境下，如何在Pycharm下安装TensorFlow环境 - Magician的博客 - CSDN博客


2018年04月14日 16:42:29[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：3898


原文转自：[https://blog.csdn.net/u012052268/article/details/74202439](https://blog.csdn.net/u012052268/article/details/74202439)
[目录](#目录)[前言：](#前言)
[1.安装Anaconda](#1安装anaconda)
[2.安装Tensorflow](#2安装tensorflow)
[3.其他问题](#3其他问题)
[4.在pycharm中使用tensorflow](#4在pycharm中使用tensorflow)


# 目录
## 前言：
最近由于工作需要要使用TensorFlow，所以只能狂补相关的知识。本来博主打算在Ubantu上玩，但是由于一些原因还是放弃了这个想法，就转移到Pycharm上来玩。以下是自己在收集资料的过程中看到一篇很好的安装教程，分享一下。
## 1.安装Anaconda
选择相应的Anaconda进行安装，下载地址点击这里，下载对应系统版本的Anaconda，官网现在的版本是Anaconda 4.3.1 for python3.6。笔者安装的是4.3.0版本的。
![这里写图片描述](https://img-blog.csdn.net/20180414162123969?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180414162123969?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
就和安装普通的软件一样，全部选择默认即可，**注意勾选将python3.6添加进环境变量。**
![这里写图片描述](https://img-blog.csdn.net/20180414162139956?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180414162139956?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这样Anaconda就安装好了，我们可以通过下面的命令来查看Anaconda已经安装了哪些包。
运行 开始菜单->Anaconda3—>Anaconda Prompt ：
```python
conda
```
```python
list
```
![这里写图片描述](https://img-blog.csdn.net/20180414162109551?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180414162109551?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可以看到已经安装了numpy、sympy等常用的包。
[

](https://img-blog.csdn.net/20180414162109551?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 2.安装Tensorflow
[
](https://img-blog.csdn.net/20180414162109551?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)TensorFlow目前在Windows下只支持python 3.5版本。
[
](https://img-blog.csdn.net/20180414162109551?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)（1）打开Anaconda Prompt，输入清华仓库镜像，这样更新会快一些：
![这里写图片描述](https://img-blog.csdn.net/2018041416223279?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
conda config --
```
```python
add
```
```python
channels https://mirrors
```
```python
.tuna
```
```python
.tsinghua
```
```python
.edu
```
```python
.cn
```
```python
/anaconda/pkgs/free/
conda config --
```
```python
set
```
```python
show_channel_urls yes
```
（2）同样在Anaconda Prompt中利用Anaconda创建一个python3.5的环境，环境名称为tensorflow ，输入下面命令：
```python
conda create -n tensorflow python
```
```python
=
```
```python
3.5
```
运行 开始菜单->Anaconda3—>Anaconda Navigator，点击左侧的Environments，可以看到tensorflow的环境已经创建好了。
![这里写图片描述](https://img-blog.csdn.net/20180414162356697?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180414162406286?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
（3）在Anaconda Prompt中启动tensorflow环境：
```python
activate
```
```python
tensorflow
```
![这里写图片描述](https://img-blog.csdn.net/20180414162506910?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180414162506910?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**注：当不使用tensorflow时，关闭tensorflow环境，命令为：deactivate**
[
](https://img-blog.csdn.net/20180414162506910?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)（4）安装cpu版本的TensorFlow
[

](https://img-blog.csdn.net/20180414162506910?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
pip
```
```python
install
```
```python
-
```
```python
-
```
```python
upgrade
```
```python
-
```
```python
-
```
```python
ignore
```
```python
-
```
```python
installed
```
```python
tensorflow
```
[
](https://img-blog.csdn.net/20180414162506910?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**注：这里没有介绍GPU版本的安装方法，GPU版本需要安装cuda8+cudnn5，如需要的请搜索其他博文。**
**注意：一定要在 刚刚创建的tensorflow的环境下安装！！！！**
这样tensorflow cpu版本就安装好了。
（5）测试tensorflow
在Anaconda Prompt中启动tensorflow环境，并进入python环境。
![这里写图片描述](https://img-blog.csdn.net/20180414162716619?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
测试代码如下:
```python
import
```
```python
tensorflow
```
```python
as
```
```python
tf
hello = tf.constant(
```
```python
'Hello, TensorFlow!'
```
```python
)
sess = tf.Session()
print(sess.run(hello))
```
运行结果：
![这里写图片描述](https://img-blog.csdn.net/20180414162741137?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 3.其他问题
或许到这里我们并没有满足，我们在Anaconda自带的ipython 和Spyder中import tensorflow的时候一直失败，提示**No module named ‘tensorflow’**，如下图，那是因为我们没有在tensorflow的环境下打开它们。
![这里写图片描述](https://img-blog.csdn.net/20180414163046908?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
为了能在ipython 和Spyder中使用tensorflow，我们需要在tensorflow的环境中安装这两个的插件。
打开Anaconda Navigator，选择Not installed，找到 ipython和Spyder并安装，笔者这里已经安装好，所以在这个页面没有显示。
![这里写图片描述](https://img-blog.csdn.net/20180414163114539?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180414163114539?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
切换到installed，可以看到两个都已经安装好，其实可以按照自己的需要安装。下图显示已经安装好的Spyder:
![这里写图片描述](https://img-blog.csdn.net/20180414163124452?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180414163124452?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
安装好插件后，我们需要测试一下。
[
](https://img-blog.csdn.net/20180414163124452?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)在Anaconda Prompt中启动tensorflow环境，并运行ipython，import tensorflow发现成功:
![这里写图片描述](https://img-blog.csdn.net/20180414163215923?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180414163215923?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
同样，在Anaconda Prompt中启动tensorflow环境，并运行Spyder，等一会儿后会启动Spyder IDE，import tensorflow 同样成功:
![这里写图片描述](https://img-blog.csdn.net/20180414163231876?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180414163231876?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180414163242859?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180414163242859?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**注意：一定要启动tensorflow 环境下的Spyder才可以import tensorflow，不要去开始菜单运行Spyder，在那里是无法运行的，如：**
![这里写图片描述](https://img-blog.csdn.net/20180414163414560?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 4.在pycharm中使用tensorflow
习惯了使用PyCharm来开发，配置如下：
新建工程后在
File-Setting–Project Interpreter选择tensorflow下的Python解释器，
例如我的解释器位置：
![这里写图片描述](https://img-blog.csdn.net/20180414163553908?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180414163553908?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180414163602588?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180414163602588?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
等部署完后便可跑个HelloWorld了
[

](https://img-blog.csdn.net/20180414163602588?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
import
```
```python
tensorflow
```
```python
as
```
```python
tf
hello = tf.constant(
```
```python
"Hello!TensorFlow"
```
```python
)
sess = tf.Session()
print(sess.run(hello))
```
[
](https://img-blog.csdn.net/20180414163602588?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)运行一下
[
](https://img-blog.csdn.net/20180414163602588?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)起飞！
[
](https://img-blog.csdn.net/20180414163602588?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)这种方式的好处：不用每次都 开启、关闭环境了。
![这里写图片描述](https://img-blog.csdn.net/2018041416353819?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/2018041416353819?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
（activate
tensorflow 、deactivate tensorflow）
[
						](https://img-blog.csdn.net/2018041416353819?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[
	](https://img-blog.csdn.net/2018041416353819?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
