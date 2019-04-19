# PADS:怎样全局替换元件封装 - xqhrs232的专栏 - CSDN博客
2015年03月26日 23:27:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1602
原文地址::[http://blog.sina.com.cn/s/blog_63fadc0d0100imnw.html](http://blog.sina.com.cn/s/blog_63fadc0d0100imnw.html)

在工程中，由于种种原因，元件的封装可能会改变。这个改变对于PCB工程师来说是致命，特别是现在元件特多的那种封装，如果不知道方法，一个一个的换的话，可能会死人的![PADS:怎样全局替换元件封装](http://www.sinaimg.cn/uc/myshow/blog/misc/gif/E___6725ZHE1SIGG.gif)。
  今天无意中，在学习ECO菜单时，发现有种方便的方法。下面由于我来一一道来。
 1.进入ECO菜单下边，选取 Change Component
![PADS:怎样全局替换元件封装](http://s15.sinaimg.cn/middle/63fadc0dx869d874a45ae&690)
   右键---Find
   在Find By中选择Part Type
   Action 下选择Select
  Part Type 选择你的封装名称  在Ref下边，用Shift全选元件编号。如下图
![PADS:怎样全局替换元件封装](http://s11.sinaimg.cn/middle/63fadc0dx73dc8bd0f95a&690)
 2。再一次按右键 --Library Browser
![PADS:怎样全局替换元件封装](http://s12.sinaimg.cn/middle/63fadc0dx869d98fc7fdb&690)
  在Part Type下边找到你要替换的新元件封装。
  按下Replace。
3.出现一个对话框，就按下Yes就可以了。
