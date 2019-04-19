# Linux 打开默认串口支持数量超过4个的方法 - xqhrs232的专栏 - CSDN博客
2016年03月24日 18:09:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1551
原文地址：[http://blog.csdn.net/sidely/article/details/41959685](http://blog.csdn.net/sidely/article/details/41959685)
相关文章
1、linux支持多串口的方法----[http://www.360doc.com/content/12/0606/17/7585456_216431032.](http://www.360doc.com/content/12/0606/17/7585456_216431032.shtml)
2、LINUX多串口的支持----[http://blog.sina.com.cn/s/blog_475cb6780100rtox.html](http://blog.sina.com.cn/s/blog_475cb6780100rtox.html)
 ① 这一步不做也可以，首先确认一下编译参数，请执行：
  cat /boot/config-`uname -r` | grep 8250
 注意观察一下屏幕输出
② vi 这个文件 /boot/grub/grub.conf 
然后找到有 kernel 开头的那一行， 在末尾加空格，还有加上下面几个字
 8250.nr_uarts=16 
 ③ 这段参数里的 16 是指打开16个串口，但不一定能和实际硬件对的上的， 请重新启动 Linux ，查看 /dev 目录下面， 数一下 ttyS×× 的数量
 ------------------------------
   以上是为了解决在Linux 下安装多串口卡后， 突破Linux 内核参数限制默认只能打开4个串口用的。
