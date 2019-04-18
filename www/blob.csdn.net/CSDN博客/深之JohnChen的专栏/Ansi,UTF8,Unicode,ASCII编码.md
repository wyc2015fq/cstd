# Ansi,UTF8,Unicode,ASCII编码 - 深之JohnChen的专栏 - CSDN博客

2008年05月07日 22:33:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：915标签：[character																[存储																[文档																[语言																[游戏																[平台](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=character&t=blog)
个人分类：[数据编码与解码](https://blog.csdn.net/byxdaz/article/category/8464620)


# ********Ansi,UTF8,Unicode,ASCII编码********

1.  ASCII和Ansi编码
**    字符内码**(charcter code)指的是用来代表字符的内码.读者在输入和存储文档时都要使用内码,内码分为 
**    单字节内码** -- Single-Byte character sets (SBCS),可以支持256个字符编码. 
**双字节内码** -- Double-Byte character sets)(DBCS),可以支持65000个字符编码.

前者即为ASCII编码，后者对应ANSI.

至于简体中文编码GB2312，实际上它是ＡＮＳＩ的一个代码页936

2. Unicode

如上，ANSI有很多代码页，使用不同代码页的内码无法在其他代码也正常显示，这就是为什么日文版／繁体中文版游戏无法在简体中文平台直接显示的原因．

    Unicode也是一种字符编码方法，不过它是由国际组织设计，可以容纳全世界所有语言文字的编码方案．它是一种２字节编码，能够提供６５５３６个字符， 这个数字是不够表示所有的字符的（汉语就有５５０００多字符），所以，通过一个代理对的机制来实现附加的９１７，４７６个字符表示，以达到所有字符都具有 唯一编码．

３．Unicode和BigEndianUnicode

    这两者只是存储顺序不同，如＂A＂的unicode编码为65 00

    其BigEndianUnicode编码为00 65

4.  UTF-8     这是为传输而设计的编码，其系列还有UTF-7和UTF-16

    其中UTF-16和Unicode编码大致一样, UTF-8就是以8位为单元对Unicode进行编码。从Unicode到UTF-8的编码方式如下：

　　　　　Unicode编码(16进制)　　　　　　UTF-8 字节流(二进制)

　　　　　0000 - 007F　　　　　　　　　0xxxxxxx

　　　　　0080 - 07FF　　　　　　　　　110xxxxx 10xxxxxx

　　　　　0800 - FFFF　　　　　　　　　1110xxxx 10xxxxxx 10xxxxxx

　 　例如“汉”字的Unicode编码是6C49。6C49在0800-FFFF之间，所以肯定要用3字节模板了：1110xxxx 10xxxxxx 10xxxxxx。将6C49写成二进制是：0110 110001 001001， 用这个比特流依次代替模板中的x，得到：11100110 10110001 10001001，即E6 B1 89。


