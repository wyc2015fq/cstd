
# TensorFlow问题：pycharm远程调试报错ImportError:libcusolver.so.9.0: cannot open shared object file - manduner的博客 - CSDN博客


2018年09月03日 16:12:05[Manduner_TJU](https://me.csdn.net/manduner)阅读数：1687


# 1，前言
本篇文章的方法转载自[https://blog.csdn.net/qq_15192373/article/details/81231095](https://blog.csdn.net/qq_15192373/article/details/81231095)，该博文提供的两种方法非常实用，亲测有效。写本篇博文，一是，为了大家遇到同样问题的时候能够更快速的找到实用的解决办法；二是，对原博文书写不当的地方进行了修正，并给出了自己测试使用后的总结
# 2，问题描述
在本地windows环境下通过pycharm远程调试GPU服务器上的python程序，程序出现如下错误：
![](https://img-blog.csdn.net/20180903154521608?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 2，解决办法
## （1）方法一：
1）在Pycharm中添加环境变量 ，右上角倒三角下拉，进入菜单Edit configurations，
2）打开重新添加环境变量 ：Environment variables 那栏
3）增加：PYTHONUNBUFFERED=1;
LD_LIBRARY_PATH=/usr/local/cuda-9.0/lib64;/usr/local/lib
![](https://img-blog.csdn.net/20180903155236197?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
添加成功以后就可以正常运行了。**这种方法的坏处是，每次新建文件，都需要手动添加环境变量。**
## （2）方法二：
1）1步骤同方法1
2）选择左边的default--python，修改Environment variables 那栏
3）3步骤同方法1
![](https://img-blog.csdn.net/20180903160319254?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**这种方法每次重启电脑后，pycharm又需要重新配置，也不是很好**
# 3，总结
（1）建议使用第二种方法，这种方法在新建一个python文件的时候，环境变量默认的就是之前设置好的，不用每次新建一个python文件都需要添加一次环境变量。
（2）看到这篇博文的方法的时候，同志们肯定是有待执行的python文件报错了，对于这些python文件，你就需要按照第一种方法，手动添加一下，要不还是会报错。第二种方法只有在你新建一个python文件的时候，才会默认带上你设置好的环境变量

