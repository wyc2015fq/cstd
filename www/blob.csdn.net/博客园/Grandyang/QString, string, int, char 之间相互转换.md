# QString, string, int, char* 之间相互转换 - Grandyang - 博客园







# [QString, string, int, char* 之间相互转换](https://www.cnblogs.com/grandyang/p/4312273.html)







这三种数据类型在实际运用中经常需要互相转换，那么这里小结下它们之间的转换方法：



- Qstring & string

Qt中封装的类十分强大，其成员函数数量之多比STD有过之而无不及，许多程序员抱怨Qt非要整个自己的QTD，为啥不直接支持STD，但是我想说某些时候QTD完全可以替代STD，就算不想完全替代，Qt也提供了完整而强大的相互转换的函数，下面我们先来看Qstring和string的相互转换。

```
// string to QString
std::string s = "hello world";
QString qs = QString::fromStdString(s);

// QString to string
QString qs = "coder";
std::string s = qs.toStdString();
```



- QString & int

```
// QString to int
QString qs = "123";
int d = qs.toInt();

// int to QString
int d = 18;
QString qs = QString::number(d);
```



- string & int

```
// string to int
std::string s = "28";
int d = atoi(s.c_str());

// int to string
int d = 123;
// method 1
char *intStr = itoa(d);
std::string s = string(intStr);
// method 2
stringstream ss;
ss << d;
std::string s = ss.str();
// method 3
std::string s = std::to_string(d);
```



- char* & string

参见我之前的博客[string to char* and char* to string 玩转 String 和 Char*](http://www.cnblogs.com/grandyang/p/4123477.html)。



- char* & QString

可先由上面的方法将char*先转为string，再和QString相互转换。



- char* & int

可先将char*转为string，或是先将int转为string，于是就变成了它们和string之间的转换。














