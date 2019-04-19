# PADS中CAM输出的相关设置 - xqhrs232的专栏 - CSDN博客
2014年04月13日 00:50:42[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2377
原文地址::[http://www.elecfans.com/article/80/112/2011/PADS.html](http://www.elecfans.com/article/80/112/2011/PADS.html)
相关文章
1、P​A​D​S​导​出​G​e​r​b​e​r​文​件----[http://wenku.baidu.com/view/ae599edad15abe23482f4dde.html](http://wenku.baidu.com/view/ae599edad15abe23482f4dde.html)
详细介绍了PADS中CAM输出的相关设置，现在介绍给大家学习
![](http://www.elecfans.com/uploads/allimg/110824/182555-110R4120032507.jpg)
　　首先导入一个标准的CAM输出文档，由IMPORT输入一个模板，该模板由他人提供，也可自己生成。
![](http://www.elecfans.com/uploads/allimg/110824/182555-110R41200392Y.jpg)
![](http://www.elecfans.com/uploads/allimg/110824/182555-110R4120046341.jpg)
　　在CAM输出界面，进入DRILL DRWAING，按OPTION，点击右上方DRILL SYMBOLS按钮，进入过孔设置界面，点击REGENERATE按钮，将生成对应本PCB的过孔文件。
![](http://www.elecfans.com/uploads/allimg/110824/182555-110R4120053S8.jpg)
　　然后在NC DRILL里按DEVICE SETUP按钮，将过孔文件设置为CAM350的标准，将LEADING TRAILING设置为2/4.
![](http://www.elecfans.com/uploads/allimg/110824/182555-110R4120059314.jpg)
　　最后要将CAM的输出格式改为RS274X，且更改该窗口上方的number of digits参数为2和4.
//===========================================================================================================================
备注：：
1》丝印图可以输出可以打印的格式文件-----*.xps;这种文件可以通过XPS Viewer软件来查看
2》还要特别注意几个文件的OFFSET，因为输出的CAM系列文件，一般是在CAM350软件里面查看，为了不重叠，最好把OFFSET偏置值设置好！
3》CAM的设置可以从IMPORT导入别人已经设置好的一个*.CAM文件,也可以把自己的设置EXPORT导出成一个*.CAM文件保存或者给别人使用

