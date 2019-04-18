# 格式化字符读入与判断scanf()/printf()/sscanf()/sprintf() - 心纯净，行致远 - CSDN博客





2018年12月03日 11:23:10[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：119








sscanf()/sprintf()是处理字符串问题的利器，可以理解成string+scanf/printf。注意scanf和cin读入数据的形式都是从左向右，而printf和cout输出数据的形式都是从右向左，这是C/C++的一个特点。

其实scanf和printf都可以表示成如下形式：

```cpp
scanf(screen,"%d",&n);
printf(screen,"%d",n);
```

sscanf和sprintf与上面类似，只是不是通过标准输入设备读进来的，比如，可以通过字符串

```cpp
sscanf(str,"%d",&n);
sprintf(str,"%d",n);
```

上面只是一些简单的应用，实际上sscanf和sprintf可以进行复杂的格式输入和输出。

![](https://img-blog.csdnimg.cn/20181211230740653.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

 类似地，下面的代码使用sprintf将int型变量n，double型变量db,char类型数组str2按照“%d:%.2f,%s"格式写到str中

```cpp
int n=12;
double db=3.14152;
char str[100],str2[100]="good";
sprintf(str,"%d:%.2f,%s",n,db,str2);
printf("str=%s\n",str);

//str=12:3.14,good
```

scanf()与printf()很熟悉，其中printf()可以指定输出的格式。

但是如果对于输入，判断是否格式正确再进行存储的时候可以使用sscanf()/sprintf()，这两个函数都在<cstdio>头文件。

**sscanf与[scanf](https://baike.baidu.com/item/scanf)类似，都是用于输入的，只是后者以键盘([stdin](https://baike.baidu.com/item/stdin))为输入源，前者以固定字符串为输入源。**

# **sscanf()**

1、一般用法
|123|`char``buf[512] = ;``sscanf``(``"123456 "``, ``"%s"``, buf);``printf``(``"%s\n"``, buf);`|
|----|----|

结果为：**123456**

　　2. 取**指定长度的字符串**。如在下例中，取最大长度为4字节的字符串。
|12|`sscanf``(``"123456 "``, ``"%4s"``, buf);``printf``(``"%s\n"``, buf);`|
|----|----|

结果为：**1234**

　　3. 取到**指定字符为止的字符串**。如在下例中，取遇到空格为止字符串。
|12|`sscanf``(``"123456 abcdedf"``, ``"%[^ ]"``, buf);``printf``(``"%s\n"``, buf);`|
|----|----|

结果为：**123456**

　　4. 取**仅包含指定字符集的字符串**。如在下例中，取仅包含1到9和小写字母的字符串。
|12|`sscanf``(``"123456abcdedfBCDEF"``, ``"%[1-9a-z]"``, buf);``printf``(``"%s\n"``, buf);`|
|----|----|

结果为：**123456abcdedf**

　　5. 取到**指定字符集为止的字符串**。如在下例中，取遇到大写字母为止的字符串。
|12|`sscanf``(``"123456abcdedfBCDEF"``, ``"%[^A-Z]"``, buf);``printf``(``"%s\n"``, buf);`|
|----|----|

结果为：**123456abcdedf**

　　6、给定一个字符串iios/12DDWDFF@122，**获取 / 和 @ 之间的字符串**，先将 "iios/"过滤掉，再将非'@'的一串内容送到buf中
|12|`sscanf``(``"iios/12DDWDFF@122"``, ``"%*[^/]/%[^@]"``, buf);``printf``(``"%s\n"``, buf);`|
|----|----|

结果为：**12DDWDFF**

　　7、给定一个字符串**"hello, world"，仅保留"world"**。（注意：“，”之后有一空格）
|12|`sscanf``(``"hello, world"``, ``"%*s%s"``, buf);``printf``(``"%s\n"``, buf);`|
|----|----|

结果为：**world**
***P.S. ***%*s表示第一个匹配到的%s被过滤掉，即hello被过滤了，

　　如果没有空格则结果为NULL。 [2]

# sprintf ()

$money = 123.1

$formatted = sprintf ("%06.2f", $money); // 此时变数 $ formatted 值为 "123.10"

$formatted = sprintf ("%08.2f", $money); // 此时变数 $ formatted 值为 "00123.10"

$formatted = sprintf ("%-08.2f", $money); // 此时变数 $ formatted 值为 "123.1000"

$formatted = sprintf ("%.2f%%", 0.95 * 100); // 格式化为百分比

%08.2f 解释:

%开始符

0是 "填空字元" 表示,如果长度不足时就用0来填满。

8格式化后总长度

2f小数位长度，即2位

第3行值为"00123.10" 解释:

因为2f是(2位)+小数点符号(1位)+前面123(3位)=6位，总长度为8位,故前面用[填空字元]0表示，即00123.10

第4行值为"123.1000" 解释:

-号为反向操作，然后填空字元0添加在最后面了

在将各种类型的数据构造成字符串时，sprintf 的强大功能很少会让你失望。由于sprintf 跟printf 在用法上几乎一样，只是打印的目的地不同而已，前者打印到字符串中，后者则直接在命令行上输出。这也导致sprintf 比printf 有用得多。

实例可以参照：

[1054 求平均值——c实现](https://blog.csdn.net/zhanshen112/article/details/83787883)



