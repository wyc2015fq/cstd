# botan库编译 - writeeee的专栏 - CSDN博客
2019年02月04日 14:28:39[writeeee](https://me.csdn.net/writeeee)阅读数：92
- 
下载源码解压
[https://github.com/randombit/botan](https://github.com/randombit/botan)
- 
安装python2.7
需要依赖python2.7，没有的话自行安装，安装完毕后，添加环境变量。
- 
执行编译
运行vs的命令行工具。
在vs的命令行工具里，切换路径到解压的botan根目录。
/执行python运行文件configure.py
```
configure.py --cc=msvc --cpu=x86
```
```
//执行vs的nmake命令
nmake
```
等待几分钟（看配置情况）就行了.
