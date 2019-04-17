# Qt之输出控制 - DoubleLi - 博客园






# 简述

在Qt项目开发过程中，往往需要对程序的一些信息进行控制，比如：打印日志、调试信息等，便于我们后期查找、跟踪及定位问题。

下面，我们来分享下常用的几种方式。




- [简述](http://blog.csdn.net/liang19890820/article/details/52279188#简述)
- [示例代码](http://blog.csdn.net/liang19890820/article/details/52279188#示例代码)
- [应用程序输出](http://blog.csdn.net/liang19890820/article/details/52279188#应用程序输出)
- [控制台输出](http://blog.csdn.net/liang19890820/article/details/52279188#控制台输出)
- [重定向至文件](http://blog.csdn.net/liang19890820/article/details/52279188#重定向至文件)





# 示例代码

我们简单地写一些测试代码，用qDebug输出一些基本信息。

```
#include <QApplication>
#include <QWidget>
#include <qDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qDebug() << QString::fromLocal8Bit("一去丶二三里");
    qDebug() << QString::fromLocal8Bit("青春不老，奋斗不止！");
    qDebug() << QString::fromLocal8Bit("纯正开源之美，有趣、好玩、靠谱。。。");

    QWidget w;
    w.show();

    return app.exec();
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17

# 应用程序输出

大部分情况下，我们建立的是【Qt Widgets Application】（用于GUI开发）项目，其输出信息在【应用程序输出】窗口。

![这里写图片描述](http://img.blog.csdn.net/20160823212139895)

这对于开发人员来说调试比较方便，但是，当你把整个程序打包出去的时候（比如：给测试人员或者其他人员），他们根本无法获取这些信息（不方便），所以我们可以采用下面两种方式。

# 控制台输出

对于【Qt Widgets Application】来说，我们也可以像【Qt Console Application】（新建项目时可以选择）一样，进行控制台输出，只不过需要自行控制：
- 
在.pro文件中添加：
`CONFIG += console`- 1
- 1

- 
选择：项目 -> 运行，勾选【Run in terminal】（在终端运行）选项。


![这里写图片描述](http://img.blog.csdn.net/20160823212451650)

这时，我们的输出信息就可以显示在控制台上了。

![这里写图片描述](http://img.blog.csdn.net/20160823212947093)

# 重定向至文件

主要采用自定义消息处理机制，重定向输出信息至文件。

其中输出格式可以自定义，比如：输出信息、文件、函数、行号、日期时间。这样就很利于我们追踪了。

详细内容请参考：[Qt之qInstallMessageHandler（重定向至文件）](http://blog.csdn.net/liang19890820/article/details/51838379)









