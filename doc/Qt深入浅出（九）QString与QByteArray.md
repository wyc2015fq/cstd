# Qt深入浅出（九）QString与QByteArray

2018年02月24日 00:32:08 [吓人的猿](https://me.csdn.net/qq769651718) 阅读数：2816



## 1 QString

​	Qt的QString类提供了很方便的对字符串操作的接口，QString没有继承任何Qt基类。



- QString::arg()用法

```cpp
 QString str = QString("%1 %2 %3").arg(1).arg(2.0).arg("hello");
```

​	%1, %2, %3作为占位符，将被后面的arg()函数中的内容依次替换，比如%1将被替换成1，%2将被替换成2.0，%3将被替换成"hello"。输出为:`"1 2 hello"`

- QString::sprintf用法

```cpp
QString
str.sprintf("%d %s", 10, "hello");
```





## 2 QByteArray

​	Qt的QByteArray类提供了很方便的对字节流操作的接口，QByteArray没有继承任何Qt基类。

- QByteArray::toHex将字节流转换成ACSII编码，例如:0x31一个字节会转换成"31"两个字节

  ```cpp
  QByteArray byte("123456");
  QByteArray byteHex = byte.toHex();
  qDebug() << "byte:" << byte << "byteHex:" << byteHex << endl;
  ```

  输出：`byte: "123456" byteHex: "313233343536"`

- QByteArray::fromHex将ACSII编码转字节流，例如:"31"会转换成0x31一个字节, 需要注意它是一个静态函数

  ```cpp
  QByteArray byte("31323334");
  QByteArray byteFromHex = QByteArray::fromHex(byte);
  qDebug() << "byte:" << byte << "byteFromHex:" << byteFromHex << endl;
  ```

  输出: `byte: "31323334" byteFromHex: "1234"`

  

- QByteArray转char*

  ```cpp
  QByteArray byte("123456");
  char* str = byte.data();
  qDebug() << "str:" << str << endl;
  ```

  输出:`str: 123456`



## 3 QString 与 QByteArray的区别

​	QString是专门用来处理字符串的，除了能处理ASCII编码字符，还包括各国语言的编码，默认情况下QString会把所有数据当做utf-8编码来处理。QByteArray只是单纯用来处理数据的，除了能处理ASCII编码字符，其它复杂的编码不能处理，直接以字节流的方式来对待。

看下面一个例子：

```cpp
QString str("小马哥");
QByteArray byte("小马哥");
qDebug() << "str:" << str << "byte:" << byte << endl;
```

​	输出结果:`str: "小马哥" byte: "\xE5\xB0\x8F\xE9\xA9\xAC\xE5\x93\xA5"`，str保留编码格式，能输出中文，但是QByteArray只把"小马哥"当做普通的字节数据来处理，utf-8编码下，一个汉字占三个字节。





## 4 QString QByteArray互转

### 4.1 QString转QByteArray

```cpp
 QString str("123abc小马哥");
 QByteArray byte1 = str.toLatin1(); //按照ASCII编码转换，无法转换中文
 QByteArray byte2 = str.toUtf8();   //按照Utf-8编码转换，可以转换中文
 qDebug() <<  "byte1:" << byte1 << "byte2:" << byte2;
```



输出结果:`byte1: "123abc???" byte2: "123abc\xE5\xB0\x8F\xE9\xA9\xAC\xE5\x93\xA5"`





### 4.2 QByteArray转QString

```cpp
QByteArray byte("123abc小马哥");
QString str(byte);
qDebug() <<  "byte:" << byte << "str:" << str;
```

输出结果:`byte: "123abc\xE5\xB0\x8F\xE9\xA9\xAC\xE5\x93\xA5" str: "123abc小马哥"`





### 4.3 QString 转char*

​	QString类型无法直接转换成char*, 需要先转换成QByteArray, 然后再转char*

```cpp
QString str("123abc小马哥");
QByteArray byte = str.toUtf8();
char* str1 = byte.data();
```