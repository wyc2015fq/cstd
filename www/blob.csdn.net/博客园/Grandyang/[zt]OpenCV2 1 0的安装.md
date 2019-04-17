# [zt]OpenCV2.1.0的安装 - Grandyang - 博客园







# [[zt]OpenCV2.1.0的安装](https://www.cnblogs.com/grandyang/p/3982813.html)





下载和安装 OpenCV 2.1.0

![](http://hi.csdn.net/attachment/201202/20/0_1329706208J932.gif)
2、添加库文件：打开VS 2008，选择菜单：Tools->options->Projects and Solutions >VC++ Directories
首先从“Show directories for”中的下拉列表中选择“library files”.
然后在库文件列表中依次添加以下库文件的路径：
D:\Program Files\OpenCV2.1\lib

如图所示。继续添加D:\Program Files\OpenCV2.1\之类的文件到相应的目录下
![](http://hi.csdn.net/attachment/201202/20/0_1329706188BKwB.gif)![](http://hi.csdn.net/attachment/201202/20/0_1329706239Tmlc.gif)

![](http://hi.csdn.net/attachment/201202/20/0_13297062554s4z.gif)
4、配置项目所需要的lib。
先打开一个项目/解决方案，在菜单栏上选择：Project－>project Properties－>Configuration Properties－>Linker－>Input ->Additional Dependencies，如下图。在附加依赖项加入cxcore210.lib cv210.lib ml210.lib cvaux210.lib highgui210.lib cxcore210d.lib cv210d.lib ml210d.lib cvaux210d.lib highgui210d.lib
等本项目需要的lib。 











