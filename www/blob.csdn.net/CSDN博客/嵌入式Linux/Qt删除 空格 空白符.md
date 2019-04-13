
# Qt删除 空格-空白符 - 嵌入式Linux - CSDN博客

2017年03月20日 13:59:52[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：5700



QStringstr="addfdsfefokkdikk";
str.remove(QRegExp("\\s"));
qDebug()<<str.remove(QRegExp("\\s"));"addfdsfefokkdikk"

还有去掉前后的空白符


Example:

QString str = "  lots\t of\nwhitespace\r\n ";
str = str.simplified();
// str == "lots of whitespace";


Example:

QString str = "  lots\t of\nwhitespace\r\n ";
str = str.trimmed();
// str == "lots\t of\nwhitespace"

这三个方法都不一样。

