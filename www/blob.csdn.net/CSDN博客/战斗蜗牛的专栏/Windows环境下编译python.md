# Windows环境下编译python - 战斗蜗牛的专栏 - CSDN博客





2012年11月03日 19:42:29[vbskj](https://me.csdn.net/vbskj)阅读数：12555








下面是我的编译过程记录，几乎同书中一样，唯一不同的是我的机器是win7 64bit的，然后用的是VS2008。 

1. 首先从python.org上下载python源码压缩包，我下的是Python-2.7.3.tgz，不放心最好用MD5校验下。 

2. 解压后目录结构大致如下 

[](http://images.cnblogs.com/cnblogs_com/heshizhu/201208/201208091524281526.png)![](https://img-my.csdn.net/uploads/201211/03/1351943091_4772.jpg)

从各目录名大概就知道该目录是存放什么的了，比如Mac是存放在Mac系统下编译的文件；我们所需要的源代码应该是在PCbuild目录下。 

3. 进入目录，打开pcbuild.sln文件 

4. 设置属性，见下图，首先启动项目设置选择“单启动项目”为Pyhton；然后设置配置属性，只编译python和pythoncore两项，其他的去除掉，并且依据你具体的平台选择是W32还是X64的。


![Image(7)](http://images.cnblogs.com/cnblogs_com/heshizhu/201208/201208091524377221.png)

[](http://images.cnblogs.com/cnblogs_com/heshizhu/201208/20120809152438469.png)![](https://img-my.csdn.net/uploads/201211/03/1351943132_9845.jpg)

![Image(9)](http://images.cnblogs.com/cnblogs_com/heshizhu/201208/201208091524401384.png)

5. 同样，先编译make_buildinfo和make_versioninfo 

![Image(10)](http://images.cnblogs.com/cnblogs_com/heshizhu/201208/201208091524416301.png)

6. 最后编译此解决方案喽，然后依据你的编译配置(Debug还是Release，Win32还是X64)，在指定的在目录下就会生成python_d.exe和python27_d.dll等文件


![Image(11)](http://images.cnblogs.com/cnblogs_com/heshizhu/201208/201208091524429790.png)

![Image(12)](http://images.cnblogs.com/cnblogs_com/heshizhu/201208/201208091524429168.png)

7. python_d.exe就是编译的python应用程序了，有没有看到，就是我们编译的时间哦！ 



