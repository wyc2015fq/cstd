# 关于fiddler导出jmeter插件的安装问题 - zhusongziye的博客 - CSDN博客





2018年05月12日 21:39:31[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：598








最近部分同学在使用fiddler导出jmeter脚本插件，出现一些问题，现在给大家做下分析和解决：

问题如下：

![](https://img-blog.csdn.net/20180512213751250?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

分析：插件放入，没有生效，是放错位置的原因

解决方案：

1.打开fiddler

![](https://img-blog.csdn.net/20180512213804597?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2.打开tools

![](https://img-blog.csdn.net/20180512213811662?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3.选择路径，打开目录，我的电脑是：C:\Users\yhxie\AppData\Local\Programs\Fiddler

![](https://img-blog.csdn.net/20180512213843103?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

4.将文件放在该目录下：

![](https://img-blog.csdn.net/20180512213900384?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

5.重启fiddler，再看，如下：

![](https://img-blog.csdn.net/20180512213916829?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

6.到此问题得到解决



