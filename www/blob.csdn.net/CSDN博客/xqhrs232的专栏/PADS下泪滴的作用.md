# PADS下泪滴的作用 - xqhrs232的专栏 - CSDN博客
2014年09月19日 23:29:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1587
原文地址::[http://blog.csdn.net/danney0510/article/details/5369186](http://blog.csdn.net/danney0510/article/details/5369186)
相关文章
1、添加泪滴焊盘的作用----[http://wenku.baidu.com/link?url=0fMeZlyr-YgmGEoJWcQbNi2B4cnSkK_AZOCxfJ-kfDKYEMZ8esSM1IKajqlL4edN-aMgO2nfx7f9MaglkbsSkPuUex0JGX_gGWLIa3CNecC](http://wenku.baidu.com/link?url=0fMeZlyr-YgmGEoJWcQbNi2B4cnSkK_AZOCxfJ-kfDKYEMZ8esSM1IKajqlL4edN-aMgO2nfx7f9MaglkbsSkPuUex0JGX_gGWLIa3CNecC)
2、PCB覆铜和泪滴的作用----[http://whoshallwe.blog.163.com/blog/static/507441552012720115317671/](http://whoshallwe.blog.163.com/blog/static/507441552012720115317671/)
添加泪滴焊盘的作用
2009-12-05 10:41
添加泪滴的作用：
泪滴     是焊盘与导线或者是导线与导孔之间的滴装连接过度，设置泪滴的目的是在电路板受到巨大外力的冲撞时，避免导线与焊盘或者导线与导孔的接触点断开，另外，设置泪滴也可使PCB电路板显得更加美观。
teardrop的作用是避免信号线宽突然变小而造成反射，可使走线与元件焊盘之间的连接趋于平稳过渡化，解决了焊盘与走线之间的连接容易断裂的问题。
1、焊接上，可以保护焊盘，避免多次焊接是焊盘的脱落
2、加强连接的可靠性（生产是可以避免蚀刻不均，过孔偏位出现的裂缝等）
3、平滑阻抗，减少阻抗的急剧跳变

在电路板设计中，为了让焊盘更坚固，防止机械制板时焊盘与导线之间断开，常在焊盘和导线之间用铜膜布置一个过渡区，形状像泪滴，故常称做补泪滴（ Teardrops ）。
泪滴的放置可以执行主菜单命令 Tools/Teardrops… ，将弹出如图1所示的 Teardrop ptions （泪滴）设置对话框。
![](http://hiphotos.baidu.com/liu_linzhong/pic/item/6fef290bbfbe84e03ac763e2.jpg)
图 1泪滴设置对话框
接下来，对泪滴设置对话框中的各个选项区域的作用进行相应的介绍。
① General 选项区域设置
General 选项区域各项的设置如下：
● All Pads 复选项：用于设置是否对所有的焊盘都进行补泪滴操作。
● All Vias 复选项：用于设置是否对所有 过孔 都进行补泪滴操作。
● Selected Objects Only 复选项：用于设置是否只对所选中的元件进行补泪滴。
● Force Teardrops 复选项：用于设置是否强制性的补泪滴。
● Create Report 复选项：用于设置补泪滴操作结束后是否生成补泪滴的报告文件。
② Action 选项区域设置
Action 选项区域各基的设置如下：
● Add 单选项：表示是泪滴的添加操作。
● Remove 单选项：表示是泪滴的删除操作。
③ teardrop Style 选项区域设置
Teardrop Style 选项区域各项的设置介绍如下：
● Arc 单选项：表示选择圆弧形补泪滴。
● Track 单选项：表示选择用导线形做补泪滴。
