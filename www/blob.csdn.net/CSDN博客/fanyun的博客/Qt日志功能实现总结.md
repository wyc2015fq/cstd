# Qt日志功能实现总结 - fanyun的博客 - CSDN博客
2017年12月04日 18:13:04[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：305
                1、信息基本分类：
qDebug : 调试信息提示
qWarning : 一般的警告提示
qCritical: 严重错误提示
qFatal : 致命错误提示
2、如何截获这些信息
Qt提供了qInstallMsgHandler 方法用来定制消息发生后如何来处理。
qInstallMsgHandler 是一个回调函数，主要是由qDebug、qWarnng、qCritical、qFatal这些函数进行触发，
也就是说，qDeubg这些函数处理的消息文本会被qInstallMsgHandler 所指向的回调函数截获，这样就允许用户自己来处理这些消息文本。
例如，你完全可以将这些消息文本输出并保存到相关的日志文件中。
3、将qDebug()\qWarning()\qCritical()\qFatal() 信息输出到指定文件中
示例1：
```cpp
#include <qapplication.h>
#include <stdio.h>
#include <stdlib.h>
void customMessageHandler(QtMsgType type, const char *msg)
{
    QString txtMessage;
  
    switch (type) 
    {    
        case QtDebugMsg:    //调试信息提示
            txtMessage = QString("Debug: %1").arg(msg);
            break;
        case QtWarningMsg:    //一般的warning提示
            txtMessage = QString("Warning: %1").arg(msg);
            break;
        case QtCriticalMsg:    //严重错误提示
            txtMessage = QString("Critical: %1").arg(msg);
            break;
        case QtFatalMsg:    //致命错误提示
            txtMessage = QString("Fatal: %1").arg(msg);
            abort();
    }
    //保存输出相关信息到指定文件
    QFile outputFile("customMessageLog.txt");
    outputFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream textStream(&outputFile);
    textStream << txtMessage << endl;
}
```
```cpp
int main( int argc, char * argv[] ) 
{
    QApplication app( argc, argv );
    qInstallMsgHandler(customMessageHandler); //注册MsgHandler回调函数
    //以后就可以像下面这样直接打日志到文件中，而且日志也会包含时间信息
    qDebug("This is a debug message");
    qWarning("This is a warning message");
    qCritical("This is a critical message");
    qFatal("This is a fatal message");
    return app.exec();
}
```
4、对应取消信息处理
     直接调用qInstallMsgHandler(0)就可以
