
# Python 学习笔记-字符串 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月16日 20:33:30[Jlins](https://me.csdn.net/dyllove98)阅读数：2076


今天学习Python对字符串的一些基础处理，感觉对于工作中的自动化脚本发送CLI命令会很有帮助。
首先最重要的是%，标称"转换说明符(conversion specifier)”，用于字符串格式化。
左侧放置一个字符串（格式化字符串），而右侧放置希望被格式化的值（待格式化的值）
|1
|2
|3
|4
|5
|left =|"Hello,%s good "|\# %s 表示期望被格式化的类型
|right =|"she's"|print left % right           \# %用来隔开格式化字符串和待格式化值
|Hello,she's good
|

注意，如果不在%后加s, 程序报错说
TypeError: float argument required, not str
如果right不是字符串，则会用str将其转化为字符串。
|1
|2
|3
|4
|print|"Price of eggs: $%d"|%|42
|print|"Price of eggs in HEX: $%x"|%|42
|Price of eggs: $|42
|Price of eggs in HEX: $2a
|


除此之外，字符串模块string 还提供了很多有用的方法，例如Template中的subsutitute方法用以替换字符串。
|1
|2
|3
|4
|5
|6
|7
|from string|import|Templates
|s=Template(|"$x loves some one"|)
|print (s.substitute(x=|'she'|))
|print s
|she loves some one
|<string.Template object at|0x105bc1350|>
|

第一次打印的是被替换后的字符串，第二次打印的是模板。
常用的字符串操作方法还有以下几种：
find ,等同于 in
|1
|2
|3
|4
|5
|6
|7
|8
|s=|"the best movie"|print s.find(|'movie'|)
|print|'movie'|in s
|print s.find(|'movie'|,|10|)  \#提供起始点，从index|10|开始找
|print s.find(|'movie'|,|1|,|5|) \#提供起始点和结束点，从index|1|找到index|59
|True
|-|1
|-|1
|

join & split, 连接和分割字符串
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|from macpath|import|join
|s=[|' '|,|'root'|,|'home'|]
|print|'//'|.join(s)
|s1=|'C:'|+|'\\'|.join(s)
|print s1
|print s1.split(|'\\'|)
|//root//home
|C: \root\home
|[|'C: '|,|'root'|,|'home'|]
|

这里需要注意的是\\, 如果只写作\, 由于非原始字符串会把   \  认作是转义符号，所以程序理解的是\r这个特殊的ASCII符号，既回车。
|1
|2
|3
|4
|5
|s1 =|'C:\root\home'
|print s1
|C:
|oot\home
|

所以我们使用\\, 用转义符号\去转义\, 即是告诉程序 \ 是字符串的一部分。
还有一种方法就是使用原始字符串，它对于反斜线不会特殊对待：

|1
|2
|3
|s=[|' '|,r|'root'|,|'home'|]
|print|'C:'|+|'/'|.join(s)
|C: /root/home
|

strip，去除字符串两侧的字符 （默认为空格）

translate, 同replace,但可以同时进行多个替换，效率更高。
例如，要同时将字符串中的c替换成k, s替换成z.
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|from string|import|maketrans
|table = maketrans(|'cs'|,|'kz'|)  \#建立一张替换规则表
|print len(table)
|print|'this is a magnificent day!'|.translate(table,|'!'|)
|\#第二个参数用来指定要删除的字符
|256
|thiz iz a magnifikent day
|

除以上外，还有lower, replace, capitalize等不常用方法。
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

