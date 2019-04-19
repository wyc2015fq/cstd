# Qt字符串特殊用法总结 - fanyun的博客 - CSDN博客
2017年12月26日 22:02:52[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1171
   QString在Qt里面被大量使用，使用QString的时候，我们不需要担心内存分配以及关于'\0'结尾的这些注意事项。QString会把这些问题解决。通常，你可以把QString看作是一个QChar的向量。另外，与C风格的字符串不同，QString中间是可以包含'\0'符号的，而length()函数则会返回整个字符串的长度，而不仅仅是从开始到'\0'的长度。
(1).QString字符串的拼接
QString也重载的+和+=运算符。这两个运算符可以把两个字符串连接到一起,而且效率很高，下面是两个操作符的使用：
```cpp
QString str = "User: "; 
str += userName + "\n";
```
(2).QString的append()函数
append()函数也有拼接的作用，例如：
```cpp
str = "User: ";  
str.append(userName);
str.append("\n");
```
(3).C语言中有printf()函数作为格式化输出，QString则提供了一个sprintf()函数实现了相同的功能,例如：
```cpp
str.sprintf("%s %.1f%%", "perfect competition", 100.0); 这句代码将输出：perfect competition 100.0%，同C语言的printf()一样。
```
(4).Qt提供的另一种格式化字符串输出的函数arg():
```cpp
str = QString("%1 %2 (%3s-%4s)")  
arg("permissive").arg("society").arg(1950).arg(1970);
```
   这段代码中，%1, %2, %3, %4作为占位符，将被后面的arg()函数中的内容依次替换，比如%1将被替换成permissive，%2将被替换成society，%3将被替换成1950，%4将被替换曾1970，最后，这句代码输出为：permissive society (1950s-1970s). arg()函数比起sprintf()来是类型安全的，同时它也接受多种的数据类型作为参数，因此建议使用arg()函数而不是传统的sprintf()。
(5).Qt使用static的函数number()可以把数字转换成字符串
```cpp
QString str = QString::number(54.3);
```
(6).Qt中也可以使用非static函数setNum()来实现相同的目的
```cpp
QString str;  
str.setNum(54.3);
```
(7).Qt的mid()函数截取子串：
```cpp
QString x = "Nine pineapples";  
QString y = x.mid(5, 4);            // y == "pine"  
QString z = x.mid(5);               // z == "pineapples"
```
   mid()函数接受两个参数，第一个是起始位置，第二个是取串的长度。如果省略第二个参数，则会从起始位置截取到末尾。
(8).Qt的left()和rigt()函数
 
  都接受一个int类型的参数n，都是对字符串进行截取。不同之处在于，left()函数从左侧截取n个字符，而right()从右侧开始截取。
```cpp
QString x = "Pineapple";  
QString y = x.left(4);      // y == "Pine"
```
(9).Qt使用indexOf()返回字符串的位置
```cpp
QString x = "sticky question";  
QString y = "sti";  
x.indexOf(y);               // returns 0  
x.indexOf(y, 1);            // returns 10  
x.indexOf(y, 10);           // returns 10  
x.indexOf(y, 11);           // returns -1
```
(10).Qt使用startsWith()和endsWith()可以检测字符串是不是以某个特定的串开始或结尾，例如：
```cpp
if (url.startsWith("http:") && url.endsWith(".png"))  
{  
} 
这段代码等价于
if (url.left(5) == "http:" && url.right(4) == ".png")  
{  
}
```
 
  前者要比后者更加清楚简洁，并且性能也更高。
(11).QString的+=即可完成const char *类型的C风格字符串转换成QString的字符串
```cpp
str += " (1870)";
```
   我们将const char * 类型的字符串" (1870)"转换成为QString类型。如果需要显式的转换，可以使用QString的强制转换操作，或者是使用函数fromAscii()等。为了将QString类型转成const char *字符串，需要进行两步操作，一是使用toAscii()获得一个QByteArray类型对象，然后调用它的data()或者constData()函数，例如：
```cpp
printf("User: %s\n", str.toAscii().data());
```
 
  为了方便使用，Qt提供了一个宏qPrintable()，这个宏等价于toAscii().constData()，例如：
```cpp
printf("User: %s\n", qPrintable(str));
```
   注意：我们调用QByteArray类上面的data()或者constData()函数，将获得QByteArray内部的一个const char*类型的字符串，因此，我们不需要担心内存泄漏等的问题，Qt会替我们管理好内存。不过这也暗示我们，注意不要使用这个指针太长时间，因为如果QByteArray被delete，那么这个指针也就成为野指针了。如果这个QByteArray对象没有被放在一个变量中，那么当语句结束后，QbyteArray对象就会被delete，这个指针也就被delete
 了。
(12).其它使用总结
 
  QString还提供了replace()函数供实现字符串的替换功能；trimmed()函数去除字符串两侧的空白字符(注意，空白字符包括空格、Tab以及换行符，而不仅仅是空格)；toLower()和toUpper()函数会将字符串转换成小写大写字符串；remove()和insert()函数提供了删除和插入字符串的能力；simplified()函数可以将串中的所有连续的空白字符替换成一个，并且把两端的空白字符去除，例如"   \t   ”会返回一个空格" "。
   QString在Qt里面被大量使用，使用QString的时候，我们不需要担心内存分配以及关于'\0'结尾的这些注意事项。QString会把这些问题解决。通常，你可以把QString看作是一个QChar的向量。另外，与C风格的字符串不同，QString中间是可以包含'\0'符号的，而length()函数则会返回整个字符串的长度，而不仅仅是从开始到'\0'的长度。
(1).QString字符串的拼接
QString也重载的+和+=运算符。这两个运算符可以把两个字符串连接到一起,而且效率很高，下面是两个操作符的使用：
```cpp
QString str = "User: "; 
str += userName + "\n";
```
(2).QString的append()函数
append()函数也有拼接的作用，例如：
```cpp
str = "User: ";  
str.append(userName);
str.append("\n");
```
(3).C语言中有printf()函数作为格式化输出，QString则提供了一个sprintf()函数实现了相同的功能,例如：
```cpp
str.sprintf("%s %.1f%%", "perfect competition", 100.0); 这句代码将输出：perfect competition 100.0%，同C语言的printf()一样。
```
(4).Qt提供的另一种格式化字符串输出的函数arg():
```cpp
str = QString("%1 %2 (%3s-%4s)")  
arg("permissive").arg("society").arg(1950).arg(1970);
```
   这段代码中，%1, %2, %3, %4作为占位符，将被后面的arg()函数中的内容依次替换，比如%1将被替换成permissive，%2将被替换成society，%3将被替换成1950，%4将被替换曾1970，最后，这句代码输出为：permissive society (1950s-1970s). arg()函数比起sprintf()来是类型安全的，同时它也接受多种的数据类型作为参数，因此建议使用arg()函数而不是传统的sprintf()。
(5).Qt使用static的函数number()可以把数字转换成字符串
```cpp
QString str = QString::number(54.3);
```
(6).Qt中也可以使用非static函数setNum()来实现相同的目的
```cpp
QString str;  
str.setNum(54.3);
```
(7).Qt的mid()函数截取子串：
```cpp
QString x = "Nine pineapples";  
QString y = x.mid(5, 4);            // y == "pine"  
QString z = x.mid(5);               // z == "pineapples"
```
   mid()函数接受两个参数，第一个是起始位置，第二个是取串的长度。如果省略第二个参数，则会从起始位置截取到末尾。
(8).Qt的left()和rigt()函数
 
  都接受一个int类型的参数n，都是对字符串进行截取。不同之处在于，left()函数从左侧截取n个字符，而right()从右侧开始截取。
```cpp
QString x = "Pineapple";  
QString y = x.left(4);      // y == "Pine"
```
(9).Qt使用indexOf()返回字符串的位置
```cpp
QString x = "sticky question";  
QString y = "sti";  
x.indexOf(y);               // returns 0  
x.indexOf(y, 1);            // returns 10  
x.indexOf(y, 10);           // returns 10  
x.indexOf(y, 11);           // returns -1
```
(10).Qt使用startsWith()和endsWith()可以检测字符串是不是以某个特定的串开始或结尾，例如：
```cpp
if (url.startsWith("http:") && url.endsWith(".png"))  
{  
} 
这段代码等价于
if (url.left(5) == "http:" && url.right(4) == ".png")  
{  
}
```
 
  前者要比后者更加清楚简洁，并且性能也更高。
(11).QString的+=即可完成const char *类型的C风格字符串转换成QString的字符串
```cpp
str += " (1870)";
```
   我们将const char * 类型的字符串" (1870)"转换成为QString类型。如果需要显式的转换，可以使用QString的强制转换操作，或者是使用函数fromAscii()等。为了将QString类型转成const char *字符串，需要进行两步操作，一是使用toAscii()获得一个QByteArray类型对象，然后调用它的data()或者constData()函数，例如：
```cpp
printf("User: %s\n", str.toAscii().data());
```
 
  为了方便使用，Qt提供了一个宏qPrintable()，这个宏等价于toAscii().constData()，例如：
```cpp
printf("User: %s\n", qPrintable(str));
```
   注意：我们调用QByteArray类上面的data()或者constData()函数，将获得QByteArray内部的一个const char*类型的字符串，因此，我们不需要担心内存泄漏等的问题，Qt会替我们管理好内存。不过这也暗示我们，注意不要使用这个指针太长时间，因为如果QByteArray被delete，那么这个指针也就成为野指针了。如果这个QByteArray对象没有被放在一个变量中，那么当语句结束后，QbyteArray对象就会被delete，这个指针也就被delete
 了。
(12).其它使用总结
 
  QString还提供了replace()函数供实现字符串的替换功能；trimmed()函数去除字符串两侧的空白字符(注意，空白字符包括空格、Tab以及换行符，而不仅仅是空格)；toLower()和toUpper()函数会将字符串转换成小写大写字符串；remove()和insert()函数提供了删除和插入字符串的能力；simplified()函数可以将串中的所有连续的空白字符替换成一个，并且把两端的空白字符去除，例如"   \t   ”会返回一个空格" "。
