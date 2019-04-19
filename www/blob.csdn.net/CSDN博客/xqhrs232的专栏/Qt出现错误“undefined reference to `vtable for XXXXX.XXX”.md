# Qt出现错误“undefined reference to `vtable for XXXXX.XXX” - xqhrs232的专栏 - CSDN博客
2018年06月07日 11:23:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：137
原文地址::[https://blog.csdn.net/seanyxie/article/details/6041380](https://blog.csdn.net/seanyxie/article/details/6041380)
如果新建一个类，继承了某个类，在这个类的头文件中式没有 Q_OBJECT宏声明的，如果想使用signal-slot信号，就需要添加Q_OBJECT宏，但是添加了这个宏后，就可能会出现这种错误，“undefined reference to vtable for LcdNumber”的编译错误
这是因为在Makefile里面没有这个类并没有Q_OBJECT信息，所以在执行Makefile时候也就没有moc xxx.h这条命令，最终导致连接失败。
知道了这个原因后，解决方法就很简答了，
重新运行qmake，生成Makefile，然后在编译就OK了
