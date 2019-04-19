# 关于PN532 读取二代证UUID____记录自己亲自实验成功！！！ - xqhrs232的专栏 - CSDN博客
2016年04月21日 09:50:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：8557
个人分类：[NFC技术](https://blog.csdn.net/xqhrs232/article/category/2906045)
原文地址::[http://www.amobbs.com/thread-5588921-1-1.html?_dsign=1fd689a6](http://www.amobbs.com/thread-5588921-1-1.html?_dsign=1fd689a6)
相关文章
1、视频: NFC（PN532）读二代证UID，读B卡----[http://v.youku.com/v_show/id_XNzk5NDcxMDgw.html](http://v.youku.com/v_show/id_XNzk5NDcxMDgw.html)
2、PN532使用教程，替代了ACR122----[http://jingyan.baidu.com/article/64d05a02418174de55f73b3b.html?qq-pf-to=pcqq.group](http://jingyan.baidu.com/article/64d05a02418174de55f73b3b.html?qq-pf-to=pcqq.group)
3、狂喜！pn532和手机终于可以通信啦！----[http://blog.sina.com.cn/s/blog_a2ae2da90101m85p.html](http://blog.sina.com.cn/s/blog_a2ae2da90101m85p.html)
4、**贴一个PN532 读取二代证 UID 的完整C程序----[http://www.amobbs.com/thread-5614403-1-1.html](http://www.amobbs.com/thread-5614403-1-1.html)**
5、射频识别技术漫谈(30)——PN512的寄存器设置----[http://blog.sina.com.cn/s/blog_9ed067ad0101fz1r.html](http://blog.sina.com.cn/s/blog_9ed067ad0101fz1r.html)
6、读取二代身份证号，做个记录----[https://blog.csdn.net/yytyu2017/article/details/74333629](https://blog.csdn.net/yytyu2017/article/details/74333629)
//================================================================================================================
备注::
1>整了一个晚上才出了点头绪，第2天一大早再起来持续折腾，总算功夫不负有心人，大脑完全朦朦中居然被我整成功了，这里做个记录好了！！！基于STM32+PN532 平台。
2>要看懂PN532执行指令后的反馈信息，这样你才能知道前进的方向！
3>还有一点就是要把PN532的使用手册读懂，要能够做到前后贯通，各个指令之间反复对照看，深入地看明白它！！！
