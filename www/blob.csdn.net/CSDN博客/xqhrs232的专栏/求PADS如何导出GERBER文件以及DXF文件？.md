# 求PADS如何导出GERBER文件以及DXF文件？ - xqhrs232的专栏 - CSDN博客
2015年03月20日 11:35:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2671
原文地址::[http://wenda.chinabaike.com/b/9642/2013/1103/616351.html](http://wenda.chinabaike.com/b/9642/2013/1103/616351.html)
相关文章
1、从PADS里导出DXF文件的步骤----[http://bbs.ednchina.com/BLOG_ARTICLE_2116101.HTM](http://bbs.ednchina.com/BLOG_ARTICLE_2116101.HTM)
2、[在PADS中导出贴片图及dxf文件](http://blog.chinaunix.net/uid-151000-id-2777464.html) ----[http://blog.chinaunix.net/uid-151000-id-2777464.html](http://blog.chinaunix.net/uid-151000-id-2777464.html)
PADS如何导出GERBER文件以及DXF文件
技术 2009-10-17 20:12 阅读 评论字号： 大大 中中 小小
一、导出网络表至PCB版图。
首先，在电路图状态下，按上方的功能按钮，PROJECT MANAGER来到工程文件夹状态栏，点击DRC，对电路图进行错误检验，确保电路图无错后，进入TOOLS菜单，选择CREAT NETLIST，选择OTHER，在FORMATTER栏选择PADS2K.DLL，将默认的输出后缀名改为.ASC，保存，同时可以将VIEW OUTPUT打勾，可以直观看到输出的二进制文件的正确性。
![](http://wenda.chinabaike.com/uploads/allimg/131103/0A042I17-0.jpg)
新建PCB文件，点击FILE/IMPORT，导入ASC文件，将各个器件及其连接线导入到PCB中，此时可以用元器件推挤工具，TOOLS/DISPERSE COMPONENTS对元器件进行平铺放置。
![](http://wenda.chinabaike.com/uploads/allimg/131103/0A0424427-1.jpg)
二、导出CAMGERBER文件。
注意，导出CAM文件时，PCB的单位状态必须为mm，这样才能在CAM350里面得到正确的显示。FILE/CAM，进入以下界面，在这个界面可以看到每一个要导出的层次及其描述。点击PREVIEW，对每一层进行预览，在CAM下面的选项选择CREAT，并把所需导出的文件夹建立在你所需要的位置，点击RUN，所有的文件被导出在该文件夹。在这里需要注意的是，对NC DRILL而言，有一个参数必须为2，4，如图中所示，为CAM的标准参数，否则DRILL输出将不正常。
![](http://wenda.chinabaike.com/uploads/allimg/131103/0A0425R8-2.jpg)
![](http://wenda.chinabaike.com/uploads/allimg/131103/0A04231T-3.jpg)
![](http://wenda.chinabaike.com/uploads/allimg/131103/0A042OB-4.jpg)
在CAM中选择AUTOIMPORT，自动导入上面操作所放置的文件夹，将显示出PCB板的CAM图纸。也可以一层一层地点击观看，确保准确无误。
![](http://wenda.chinabaike.com/uploads/allimg/131103/0A0422146-5.jpg)
三、导出CAD支持的DXF文件。
导出CAD文件是为了工程人员对PCB的外壳和器件的放置位置进行确认，单位必须为mm。直接选择导出文件，后缀名改为DXF，该文件就能被CAD打开，并进行观看。
![](http://wenda.chinabaike.com/uploads/allimg/131103/0A0421136-6.jpg)
![](http://wenda.chinabaike.com/uploads/allimg/131103/0A042GJ-7.jpg)
![](http://wenda.chinabaike.com/uploads/allimg/131103/0A0423361-8.jpg)
四、原理图与PCB的比较，及PCB之间的比较。
点击COMPARE ECO，弹出比较菜单，在此时，可以选择对两个不同的PCB文件进行比较，对原理图的比较必须先从原理图导出ASC，然后生成一个新的PCB文件，用该文件对先文件比较也就等同于PCB至于SCH的比较了。
![](http://wenda.chinabaike.com/uploads/allimg/131103/0A0425434-9.jpg)
五、ECO模式下更改元件的标识符。如C2改为C21。
点击U1-U2就可以对某一个文件名进行替换，然后再在原理图中替换完，进行比较确认无误。
![](http://wenda.chinabaike.com/uploads/allimg/131103/0A042HK-10.jpg)
![](http://wenda.chinabaike.com/uploads/allimg/131103/0A0425245-11.jpg)
