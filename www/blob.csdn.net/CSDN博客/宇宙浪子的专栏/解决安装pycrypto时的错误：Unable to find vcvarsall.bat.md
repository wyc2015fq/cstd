# 解决安装pycrypto时的错误：Unable to find vcvarsall.bat - 宇宙浪子的专栏 - CSDN博客
2016年04月29日 19:10:59[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：3260
python在安装pycrypto时可能会报：Unable to find vcvarsall.bat错误，需要安装VS才能解决，太耗时间，这里提供编译好的二进制文件，可以直接安装。
如果安装的whl文件，安装后需要将site-packages目录下的crypto改成Crypto。
python3.5版本的pycrypto whl文件：[https://github.com/bluehawksky/PyCrypto-Wheels](https://github.com/bluehawksky/PyCrypto-Wheels)
其他python版本的下载链接：[http://www.voidspace.org.uk/python/modules.shtml#pycrypto](http://www.voidspace.org.uk/python/modules.shtml#pycrypto)
