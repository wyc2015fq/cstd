# TI CC DEBUGGER优点和缺点分析及对策 - xqhrs232的专栏 - CSDN博客
2015年05月05日 14:50:54[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2104
原文地址::[http://blog.csdn.net/lxw55/article/details/44085465](http://blog.csdn.net/lxw55/article/details/44085465)
相关文章
1、CC_debug 官方原理图----[http://download.csdn.net/detail/u010447802/5298302](http://download.csdn.net/detail/u010447802/5298302)
2、CC debuger原理图----[http://wenku.baidu.com/link?url=yDNmd6iVeGPbM0EyJd5DfUH3SYLbXerG56l4IUnxntDKW6u2vo5QPNtvVp_CHscrcbqjOaA29Hs4AVidtbfHTKX6L0Nm2TUF1vqnbEGFQf_](http://wenku.baidu.com/link?url=yDNmd6iVeGPbM0EyJd5DfUH3SYLbXerG56l4IUnxntDKW6u2vo5QPNtvVp_CHscrcbqjOaA29Hs4AVidtbfHTKX6L0Nm2TUF1vqnbEGFQf_)
3、CC Debugger----[http://wenku.baidu.com/view/fa5f490852ea551810a68742.html](http://wenku.baidu.com/view/fa5f490852ea551810a68742.html)
4、TI-CC_Debugger多功能仿真器原理图----[http://wenku.baidu.com/link?url=40ltGe1ypQNqJailquGwIc2XCRSkplnNvJZtE_1u0Fi9wsbTXXZF2hgrDKHqvuwGrDNcMgCIcX4TeAESDvc_bjjZnoNBkRlVb3xvU2jHQuW](http://wenku.baidu.com/link?url=40ltGe1ypQNqJailquGwIc2XCRSkplnNvJZtE_1u0Fi9wsbTXXZF2hgrDKHqvuwGrDNcMgCIcX4TeAESDvc_bjjZnoNBkRlVb3xvU2jHQuW)
5、smartrf04EB原理图，zigbee调试工具，CCdebugger----[http://download.csdn.net/detail/zhaigf0441/4757234](http://download.csdn.net/detail/zhaigf0441/4757234)
6、CC Debugger固件下载简易步骤----[http://wenku.baidu.com/link?url=Pt5fZzUQMan1WN7GwlwL0Cmne9PIF_iPk-7CCgtgFeWoaTKCSliGESkzGhOgdfHNFIS2tf2qNb1HzetP2Jck34o8fyChPq8wpFNH8NLuEUq](http://wenku.baidu.com/link?url=Pt5fZzUQMan1WN7GwlwL0Cmne9PIF_iPk-7CCgtgFeWoaTKCSliGESkzGhOgdfHNFIS2tf2qNb1HzetP2Jck34o8fyChPq8wpFNH8NLuEUq)
TI CC DEBUGGER的优点和缺点，及自由蜂的对策
     相信大伙都使用过了it官方CC DEBUGGER以及其复制品，然而你是否知道其优点和缺点呢？笔者稍微做了些分析，供大家参考
      CC DEBUGGER相对于SmartRF04EB做了很多改善，主要在于：
一、4颗缓存芯片更换了型号，不仅是芯片的体积变小了，对于二次开发来说，pcb板面积可以设计小些，成本有所降低
二、支持芯片的类型大大的增加了。支持CC111x/CC243x/CC253x/CC251x/cc2540;然而，CC DEBUGGER所能支持的cc系列芯片却有这么多：
1)、可以编程和仿真的芯片有：CC1110, CC1111;    CC2430,
 CC2431;   CC2510, CC2511;  *CC2530, CC2531, CC2533*; **CC2540, CC2541;**CC2543,
 CC2544, CC2545
2)、可以通过SmartRF Studio控制的芯片有：CC1120, CC1121, CC1125, CC1175;   CC1100,
 CC1101,CC110L, CC113L, CC115L;  CC2500,CC2520
3)、可以用来编程PurePath无线芯片CC8520, CC8521;   CC8530,
 CC8531
![微笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/smile.gif)看得是不是有点眼花呢？？？
三、**支持的工具软件还有这些呢：**SmartRF
 Studio，SmartRF Flash Programmer，IEEE Address Programmer，PacketSniffer，PurePathWireless，IAR for 8051多个版本（包含最新z-stack协议栈所使用的8.10）等。
当然支持WIN7_64bit也是杠杠的，这是SmartRF04EB所不能及之处（支持操作系统：WINXP,WIN7_32bit,WIN7_64bit）
      好吧，芯片和软件的主持度是提高了不少，但是硬件方面呢？却是没什么改善，缺陷依然存在！！！
![](https://img-blog.csdn.net/20150305175353262?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHh3NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
都知道这个脚位定义图吧，
       在使用过程中有没发觉pin2和pin9是分开着而带来不便呢？！
       是不是有时为了要给目标板供电而不得不将pin2和pin9飞线连接起来呢？飞线过程发生错误而造成烧坏芯片的事情就时有发生了。
      有时候，要取消目标板的供电有得拆去飞线，麻烦，消耗开发和生产时间啊
另外的问题当然就是ubs的接口了，还是使用mini 5pin USB接口，那么，如果USB丢失了，怎办呢，凉拌，等着再买一根了，
       综上这些问题，缺陷是不可避免的
       有这么一款CC DEBUGGER却是解决了这些问题，不妨看下图的功能介绍了
![](https://img-blog.csdn.net/20150305180338015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHh3NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
     支持式USB接口：支持USB即插即用，丢失USB延长线也无压力，照样使用。
**target PWR selection：就是将pin2和pin9用跳针和跳帽连接起来，电源供电方式分不分开，有你来决定哦！**
**---------http://item.taobao.com/item.htm?id=42980475444---------------------------**
--------http://item.taobao.com/item.htm?id=43358073251--------------------
