# MIFARE 1卡读写控制 - xqhrs232的专栏 - CSDN博客
2016年02月28日 22:24:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3803
原文地址::[http://wenku.baidu.com/link?url=2HThgd75DWH083Z7hdzUjsWceLT524fShl_KOivgBcAsjdtR-6gcmi_GZbP0YIgJYyh-r86e-ZA7hqTMTWiUFZj6S2Uj8XFN9NShKznfpA7](http://wenku.baidu.com/link?url=2HThgd75DWH083Z7hdzUjsWceLT524fShl_KOivgBcAsjdtR-6gcmi_GZbP0YIgJYyh-r86e-ZA7hqTMTWiUFZj6S2Uj8XFN9NShKznfpA7)
相关文章
1、3.Mifare指令集----[http://wenku.baidu.com/link?url=ypr8hVqGOpB7R46iCvNO_gIEXkEi-QSb365y6Y0Y2HT3_WbuPNAyyA1rJJhSb96a4d5FvkaZMp_Q3lmmTCa6jyp16o86lHVTwdCKL8ZUD7a](http://wenku.baidu.com/link?url=ypr8hVqGOpB7R46iCvNO_gIEXkEi-QSb365y6Y0Y2HT3_WbuPNAyyA1rJJhSb96a4d5FvkaZMp_Q3lmmTCa6jyp16o86lHVTwdCKL8ZUD7a)
2、 [MIFARE系列1《MIFARE简介》](http://blog.csdn.net/yxstars/article/details/38079827)----[http://blog.csdn.net/yxstars/article/details/38079827](http://blog.csdn.net/yxstars/article/details/38079827)
3、Mifare卡----[http://blog.sina.com.cn/s/blog_683b6e4f0102vtfm.html](http://blog.sina.com.cn/s/blog_683b6e4f0102vtfm.html)
4、mifare s50芯片说明----[http://www.docin.com/p-861138786.html](http://www.docin.com/p-861138786.html)
5、Mifare ONE命令说明----[http://write.blog.csdn.net/postedit/50762185](http://write.blog.csdn.net/postedit/50762185)
6、Mifare1技术说明(M1卡说明文档)----[http://wenku.baidu.com/link?url=B-zeXQPI9LHfXVlDAjmfVvxcRtEy2fKUE2cZCMmfgEs6598EWi_EnJCMrtVkl6u-Os-bXLDHxz9tCNlZ_OH-6Rn8FbVSwGVMm5Y4NUU1d_7](http://wenku.baidu.com/link?url=B-zeXQPI9LHfXVlDAjmfVvxcRtEy2fKUE2cZCMmfgEs6598EWi_EnJCMrtVkl6u-Os-bXLDHxz9tCNlZ_OH-6Rn8FbVSwGVMm5Y4NUU1d_7)
7、M1卡介绍----[http://wenku.baidu.com/view/62cccb122e3f5727a4e9625c.html](http://wenku.baidu.com/view/62cccb122e3f5727a4e9625c.html)
![](https://img-blog.csdn.net/20160229153600296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160229153817642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
//=================================================================================================================================
备注::
1>今天调Mifare的读写调了老半天，读很快就调好了，可写死活就是不行，开始还以为是Mifare卡的每个扇区的第3块控制数据限制了Mifare卡其它块区的写，也一直想不明白为什么，读跟写其实没什么区别的(同样的级别，同样的权限)，为什么读没问题，写有问题呢？原来写要分成2次操作！！！都没注意到这个小细节！！！这里记录一下！！！



