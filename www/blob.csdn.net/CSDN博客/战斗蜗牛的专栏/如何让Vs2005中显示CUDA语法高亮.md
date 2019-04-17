# 如何让Vs2005中显示CUDA语法高亮 - 战斗蜗牛的专栏 - CSDN博客





2012年11月03日 09:31:07[vbskj](https://me.csdn.net/vbskj)阅读数：1566
个人分类：[CUDA](https://blog.csdn.net/vbskj/article/category/643641)









1）安装CUDA wizard后，还需要编译share目录下面的共享库，然后添加入自己的解决方案中，包括自己的工程、cutil工程、shrUtils工程；

![](https://img-my.csdn.net/uploads/201211/03/1351906532_1233.jpg)

2）界面的工具菜单--》选项--》文本编辑器--》扩展名，添加“cu”，然后选择“MVC”编辑器，应用之后重启VS2005，就能支持语法高亮了。


![](https://img-my.csdn.net/uploads/201211/03/1351906470_7091.jpg)


