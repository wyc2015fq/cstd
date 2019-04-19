# PADS保护已布线段 - xqhrs232的专栏 - CSDN博客
2013年12月02日 11:05:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1813
原文地址::[http://hi.baidu.com/leafing/item/14c7d1c4242f582cef4665a7](http://hi.baidu.com/leafing/item/14c7d1c4242f582cef4665a7)
相关网帖
1、pads中如何解除元件的锁定/保护(Lock/protect)状态----[http://zhidao.baidu.com/link?url=M7oWcd8YtNSoW9gG0brgQMSaKEd12GqXDbfljnCnfAypF369X6vSccUvLa4Kwm_d69hfEFPR9oXPkeSrCS7X2a](http://zhidao.baidu.com/link?url=M7oWcd8YtNSoW9gG0brgQMSaKEd12GqXDbfljnCnfAypF369X6vSccUvLa4Kwm_d69hfEFPR9oXPkeSrCS7X2a)
说来真是惭愧，用了PADS好长时间，一直是手动布线的，今天是在懒了，就打开router准备让他自个
布线，可是有一大半的线段都已经布好了，感觉布的挺好的，不想让机器把那些已布的线段给重新布，
于是就想到要保护那些已布的线段，记得protel自动布线的时候有一个选项，可以保护所有已布的线段，
只要点个勾就可以了，这PADS怎么搞？
摸索了半天终于发现了：
在pads layout下选择过滤器到select nets
然后选中所有的net
按下ctrl+q建，在弹出的界面上如下操作即可
![](http://hiphotos.baidu.com/leafing/pic/item/a7a0073ba58a02dd15cecbd7.jpg)
哈哈，可以偷懒了
