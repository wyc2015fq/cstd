# NFC笔记——NFCIP-1协议命令集(NFCIP-1 Protocol Command Set) - xqhrs232的专栏 - CSDN博客
2015年04月14日 00:31:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：909
原文地址::[http://www.cnblogs.com/journeyonmyway/archive/2012/08/20/2647885.html](http://www.cnblogs.com/journeyonmyway/archive/2012/08/20/2647885.html)
相关文章
1、NFC协议NFCIP-1----[http://download.csdn.net/download/ldipper/1878267](http://download.csdn.net/download/ldipper/1878267)
2、ISO18092标准协议----[http://download.csdn.net/detail/yanweidie321/8181423](http://download.csdn.net/detail/yanweidie321/8181423)

下图是标准的P43, Table 19 — NFCIP-1 Protocol Command Set
![](http://pic002.cnblogs.com/images/2012/141575/2012082017231280.png)
如表可见，命令由2个Byte组成，称为 CMD0 和 CMD1，initiator 发送的命令会设定 CMD0 = D4，Target 回应的命令会设定 CMD0 = D5。
至于 CMD1，则顺序为 00 - 0B，估计是为了将来扩充命令方便。
