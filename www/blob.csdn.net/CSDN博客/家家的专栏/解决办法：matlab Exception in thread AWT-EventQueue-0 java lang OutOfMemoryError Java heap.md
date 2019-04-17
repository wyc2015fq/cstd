# 解决办法：matlab Exception in thread "AWT-EventQueue-0" java.lang.OutOfMemoryError: Java heap - 家家的专栏 - CSDN博客





2015年07月30日 10:34:07[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：12098
个人分类：[数学 Matlab 基本知识](https://blog.csdn.net/yihaizhiyan/article/category/749753)









好几天没用matlab。。。

竟然有问题了，不停的弹出。。。

Exception in thread "AWT-EventQueue-0" java.lang.OutOfMemoryError: Java heap








解决办法：

在C:\Users\$username$\AppData\Roaming\MathWorks\MATLAB\R2013a目录下，找到matlab.prf文件。。。

在最后一行。

添加 

JavaMemHeapMax=I190000





重启matlab，就okay了。




参考网址：http://www.mathworks.com/matlabcentral/answers/92813-how-do-i-increase-the-heap-space-for-the-java-vm-in-matlab-6-0-r12-and-later-versions



