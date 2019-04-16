# 关于在win8系统下用VMware 9.0装系统导致物理机不断重启的解决办法 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [关于在win8系统下用VMware 9.0装系统导致物理机不断重启的解决办法](https://www.cnblogs.com/xdp-gacl/p/4297994.html)



## 一、问题描述

　　前段时间将操作系统换成了Win8，安装上VMware 9.0英文版。然后在VMware中安装centos系统，结果每次到安装系统的时候，物理机系统就会莫名其妙地自动重启，毫无征兆地出现，测试几次都是如此。在虚拟机中安装操作系统会导致物理机系统自动重启，这个问题还是头一次遇到，万般无奈之下，我从朋友那边拷贝已经安装好的系统过来用，结果在使用VMware打开已经安装好的系统时，也依然会导致我的电脑重启，这个问题一直困扰了我好久，今天在网上看到到了一篇文章http://blog.csdn.net/michaeljscofield/article/details/8895376，终于知道了出现这个问题的原因，原来是我的物理机没有开启虚拟化才会导致这个问题出现的。

## 二、问题解决过程

### 2.1、笔记本环境

　　先说说我自己的环境吧，我的笔记本电脑是联想Z500，win8企业版，处理器是I5，查过支持虚拟化，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201502/231655271894611.png)

### 2.2、具体解决步骤

A:进入BIOS

   1.重启电脑

    2.在看见 lenovo 大logo后,按Fn+F2组合功能键进入BIOS

B.开启VT

   1.按→方向键移动去Configuration 页，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201502/231705316742304.png)

    2.移动去下图Inter Virtual Technology 改成如下图那样，之前是 Disabled.

![](https://images0.cnblogs.com/blog/289233/201502/231708497685185.png)

　　最后按下F10保存退出。这下就可以使用VMware正常安装和打开操作系统了。









