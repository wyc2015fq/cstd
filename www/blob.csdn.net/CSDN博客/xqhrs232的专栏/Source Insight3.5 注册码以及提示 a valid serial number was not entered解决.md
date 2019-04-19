# Source Insight3.5 注册码以及提示 a valid serial number was not entered解决 - xqhrs232的专栏 - CSDN博客
2014年05月14日 23:59:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：907
个人分类：[SI/Notepad++/EditPlus](https://blog.csdn.net/xqhrs232/article/category/906927)
原文地址::[http://blog.csdn.net/codectq/article/details/18132457](http://blog.csdn.net/codectq/article/details/18132457)
相关文章
1、SourceInsight最新版安装，出现an invalid source insight serial number was detected----[http://hi.baidu.com/pepsi360/item/2acca8a7cd2413248919d3a8](http://hi.baidu.com/pepsi360/item/2acca8a7cd2413248919d3a8)
 程序员大都习惯使用Source Insight工具来查看代码。正所谓“工欲善其事，必先利其器”。刚刚想查看代码的时候，忘记已经在这台电脑上安装了此软件，没有仔细查看便重新进行了安装，结果导致破坏了原来的破解版。导致启动之后提示 a valid serial number was not entered，然后闪退  根本无法使用。
![](https://img-blog.csdn.net/20140111093649109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY29kZWN0cQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       卸载之后重新安装也始终无济于事，好坏不说搞计算机久矣，所以还知道应该删除注册表信息，win+r运行regedit对注册表进行编辑，删除了source dynamits但是终究于事无补。最后在注册表中搜索“Source Insight”将所有的相关文件都删除了，之后重新安装提示输入注册码，才将这个问题解决。仅此小记，一儆后来！
另外，提供一组注册码。
SI3US-205035-36448 
SI3US-466908-65897 
SI3US-368932-59383 
SI3US-065458-30661 
SI3US-759512-70207
//=======================================================================================
备注::
1》我自己试了一下不行，不知道那个地方还没操作对？！

