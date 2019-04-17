# Appium +IOS 自动化测试多设备并行执行实现 - zhusongziye的博客 - CSDN博客





2018年08月12日 19:41:04[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：855








最近尝试做 IOS Appium 多设备支持，并行执行。



方案

1.通过命令定义不同端口启动多个appium server。



2.脚本里传入不同端口，实现多设备执行

具体如下。启动多个appium server方式：



![](https://img-blog.csdn.net/20180812193924327?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



实例



![](https://img-blog.csdn.net/20180812193934347?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



代码里传入启动server时的不同的-p端口，即可实现多设备执行。

![](https://img-blog.csdn.net/2018081219402857?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



