# Qt中的字符串类QString - L_Andy的专栏 - CSDN博客

2015年11月14日 17:51:01[卡哥](https://me.csdn.net/L_Andy)阅读数：5365



Qt下面，字符串都用QString，确实给开发者提供了方便，想想VC里面定义的各种变量类型，而且函数参数类型五花八门，经常需要今年新那个类型转换

Qt再使用第三方开源库时，由于库的类型基本上都是标准的类型，字符串遇的多的就是Char*类型

在Qt下怎样将QString转char*呢，需要用到QByteArray类，QByteArray类的说明详见Qt帮助文档。

因为char*最后都有一个‘/0’作为结束符，而采用QString::toLatin1()时会在字符串后面加上‘/0’

方法如下：

Qstring  str;

char*  ch;

QByteArray ba = str.toLatin1();    

ch=ba.data();

这样就完成了QString向char*的转化。经测试程序运行时不会出现bug

注意第三行，一定要加上，不可以str.toLatin1().data()这样一部完成，可能会出错。

补充：以上方法当QString里不含中文时，没有问题，但是QString内含有中文时，转换为char*就是乱码，采用如下方法解决：

方法1：

添加GBK编码支持：

#include <QTextCodec>

QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));

QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

然后改变上面的第三行为：QByteArray ba = str.toLoacl8Bit();      toLoacl8Bit支持中文

方法2：

先将QString转为标准库中的string类型，然后将string转为char*，如下：

std::string str = filename.toStdString();

const char* ch = str.c_str();

**QString与QChar**

Qt是一个跨平台的框架中，自然在设计的时候就起点不低。。。

它封装了一个QString类来代表字符串，它是 一个Unicode16编码的类。

同时封装了一个QChar类代表宽字符

**在Qt代码里使用中文**

默认是乱码：

QString str = "你好中国"; ui.lineEdit‐>setText(str);

原因：cpp是GBK编码，不能直接给QString

**GBK <‐> QString**

//GBK ‐>QString

QString str =QString::fromLocal8Bit("你好中国");

//QString ‐>GBK

QString text = ui.lineEdit‐>text(); 

QByteArray bytes =text.toLocal8Bit(); 

const char* gbk = bytes.data();
注：QByteArray是Qt由于封装字节数组的一个类，详见Qt的帮助文档。

**GBK <‐> QString**

本地编码：Local8Bits

由于我们使用的是中文Windows，所以本地编码就 是GBK

然而，如果我们的系统是中文Linux，那本地编 码。。。很可能是UTF‐8

这意味着以上代码不保险。。。

**字符串字面常量的编码**

在cpp中使用字符串字面常量时，要注意cpp文件本 身的编码是GBK还是UTF‐8。

比如，

char text[] ="你好中国";

这个字符串由几个字节组成？是什么编码？
这取决于这个cpp文件本身是按什么编码存储的

**使用UTF‐8**

UTF‐8 ‐>QString : C风格字符串 ‐> QString

char utf8[] = {0xE4 ,0xBD, 0xA0, 0xE5, 0xA5, 0xBD, 0}; 

QStringstr2 = QString::fromUtf8(utf8);

QString ‐> UTF‐8 : 

QString ‐> C风格字符串

QString text =ui.lineEdit‐>text(); 

QByteArray bytes = text.toUtf8(); 

const char* utf8 =bytes.data();

int size = bytes.size();

使用QTextCodec

**QTextCodec**

用于手工转换编码(QString内部调用此类）

//QString(Unicode) ‐> std::string (GBK)

static stringFromUnicode(const QString& qstr)

{

QTextCodec* pCodec =QTextCodec::codecForName("gb2312");

 if(!pCodec) return "";

 QByteArray arr = pCodec‐>fromUnicode(qstr); string cstr =arr.data();

returncstr;

}

**使用QTextCodec**

gbk 到 QString

// std::string (GBK) ‐> QString(Unicode)

 static QString    ToUnicode(const string& cstr)

{

QTextCodec* pCodec = QTextCodec::codecForName("gb2312"); 

if(!pCodec) return "";

QStringqstr = pCodec‐>toUnicode(cstr.c_str(), cstr.length()); 

return qstr;

}

**发布程序**

当程序中带有中文转换处理时，需要将Qt使用的文 字编码库一同发布。

Windows下QT应用程序：

(1)   找到Qt的编码库：

C:\Qt\4.7.3\plugins\codecs

Qt相关的库：

从C:\Qt\4.7.3\bin的目录下直接拷贝QtCored4.dll,QtGuid4.dll

(2)  拷贝codecs这个文件夹，放到目标exe文件所在目 录下

(3)  把VS的运行时库拷贝到codecs目录下

小结：

中文问题的解决：

（1）GBK：普通的单机应用程序

使用QTextCodec()

(2)UTF-8：大型系统，网络传输

一般在大型应用程序中，统一使用utf-8编码。

