# 禁止android进去休眠状态 - xqhrs232的专栏 - CSDN博客
2011年09月19日 15:43:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1192标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.sina.com.cn/s/blog_70b546060100lpvj.html](http://blog.sina.com.cn/s/blog_70b546060100lpvj.html)
**一、现象**
android 启动后，每个3秒钟发送休眠指令，系统进入休眠状态。
**二、解决方法**
1、使用命令make distclean清楚掉上一次编译留下的文件。
2、使用make mrproper 命令检查互相依赖关系。
3、使用make imx51_android_defconfig，把IMX51_evk板子内核配置写入到.config中。
4、使用make menuconfig命令（此时，菜单显示内容为.config中的配置，即I.MX51_EVK配置）
![图片](http://b39.photo.store.qq.com/http_imgload.cgi?/rurl4_b=c037a54bbc37c832374e9b0e9d14f4255da2ee945df2432c2337edc59a5469655f4cc793a989074adb10064d15b4d18c466e68fbbd6c2837bba9570f2b15e62c76f48b898698c2a90e8c6a3d0642a2ef1f2c853d&a=41&b=39)
进入Power management options--->
![图片](http://b39.photo.store.qq.com/http_imgload.cgi?/rurl4_b=c037a54bbc37c832374e9b0e9d14f4256c57b6b7e04e4edfe1668a4d953f26a7f6ca15702f7040172e0de1a6f4e0c91c4edfb5dbcbdd392b2d37fc5cf71dd1d7c35edd7d33d4419f52fc31f3237742cb81e58bb4&a=38&b=39)
去掉userspace wake locks和early suspend（suspend to RAM and standby选中，因为suspend和wait lock是一一对应的）
**5、保存退出后，重新编译内核。**
**6、在 uboot命令行，运行：run bootcmd_net从NFS启动系统。**
