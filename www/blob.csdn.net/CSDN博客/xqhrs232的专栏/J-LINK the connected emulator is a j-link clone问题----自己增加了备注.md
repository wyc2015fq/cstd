# J-LINK the connected emulator is a j-link clone问题----自己增加了备注 - xqhrs232的专栏 - CSDN博客
2017年03月21日 22:04:07[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3258
原文地址::[http://blog.sina.com.cn/s/blog_8a30d1990102vo66.html](http://blog.sina.com.cn/s/blog_8a30d1990102vo66.html)
相关文章
1、[Jlink更新至V4.65d后，Jlink不能使用的问题解决办法](http://blog.csdn.net/lxc1014/article/details/8214057)----[http://blog.csdn.net/lxc1014/article/details/8214057](http://blog.csdn.net/lxc1014/article/details/8214057)
2、[the connected emulator is a j-link clone](http://blog.csdn.net/blue0432/article/details/11616837)----[http://blog.csdn.net/blue0432/article/details/11616837](http://blog.csdn.net/blue0432/article/details/11616837)
3、问题求解：仿真调试不能运用The connected emulator is a J-Link clone----[http://bbs.csdn.net/topics/390270806](http://bbs.csdn.net/topics/390270806)
4、关于JLINK固件丢失或升级固件后提示Clone的解决办法----[https://wenku.baidu.com/view/0402612469dc5022abea0089.html](https://wenku.baidu.com/view/0402612469dc5022abea0089.html)
5、[解决]山寨J-Link V8仿真器固件升级/破解Clone提示/USB设备无法识别----[http://blog.sina.com.cn/s/blog_6cda5b3d0102vjyz.html](http://blog.sina.com.cn/s/blog_6cda5b3d0102vjyz.html)

新装WIN8.1系统,安装MDK5.11A开发STM32F407，在设置J-LINK仿真时出现the
 connected emulator is a j-link clone问题，
![J-LINK <wbr>the <wbr>connected <wbr>emulator <wbr>is <wbr>a <wbr>j-link <wbr>clone问题](http://s11.sinaimg.cn/mw690/002wU06Bty6RNkcwaee9a&690)
然后MDK直接闪退，大概意思是链接的J-LINK是盗版，是非法的，本人J-LINK是2012年不到一百块钱淘来的，要知道一个正版要一千多大洋呢，相信大家和我都一样用盗版，当时的J-link固件对地版本的MDK，如MDK3.8是适用的，但对新版本存在盗版问题。
百般无奈各种查论坛，有大神说用SEGGER安装目录下的JLinkARM.dll替换掉MDK安装目录下的./ARM/Segger/JLinkARM.dll，试着替换，结果替换后MDK提示打开JL2CM3.dll出现问题，无奈换回去，然后又查到可以重刷J-LINK估计，于是拆开J-LINK壳子短接ERASE擦除固件，在短接TST后发现电脑压根不能识别J-LINK了，然后伤感半天后接朋友的J-LINK试着下载，也不行，然后继续伤感，并在网上订了一个新的，但需要几天才能来，实在忍不住了，逛论坛发现说32位XP或win7才可以重刷固件，于是装了VBOX虚拟机，然后装了虚拟XP,发现VBOX虚拟机的普通USB可以映射到虚拟机，但通用串行总线映射不进去（可能是我没掌握方法），无奈又装了VM虚拟机，再虚拟XP，成功映射了ATMEL这个J-link接口，然后找到一个网上大家都说可以用的J-LINK固件用AT91-ISP这个软件烧进去，很高兴，接着设置J-LINK下载，然后。。。the
 connected emulator is a j-link clone，泪奔![J-LINK <wbr>the <wbr>connected <wbr>emulator <wbr>is <wbr>a <wbr>j-link <wbr>clone问题](http://www.sinaimg.cn/uc/myshow/blog/misc/gif/E___6706EN00SIGG.gif)![J-LINK <wbr>the <wbr>connected <wbr>emulator <wbr>is <wbr>a <wbr>j-link <wbr>clone问题](http://www.sinaimg.cn/uc/myshow/blog/misc/gif/E___6706EN00SIGG.gif)
还不服气重来，用Winhex打开网上找到J-LINK V8.bin固件，将0xff00为起始地址的4Bytes修改为0xffffffff,（这个是固件SN，一定不要为原来的11111117
 或 805306163，MDK对此强制检验的）同时将0xff30后面的GDBFull对应改成GDBFULL，对应ASCII码为47
 44 42 46 55 4C 4C，目的是跳过GDBFull检查，然后在虚拟机再次烧入，再到win8.1设置J-LINK，直接成功，并且在MDK下升级J-LINK后同样没有问题，于是乎把朋友那个也修了一遍修好了。
在此提供Winhex软件（这个用setup要安装一下，否则会提示无法保存修改的文件），同时提供我已经改好的J-LINK
 V8.bin固件，还有AT91-ISP这个软件以及擦除和烧写J-LINK的PDF教程，希望大家遇到这样的问题不要像我一样走这么多弯路啦
下载修复包——百度网盘：http://pan.baidu.com/s/1ntA8prb
//=========================================================================================================
备注::
1>按上面的方法自己亲自测试了,确实是可行的,但也要注意操作的具体细节,这里贴个图做个记录!!!
![](https://img-blog.csdn.net/20170322083811372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHFocnMyMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
