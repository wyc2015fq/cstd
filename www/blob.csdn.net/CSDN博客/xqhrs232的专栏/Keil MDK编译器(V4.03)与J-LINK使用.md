# Keil MDK编译器(V4.03)与J-LINK使用 - xqhrs232的专栏 - CSDN博客
2012年12月15日 23:55:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：813
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://blog.csdn.net/zhzht19861011/article/details/5924218](http://blog.csdn.net/zhzht19861011/article/details/5924218)
前几天进手了一个J-LINK，因为H-JTAG毕竟对MDK支持的不是太完美，比如用keil mdk上面的下载按钮烧录程序，不是直接就能烧录进去，而是弹出H-Flash软件，再手工选择文件烧录；而且用H_JTAG单步调试还容易莫名其妙的跑飞。好在J-LINK不是太贵（我是说国内仿制的），mdk又能比较完美的支持J-link，今天就将我使用mdk和J-link的情况做一下记录。
       我把MDK升级到了V4.10，之前使用的V4.01，当然这个V4.10也不是最新版，最新版应该到了V4.12。
       首先需要安装mdk编译器，可以到[http://www.embedinfo.com/down-class.asp?lx=big&anid=51](http://www.embedinfo.com/down-class.asp?lx=big&anid=51)上下载mdk编译器；其次是需要J-link的驱动，可以到[http://www.segger.com/cms/jlink-software.html](http://www.segger.com/cms/jlink-software.html)上下载最新驱动。
       安装上面两个软件后，准备工作完成。
**第一：使用keil界面下的“load”按钮（![](http://hi.csdn.net/attachment/201010/6/0_1286368062ee88.gif)）下载程序。**
       a.首先将J-link正确的连接电脑和开发板，给开发板上电；然后打开keil mdk，点击Options for target‘target 1’...快捷图标，如下图所示：
![](http://hi.csdn.net/attachment/201010/6/0_1286366506Yz6n.gif)
       b.弹出Options for target‘target 1’...对话框，如下图所示：
![](http://hi.csdn.net/attachment/201010/6/0_1286366765HYAO.gif)
1.选中Utilities标签。
2.选中Use Target Driver for Flash Programming。
3.在下拉列表中选中J-LINK/J-TRACE。
4.点击Settings，设置J-link烧录选项，如下图所示：
![](http://hi.csdn.net/attachment/201010/6/0_1286367162YCyy.gif)
        c. 其他设置可按照自己情况设置，点击“Add”按钮，弹出如下添加Flash界面：
![](http://hi.csdn.net/attachment/201010/6/0_12863675053f32.gif)
1.这里我选择的是LPC2114，可以根据自己的芯片选择。
2.单击“Add”。
        d. 到此，设置基本完毕，一路点击“ok”，编译程序无误后，点击“Load”快捷图标，如下图所示：
![](http://hi.csdn.net/attachment/201010/6/0_1286367916b7py.gif)
可以看到J-link的指示灯一阵闪烁，很快就可以下载完毕。
**第二：使用J-link单步调试。**
mdk下使用J-link单步调试比较容易。
      a. 重复上一部分的步骤a，打开Options for target‘target 1’...对话框，如下图所示：
![](http://hi.csdn.net/attachment/201010/6/0_1286368782Zm8k.gif)
        b. 如图执行1,2,3,4步骤，点击Setting后，弹出如下所示的J-link设备设置选项对话框：
![](http://hi.csdn.net/attachment/201010/6/0_1286368905ZEgt.gif)
如果没有其他要求默认即可，一路点击OK退出设置。
      c. 程序编译无误后，先Flash-Download，下载程序，再按下Ctrl+F5(或快捷图标![](http://hi.csdn.net/attachment/201010/6/0_1286369164qrkk.gif)) 进入调试界面,具体调试步骤不再详述.
**第三:使用J-Flash ARM烧录程序**
安装J-link的驱动之后,在开始-程序(或者是'所有程序')-SEGGER-J-link ARM Vx.xx下,点击J-Flash ARM图标,就可以用这个软件方便的烧录程序,这个软件类似于H-Flash.
      a. 打开J-Flash ARM后,首先点击File-Open Project...,打开默认的flash器件选择界面,从中选择一款合适你自己的.可能这个里面并没有合适你自己的,比如lpc2114就没有,这个时候,点击File-New Projec,再点击Options-Project settings...弹出如下所示的工程设置对话框:
![](http://hi.csdn.net/attachment/201010/6/0_1286370325AJAN.gif)
按照图中的1,2,3设置,再点击CPU选项卡,如下所示:
![](http://hi.csdn.net/attachment/201010/6/0_1286370531h2TP.gif)
按照图中1,2,3设置,第3步可以根据自己的芯片选择,这里我选择的是lpc2114。
其他选项卡可以默认,也可以根据自己需要进行设置.
      b. 一路点击ok退出工程设置,这时候可以选择File-Save Projec保存这次的设置,方便下一次使用.
      c. 点击File-Open data file...选择要烧录的可执行文件,可支持hex或bin文件.
      d.选择烧录文件后,点击Target-connect,链接一下硬件是否通.
      e. 硬件链接上之后,按F3擦出芯片,F5程序烧录.
按下F5后,会弹出是否设置校验的提示对话框,为安全起见,选择后yes.烧录成功后会弹出对话框进行提示.
PS: keil mdk V4.01以上版本以及J-link驱动都是支持win 7的.
