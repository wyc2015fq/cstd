# smartdrv.exe的使用及简单说明和相关程序下载 - ljx0305的专栏 - CSDN博客
2008年07月04日 14:33:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：6252
smartdrv.exe是一个磁盘缓存实用程序
加载这个程序可以将内存设置为硬盘的高速缓存
一般在windows98的安装文件夹下面有这个程序
DOS下安装2000或者XP执行完winnt后，会看到一个复制文件的界面，这时安装程序正在将I386里的文件重新复制到C盘的临时文件夹里（$WINNT_NT$.~BT $WINNT_NT$.~LB等等），并创建一些引导文件和配置文件。安装速度的快慢区别其实就在这个复制文件的过程里。不加载Smartdrv.exe，由于硬盘的高速缓存很小，磁盘的读写速度势必会很慢。如果加载Smartdrv.exe，硬盘的高速缓存会增加好几十倍，这样读写速度会快很多，复制文件的速度自然就快了，安装系统的时间也就大大减少了。
Smartdrv.exe的用法（之前要在Config.sys里加载Himem.sys）。
smartdrv /x 　执行并驻留内存，开辟磁盘加速缓存区，并屏蔽所有驱动器 缓存，是较常用的方法（一般用这个命令就够了） 
  smartdrv /c 将缓存内的信息都写入硬盘 
  smartdrv /e: 数字， 设置一次移动的信息量 
  smartdrv /b: 数字， 设置预先读取的缓冲区大小
  smartdrv 32768 ，32768 表示拿出32MB的内存作为磁盘的缓存
[http://www.qzcc.com/inhtm/smartdrv.htm](http://www.qzcc.com/inhtm/smartdrv.htm)
[下载地址:http://www.qzcc.com/qzccfiles/smartdrv.exe](http://www.qzcc.com/qzccfiles/smartdrv.exe)
