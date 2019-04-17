# 使用eagle导出国内PCB制板商可用的gerber文件 | APLUSA +复制链接 - V__KING__的专栏 - CSDN博客





2013年10月19日 12:01:34[v__king__](https://me.csdn.net/V__KING__)阅读数：2634











使用eagle导出国内PCB制板商可用的gerber文件

发表于770 天前⁄ [嵌入式硬件](http://aplusa.sinaapp.com/archives/category/%E5%B5%8C%E5%85%A5%E5%BC%8F%E7%A1%AC%E4%BB%B6)⁄ [暂无评论](http://aplusa.sinaapp.com/archives/61#respond)





eagle是一款在国外广泛流行的电路板绘制CAD软件，与protel99se类似。但功能个人感觉更为强大。关于eagle的说明、下载和安装就不多说了，google一下能看到很多。Eagle足够好用，但问题是目前国内的PCB厂商绝大多数并不支持eagle直接绘制的.brd扩展文件（相当于protel中的.PCB文件），所以我们需要把eagle绘制好的.brd文件转换成大多数protel厂商都熟知的gerber文件。网上暂时没有找到完整的使用eagle在国内制板的介绍。在此，我整理出这份文档，制作的是**ST公司iNEMO比赛的扩展板**。每个步骤都是实际操作，大家可以放心。

Eagle版本：5.10.0

CAM350版本：9.5

在操作之前，将附件rar文件中的两个cam文件复制到你的eagle安装目录下的cam文件夹中。

首先，我们打开已经绘制好的.brd文件，如下图所示。在红色的画圈的地方输入 run drillcfg ，回车运行。



[](http://www.eefocus.com/blog/link2url.php?link=http%3A%2F%2Fwww.eefocus.com%2Fdata%2F11-09%2F1315408322_64da4caa.jpg)![](http://www.eefocus.com/include/picture/500/400/data/11-09/1315408322_64da4caa.jpg)



运行这个ulp文件后，得到如下图的对话框，将单位调整为inch。

![](http://www.eefocus.com/blog/link2url.php?link=http%3A%2F%2Fwww.eefocus.com%2Fdata%2F11-09%2F1315408330_c0946813.jpg)



确定后到这个界面，直接OK



![](http://www.eefocus.com/blog/link2url.php?link=http%3A%2F%2Fwww.eefocus.com%2Fdata%2F11-09%2F1315408337_d5280934.jpg)



选择保存的位置后（我保存在与.sch原理图文件及.brd的PCB文件一个文件夹），可以看到多出了.drl文件。



![](http://www.eefocus.com/blog/link2url.php?link=http%3A%2F%2Fwww.eefocus.com%2Fdata%2F11-09%2F1315408345_62241da9.jpg)



然后我们运行两个cam文件。点击图中的CAM标志。



![](http://www.eefocus.com/blog/link2url.php?link=http%3A%2F%2Fwww.eefocus.com%2Fdata%2F11-09%2F1315408355_2d5d96fb.jpg)



选择“文件”>>“打开”>>“作业”



![](http://www.eefocus.com/blog/link2url.php?link=http%3A%2F%2Fwww.eefocus.com%2Fdata%2F11-09%2F1315408362_594d3b68.jpg)



选择SparkFun_sfe-gerb274x.cam，注意不要修改默认的层设置。因为如果不注意，可能会把丝印层和top层重叠，导致短路的发生。按照默认设置，选择“处理作业”。界面如下图所示。



![](http://www.eefocus.com/blog/link2url.php?link=http%3A%2F%2Fwww.eefocus.com%2Fdata%2F11-09%2F1315408369_b99590ac.jpg)



完成后再按照同样方式打开excellon.cam，并“处理作业”。处理后打开刚才的文件夹，发现多出了很多文件:



![](http://www.eefocus.com/blog/link2url.php?link=http%3A%2F%2Fwww.eefocus.com%2Fdata%2F11-09%2F1315409642_25f54fc2.jpg)



我们把这些上述所有步骤产生的文件打包发送给PCB产商就可以了。各层的含义自己搜索吧，可以使用CAM350查看生成的gerber文件（如下图所示）。附件中的rar文件中包含用到的两个cam文件：SparkFun_sfe-gerb274x.cam，excellon.cam



![](http://www.eefocus.com/blog/link2url.php?link=http%3A%2F%2Fwww.eefocus.com%2Fdata%2F11-09%2F1315408384_6a0e6cb6.jpg)
[附件：cam.rar](http://1.aplusa.sinaapp.com/datadownload/cam.rar)


author：santiago of Aplusa

**2011-09-07**







[返回](http://aplusa.sinaapp.com/archives/61)








![](http://1.gravatar.com/avatar/17873aed758a7990e3e75b3347b29759?s=48&d=identicon&r=G)




#### 作者: [aplusa](http://aplusa.sinaapp.com/archives/author/aplusa)


该日志由 aplusa 于2011年09月10日发表在[嵌入式硬件](http://aplusa.sinaapp.com/archives/category/%E5%B5%8C%E5%85%A5%E5%BC%8F%E7%A1%AC%E4%BB%B6)分类下，
 你可以[发表评论](http://aplusa.sinaapp.com/archives/61#respond)，并在保留[原文地址](http://aplusa.sinaapp.com/archives/61)及作者的情况下[引用](http://aplusa.sinaapp.com/archives/61/trackback)到你的网站或博客。 

转载请注明: [使用eagle导出国内PCB制板商可用的gerber文件 | APLUSA](http://aplusa.sinaapp.com/archives/61)[+复制链接](http://aplusa.sinaapp.com/archives/61#)



