# Windows下用Anaconda安装TensorFlow，并在pycharm中使用 - 博客堂 - CSDN博客





2017年07月03日 15:39:05[最小森林](https://me.csdn.net/u012052268)阅读数：40326








笔者之前在学习TensorFlow，也在自己的笔记本上完成了安装，在PyCharm中进行学习。但是最近为了使用[Python](http://lib.csdn.net/base/python)的科学计算环境，我把之前的环境卸载了，并用Anaconda重新安装了TensorFlow，由于自己的笔记本已经很旧了，显卡不行，所以这里介绍一下cpu版本的安装方法和自己遇到的一些坑，截图甚多。

这里直接上干货：

#### 1.安装Anaconda

选择相应的Anaconda进行安装，下载地址点击[这里](https://www.continuum.io/downloads/)，下载对应系统版本的Anaconda，官网现在的版本是Anaconda 4.3.1 for python3.6。笔者安装的是4.3.0版本的。 
![这里写图片描述](https://img-blog.csdn.net/20170320154109652?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg1ODYwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

就和安装普通的软件一样，全部选择默认即可，注意勾选将python3.6添加进环境变量。 
![这里写图片描述](https://img-blog.csdn.net/20170320154332280?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg1ODYwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这样Anaconda就安装好了，我们可以通过下面的命令来查看Anaconda已经安装了哪些包。 
运行 `开始菜单->Anaconda3—>Anaconda Prompt` ：

```php
conda list
```
- 1
- 1

可以看到已经安装了numpy、sympy等常用的包。

#### 2.安装Tensorflow

TensorFlow目前在Windows下只支持[python](http://lib.csdn.net/base/python) 3.5版本。

**（1）**打开Anaconda Prompt，输入清华仓库镜像，这样更新会快一些： 
![这里写图片描述](https://img-blog.csdn.net/20170320160803921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg1ODYwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
conda config --set show_channel_urls yes
```
- 1
- 2
- 1
- 2

**（2）**同样在Anaconda Prompt中利用Anaconda创建一个python3.5的环境，环境名称为tensorflow ，输入下面命令：
`conda create -n tensorflow python=3.5`- 1
- 1

运行 `开始菜单->Anaconda3—>Anaconda Navigator`，点击左侧的`Environments`，可以看到`tensorflow`的环境已经创建好了。

![这里写图片描述](https://img-blog.csdn.net/20170320160730586?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg1ODYwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170320160746414?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg1ODYwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**（3）**在Anaconda Prompt中启动tensorflow环境：
`activate tensorflow`- 1
- 1

![这里写图片描述](https://img-blog.csdn.net/20170320161840217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg1ODYwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 
注：当不使用tensorflow时，关闭tensorflow环境，命令为：`deactivate`


**（4）**安装cpu版本的TensorFlow

```python
pip install --upgrade --ignore-installed tensorflow
```

> 
注：这里没有介绍GPU版本的安装方法，GPU版本需要安装cuda8+cudnn5，如需要的请搜索其他博文。

注意：一定要在 刚刚创建的tensorflow的环境下安装！！！！


这样tensorflow cpu版本就安装好了。

**（5）**[测试](http://lib.csdn.net/base/softwaretest)tensorflow 
在Anaconda Prompt中启动tensorflow环境，并进入python环境。 
![这里写图片描述](https://img-blog.csdn.net/20170320162548220?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg1ODYwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
测试代码如下:

```python
import tensorflow as tf
hello = tf.constant('Hello, TensorFlow!')
sess = tf.Session()
print(sess.run(hello))
```
- 1
- 2
- 3
- 4
- 1
- 2
- 3
- 4

运行结果： 
![这里写图片描述](https://img-blog.csdn.net/20170320162727619?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg1ODYwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







#### 3.其他问题

或许到这里我们并没有满足，我们在Anaconda自带的ipython 和Spyder中import tensorflow的时候一直失败，提示 No module named ‘tensorflow’，如下图，那是因为我们没有在tensorflow的环境下打开它们。

![这里写图片描述](https://img-blog.csdn.net/20170320163031546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg1ODYwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

为了能在ipython 和Spyder中使用tensorflow，我们需要在tensorflow的环境中安装这两个的插件。

打开`Anaconda Navigator`，选择`Not installed`，找到 ipython和Spyder并安装，笔者这里已经安装好，所以在这个页面没有显示。 
![这里写图片描述](https://img-blog.csdn.net/20170320163547834?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg1ODYwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

切换到`installed`，可以看到两个都已经安装好，其实可以按照自己的需要安装。下图显示已经安装好的Spyder: 
![这里写图片描述](https://img-blog.csdn.net/20170320163936273?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg1ODYwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

安装好插件后，我们需要测试一下。

在Anaconda Prompt中启动tensorflow环境，并运行`ipython`，import tensorflow发现成功: 
![这里写图片描述](https://img-blog.csdn.net/20170320164301547?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg1ODYwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

同样，在Anaconda Prompt中启动tensorflow环境，并运行`Spyder`，等一会儿后会启动Spyder IDE，import tensorflow 同样成功: 
![这里写图片描述](https://img-blog.csdn.net/20170320164537824?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg1ODYwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170320164619090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg1ODYwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 
注意：一定要启动tensorflow 环境下的Spyder才可以import tensorflow，不要去开始菜单运行Spyder，在那里是无法运行的，如： 
![这里写图片描述](https://img-blog.csdn.net/20170320164806243?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg1ODYwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







**4.在pycharm中使用tensorflow**




习惯了使用PyCharm来开发，配置如下： 
新建工程后在

File-Setting–Project Interpreter选择tensorflow下的Python解释器，

例如我的解释器位置： 



![](https://img-blog.csdn.net/20170718204923486?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjA1MjI2OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170718204928622?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjA1MjI2OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




等部署完后便可跑个HelloWorld了

```
import tensorflow as tf

hello = tf.constant("Hello!TensorFlow")
sess = tf.Session()
print(sess.run(hello))
```

运行一下 
![这里写图片描述](https://img-blog.csdn.net/20170308201822989?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3g3Nzg4MjUw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

起飞！

这种方式的好处：不用每次都 开启、关闭环境了。

（activate tensorflow 、deactivate tensorflow）




