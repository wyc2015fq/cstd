# QString与char *的相互转换 - fanyun的博客 - CSDN博客
2018年02月25日 22:12:22[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：947
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**1.QString 转换为 char ***
    将 QString 转 char *，需要用到 QByteArray 类 。因为 char * 最后都有一个`'\0'`作为结束符，而采用 QString::toLatin1() 时会在字符串后面加上`'\0'。`
`（1）.中文字符串转char*：`
```cpp
```
#include <QTextCodec>
QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
QString  str;
char*  ch;
QByteArray byte = str.toLocal8Bit();  // toLocal8Bit 支持中文
ch=byte.data();
  (2).非中文字符串转char*:
```cpp
QString  str;
char*  ch;
QByteArray byte = str.toLatin1();//必须加
ch=byte.data();
```
**2.先将 QString 转为标准库中的 string 类型，然后将 string 转为 char ***
```cpp
QString  filename;
std::string str = filename.toStdString();
const char* ch = str.c_str();
```
1.char *转换成QString
（1）如果char*中没有中文字符，那么转换的时候，非常的简单。
char *ch = "abcd";
QString strzw(ch); strzw = ch;等等
（2）如果char*中含有中文字符，那么转换的时候必须注意了：
char *ch = "abcd中国"；
QString strzw = QString::fromLocal8Bit(ch);
但是如果是这样的：
char ch[] = "abcd中国";
QString strzw = QString::fromLocal8Bit(ch);也是一样的。
2.QString转换成char*
无论QString字符串中有无中文字符，用这种方式进行转换
QString str("abcd中文");
QByteArray cpath = str.toLocal8Bit();
char*p = cpath.data();
