# Android开发中遇到的问题(三)——eclipse创建android项目无法正常预览布局文件 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Android开发中遇到的问题(三)——eclipse创建android项目无法正常预览布局文件](https://www.cnblogs.com/xdp-gacl/p/4333078.html)



## 一、问题描述

　　今天使用SDK Manager将Android SDK的版本更新到了Android 5.1的版本，eclipse创建android项目时，预览activity_main.xml文件时提示： This version of the rendering library is more recent than your version of ADT plug-in. Please update ADT plug-in，导致无法正常预览布局文件，现象如下图所示： 

![](https://images0.cnblogs.com/blog2015/289233/201503/121651582615664.png)

　　上网查了一下原因，**问题根源：SDK版本过高，ADT版本过低。**

## 二、问题的解决办法——升级ADT插件

　　查看现在的ADT插件的版本

![](https://images0.cnblogs.com/blog2015/289233/201503/121716424647995.png)

　　在线更新ADT插件：Help --> Install New Softwares...，

![](https://images0.cnblogs.com/blog2015/289233/201503/121733306524303.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/121734320422391.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/121736423867046.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/121737323248168.png)

　　点击【Finish】按钮就开始安装了，安装完成之后重启Eclipse就可以了。更新了ADT插件之后，就可以正常预览到视图了，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/121740286208027.png)

　　这是通过在线更新ADT插件来解决问题的，这个在线更新ADT插件需要FQ才能够正常更新成功。









