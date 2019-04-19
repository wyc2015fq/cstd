# QString 截取分割字符串 - xqhrs232的专栏 - CSDN博客
2018年05月14日 17:59:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：949
原文地址::[https://blog.csdn.net/xuleisdjn/article/details/51438162](https://blog.csdn.net/xuleisdjn/article/details/51438162)
相关文章
1、QT中QString 类的使用--获取指定字符位置、截取子字符串等----[https://blog.csdn.net/haiross/article/details/49861823](https://blog.csdn.net/haiross/article/details/49861823)
## Title :
- QString
- QString::section()
- QString::split()
Q :
如何从一段由特殊符号分隔的 QString 中获取被分隔的子串？
- 从字符串“one, two, three, four”中获取第二个由‘，’分隔的子串，即“two” ；
```
QString str = "one, two, three, four";
cout << str.section(',', 1, 1).trimmed().toStdString() << endl;
```
- 1
- 2
结果是 “two”，前后不包含空格。上面的函数 trimmed() 是去掉字符串前后的ASCII字符 ‘\t’, ‘\n’, ‘\v’, ‘\f’, ‘\r’, and ’ ‘，这些字符用QChar::isSpace()判断都返回true。重点是 section()函数，见下面详细。
- 从字符串 “one, two* three / four / five ^ six”中获取第四个由 ‘,’、‘*’、 ‘/’和’^’分隔的子串，即“four”；
```
QString str = "one, two* three / four / five ^ six";
cout << str.section(QRegExp("[,*/^]"), 3, 3).trimmed().toStdString() << endl;
```
- 1
- 2
上面用到了一个简单的正则表达式，在Qt中可以由类QRegExp构造，函数 section() 支持使用正则表达式。
重点内容
Fn 1 :
`QString QString::section ( QChar sep, int start, int end = -1, SectionFlags flags = SectionDefault ) const`- 1
这个函数把字符串看成是几个块，这些块由 sep 分隔，start 和 end 指定块号，end 默认为 –1 ，返回的是[ start, end ]内的块组成的字符串，如果 start 和 end 都是负数，那么将从字符串的后面往前面数，返回 [ -end, –start ]内的块组成的字符串。SectionFlags是一些标记，如SectionSkipEmpty表示如果两个分隔符之间是空串，那么就会跳过。下面的代码是摘自QtDoc 4.7：
```
QString str;
QString csv = "forename,middlename,surname,phone";
QString path = "/usr/local/bin/myapp"; // First field is empty
QString::SectionFlag flag = QString::SectionSkipEmpty;
str = csv.section(',', 2, 2);   // str == "surname"
str = path.section('/', 3, 4);  // str == "bin/myapp"
str = path.section('/', 3, 3, flag); // str == "myapp"
str = csv.section(',', -3, -2);  // str == "middlename,surname"
str = path.section('/', -1); // str == "myapp"
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
另外这个函数的另两个重载函数如下：
```
QString QString::section ( const QString & sep, int start, int end = -1, SectionFlags flags = SectionDefault ) const
QString QString::section ( const QRegExp & reg, int start, int end = -1, SectionFlags flags = SectionDefault ) const
```
- 1
- 2
第二个支持正则表达式作为分隔符判定，在某些场合下是不可替代的，如上面的问题中的第二个。
Fn 2 :
`QStringList QString::split ( const QChar & sep, SplitBehavior behavior = KeepEmptyParts, Qt::CaseSensitivity cs = Qt::CaseSensitive ) const`- 1
这个函数把所有的由 sep 分隔的块装进一个 QStringList 中返回， 这个函数同样有两个重载：
```
QStringList QString::split ( const QString & sep, SplitBehavior behavior = KeepEmptyParts, Qt::CaseSensitivity cs = Qt::CaseSensitive ) const
QStringList QString::split ( const QRegExp & rx, SplitBehavior behavior = KeepEmptyParts ) const
```
- 1
- 2
使用 split() 函数对上述两个问题中的第二个进行求解：
```
QString str = "one, two* three / four / five ^ six";
QStringList sections = str.split(QRegExp("[,*/^]")); //把每一个块装进一个QStringList中
cout << sections.at(3).trimmed().toStdString() << endl;
```
- 1
- 2
- 3
End:
Author : Ggicci
多谢阅读，有误希望指正！
[http://tmjfzy.blog.163.com/blog/static/664470252012645536321/](http://tmjfzy.blog.163.com/blog/static/664470252012645536321/)
QT中QString 类的使用--获取指定字符位置、截取子字符串等
//============================================================
备注::
1>QStringListsections=sections1.at(0).split(QRegExp("\\*"));-------*字符是特殊字符，需要转义
----你*代表所有的，你不转义哈，能行？

