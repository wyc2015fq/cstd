# QTextCodec中的setCodecForTr等终于消失了 (Qt5) - L_Andy的专栏 - CSDN博客

2017年08月18日 10:11:10[卡哥](https://me.csdn.net/L_Andy)阅读数：274


- 在Qt4中，国内很多新手都喜欢——不分青红皂白地使用如下3行代码

QTextCodec::setCodecForTr(...)
QTextCodec::setCodecForCStrings(...)
QTextCodec::setCodecForLocale(...)

尽管之前零零散散写过一点这方面的内容，比如
- 
[QString
 与中文问题](http://hi.baidu.com/cyclone/blog/item/9d7293130e5a498d6538dbf1.html)

- 
[Qt中translate、tr关系
 与中文问题](http://hi.baidu.com/cyclone/blog/item/aa56e5dd1a79f7e176c638be.html)

- 
[Qt国际化（源码含中文时）的点滴分析](http://blog.csdn.net/dbzhang800/article/details/6334852)

但是，敢死队员还是一批一批的 ... 让人少多有些无奈

## 症结？

在国内，之所以用很多人用这3行代码，是因为他们在源代码的const char*这种窄字符串中使用了中文。

然后，发现。不对啊，咋全是乱码呢？于是上网搜索，发现其他人用的这3行代码

于是，加上试试。发现：在自己的当前环境下（固定系统、固定地区语言设置、固定编译环境）还真的没有乱码了。

于是乎？完全不考虑那3行代码的作用了，反正自己能用就行了，管它什么副作用呢！！

## Qt5

在Qt5中，这个问题终于不复存在了，因为

这两个函数
QTextCodec::setCodecForTr(...)
QTextCodec::setCodecForCStrings(...)

被直接去掉了。

这样一来，受影响的直接是如下代码了：
QString s1 = "我是中文";
QString s2("我是中文");
QString s3 = QObject::tr("我是中文")
## 好消息/坏消息？
- 
**坏消息**：

现在Qt5中尽管去掉了setCodecXXX这两个函数，但是默认编码还是latin1。如果你要想使用
"我是中文"

这样的字符串，必须自己使用QTextCodec或这QString::fromXXX 这种东西进行转换
- 
**好消息** 是：

Qt5发布之时，默认将会是utf8编码，完全可以将你从Qt编码问题解放出来。
- 
**坏消息**，如果使用的是 MSVC2005/2008/2010，可能无法使用utf8编码，于是

下面的代码
QString s1 = "我是中文";
QString s2("我是中文");
QString s3 = QObject::tr("我是中文")

将不会工作。
- 因为从MSVC2005起，你无法给编译器设置字符串要使用的编码。尽管2003之前，也无法设置，但是它会遵循源码文件的编码。而2005就自作聪明了，即使你源码文件保存成不带BOM的utf8，它都会试图帮你转换一下。
- 
**好消息**是什么呢？

如果你在Windows下，且使用的是MinGW，那么只要你将源码文件保存成utf8，前面的代码将直接可以工作（无须其他设置）

如果你在其他平台了，那么应该都默认是utf8文件。同样无须进行设置。
- 
**好消息**

如果你的编译器支持C++11，那么，你可以直接使用

QString s = u8"我是中文";- 
**坏消息**

主流编译器还都不怎么支持C++11

