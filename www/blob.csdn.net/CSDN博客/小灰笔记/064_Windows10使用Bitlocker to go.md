# 064_Windows10使用Bitlocker to go - 小灰笔记 - CSDN博客





2019年01月19日 18:47:05[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：76








         看到这个功能的时候先去网上搜索查看了解了一下，Bitlocker to go说起来不是新功能了，但是我一直没有用过。

         从网络资料了解，这其实是一个加密的功能。这让我多少考虑到了GNU PG的功能，看起来以后后期别人再跟我各种吹嘘类似多功能的时候其实可以把这个基础功能扔给他。而且，这个功能的使用不限于简单的U盘，还有电脑本身的磁盘。

         U盘比较简单，先拿U盘做一个测试。

![](https://img-blog.csdnimg.cn/20190119184653935.png)

         鼠标右键点击U盘，选择启用BitLocker。

![](https://img-blog.csdnimg.cn/20190119184653935.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         比较简单的方式可以使用密码，那就选择密码。但是设置密码的时候需要达到一定长度，密码设置之后会提示做一个备份。

![](https://img-blog.csdnimg.cn/20190119184653957.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

继续

![](https://img-blog.csdnimg.cn/20190119184653968.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         继续向下，会提示加密的过程。

![](https://img-blog.csdnimg.cn/20190119184653994.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

开始加密，加密完的提示如下：

![](https://img-blog.csdnimg.cn/2019011918465427.png)

         这样，后期使用U盘U盘是被加锁的，需要密码，同时带着一个锁的图标提示。使用的时候，需要输入密码：

![](https://img-blog.csdnimg.cn/20190119184653994.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         解锁之后就可以照常使用。这个过程也是可逆的，可以把这个加密功能去掉。

![](https://img-blog.csdnimg.cn/201901191846543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)



