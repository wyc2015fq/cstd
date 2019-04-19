# qt creator修改程序编码（解决中文乱码问题）的方法 - xqhrs232的专栏 - CSDN博客
2017年08月07日 18:13:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1416
原文地址::[http://blog.csdn.net/lyc_daniel/article/details/13772485](http://blog.csdn.net/lyc_daniel/article/details/13772485)
qt creator修改程序编码（解决中文乱码问题）的方法
qt creator修改程序编码的功能有几处。
1.edit - select encoding
![](https://img-blog.csdn.net/20131031144738796)
选择载入（显示）编码和储存编码，其中GB2312和UTF-8是常用编码方式。
![](https://img-blog.csdn.net/20131031144948312)
这种方法主要是修改已经建立的程序文件。
2.tools - options - text editor - behavior - file encodings
![](https://img-blog.csdn.net/20131031145127812)
这主要针对新建的程序文件和工程运行时的编码情况。
3.在main()函数内的添加代码，利用QTextCodec来进行设置。
**[cpp]**[view
 plain](http://blog.csdn.net/lyc_daniel/article/details/13772485#)[copy](http://blog.csdn.net/lyc_daniel/article/details/13772485#)
- // 以下部分解决中文乱码
-    QTextCodec *codec = QTextCodec::codecForName("utf8"); //GBK gbk
-    QTextCodec::setCodecForTr(codec);  
-    QTextCodec::setCodecForLocale(codec);  
-    QTextCodec::setCodecForCStrings(codec);  
- // 以上部分解决中文乱码
这种方式设置程序对中文名称、界面中文等编码设置。
可参考：[Linux下QT编程中文显示乱码解决](http://blog.csdn.net/slj_win/article/details/6801118)
