# QT 保证Qt程序只运行一个实例 - xqhrs232的专栏 - CSDN博客
2018年07月12日 16:27:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：423
原文地址::[https://blog.csdn.net/wang_lichun/article/details/7815923](https://blog.csdn.net/wang_lichun/article/details/7815923)
相关文章
1、Qt中单例程序的实现----[https://blog.csdn.net/u012952807/article/details/52159540](https://blog.csdn.net/u012952807/article/details/52159540)
2、使Qt 程序只能运行一个实例的3种方法----[https://blog.csdn.net/robertkun/article/details/8518576](https://blog.csdn.net/robertkun/article/details/8518576)
让Qt程序只运行一个实例，当然用win下的native API是很不靠谱的，因为这样会牺牲掉Qt跨平台的特性。所以我给出下面两种解决方案。原理上就是进程间通讯。 QSingleApplication用的是socket ，而我使用的是共享内存。
一 使用 QSingleApplication
　　QSingleApplication是Qt提供的一个解决方案，它不包含在Qt 的library 中。遵循LGPL 协议。关于如何使用，下载了这个解决方案之后，里面有例子。还有，QtCreator 中还用到了它。你也可以翻一番QtCreator的源代码。
二 使用共享内存
    将qt程序中的main.cpp中的main函数修改如下：
```cpp
```
- 
intmain(int argc, char *argv[])
- 
{
- 
QApplication a(argc, argv);
- 
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
- 
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
- 
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
- 
- 
QSharedMemory mem("SystemName");//以系统exe名称为参数，定义共享内存mem
- 
if(!mem.create(1))//创建共享内存mem，如果该共享内存已存在，则弹出提示对话框，并退出
- 
    {
- 
        QMessageBox::information(0,QObject::tr("Warning"),QObject::tr("An instance is running"));
- 
return0;
- 
    }
- 
    MainWindow w;
- 
    w.show();
- 
- 
return a.exec();
- 
}
- 
