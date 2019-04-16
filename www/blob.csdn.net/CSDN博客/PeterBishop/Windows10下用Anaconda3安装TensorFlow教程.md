# Windows10下用Anaconda3安装TensorFlow教程 - PeterBishop - CSDN博客





2019年01月30日 09:28:17[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：207








### 这是我在自己的笔记本电脑上用Anaconda3安装TensorFlow的教程

**1. 安装好Anaconda3版本**
- (1) 注：可以发现最新版本是[Anaconda5](https://www.anaconda.com/download/)了（没关系，下载就是了）
![](https://images2017.cnblogs.com/blog/1245030/201801/1245030-20180130161642734-1206320460.png)
- (2) 注意安装anaconda时一定要把环境变量加入windows环境中。要没有勾选，安装完后还有手动加入。而且注意3.4版本是默认不加入anaconda的文件路径到环境变量的。
![](https://images2017.cnblogs.com/blog/1245030/201801/1245030-20180130162948828-1518848944.jpg)
- (3) 安装好了后，运行`开始菜单—>Anaconda3—>Anaconda Prompt`
`conda list`
![](https://images2017.cnblogs.com/blog/1245030/201801/1245030-20180130163509437-1335518826.png)

可以看到已经安装了numpy、sympy等常用的包。

**2. 安装Tensorflow**

有两种方法可以安装：

> 
**[方法一]**`cpu版本`：`pip install --ignore-installed --upgrade tensorflow`


> 
**[方法二]**`gpu版本`：`pip install --ignore-installed --upgrade tensorflow-gpu`注意gpu版要事先选好并装好CUDA和cuDNN

本文限于篇幅，只总结了方法一的安装流程


**方法一：CPU版本**
- (1) 写这篇文章时，TensorFlow在Windows下已经支持支持Python 3.6版本。
- (2) 打开Anaconda Prompt，输入清华仓库镜像，这样更新会快一些：

```
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
conda config --set show_channel_urls yes
```
- (3) 在`Anaconda Prompt`中，用Anaconda3创建一个python3.6的环境，环境名称为tensorflow ，输入下面命令：
`conda create -n tensorflow python=3.6`
![](https://images2017.cnblogs.com/blog/1245030/201801/1245030-20180130164321437-797923353.png)
![](https://images2017.cnblogs.com/blog/1245030/201801/1245030-20180130171219765-1885712140.png)

运行 `开始菜单—>Anaconda3—>Anaconda Navigator`，点击左侧的`Environments`，可以看到`tensorflow`的环境已经创建好了。

![](https://images2017.cnblogs.com/blog/1245030/201801/1245030-20180130174027359-387730422.jpg)
- (4) 在`Anaconda Prompt`中启动`tensorflow`环境：
`activate tensorflow`
![](https://images2017.cnblogs.com/blog/1245030/201801/1245030-20180130183435906-1438844577.jpg)
- (5) 安装cpu版本的TensorFlow
`pip install --upgrade --ignore-installed tensorflow`
![](https://images2017.cnblogs.com/blog/1245030/201801/1245030-20180130184559000-973412227.jpg)

这样tensorflow cpu版本就安装好了。

### 当不使用TensorFlow时，可以通过`deactivate`来关闭TensorFlow环境：![](https://images2017.cnblogs.com/blog/1245030/201801/1245030-20180130190226187-992038456.jpg)!!!!!!!!但是版本一般都是比较低的，所以安装高版本看这里[https://blog.csdn.net/qq_35203425/article/details/79965389](https://blog.csdn.net/qq_35203425/article/details/79965389)
- (6) 测试cpu版本的TensorFlow

	重新打开`Anaconda Prompt`—>`activate tensorflow`—>`python`来启动tensorflow，并进入python环境
![](https://images2017.cnblogs.com/blog/1245030/201801/1245030-20180130192757921-1908544695.jpg)

	测试代码如下:

```
#TensorFlow使用图(Graph)来表示计算任务；并使用会话(Session)来执行图，通过Session.close()来关闭会话（这是一种显式关闭会话的方式）。会话方式有显式和隐式会话之分。
import tensorflow as tf
hello = tf.constant('Hello, TensorFlow!')  #初始化一个TensorFlow的常量
sess = tf.Session()  #启动一个会话
print(sess.run(hello))
```

> 
![](https://images2017.cnblogs.com/blog/1245030/201801/1245030-20180130194137125-409869030.jpg)

- 
(7) 其他问题

> 当我们用Anaconda自带的iPython和Spyder以及jupyter notebook中输入`import tensorflow as tf`的时候会失败，显示如下`No module named 'tensorflow‘`，原因是我们没有在TensorFlow的环境下打开它们。


	为此，我们需要在TensorFlow环境下安装这插件。
	
**jupyter notebook: 直接conda install jupyter notebook(别去图形界面安装极其坑！！！而且一定是jupyter notebook不是jupyter！！！坑)**


**(7).1 好的，安装插件后，我们用例子分别来运行试试（都要事先`activate tensorflow`来启动tensorflow）：**

**jupyter notebook:**

> 
![](https://images2017.cnblogs.com/blog/1245030/201801/1245030-20180130215427671-94527920.jpg)


立刻唤醒`jupyter notebook`，并运行：

> 
![](https://images2017.cnblogs.com/blog/1245030/201801/1245030-20180130211628750-2107364414.jpg)





