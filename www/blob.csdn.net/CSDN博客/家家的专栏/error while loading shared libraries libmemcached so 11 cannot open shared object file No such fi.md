# error while loading shared libraries: libmemcached.so.11: cannot open shared object file: No such fi - 家家的专栏 - CSDN博客





2015年03月14日 19:21:25[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2953








**error while loading shared libraries: libmemcached.so.11: cannot open shared object file: No such file or directory**


出现上述错误一般是没有设置memcached环境变量

运行过程中，需要将libmemcached.so文件所在目录加入到变量LD_LIBRARY_PATH中，即**export LD_LIBRARY_PATH=/usr/local/libmemcached/lib:$LD_LIBRARY_PATH**，否则运行时会提示类似错误




一定要保证包含目录路径是正确的！

在.bashrc文件中添加目录！


如果不知道在那个目录，直接locate 文件名，即可找到。

转自：http://www.jsjtt.com/xitongyingyong/linux/57.html



