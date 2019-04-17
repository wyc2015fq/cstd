# Mac下单机安装pyspark，更改python编译器为anaconda的编译器，并且配置pycharm - 快来学习鸭～～～ - CSDN博客





2018年05月10日 22:55:43[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：1292








**本人习惯用pycharm开发，并且喜欢notebook以及anaconda自带的各种库，开发起来方便很多，所以在安装好spark后，修改了默认的python编译器到anaconda下，可以在notebook和pycharm中使用spark，全部安装完成花了点时间，所以记录一下。**

1、下载并安装pycharm，这个自己下载吧：[https://www.jetbrains.com/pycharm/](https://www.jetbrains.com/pycharm/)，安装好后需要序列码激活，序列码自己百度下，很多； 

   下载anaconda，我安装的是python2的 

    下载spark：[http://spark.apache.org/downloads.html](http://spark.apache.org/downloads.html)

    选择版本，我选择的spark-2.3.0-bin-hadoop2.7.tgz 

2、将下载的spark选择一个位置进行解压`tar zxvf spark-2.3.0-bin-hadoop2.7.tgz`
3、解压好后，配置spark环境变量 

    打开/etc/profile ，在profile文件中追加以下配置

```bash
#SPARK VARIABLES START  
export SPARK_HOME = /XXX/XXXX/spark-2.3.0-bin-hadoop2.7 
export PATH = ${PATH}:${SPARK_HOME}/bin  
#SPARK VARIABLES END
```

其中/XXX/XXX/是刚才解压spark的地址，后面是具体的文件名，自己改成自己的版本； 

然后是配置java环境变量，如果没有jdk的话需要先安装jdk，可以在终端输入java试试，如果提示没有这个命令就是没装，直接在app store里下一个，安装下。 

安装完成后添加java环境变量，还是在刚才的这个文件/etc/profile下，追加
```bash
export JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_171.jdk/Contents/Home
```

具体版本和地址自己安装自己的实际情况改一下。 

完成以后保存退出。 

这时可以直接关闭终端，这样我们配置的环境变量就自动生效了，或者不关终端，在终端输入：source /etc/profile 使环境变量立刻生效。 

- **注意一点:** 因为/etc/profile是系统的配置文件，所以可能没有权限修改，会出现readonly option is set (add ! to override)的提示，所以需要root权限； 

可以在打开文件的时候加sudo，如
```bash
sudo vi /etc/profile
```

然后保存的时候，加！强制保存。或者在终端通过sudo开启root权限再进行操作，如下

```
shixiangfu:~ shixiangfu$ su - root  
    Password:  
    su: Sorry  
    shixiangfu:~ shixiangfu$ su - root  
    Password:  
    su: Sorry  
    shixiangfu:~ shixiangfu$ sudo passwd root  
    Changing password for root.  
    New password:  
    Retype new password:  
    shixiangfu:~ shixiangfu$ su - root  
    Password:  
    shixiangfu:~ root# exit  
    logout  
    shixiangfu:~ shixiangfu$
```

弄好后，强制保存下，wq！。并且 更新下配置 source /etc/profile 

4、这里弄好好，测试下pyspark 

打开终端，输入：pyspark 

不出意外会出现：
```
MacBook-Pro-139:spark-2.3.0-bin-hadoop2.7 shixiangfu$ pyspark
Python 2.7.14 |Anaconda, Inc.| (default, Dec  7 2017, 11:07:58) 
[GCC 4.2.1 Compatible Clang 4.0.1 (tags/RELEASE_401/final)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
2018-05-10 15:28:52 WARN  NativeCodeLoader:62 - Unable to load native-hadoop library for your platform... using builtin-java classes where applicable
Setting default log level to "WARN".
To adjust logging level use sc.setLogLevel(newLevel). For SparkR, use setLogLevel(newLevel).
Welcome to
      ____              __
     / __/__  ___ _____/ /__
    _\ \/ _ \/ _ `/ __/  '_/
   /__ / .__/\_,_/_/ /_/\_\   version 2.3.0
      /_/

Using Python version 2.7.14 (default, Dec  7 2017 11:07:58)
SparkSession available as 'spark'.
>>>
```

我们spark是安装好了 

但是现在的spark使用的是自带的python，并且我们无法在pycham和anaconda notebook中使用，这绝对不能忍。 

5、先配置pycharm吧 

前面应该已经安装好pycharm和anaconda了，打开pycharm，先配置默认python编译器 

打开File->default setting->project interpreter,选择anaonda2/bin/python的python，如图 
![这里写图片描述](https://img-blog.csdn.net/20180510223810418?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果没有，可以在后面的设置里手动找到地址，添加；这里不详细讲了，可以自己百度下。 

然后建个py文件，把下面这段测试代码放进去
```
from pyspark import SparkContext
logFile = "/Users/shixiangfu/spark/spark-2.3.0-bin-hadoop2.7/README.md"
sc = SparkContext("local","Simple App")
logData = sc.textFile(logFile).cache()

numAs = logData.filter(lambda s: 'a' in s).count()
numBs = logData.filter(lambda s: 'b' in s).count()

print("Lines with a: %i, lines with b: %i"%(numAs, numBs))
```

然后打开pycharm菜单栏的 run->environment variables—然后点击‘…‘会出来一个框，点击+号添加环境变量，我看其他博客是配置了两个： 

一个SPARK_HOME，值是我们上面配置文件中填过的路径：/XXX/XXXX/spark-2.3.0-bin-hadoop2.7 

还有一个是PYTHONPATH，值是$SPARK_HOME/python 

但是我只写了第一个，没有写PYTHONPATH，因为我修改了自己的python为anaconda下的python，具体如下： 

我们继续打开/etc/profile，在里面修改python环境变量，追加
```bash
export ANACONDA_ROOT=~/anaconda2
export PYSPARK_DRIVER_PYTHON=$ANACONDA_ROOT/bin/ipython notebook
export PYSPARK_PYTHON=$ANACONDA_ROOT/bin/python
export PYSPARK_DRIVER_PYTHON_OPTS="notebook"
```

anaconda的路径自己改下 

然后保存，生效 source /etc/profile 

这里改好后，打开pycharm的preferences->project structure ,右边有个add context root，把spark目录下有个python文件夹下的lib下的py4j-some-version.zip和pyspark.zip这两个添加进去。 

最后，运行以下刚才建立的测试代码.py 

不出意外是可以运行的，如果还有问题，可以看下下面的这几个博客 

1、[https://blog.csdn.net/u013041398/article/details/55253782](https://blog.csdn.net/u013041398/article/details/55253782)

2、[https://blog.csdn.net/rifengxxc/article/details/74503119](https://blog.csdn.net/rifengxxc/article/details/74503119)

3、[https://blog.csdn.net/wuweilong/article/details/46361009](https://blog.csdn.net/wuweilong/article/details/46361009)

4、[https://blog.csdn.net/duxu24/article/details/53587451](https://blog.csdn.net/duxu24/article/details/53587451)









