# PADS2007如何打印PCB元件的丝印图 - xqhrs232的专栏 - CSDN博客
2013年10月27日 19:57:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2505
原文地址::[http://1x9w8j6.blog.163.com/blog/static/613963442011716111257324/](http://1x9w8j6.blog.163.com/blog/static/613963442011716111257324/)
1.File/cam.../add
2.Document name --- silkscreen top / document type-- silkscreen
3.output device --- print (注：只打印时选此项，转GERBER则选Photo）
4.customize document --- layers
5.选中需要打印的层，勾选 board outline / ref.des /outline/mounted
6.如果要印彩色，则将各已选项后面的颜色改为自己喜欢的颜色即可。
7.按 preview ,可以预览一下效果，如果满意则按 OK
8.再按 OK, 按 RUN 即可打印。
//===============================================================================================================
备注::
1》已经创建的CAM Documents会在CAM Documents列表中列出
2》每个CAM Documents文件的设置情况会在Summary栏列出
3》通过Add...可以创建自己的CAM Documents文件，在Add Document属性页可以对新建的CAM Documents进行设置------比如该文件打印的那一层的数据，打印的是些
什么数据(Board outline/Text/Outlines)
4》举例要打印底层贴片图----Summary栏
Silkscreen:(Board,Outline Top)
Top:(Text,Ref.Des,Attributes,Keepouts,Outlines)
Silkscreen Top:(Lines,Text,Outlines)
