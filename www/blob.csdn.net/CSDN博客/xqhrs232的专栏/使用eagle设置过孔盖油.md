# 使用eagle设置过孔盖油 - xqhrs232的专栏 - CSDN博客
2015年02月11日 16:01:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2238
原文地址::[http://bbs.ednchina.com/BLOG_ARTICLE_3017205.HTM](http://bbs.ednchina.com/BLOG_ARTICLE_3017205.HTM)
相关文章
1、Eagle的Gerber文件输出设置请教----[http://www.amobbs.com/archiver/tid-3901599.html](http://www.amobbs.com/archiver/tid-3901599.html)
2、如何设置Eagle PCB的过孔盖油----[http://blog.sina.com.cn/s/blog_c31407eb0101ha53.html](http://blog.sina.com.cn/s/blog_c31407eb0101ha53.html)
制版厂家受到gerber文件之后，会按照gerber文件生产（据我所知嘉立创是这样的），而无法再设置过孔是否盖油，所以过孔是否盖油需要在gerber文件中体现出来。
众所周知，电路板分很多层，cream（paste）是指光亮的焊盘层，stop（solder）是指阻焊层，place（silk）是指表面印刷的白色文字层。使用gerbv打开gerber文件的stop层，就会发现，它所指示的区域之外都是要刷上绿油的。设置过孔盖油，就是要将stop层的区域，保留cream层并且除去via层。
首先设置：
edit->design rules->masks->limit
(如果板上最大的过孔孔径是20mil，就将limit设置为20mil。它的意思就是，solder层只会覆盖比它更大孔径的smd/pads/vias。）
然后设置：
gerber文件的阻焊层为stop（29）层。
如果同时选中via（18）层，则过孔开窗；
如果没有选中via（18）层，则过孔盖油。
使用gerbv查看两种情况的gerber文件，就明白了。
//=================================================================================================================================
备注::
1》要特别注意前面的一个设置----edit->design rules->masks->limit；过孔是多大就把这个limit值设置为多大
2》1inch=1000mil,Eagle软件里面的单位是inch(英寸)
