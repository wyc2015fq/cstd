# PADS导出贴片图全过程 ----输出PDF格式文件 - xqhrs232的专栏 - CSDN博客
2014年10月23日 11:25:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：16375
原文地址::[http://lastnight1034.blog.163.com/blog/static/167118149201152133132118/](http://lastnight1034.blog.163.com/blog/static/167118149201152133132118/)
相关文章
1、PADS layout pdf打印输出----[http://wenku.baidu.com/link?url=Hh6jxUrxrBn2MaZ21tv5CaD3XNPcVq_YAw5aV2up_m595AfBUespH2Jm1w85wMsf6dAI2tXBkrDIX1PZ5K4QLAgq8fB-qB3tecg5puVwMv3](http://wenku.baidu.com/link?url=Hh6jxUrxrBn2MaZ21tv5CaD3XNPcVq_YAw5aV2up_m595AfBUespH2Jm1w85wMsf6dAI2tXBkrDIX1PZ5K4QLAgq8fB-qB3tecg5puVwMv3)
2、pads2007怎么生成元件丝印PDF----[http://zhidao.baidu.com/link?url=vXSoOEUZGZSwTuSj_wpnqEhsTG0j-D9raaNqsYxD5gyea97HjnPAZsVwS5cjSL79pD6Vzmvhh7qgkJ32xQ1oQK](http://zhidao.baidu.com/link?url=vXSoOEUZGZSwTuSj_wpnqEhsTG0j-D9raaNqsYxD5gyea97HjnPAZsVwS5cjSL79pD6Vzmvhh7qgkJ32xQ1oQK)
3、[pdfFactory
 Prov5.20](http://www.crsky.com/soft/4797.html)下载地址----[http://www.crsky.com/soft/4797.html](http://www.crsky.com/soft/4797.html)
4、pdfFactory Pro PDF虚拟打印机（破解版）----[http://download.csdn.net/detail/jyedu/3082791](http://download.csdn.net/detail/jyedu/3082791)
首先打开Layout。
在File菜单下选中CAM选项，在跳出的Define CAM Documents中单击ADD，添加个输出文件：
![PADS导出贴片图全过程 - lastnight1034 - lastnight1034的博客](http://img.ph.126.net/wZiRNGDmTX_KBo5FaNI1Nw==/2556355738502956313.bmp)
 以导出顶层贴片图为例：
     在Document Name中填TOP，以便区分。
     下面有两个分类：Customize Document和Output Device,我们分别来设置：
     Layers中我们选择各个层的设置选项：
![PADS导出贴片图全过程 - lastnight1034 - lastnight1034的博客](http://img.ph.126.net/XAgj1D0BjrCaFsxPDIkAFg==/569705352879146232.bmp)
 个人不喜欢勾选Text选项，极少在顶层或底层添加Text，尽量在丝印层添加Text或Copper。（BOTTOM层这里设置唯一不同的是Component outlines 中需要勾选的是Bottom Mounted）
![PADS导出贴片图全过程 - lastnight1034 - lastnight1034的博客](http://img96.ph.126.net/EwteELZj_ZmYUagBLhdIwg==/731553464473080843.bmp)
 Silkscreen层根据需要选择加不加进去。点击OK退出，至此Layers选项设置完毕，点击Options。
![PADS导出贴片图全过程 - lastnight1034 - lastnight1034的博客](http://img763.ph.126.net/PdtH3AP293XPD4lWDXjtqw==/1996220534833517983.bmp)
 这个页面只需设置注意两个地方，Justification中选中下拉列表的Scale To Fit，将导出的贴片图在纸张范围内最大化，可以从右边的Preview预览到各个选项的不同。点击OK，之后点击Options下面的Preview selections就可以看到需要输出的贴片图。特别需要注意的是：BOTTOM层设置时，必须勾选Mirror Image使所有丝印镜像。
![PADS导出贴片图全过程 - lastnight1034 - lastnight1034的博客](http://img610.ph.126.net/hzl9EIEg3khgfphnx1JdnA==/1949214213723145737.bmp)
这是没有勾选Mirror Image的效果。
最后一步也事关重要，Output Device选择Print,之后再选择Device Setup选择打印机，可以使用虚拟打印机打出PDF或图片格式保存。
![PADS导出贴片图全过程 - lastnight1034 - lastnight1034的博客](http://img233.ph.126.net/Xb0XgagluBWOA905RNAsTA==/1754715004816357700.bmp)
//================================================================================================================================
备注:
1>需要安装pdfFactory Pro(虚拟打印机)
pdfFactory Pro(虚拟打印机)是一个无须 Acrobat 创建 Adobe PDF 文件的打印机驱动程序。pdfFactory Pro(虚拟打印机)提供的创建 PDF 文件的方法比其他方法更方便和高效。 功能包括: 多个文档整合到一个 PDF 文件中; 内嵌字体; 通过 E-mail 发送;
 预览; 自动压缩优化。

