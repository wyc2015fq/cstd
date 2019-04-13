
# Android系统root破解原理分析 - 嵌入式Linux - CSDN博客

2018年10月24日 11:53:51[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：252



# 上一篇文章
[Android adb 源码分析](https://blog.csdn.net/weiqifa0/article/details/83310629)
# 理论基础
root破解过程的终极目标是替换掉系统中的su程序。但是要想替换掉系统中su程序本身就是需要root权限的，怎样在root破解过程中获得root权限，成为我们研究的重点了。下面我们先清点一下我们需要破解系统情况，假设需要破解的Android系统具备如下条件：

## 办法
1、可以通过adb连接到设备，一般意味着驱动程序已经安装。
2、但是adb获得用户权限是shell用户，而不是root。（漏洞）

想理解root破解过程我们首先需要了解一下adb工具，SDK中包含adb工具，设备端有adbd服务程序后台运行，为开发机的adb程序提供服务，adbd的权限，决定了adb的权限。具体用户可查看/system/core/adb下的源码，查看Android.mk你将会发现adb和adbd其实是一份代码，然后通过宏来编译。
查看adb.c的adb_main函数你将会发现adbd中有如下代码：
```python
int adb_main(int is_daemon) 
    { 
       ...... 
      property_get("ro.secure", value, ""); 
       if (strcmp(value, "1") == 0) { 
         // don't run as root if ro.secure is set... 
           secure = 1; 
           ...... 
      } 
    
      if (secure) { 
          ...... 
         setgid(AID_SHELL); 
         setuid(AID_SHELL); 
         ...... 
     } 
  }
```
从中我们可以看到adbd会检测系统的ro.secure属性，如果该属性为1则将会把自己的用户权限降级成shell用户。一般设备出厂的时候在/default.prop文件中都会有：
ro.secure=1
这样将会使adbd启动的时候自动降级成shell用户。
然后我们再介绍一下adbd在什么时候启动的呢？答案是在init.rc中配置的系统服务，由init进程启动。我们查看init.rc中有如下内容：
```python
# adbd is controlled by the persist.service.adb.enable system property 
   2: service adbd /sbin/adbd 
   3:     disabled
```
对Android属性系统少有了解的朋友将会知道，在init.rc中配置的系统服务启动的时候都是root权限（因为init进行是root权限，其子程序也是root）。由此我们可以知道在adbd程序在执行：
```python
/* then switch user and group to "shell" */ 
   setgid(AID_SHELL); 
   setuid(AID_SHELL);
```
代码之前都是root权限，只有执行这两句之后才变成shell权限的。
这样我们就可以引出root破解过程中获得root权限的方法了，那就是让以上面setgid和setuid函数执行失败，也就是降级失败，那就继续在root权限下面运行了。
这其实利用了一个RageAgainstTheCage漏洞，具体分析请参考《Android adb setuid提权漏洞的分析》和《RageAgainstTheCage》
通过上面的介绍我们发现利用RageAgainstTheCage漏洞，可以使adbd获得root权限，也就是adb获得了root权限。拿到root权限剩下的问题就好办了，复制破解之后的su程序到系统中

