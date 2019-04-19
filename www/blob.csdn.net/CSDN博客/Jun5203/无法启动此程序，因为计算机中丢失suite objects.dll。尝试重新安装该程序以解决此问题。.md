# 无法启动此程序，因为计算机中丢失suite objects.dll。尝试重新安装该程序以解决此问题。 - Jun5203 - CSDN博客
2018年09月28日 14:37:33[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：435
所属专栏：[UML](https://blog.csdn.net/column/details/26918.html)
### 问题
无法启动此程序，因为计算机中丢失suite objects.dll。尝试重新安装该程序以解决此问题。
![在这里插入图片描述](https://img-blog.csdn.net/20180928142535821?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
本来以为真的是找不到suite objects.dll，可是一查找，居然发现~
![在这里插入图片描述](https://img-blog.csdn.net/20180928142725118?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后站在巨人的肩膀上，原来找不到的原因是安装程序自动设置在Path中的环境变量有误~
### 解决方法
1、打开控制面板-系统和安全-系统；
![在这里插入图片描述](https://img-blog.csdn.net/20180928143147599?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2、点击高级-环境变量；
![在这里插入图片描述](https://img-blog.csdn.net/20180928143210901?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3、双击Path；
![在这里插入图片描述](https://img-blog.csdn.net/20180928143225150?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4、选中红框中的内容，点击编辑，将common改为Common；
![在这里插入图片描述](https://img-blog.csdn.net/20180928143235480?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
5、点击确定，大功告成。
### 感悟
学无止境，在学习的路上，总会遇到各种各种的问题，关键在于你用什么样的心态去对待他，投之以桃，报之以李，付出总有回报，加油！
