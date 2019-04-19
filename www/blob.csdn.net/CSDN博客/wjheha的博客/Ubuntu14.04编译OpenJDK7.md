# Ubuntu14.04编译OpenJDK7 - wjheha的博客 - CSDN博客
2018年04月07日 16:59:11[wjheha](https://me.csdn.net/wjheha)阅读数：90
**花了半天时间，在Ubuntu14.04上编译了OpenJDK7**，期间掉进了很多坑，找到了很多好资料，分享出来：
Ubuntu14.04（64bit）下编译OpenJDK：[https://zhuanlan.zhihu.com/p/23478436](https://zhuanlan.zhihu.com/p/23478436)
编译OpenJDK7 ： [http://jaychang.iteye.com/blog/2397232](http://jaychang.iteye.com/blog/2397232)
Linux下编译OpenJDK源码以及在Eclipse中调试Hotspot虚拟机：[https://blog.csdn.net/leonliu06/article/details/78495035](https://blog.csdn.net/leonliu06/article/details/78495035)
Linux下在Eclipse中调试Hotspot虚拟机源码：[https://blog.csdn.net/leonliu06/article/details/78495202](https://blog.csdn.net/leonliu06/article/details/78495202)
【深入理解Java虚拟机】之自己编译JDK（已经完成，其中有自己遇到的各种问题及解决办法）：[https://blog.csdn.net/u013190088/article/details/71703255](https://blog.csdn.net/u013190088/article/details/71703255)
使用Netbeans开发调试OpenJDK中的HotSpot：[https://blog.csdn.net/pange1991/article/details/51580969](https://blog.csdn.net/pange1991/article/details/51580969)
由于使用的是虚拟机，安装netbeans图形化界面后，电脑卡的不行，就没弄了。
**提醒一下**：在编译完OpenJDK后，会在build目录下出现编译结果，在Bootstrap JDK用来编译后，后面就不需要再使用它了，周志明那本《深入理解Java虚拟机》中原文“进入OpenJDK源码下的build/j2sdk-image目录下，这是整个JDK的完整编译结果，复制到JAVA_HOME目录，就可以作为一个完整的JDK来使用”，作者后面的使用的目录都是Bootstrap JDK的目录，但是由于覆盖了JAVA_HOME目录，**其实使用的是自己编译出来的JDK**，这一点有一定歧义，上文基本都没说清楚。
最后，附上一张编译成功的截图： 
![这里写图片描述](https://img-blog.csdn.net/20180407170112916?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dqaGVoYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
