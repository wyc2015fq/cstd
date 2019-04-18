# Vs2010无法打开文件“Kernel32.lib”、无法打开“libcpmt.lib”"msvcprt.lib" - wishchinYang的专栏 - CSDN博客
2015年04月02日 17:42:25[wishchin](https://me.csdn.net/wishchin)阅读数：4645
1.对于无法打开“Kernel”问题，即使复制lib文件到目录，仍然会出现最后的错误;
  原因：WindowsSdk 安装失败！
  方法：重装 microsoft SDK6.0 ，再在VS中添加包含目录;
2.无法打开“libcpmt.lib”"msvcprt.lib"
解决方法： 在VC的库包含目录里面，库目录 lib 添加：$(LibraryPath)
3.硬盘有那么多的COde，忽然间坏掉了！原因不明，所有的东西都蛋疼的随风而去了....
   所以，就弹了一会琴.............................
