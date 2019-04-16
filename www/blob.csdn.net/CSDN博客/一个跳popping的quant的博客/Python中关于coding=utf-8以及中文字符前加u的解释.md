# Python中关于coding=utf-8以及中文字符前加u的解释 - 一个跳popping的quant的博客 - CSDN博客





2018年02月05日 20:19:13[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：14459
所属专栏：[学python的路上](https://blog.csdn.net/column/details/20561.html)









写了很久的Python了，每次写之前都要在开头加上coding=utf-8，只知道是设置编码格式，但并没有太在意，今天在写socket编程时才发现编码格式的重要性。

**一、关于开头coding=utf-8**

开头的coding=utf-8和coding:utf-8的作用是一样的。

作用是声明python代码的文本格式是utf-8，python按照utf-8的方式来读取程序。

如果不加这个声明，无论代码中还是注释中有中文都会报错。


以下几种方式都可以：


![](https://img-blog.csdn.net/20180205193142233?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20180205193232619?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

注意一点无论中间是：还是=，其中coding与它们之间都不能有空格。否则也会报错。

![](https://img-blog.csdn.net/20180205193438699?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**二、关于中文字符前面的u**

加上u的作用是对后面的字符串进行unicode编码。unicode是书写国际文本的标准方法，如果你想用你的母语来写文本的时候，就需要一个支持unicode的编译器，python允许处理Unicode文本，只需在字符串前加上前缀u即可。一般英文的字符串不需要进行任何编码的处理，但是当你使用中文的时候，就需要在前面加上u了。

再补充下utf-8、unicode以及其他编码之间的关系：


- Unicode 是字符集
- UTF-8 是编码规则

Unicode字符集的作用就是给每一个字符分配一个唯一的ID（又称码位）。



编码规则是将码位转换为字节序列的规则。

引用知乎上某个大佬举的例子：https://www.zhihu.com/question/23374078/answer/65352538






举一个例子：It's 知乎日报

你看到的unicode字符集是这样的编码表：


```
I 0049
t 0074
' 0027
s 0073
  0020
知 77e5
乎 4e4e
日 65e5
报 62a5
```


每一个字符对应一个十六进制数字。

计算机只懂二进制，因此，严格按照unicode的方式(UCS-2)，应该这样存储：


```
I 00000000 01001001
t 00000000 01110100
' 00000000 00100111
s 00000000 01110011
  00000000 00100000
知 01110111 11100101
乎 01001110 01001110
日 01100101 11100101
报 01100010 10100101
```


这个字符串总共占用了18个字节，但是对比中英文的二进制码，可以发现，英文前9位都是0！浪费啊，浪费硬盘，浪费流量。

怎么办？

UTF。

UTF-8是这样做的：

1. 单字节的字符，字节的第一位设为0，对于英语文本，UTF-8码只占用一个字节，和ASCII码完全相同；

2. n个字节的字符(n>1)，第一个字节的前n位设为1，第n+1位设为0，后面字节的前两位都设为10，这n个字节的其余空位填充该字符unicode码，高位用0补足。

这样就形成了如下的UTF-8标记位：

0xxxxxxx

110xxxxx 10xxxxxx

1110xxxx 10xxxxxx 10xxxxxx

11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

... ...

于是，”It's 知乎日报“就变成了：


```
I 01001001
t 01110100
' 00100111
s 01110011
  00100000
知 11100111 10011111 10100101
乎 11100100 10111001 10001110
日 11100110 10010111 10100101
报 11100110 10001010 10100101
```


和上边的方案对比一下，英文短了，每个中文字符却多用了一个字节。但是整个字符串只用了17个字节，比上边的18个短了一点点。

另外一个汉字在UTF-8编码的情况下可能需要三个字节，而使用gbk两个字节就足够了，所以有时候为了节省流量和硬盘，在使用中文的时候，也有用gbk或者gbk2312编码的。


















