# PADS覆铜地线为什么不能连在一起呢? - xqhrs232的专栏 - CSDN博客
2016年03月20日 09:14:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4571
原文地址::[http://zhidao.baidu.com/link?url=YgLOYcr30B-DCXrrp10TxBw8Mp1VW8itWzqQ18RsK_sqN4x7-_TtWCq7WeuJi8HnFLIv3DOO5B3dzvbd2tfw3L65pKdEqBoPtovg9gH7HNS](http://zhidao.baidu.com/link?url=YgLOYcr30B-DCXrrp10TxBw8Mp1VW8itWzqQ18RsK_sqN4x7-_TtWCq7WeuJi8HnFLIv3DOO5B3dzvbd2tfw3L65pKdEqBoPtovg9gH7HNS)
相关文章
1、**PADS中GND管脚覆铜不上，求教----[http://bbs.21ic.com/icview-627352-1-1.html](http://bbs.21ic.com/icview-627352-1-1.html)**
2、关于PADS Layout中焊盘与相同网络覆铜无法连接的解决办法----[http://blog.sina.com.cn/s/blog_a85e142101010c9b.html](http://blog.sina.com.cn/s/blog_a85e142101010c9b.html)
3、PADS Layout中焊盘与相同网络覆铜无法连接----[http://bbs.elecfans.com/forum.php?mod=viewthread&tid=467376](http://bbs.elecfans.com/forum.php?mod=viewthread&tid=467376)
4、PADS覆铜出现这个问题怎么解决啊 ----[http://bbs.ntpcb.com/read.php?tid-26010-page-e.html](http://bbs.ntpcb.com/read.php?tid-26010-page-e.html)
![](https://img-blog.csdn.net/20160320091318947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如上图IC的地线不能与覆铜地线连接起来，是什么原因导致的呢？
![](https://img-blog.csdn.net/20160320091348994?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
//=============================================================================================
备注::
1>你这个问题这么解决，Preferences->Thermals-》Pad
 Shape中的四个都选为Flood Over，试试

