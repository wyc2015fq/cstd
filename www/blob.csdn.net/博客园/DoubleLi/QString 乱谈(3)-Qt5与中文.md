# QString 乱谈(3)-Qt5与中文 - DoubleLi - 博客园





- 
两个月前，简单写过[QTextCodec中的setCodecForTr等终于消失了 (Qt5)](http://blog.csdn.net/dbzhang800/article/details/7325698) ，在Qt论坛上，不少用户都对去掉这两个函数表示特别的不了解。为什么会这样？我想多少能说明不少用户对C++中[源码字符集和执行字符集](http://blog.csdn.net/dbzhang800/article/details/7540905)的不太了解，从而造成对这种函数的依赖或误用。


今天，随着[Change QString's default codec to be UTF-8](https://codereview.qt-project.org/24700) 进入Qt5的master分支，我们总算可以重新审视一下Qt的中文支持问题。

20120516更新：建议阅读QtCore模块维护者Thiago Macieira 的文章 [Source code must be UTF-8 and QString wants it](http://labs.qt.nokia.com.cn/2012/05/16/source-code-must-be-utf-8-and-qstring-wants-it/)


## 没有了setCodecXXX的Qt5
- 
**Qt5假定的执行字符集是UTF8，不再允许用户擅自改动。这样一来，Qt4中setCodecXXX的各种副作用不再存在，而且中文问题更为简单。**



QString s1 = "汉语";
QString s2("漢語");
QString s3 = tr("中文")
QString s4 = QStringLiteral("中文");//只要字符串不需要翻译，请关注这个
QString s5 = QString::fromWCharArray(L"中文");
QString s6 = u8"中文";//C++11
QString s7 = tr(u8"中文")
...
所有这些在Qt5默认都会正常工作，唯一要求就是：确保你的C++的[执行字符集(the execution character set)](http://blog.csdn.net/dbzhang800/article/details/7540905)是UTF-8

## 各种写法PK？

### 简单不一定好

最简单直接的用法，当属：
QString s1 = "汉语";
QString s2("漢語");
QString s6 = u8"中文";//C++11
...
这有什么问题呢？
- 
定义宏QT_NO_CAST_FROM_ASCII之后，上述代码无法将通过编译（对了，这个宏似乎应该改个名字才对，叫QT_NO_CAST_FROM_CSTRING会名副其实一些）


### 被误用最多的

在Qt4中，QObject::tr()是被滥用(误用)的函数之一：
QString s3 = tr("中文")
...
原因：
- 在Qt4，不少用户被铺天盖地的setCodecForTr()所影响，进而靠它来解决中文问题。

它的用途是用来进行翻译(I18N和L10N)的，如果你没有这方面的需求，真的没必要用它。(在Qt4中，我只注意到有2个大陆网友和1个日本网友有需求并真正进行过这方面的尝试，那么其他应该算误用吧？)

### 让人困惑的wchar_t

刚开始接触Qt和QString时，曾多次想过，为什么不用wchar_t，为什么，...
QString s5 = QString::fromWCharArray(L"中文");
这个东西在Windows下真的很有用：首先它是Windows系统API所用字符串，其次它和QString内部表示相同。但是由于MSVC处于种种考虑，鼓励大家使用TEXT/_T，反倒使大家对它比较陌生。

但是从C++标准来说，wchar_t毕竟不是char16_t，所以跨平台性不好。在linux下，这行代码需要utf32到utf16的转换。

## QStringLiteral

这是一个宏，一个蛮复杂的宏：
QString s4 = QStringLiteral("中文");
### 之前？

在介绍这个宏之前，我们先看看下面写法有什么劣势:
QString s1 = "汉语";
QString s2("漢語");
QString s3 = tr("中文")
QString s6 = u8"中文";//C++11
...
首先，2个汉字的字符串以UTF-8编码的形式被编译器放到了常量区。(至少占7个字节吧？)

然后，程序运行时，构造QString实例，需要在堆上申请空间，存放utf16格式的相应字符串。

有没有存在浪费？

### 方案

QString 内部是UTF16，如果C++编译器在编译期直接提供了UTF16的字符串，那么我们在QString内部直接保存也就够了。这样
- 省掉存在两份不同的拷贝(即相应的转换，malloc的成本)
- 对汉字来说，UTF16本身就是UTF8省空间

### 现实

目前，我们还没有可靠的方式在C++使用UTF16的执行字符集(the execution character set)。
- 尽管 L"..."(wchar_t*) 在Windows下是UTF16，但是不具备跨平台性。
- C++11可以保证这一点，u"..."(char16_t)，但主流编译器尚未提供完美支持。

这两点，导致了QStringLiteral的复杂性

### 实现

源码见 qtbase/src/corelib/tools/qstring.h

（代码中使用宏、模板、lambda表达式，还是相当复杂的，此处只摘片段）
- 如果编译器支持char16_t，则直接使用


#define QT_UNICODE_LITERAL_II(str) u"" str
typedef char16_t qunicodechar;
...- 否则。如果在Windows平台下，或者在其他的wchar_t宽度为2的环境下，使用wchar_t


#if defined(Q_CC_MSVC)
#    define QT_UNICODE_LITERAL_II(str) L##str
#else
#    define QT_UNICODE_LITERAL_II(str) L"" str
#endif
typedef wchar_t qunicodechar;
...- 否则。编译器不支持，Qt作为一个库，肯定也没有办法


# define QStringLiteral(str) QString::fromUtf8(str, sizeof(str) - 1)

from:http://blog.csdn.net/dbzhang800/article/details/7542672








