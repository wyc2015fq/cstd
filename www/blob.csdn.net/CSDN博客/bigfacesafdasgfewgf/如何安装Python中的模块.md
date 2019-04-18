# 如何安装Python中的模块 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 16:33:18[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：671








在学习python的时候，一定要注意对几个重要module的学习。例如：主要做数值计算和数据分析的Numpy，做机器学习的Scikit-Learn（sklean）。
在import这些模块之后，我们才可以使用里面封装了一些超级好用的方法。

那么在调用这些模块之前，就有一个问题：如何安装呢？就像在MATLAB中，也需要经常安装一些常用的工具包一样。




主要来说，有两种安装方法：




1、通过cmd命令




Step 1：下载module，放在一个盘下面，例如D盘。一般下载到的module里面都有一个setup.py文件；

Step 2：打开cmd，通过cd等命令，打开module所在的文件夹。如下图：



![](https://img-blog.csdn.net/20140927163359116?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


可以发现，使用cd命令，可以进入文件夹。

Step 3：输入setup.py install命令，就可以安装模块。







2、使用pip




pip是一种帮助安装python的模块的包管理器。

首先下载pip，我的百度云盘上面已经有了，然后安装好了之后，直接在cmd窗口下输入：

pip install xlrd （module的名字）即可。

这样方法比第一种要方便许多。








