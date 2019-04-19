# 可修改UID的白卡，请问为什么一般读写器不能修改这种卡的UID，必须要特定的读写器才能修改？ - xqhrs232的专栏 - CSDN博客
2016年05月12日 22:54:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：14088
个人分类：[NFC技术](https://blog.csdn.net/xqhrs232/article/category/2906045)
原文地址::[http://www.zhihu.com/question/36918910/answer/86410655](http://www.zhihu.com/question/36918910/answer/86410655)
相关文章
1、[[C51]](http://bbs.mydigit.cn/thread.php?fid=108&type=216)RC522基站能否写UID卡0扇区0块----[http://bbs.mydigit.cn/read.php?tid=1392135&page=1](http://bbs.mydigit.cn/read.php?tid=1392135&page=1)
2、
# 关于重写UID卡0扇区0块问题----[http://tieba.baidu.com/p/3542848189](http://tieba.baidu.com/p/3542848189)
作者：wan chang
链接：http://www.zhihu.com/question/36918910/answer/86410655
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
可克隆卡写0扇区有特殊指令 下面代码以测
下面摘自互联网
Sent bits: 26 (7 bits) //尋卡 0x26 / 0x52 都可以
Received bits: 04 00
Sent bits: 93 20 //防衝撞
Received bits: [01 23 45 67 00](tel:012023204520672000)
Sent bits: [93 70 01 23 45 67 00](tel:93207020012023204520672000) d0 6f //選卡
Received bits: 08 b6 dd （SAK）
（不可以認證密鑰，不然後門打不開）
好，重點來了，
Sent bits: 50 00 57 cd //休眠，50 00 就是 PcdHalt()
Sent bits: 40 (7 bits) （特殊指令）//開後門第一條指令，要設定 BitFramingReg 使傳送 7 個位元，必須要 7 個
eceived bits: a (4 bits)
Sent bits: 43 （特殊指令）//開後門第二條指令
Received bits: 0a
Sent bits: a0 00 5f b1 //正常的寫區塊第一次交握
Received bits: 0a
Sent bits: 00 dc 44 20 b8 [08 04 00 46 59 25 58](tel:08200420002046205920252058) 49 10 23 02 c0 10 //正常的寫 block 0 資料
Received bits: 0a
重點就是要：
1. 不可以進行 3 Pass Authenticaiton
2. 發 PcdHalt()
3. 發 0x40 in 7-bit
4. 發 0x43
//================================================================================================================
备注::
1>听同行说在PN532下发上面特殊的命令就可以修改UID卡的0块，但试了不成功，这里也记录一下吧！
