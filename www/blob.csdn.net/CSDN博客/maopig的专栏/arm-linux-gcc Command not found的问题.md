# arm-linux-gcc:Command not found的问题 - maopig的专栏 - CSDN博客
2017年01月19日 20:32:18[maopig](https://me.csdn.net/maopig)阅读数：861
标签： [ubuntu](http://www.csdn.net/tag/ubuntu)[linux](http://www.csdn.net/tag/linux)
2015-05-15 10:47
680人阅读[评论](http://blog.csdn.net/u013256622/article/details/45741731#comments)(0)收藏[举报](http://blog.csdn.net/u013256622/article/details/45741731#report)
![](http://static.blog.csdn.net/images/category_icon.jpg)分类：
Ubuntu*（23）*![](http://static.blog.csdn.net/images/arrow_triangle%20_down.jpg)/etc/profile gcc*（9）*![](http://static.blog.csdn.net/images/arrow_triangle%20_down.jpg)ARM汇编指令*（4）*![](http://static.blog.csdn.net/images/arrow_triangle%20_down.jpg)
折腾了一天，终于搞定了。
ubuntu没有root权限，刚开始用碰到很多麻烦，查了好多资料，终于把arm-[Linux](http://lib.csdn.net/base/linux)-gcc:Command not found 的问题解决了。
问题：
sudo tar jxvfcross-2.95.3.tar.bz2
exportPATH=$PATH:/usr/local/arm/2.95.3/bin
使用arm-linux-gcc
–v检查交叉编译器安装成功
tar jxvfkernel.tar.bz2
解压之后生成kernel目录
sudo make clean
sudo makemenuconfig
sudo make zImage
提示错误：
arm-linux-gcc: Command notfound
PATH里有/usr/local/arm/2.95.3/bin，/usr/local/arm/2.95.3/bin/下有arm-linux-gcc文件，但是make的时候，就是找不到arm-linux-gcc
原因：
exportPATH=$PATH:/usr/local/arm/2.95.3/bin
是设置当前用户的PATH，而sudo执行make的时候，使用的是超级用户权限，那也就使用了超级用户的PATH（但是这个PATH里，并没有/usr/local/arm/3.4.1/bin）
解决方法：
先打开一个超级用户权限的shell：
sudo –s
在当前shell下，设置环境变量：
exportPATH=$PATH:/usr/local/arm/2.95.3/bin
再进入到kernel目录，make zImage，就可以找到arm-linux-gcc了
转载地址：http://blog.sina.com.cn/s/blog_4670451c0100e1uw.html
