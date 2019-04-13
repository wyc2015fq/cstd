
# 双系统下Linux（ubuntu）不能进入windows的NTFS分区之挂载错误问题解决 - 郭云飞的专栏 - CSDN博客


2016年07月18日 20:14:49[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：193个人分类：[计算机应用																](https://blog.csdn.net/guoyunfei20/article/category/1848145)


安装windows8.1和unbuntu14 后，可以在ubuntu下访问window下的任何磁盘分区，用了一段时间后发现不能访问了，很少让人蛋疼啊，没办法网上自己找到了解决办法，亲测可用，分享一下：
**问题：**
安装完ubuntu后，不能访问win8里面的分区，访问会提示不能访问XXX ，Error mounting /dev/sda8 at /media/my/XXX: Command-line `mount -t "ntfs" -o
**解决办法：**
1、打开终端：如果没有安装ntfs-3g就要安装：sudo apt-get install ntfs-3g
2、修复挂载错误的相应的分区如提示中的/dev/sda8，输入：
sudo ntfsfix /dev/sda8
回车就可以了。
![图片说明](http://img.ask.csdn.net/upload/201506/14/1434263754_470307.png)


