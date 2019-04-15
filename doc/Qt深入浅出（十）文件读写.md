# Qt深入浅出（十）文件读写

2018年02月24日 00:33:01 [吓人的猿](https://me.csdn.net/qq769651718) 阅读数：1314



# 文件读写

​	Qt中使用QFile类来操作文件的输入/输出。继承至QIODevice，QIODevice类是输入/输出设备的基类，

为设备提供了公共实现和抽象接口用于读写块数据。QIODevice又继承至QObject。



## 1 使用QFile打开文件



- QFile的构造函数

```cpp
QFile(const QString &name) //传入一个文件路径
```



- 构造完成后，并没有打开文件，需要使用QFile::open函数来打开文件

```cpp
[virtual] bool QFile::open(OpenMode mode);
/*
*OpenMode mode 打开方式，是一个枚举类型
*QIODevice::NotOpen 不打开
*QIODevice::ReadOnly 只读方式
*QIODevice::WriteOnly 读写方式
*QIODevice::ReadWrite 读写方式
*QIODevice::Append   追加方式
*QIODevice::Truncate 阶段方式
*QIODevice::Text     转换不同平台的换行，读的时候把所有换行转成'\n',写的时候再把'\n'转换对应平台的换行
*QIODevice::Unbuffered 不使用缓冲区
*/
```



例如:

```cpp
 QFile file("d:/123.txt");
 file.open(QIODevice::ReadOnly);
```







## 2 关闭文件

```cpp
[virtual] void QFileDevice::close(); //刷新缓冲区，并关闭文件
```



## 3 文件读操作

- QIODevice::read函数

```cpp
QByteArray QIODevice::read(qint64 maxSize);//读取maxSize个字节，内部位置指针后移maxSize，并返回一个QByteArray对象。
```

例如：

```cpp
QFile file("d:/123.txt");
file.open(QIODevice::ReadOnly);
qDebug() << file.read(10) << endl;
file.close();
```

- QIODevice::readLine函数

```cpp
QByteArray QIODevice::readLine(qint64 maxSize = 0) //读取一行，但是这一行不能超过maxSize字节，maxSize = 0代表不限制行字节数。
```

例如：

```cpp
QFile file("d:/123.txt");
file.open(QIODevice::ReadOnly);
qDebug() << file.readLine(10) << endl; 
file.close();
```

- QIODevice::readAll函数

```cpp
QByteArray QIODevice::readAll()
```



## 4 文件写操作

- QIODevice::write函数

```cpp
qint64 QIODevice::write(const QByteArray &byteArray); //将byteArray写入文件，写完内部位置指针后移
```

例如:

```cpp
QFile file("d:/123.txt");
file.open(QIODevice::ReadWrite | QIODevice::Text); //打开模式可以使用‘|’组合
QByteArray byte("hellworld");
file.write(byte);
file.write(byte);
file.close();
```





## 5 使用QDataStream

​	流控文件输入输出可以使用QDataStream。

- 流控写入

```cpp
#include <QDataStream>
#include <QDebug>
#include <QFile>
int main(int argc, char**argv)
{
    QFile file("d:/123.txt");
    file.open(QIODevice::ReadWrite);
    QDataStream stream(&file);
    int a = 10;
    QString str = "helloworld";
    stream << a << str;
    file.close();
    return 0;
}
```

- 流控读取

```cpp
#include <QDataStream>
#include <QDebug>
#include <QFile>
int main(int argc, char**argv)
{
    QFile file("d:/123.txt");
    file.open(QIODevice::ReadWrite);
​
    QDataStream stream(&file);
    int a;
    QString str;
    stream >> a >> str;
    qDebug() << "a:" << a << "str:" << str << endl;
    file.close();
    return 0;
}
```