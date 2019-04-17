# Qt qDebug() 的使用方法 - Grandyang - 博客园







# [Qt qDebug() 的使用方法](https://www.cnblogs.com/grandyang/p/4741544.html)







在Qt程序调试的时候，经常需要打印一些变量，那么我们就需要使用qDebug()函数，这种函数有两种使用方法，如下所示：



```
QString s = "Jack";
qDebug() << "My name is " << s << ", nice to meet you!" << endl;
qDebug("My name is " + s.toLatin1() + ", nice to meet you!\n");
```



其中第一种使用方法，qDebug()相当于c++中的cout，后面的使用规则都一样，下面那种有些类似C语言的使用规范，但是不同之处在于它也可以连接字符串，需要注意的是不能直接连接QString类型的，要用toLatin1()函数将其转为为QByteArray才行。












