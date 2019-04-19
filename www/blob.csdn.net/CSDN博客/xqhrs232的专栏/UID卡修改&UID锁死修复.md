# UID卡修改&UID锁死修复 - xqhrs232的专栏 - CSDN博客
2016年05月08日 17:32:19[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：10204
个人分类：[NFC技术](https://blog.csdn.net/xqhrs232/article/category/2906045)
原文地址::[http://www.hackdig.com/?10/hack-15044.htm](http://www.hackdig.com/?10/hack-15044.htm)
相关文章
1、论如何优雅地蹭饭：克隆篡改公司饭卡（M1卡）----[http://www.freebuf.com/articles/wireless/50123.html](http://www.freebuf.com/articles/wireless/50123.html)
2、什么是UID卡，它与普通卡有什么不同？----[https://club.1688.com/article/47826119.html](https://club.1688.com/article/47826119.html)
3、IC UID 卡克隆全套----  [http://download.csdn.net/detail/stefan_yang/7974849](http://download.csdn.net/detail/stefan_yang/7974849)

好久没发RFID类文章，最近有小伙伴问到UID卡的问题，在这里就写一写吧。
首先是UID修改的问题，只要卡是UID卡，就都可以修改UID，首先读卡器连接电脑，卡片放到读卡器上。
![IMG_20141010_221639](http://cdn.bobylive.com/wp-upload/IMG_20141010_221639.jpg)
然后我们要用一个工具，[UID207](http://s1.boby.im/other/UID207.exe)。打开UID207.exe，点Initialize，初始化。
![2014-10-10_222718](http://cdn.bobylive.com/wp-upload/2014-10-10_222718.png)
然后点Connect连接。
![2014-10-10_222814](http://cdn.bobylive.com/wp-upload/2014-10-10_222814.png)
然后可以看到连接成功ACR122U了，点Read就可以读取卡片的0扇区0段了。
![2014-10-10_222929](http://cdn.bobylive.com/wp-upload/2014-10-10_222929.png)
Mifare卡的结构是0扇区0段前4字节就是UID，第5字节是UID的效验位，所以，0786ACEF就是这张卡的UID，C2就是效验位。
![2014-10-10_223119](http://cdn.bobylive.com/wp-upload/2014-10-10_223119.png)
下面我们要用[XOR效验与算KEY程序](http://s1.boby.im/other/XOR&KEY.exe)来计算UID，我们算算1234ABCD这个UID吧。
![2014-10-10_224251](http://cdn.bobylive.com/wp-upload/2014-10-10_224251.png)
然后把计算好的UID替换掉UID207中前10位，点Write就可以写入。
![2014-10-10_224356](http://cdn.bobylive.com/wp-upload/2014-10-10_224356.png)
写入成功， ReConnect然后再Read就可以看到新的UID了。
![2014-10-10_224455](http://cdn.bobylive.com/wp-upload/2014-10-10_224455.png)
修改成功。
![2014-10-10_224544](http://cdn.bobylive.com/wp-upload/2014-10-10_224544.png)
接下来看看如何修复被锁死的UID卡。我先拿一张UID卡然后锁死它，并且在卡片上做好标记。
![IMG_20141010_221015](http://cdn.bobylive.com/wp-upload/IMG_20141010_221015.jpg)
![Screenshot_2014-10-10-22-07-07](http://cdn.bobylive.com/wp-upload/Screenshot_2014-10-10-22-07-07.png)
接下来我们先放一张好的UID卡在读卡器上！
![IMG_20141010_221639](http://cdn.bobylive.com/wp-upload/IMG_20141010_221639.jpg)
用UID207先Read一次，然后选择一个[空白的dump文件](http://s1.boby.im/other/blank.dump)。
![2014-10-10_225411](http://cdn.bobylive.com/wp-upload/2014-10-10_225411.png)
接下来这步很重要！不要拿走你的正常UID卡，然后把锁死的UID卡放在正常UID卡上面。然后点Copy Card开始写入。
![IMG_20141010_225751](http://cdn.bobylive.com/wp-upload/IMG_20141010_225751.jpg)
![2014-10-10_225917](http://cdn.bobylive.com/wp-upload/2014-10-10_225917.png)
![2014-10-10_225953](http://cdn.bobylive.com/wp-upload/2014-10-10_225953.png)
然后坏卡就被修复了。两张卡的内容完全一样，UID卡可以多张这样复制。
