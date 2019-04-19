# JTAG故障后，如何自己修复（教学贴） - xqhrs232的专栏 - CSDN博客
2015年11月26日 23:06:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4821
原文地址::[http://www.openedv.com/posts/list/107.htm](http://www.openedv.com/posts/list/107.htm)
相关网帖
1、**JLINK坏了不用怕，分享JLINK修复技巧**** ----[http://www.amobbs.com/thread-5464280-1-1.html](http://www.amobbs.com/thread-5464280-1-1.html)**
2、真正的J-Link固件修复-可升级!----[http://wenku.baidu.com/link?url=5yWbh7mVo3Ix0rbcHQXr0fTsPUR0bQrDscmeF5IZ1HDr8ftT7eYVoFn7jW46Rs9GiWYpFEDjg2Di6vxGCKDJUDJLKco4bgi3AUPsMWjlirq](http://wenku.baidu.com/link?url=5yWbh7mVo3Ix0rbcHQXr0fTsPUR0bQrDscmeF5IZ1HDr8ftT7eYVoFn7jW46Rs9GiWYpFEDjg2Di6vxGCKDJUDJLKco4bgi3AUPsMWjlirq)
3、[[原]如何自己动手jlink v8固件修复](http://m.blog.csdn.net/blog/ZacharyChiy/43404255#)----[http://m.blog.csdn.net/blog/ZacharyChiy/43404255](http://m.blog.csdn.net/blog/ZacharyChiy/43404255)
4、**j-link v8 固件修复----[http://www.amobbs.com/forum.php?mod=viewthread&tid=4427819](http://www.amobbs.com/forum.php?mod=viewthread&tid=4427819)**

  有些朋友在jlink V8发生问题后，以为就挂了。确实，因为主控芯片的程序，可能被错误的操作搞乱。从而使JTAG “挂了”，此时我们一般可以通过如下方法修复：
**JTAG V8 固件烧录指导**
# 1      JTAG V8固件烧录指导
JTAG 是使用过程中，如果内部固件意外损坏或丢失，请参考下面操作步骤说明，重新烧录JTAG固件。
## 1.1     1.安装固件烧录软件
²请在ATMEL官方网址下载AT91-ISP下载软件。
  软件下载地址：http://www.atmel.com/dyn/products/tools_card.asp?tool_id=3883
在打开的网页中，下载下图中红色框所示软件（附件已经有该软件了），如图1所示：
![](http://www.openedv.com/upload/2010/12/20/b3eec3f4524f7396c7749779ec060551_903.jpg)
图1
²安装下载好的Install AT91-ISP
 v1.13.exe软件
双击Install AT91-ISP v1.13.exe运行，选择默认设置，安装好以后，桌面上生成如下两个图标，如图2所示：
![](http://www.openedv.com/upload/2010/12/20/c8d2a336ce84ef3aed02f921db159c37_330.jpg)
图2
## 1.2      2.擦除芯片并进入编程模式
²打开JTAG 外壳，见到JTAG内部电路如图3所示：
![](http://www.openedv.com/upload/2010/12/20/f8b0202b4865e3758e452b95e2c06927_380.jpg)
图3
²使用USB线连接JTAG与PC机，以提供JTAG工作电源。
²短接图3中A的两个过孔约5秒。断开A位置的两个过孔的短接。然后拔掉JTAG与P
 C间的USB线（注意先后顺序）。
²短接图3中,B处的两个过孔。
²使用USB线连接JTAG与PC机，至少超过10秒后，拔掉USB线，停止给JTAG供电（说明：请确保此过程中，图3中B处一直处于可靠的端接状态）。
²断开图3中B处的短接。
## 1.3     3.更新固件
²双击桌面上的SAM-PROG v2.4图标，运行SAM-PROG
 v2.4烧录软件，按图4所示进行设置。
![](http://www.openedv.com/upload/2010/12/20/496ff9d85af6da288a59e9be51cac3f8_991.jpg)
²将JTAG V8通过USB线与PC机连接。此时，SAM-PROG
 v2.4软件中的Write Flash按钮将变为有效。（注意，先打开SAM-PROG v2.4再连接JTAG 与PC机）。
²点击Write Flash 按钮，烧录固件，待烧录完成后,Active
 Connection：将变为1。
²拔掉JTAG与PC机之间的USB线。
至此,JTAG V8的固件已经更新完毕，正常情况下，连接电脑与JTAG
 V8时，JTAG V的指示灯将闪烁，然后常绿。请参照JTAG V8用户手册说明，使用JTAG
 V8进行调试仿真。
       注:**在光盘的:E:\我的资料\ALIENTEK 开发板资料\其他资料\JTAG V8恢复   下有详细资料**
**附件1：**[JLINK
 V8恢复.rar (文件大小: 4 MB 下载次数:19907次) ![AttachmentIcon](http://www.openedv.com/templates/default/images/attachmentIcons/rar.gif)](http://www.openedv.com/posts/downloadAttach/141.htm;jsessionid=E60D657522CD5C6E7C4A2CC659383746)
