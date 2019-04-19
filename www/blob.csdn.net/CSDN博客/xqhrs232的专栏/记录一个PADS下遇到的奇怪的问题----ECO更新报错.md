# 记录一个PADS下遇到的奇怪的问题----ECO更新报错 - xqhrs232的专栏 - CSDN博客
2015年03月22日 00:12:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1804
相关文章
1、**高手请进！使用POWERPCB时，遇到的怪情况！----http://www.baisi.net/thread-1355513-1-1.html**
**2、**请教各位PADS高手帮忙感激不尽：在logic里更改原理图ECO到PCB时出错----http://www.52rd.com/bbs/Archive_Thread.asp?SID=262687&TID=3
今天也遇到这样的奇怪问题！！！
ERROR: Wrong gate pin data at the line:
 RESET_N 
 21.0.U.AVDD5 
 22.0.U.XOSC_Q1 
 23.0.U.XOSC_Q2 
 24.0.U.AVDD3 
最后把器件的CAE DECAL跟PCB DECAL全部新建了一遍，再把整个器件类型新建了一个，然后去更新SCH，然后从SCH导出ASC文件交给PADS LAYOUT去导入，也就是把PCB都重新新建一遍，这样最后对应的SCH总算可以正确ECO更新到PCB文件里面去了。折腾了个老半天，试来试去，
总算把这个问题解决了。记录一下！！！
