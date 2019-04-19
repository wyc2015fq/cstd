# 在VS下使用QDebug打印调试信息 - xqhrs232的专栏 - CSDN博客
2018年09月05日 22:33:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：58
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[https://blog.csdn.net/ussr561/article/details/60335275](https://blog.csdn.net/ussr561/article/details/60335275)
```
#include <QDebug>
...
qDebug() << "Brush:" << myQBrush << "Other value:" << i;
```
- 1
- 2
- 3
VS进入 Project Properties > Linker > System : 设置 SubSystem to “SUBSYSTEM:CONSOLE”，然后你在VS下运行你的Qt程序时就会发现多了一个cmd的窗口，会输出一些调试信息。
参考资料：
- [http://doc.qt.io/qt-5/qtglobal.html#qDebug](http://doc.qt.io/qt-5/qtglobal.html#qDebug)
- [http://stackoverflow.com/questions/11184590/how-to-view-qdebug-in-visual-studio](http://stackoverflow.com/questions/11184590/how-to-view-qdebug-in-visual-studio)
