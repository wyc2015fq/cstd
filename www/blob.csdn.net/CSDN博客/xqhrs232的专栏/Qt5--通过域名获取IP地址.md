# Qt5--通过域名获取IP地址 - xqhrs232的专栏 - CSDN博客
2018年08月30日 14:59:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：101
原文地址::[https://blog.csdn.net/qq_28877125/article/details/81101966](https://blog.csdn.net/qq_28877125/article/details/81101966)
相关文章
1、Qt通过域名获取ip----[https://blog.csdn.net/Tohnanye/article/details/78615188](https://blog.csdn.net/Tohnanye/article/details/78615188)
方法一：
在上面讲述了用本机的计算机名获取本机的IP地址。其实QHostInfo类也可以用来获取任意主机名的IP地址，如一个网站的IP地址。在这里我们可以使用lookupHost()函数。它是基于信号和槽的，一旦查找到了IP地址，就会触发槽函数。具体用法如下。
我们在widget.h文件中添加一个私有槽函数：
```
private slots:
void lookedUp(const QHostInfo &host);
```
- 1
- 2
然后在widget.cpp中的构造函数中先将上面添加的代码全部注释（可以通过选中所有代码，然后按下Ctrl+/快捷键来注释代码），然后添加以下代码：
`QHostInfo::lookupHost("www.qter.org", this,SLOT(lookedUp(QHostInfo)));`- 1
这里我们查询Qter开源社区的IP地址，如果查找到，就会执行我们的lookedUp()函数。
在widget.cpp中添加lookedUp()函数的实现代码：
```
void Widget::lookedUp(const QHostInfo &host)
{
    qDebug() << host.addresses().first().toString();
}
```
- 1
- 2
- 3
- 4
方法二：
```
QHostInfo info = QHostInfo::fromName("www.qter.org");
    qDebug() << info.addresses().first().toString();
    QHostAddress host(info.addresses().first().toString());
```
