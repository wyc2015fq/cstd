# 彻底弄懂Qt的编码（汉字乱码问题及相关函数作用） - xqhrs232的专栏 - CSDN博客
2017年10月15日 21:19:07[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：160
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://blog.csdn.net/u012790503/article/details/52485263](http://blog.csdn.net/u012790503/article/details/52485263)
测试1
新建test工程用于测试，main.c文件内容如下：
```
#include <QCoreApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString str_hanzi("百度"); // 汉字
    QString str_ascii("baidu.com"); // 字母
    qDebug() << str_hanzi;
    qDebug() << str_ascii;
    return a.exec();
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
运行, 输出结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20160909111621561)
不出意料的乱码。
#### 下面开始分析。
此时，源代码main.c的编码是UTF-8 BOM。 
![这里写图片描述](https://img-blog.csdn.net/20160909111754062)
使用Notepad++查看，编码也是UTF-8 BOM。说明的确是UTF-8编码。
查看目标文件main.obj，目标文件路径如下： 
![这里写图片描述](https://img-blog.csdn.net/20160909112302769)
使用Notepad++打开，搜索”百度”二字，结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20160909112454963)
同时可以看到此时Notepad++显示文件内容使用的编码是ANSI，即本地编码，可以正确的显示汉字。那就说明，编译生成*.obj目标文件时，编译器将源码中的字符串由UTF-8 BOM转换成了本地编码。
接下来查看test.exe的内容，同样搜索”百度”二字，如下图： 
![这里写图片描述](https://img-blog.csdn.net/20160909113010366)
和理论一样，因为.exe文件就是.obj文件链接起来生成的。
到此为止，我们可以看到，源码中的汉字字符串在生成可执行文件的过程中被转换成了本地编码（实际上不止汉字，字母也会被转换）。
那么和乱码有什么关系呢？ 
我们知道，Qt内部是使用Unicode编码的，即QString保存的是Unicode编码的字符串。所有使用QString的函数都认为QString内部是Unicode字符串。 
那么，当test.exe执行的时候，它在读取”百度”二字的时候，它认为该字符串是Unicode编码，而实际上呢？是本地编码。乱码问题发生的时机就在于此。 
这样一来，乱码的根本原因就一目了然了。
那么，那些QTextCodec类以及QString::fromLocal8bit的作用和原理是什么呢？
先说QSring::fromLocal8bit吧，这个比较简单，意思就是从一个本地编码的字符串生成一个QString字符串（Unicode），这样的话，将该字符串传递给其他函数，就不会出现乱码了。实验之，首先修改程序如下：
```
#include <QCoreApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString str_hanzi("百度"); // 汉字
    QString str_ascii("baidu.com"); // 字母
    qDebug() << QString::fromLocal8Bit("百度"); // 修改了此处
    qDebug() << str_ascii;
    return a.exec();
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
输出结果： 
![这里写图片描述](https://img-blog.csdn.net/20160909130212731)
可以看到它实现了汉字的正常的显示，和理论相同。
那么QTextCodec这个类是做什么的呢？ 
顾名思义，它的中文名应该是”文本编码转换器”(Text coding converter)，下面是网上流传的一段代码：
```
QTextCodec *codec = QTextCodec::codecForName("GBK");
QTextCodec::setCodecForTr(codec);
QTextCodec::setCodecForLocale(codec);
QTextCodec::setCodecForCStrings(codec);
```
- 1
- 2
- 3
- 4
下面修改程序：
```
#include <QCoreApplication>
#include <QDebug>
#include <QTextCodec>
#include <QFile>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForTr(codec);
    QString str_hanzi("百度"); // 汉字
    QString str_ascii("baidu.com"); // 字母
    qDebug() << QString::fromLocal8Bit("百度");
    qDebug() << str_ascii;
    qDebug() << QObject::tr("百度");
    return a.exec();
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
- 18
- 19
- 20
- 21
- 22
输出结果： 
![这里写图片描述](https://img-blog.csdn.net/20160909132508525)
可以看到，这种方法也能实现正确显示汉字。 
但是这种方法的原理是什么？ 
关键在于QObject::tr()函数。它是翻译函数(translate)，同时会进行编码转换。它的默认行为是认为传入的参数是Unicode编码，不需要转换。当你在程序中添加了设置tr编码格式的代码（如上面的程序，设置为GBK）时，会导致编码从你指定的编码(GBK)转换成Unicode。如果把上面的程序中的GBK改成UTF-8，则会乱码，因为该字符串的真正编码是本地编码GBK。实验结果如下图： 
![这里写图片描述](https://img-blog.csdn.net/20160909133135325)
同理，如果将本地编码设置成其他编码，修改代码如下
```
#include <QCoreApplication>
#include <QDebug>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QString str_hanzi("百度"); // 汉字
    QString str_ascii("baidu.com"); // 字母
    qDebug() << QString::fromLocal8Bit("百度");
    qDebug() << str_ascii;
    qDebug() << QObject::tr("百度");
    return a.exec();
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
- 18
- 19
- 20
- 21
这样的话，使用QString::fromLocal8bit的转换就会不正确，结果如下图： 
![这里写图片描述](https://img-blog.csdn.net/20160909133724889)
出乎意料的是，tr的转换也出错了。将UTF-8改回GBK，tr也还是乱码： 
![这里写图片描述](https://img-blog.csdn.net/20160909134121954)
于是添加一行QTextCodec::setCodecForTr(codec);，则显示正确。它们之间会相互干扰，这是我们没有预料到的，不过还好这不是什么大问题，可以通过显式设置来纠正。
### 结论
说了那么多，总结如下： 
1、exe中的字符串编码始终是本地编码，与源代码文件的编码无关。 
2、Qt内部需要使用Unicode编码的字符串才能正确处理（显示等操作）。 
3、由于二者不同，所以对于汉字来说，必须经过转换，第一是通过QString::fromLocal8bit函数来转换，第二是通过QTextCodec来转换。但是setTextCodecForTr、setTextCodecForLocale在高版本已被移除。
注意：
上述测试使用的是Qt4.8.5 msvc2010的版本。minGW还未测试，如果能直接在生成exe文件时直接生成Unicode字符串，那就不需要转换了。
另外还有一种防止乱码的方法，此种方法也可以解决国际化问题导致的乱码，就是使用翻译文件。在源码中统一使用英文。在翻译文件中实现不同版本的语言。
不得不提最后一种比较高级的方法：QStringLiteral宏。它可以直接生成Unicode字符串保存在可执行文件中的只读区域。这样运行时不会发生任何转换。可以显著提高程序运行效率。
测试代码如下：
```
#include <QCoreApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString str_ascii("baidu.com"); // 字母
    qDebug() << QStringLiteral("a百度a");
    qDebug() << str_ascii;
    return a.exec();
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
生成的可执行文件再也找不到”百度”二字了。
但是QStringLiteral需要编译器支持，如支持C++11就具有这种特性。Qt高版本一般也支持。具体性能方面的影响请看Qt的帮助文档。

