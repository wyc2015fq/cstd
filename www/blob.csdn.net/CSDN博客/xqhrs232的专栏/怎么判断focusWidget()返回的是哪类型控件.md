# 怎么判断focusWidget()返回的是哪类型控件 - xqhrs232的专栏 - CSDN博客
2017年11月08日 16:53:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：161
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://blog.csdn.net/j8121/article/details/51931457](http://blog.csdn.net/j8121/article/details/51931457)
在QT开发过程中经常会遇到这样的问题，根据鼠标焦点的定位到哪个控件上？
如何判断focusWidget()返回的是哪类型控件
如何判断focusWidget()返回的是哪类型控件
比如：有QLineEdit, QSpinBox等控件，通过focusWidget()返回值如何判断是哪个控件。
解决方案：QWidget->metaObject()->className()你就可以看到了。

