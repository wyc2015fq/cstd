# Viso文件转eps - 家家的专栏 - CSDN博客





2012年10月28日 16:47:46[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2964









将viso的vsd图转成bmp图，然后再转成eps格式。但这样做呢，画面严重失真。因为vsd是矢量图，bmp是位图，eps是矢量图，经过两次转换，导致画质降低。




一、安装Adobe PDF

    1.一般的情况下，我安装的是Adobe_Pdf_8.1.2

    2.在安装完Adobe PDF后，需要在“开始”->“设备或打印机”下找到你刚刚安装的Adobe PDF虚拟打印机，然后点右键，打开“打印机属

      性”，找到“高级”，并将“直接打印到打印机”勾上，点“应用”就OK了。环境配置成功。


二、将vsd打印输出为ps文件格式


    1、在viso中，制好图；


    2、选择“打印”，选择“adobe pdf”,然后选择“属性”；


    3、在“布局”中选“高级”；将“文档选项”下的“PostScript Options”下的“PostScript Output Option”选为“Encapsulated  

       PostScript (EPS)”.


    4、选择“打印到文件”


    5、最后填入要输出文件名称为，“xxx.ps”。一定要写上后缀。

三、将ps文件转成eps文件

    1、打开“xxx.ps”，它会默认自动打印为“xxx.pdf”，然后将“xxx.pdf”打开后，另存为“xxx.eps”，就完成了EPS的整个过程。


    2、安装GSview，运行GSview,正常显示EPS文件中的图。



通过上面两步骤，基本OK.



但需要注意，如果转换后的eps文件图，有些背景颜色或线不满足要求，请重新调整vsd.  特别是背景填充色时，必须将“填充”下的颜色选择为“纯色”，透明度设为 0% 。否则，最终的 EPS 会出现莫名其妙的 花格！




转自勇哥的qq空间：[http://user.qzone.qq.com/872980668/blog/1351171715?ptlang=2052&ADUIN=376222695&ADSESSION=1351411506&ADTAG=CLIENT.QQ.4687_FriendTip_QzoneFolder.0#!app=2&via=QZ.HashRefresh&pos=1351171715](http://user.qzone.qq.com/872980668/blog/1351171715?ptlang=2052&ADUIN=376222695&ADSESSION=1351411506&ADTAG=CLIENT.QQ.4687_FriendTip_QzoneFolder.0#!app=2&via=QZ.HashRefresh&pos=1351171715)




