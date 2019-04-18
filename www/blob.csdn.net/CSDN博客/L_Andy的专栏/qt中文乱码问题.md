# qt中文乱码问题 - L_Andy的专栏 - CSDN博客

2017年08月18日 10:56:21[卡哥](https://me.csdn.net/L_Andy)阅读数：172
个人分类：[Qt](https://blog.csdn.net/L_Andy/article/category/5882119)



首先呢，声明一下，QString 是不存在中文支持问题的，很多人遇到问题，并不是本身 QString 的问题，而是没有将自己希望的字符串正确赋给QString。

很简单的问题，"我是中文"这样写的时候，它是传统的 char 类型的窄字符串，我们需要的只不过是通过某种方式告诉QString 这四个汉字采用的那种编码。而问题一般都出在很多用户对自己当前的编码没太多概念，

于是

## 一个简 单的 Qt 程序

下面这个小程序，估计大家会感到比较亲切。似乎有相当多的中文用户尝试写过这样的代码：

#include <QtGui/QApplication>#include <QtGui/QLabel>int main(int argc, char **argv){    QApplication app(argc, argv);    QString a= "我是汉字";    QLabel label(a);    label.show();    return app.exec();}

编码，保存，编译，运行，一切都很顺利，可是结果呢：
- |多数用户看到|其他用户看到|
|----|----|
|ÎÒÊÇºº×Ö|æˆ‘æ˜¯æ±‰å —|

出乎意料，界面上中文没显示出来，出现了不认识字符。 于是开始用搜索引擎搜索，开始上论坛发帖或抱怨

最后被告知，下面的语句之一可以解决问题：

QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

两条指令挨个一试，确实可以解决(多数用户是第一条，其他用户是第二条)。那么，为什么会这样呢？

## 两种乱码什么时候出现

对这个问题，我想大家可能都有话说。在继续之前，我们先列个表，看看两种乱码分别在那种情况下出现：

我们只列举大家最常用的3个编译器(微软VS的中的**cl**，Mingw中的g++，Linux下的**g++**)，源代码分别采用**GBK** 和 **不带BOM的UTF-8 **以及 **带BOM的UTF-8 **这3中编码进行保存。
- |源代码的编码|编译器|结果||
|----|----|----|----|
|GBK|cl|1|*|
|mingw-g++|1|*| |
|g++|1|| |
|UTF-8(不带BOM)|cl|2||
|mingw-g++|2|| |
|g++|2|*| |
|UTF-8(带BOM)|cl|1||
|mingw-g++|2|| |
|g++|编译失败|| |

采用3种不同编码保存的源代码文件，分别用3种不同的编译器编译，形成9种组合，除掉一种不能工作的情况，两种乱码出现的情况各占一半。

从中我们也可以看出，乱码和操作系统原本是没有关系的。但我们在 Windows 一般用的GBK，linux一般用的是不带BOM的UTF-8。如果我们只考虑带*****的情况，也可以说两种乱码和系统有关。

## QString 为什么会乱码呢

真的是 QString 乱码了吗？我们可以问问自己，我们抱怨的对象是不是搞错了？

继续之前，先明确几个概念：

### 明确概念0：
- "我是汉字" 是C语言中的字符串，它是char型的窄字符串。上面的例子可写为

const char * str = "我是汉字";
QString a= str;

或
char str[] = "我是汉字";
QString a= str;

等

### 明确概念1：
- 源文件是有编码的，但是这种纯文本文件却不会记录自己采用的编码

这个是问题的根源，不妨做个试验，将前面的源代码保存成GBK编码，用16进制编辑器能看到引号内是ce d2 ca c7 ba ba d7 d6这样8个字节。

现在将该文件拷贝到正体(繁体)中文的Windows中，用记事本打开会什么样子呢？

...    QString a= "扂岆犖趼";    QLabel label(a);    label.show();...

那么放到欧美人的Windows系统中，再用记事本打开呢？

...    QString a= "ÎÒÊÇºº×Ö";    QLabel label(a);    label.show();...

同一个文件，未做任何修改，但其中的8个字节ce d2 ca c7 ba ba d7 d6,对用GBK的大陆人，用BIG5的港澳台同胞，以及用Latin-1的欧洲人看来，看到的却是完全不同的文字。

### 明确概念2：
- 如同我们都了解的'A'与'\x41'等价一样。

GBK编码下的
const char * str = "我是汉字"

等价于
const char * str = "\xce\xd2\xca\xc7\xba\xba\xd7\xd6";

当用UTF-8编码时，等价于
const char * str = "\xe6\x88\x91\xe6\x98\xaf\xe6\xb1\x89\xe5\xad\x97";

注意：这个说法不全对，比如保存成带BOM的UTF-8，用cl编译器时，汉字本身是UTF-8编码，但程序内保存时却是对应的GBK编码。

### 明确概念3：
- QString 内部采用的是Unicode。

QString内部采用的是 Unicode，它可以同时存放GBK中的字符"我是汉字",BIG5中的字符"扂岆犖趼" 以及Latin-1中的字符"ÎÒÊÇºº×Ö"。

一个问题是，源代码中的这8个字节"\xce\xd2\xca\xc7\xba\xba\xd7\xd6"，该怎么转换成Unicode并存到 QString 内？按照GBK、BIG5、Latin-1还是其他方式...

在你不告诉它的情况下，它默认选择了Latin-1，于是8个字符"ÎÒÊÇºº×Ö"的unicode码被存进了QString中。最终，8个Latin字符出现在你期盼看到4中文字符的地方，所谓的乱码出现了

## QString 工作方式

const char * str = "我是汉字";
QString a= str;

其实很简单的一个问题，当你需要从窄字符串 char* 转成Unicode的QString字符串的，你需要告诉QString你的这串char* 中究竟是什么编码？GBK、BIG5、Latin-1

理想情况就是：将char* 传给QString时，同时告诉QString自己的编码是什么：

就像下面的函数一样，QString的成员函数知道按照何种编码来处理 C 字符串
QString QString::fromAscii ( const char * str, int size = -1 )
QString QString::fromLatin1 ( const char * str, int size = -1 ) 
QString QString::fromLocal8Bit ( const char * str, int size = -1 )
QString QString::fromUtf8 ( const char * str, int size = -1 )

单QString 只提供了这几个成员函数，远远满足不了大家的需求，比如，在简体中文Windows下，local8Bit是GBK，可是有一个char串是 BIG5 或 Latin-2怎么办？

那就动用强大的QTextCodec吧，首先QTextCodec肯定知道自己所负责的编码的，然后你把一个char串送给它，它就能正确将其转成Unicode了。
QString QTextCodec::toUnicode ( const char * chars ) const

可是这个调用太麻烦了，我就想直接
QString a= str;

或
QString a(str);

这样用怎么办？

这样一来肯定没办法同时告诉 QString 你的str是何种编码了，只能通过其他方式了。这也就是开头提到的
QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

设置QString默认采用的编码。而究竟采用哪一个，一般来说就是源代码是GBK，就用GBK，源代码是UTF-8就用UTF-8。但有一个例外，如果你保存成了带BOM的UTF-8而且用的微软的cl编译器，此时仍是GBK。

**总结下，乱码出现的原因主要是：**

QString内部采用的是 Unicode，它可以同时存放GBK中的字符"我是汉字",BIG5中的字符"扂岆犖趼" 以及Latin-1中的字符"ÎÒÊÇºº×Ö"。

当你需要从窄字符串 char* 转成Unicode的QString字符串的，你需要告诉QString你的这串char* 中究竟是什么编码？GBK、BIG5、Latin-1？

在你不告诉它的情况下，它默认选择了Latin-1，于是8个字符"ÎÒÊÇºº×Ö"的unicode码被存进了QString中。最终，8个Latin字符出现在你期盼看到4中文字符的地方，

所谓的乱码出现了。

网上有很多方法介绍直接在main.cpp里设置：

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    QTextCodec::setCodecForTr(codec);

    QTextCodec::setCodecForLocale(codec);

    QTextCodec::setCodecForCStrings(codec);

其实这在某些情况下也是有问题的，因为程序可能读到系统的中文路径，或者调用中文路径下的外部程序，这时候如果系统是gb2312就有问题了。

因为中文路径的编码是采用utf-8存到QString里的，系统读中文路径解码的时候采用的却是系统的gb2312，所以会调不起带中文路径的外部程序。

以上问题下面方法可以解决：

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    QTextCodec::setCodecForTr(codec);

    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

对于外部字符串编码解码全部采用本地编码。

