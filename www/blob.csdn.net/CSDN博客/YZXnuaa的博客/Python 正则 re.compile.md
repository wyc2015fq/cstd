# Python 正则 re.compile - YZXnuaa的博客 - CSDN博客
2018年02月22日 13:53:44[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：224
**首先我们看下官方文档里关于的compile的说明：**
**[python]**[view plain](http://blog.csdn.net/drdairen/article/details/51134816#)[copy](http://blog.csdn.net/drdairen/article/details/51134816#)
- re.compile(pattern, flags=0)  
- Compile a regular expression pattern into a regular expression object, which can be used for matching using its match() and search() methods, described below.The expression’s behaviour can be modified by specifying a flags value. Values can be any of the following variables, combined using bitwise OR (the | operator).  
**[python]**[view plain](http://blog.csdn.net/drdairen/article/details/51134816#)[copy](http://blog.csdn.net/drdairen/article/details/51134816#)
- </pre><pre name="code"class="python">The sequence：  
- prog = re.compile(pattern)  
- result = prog.match(string)  
- <strong><span style="font-size:24px;">is equivalent to</span></strong>  
- result = re.match(pattern, string)  
- but using re.compile() and saving the resulting regular expression object for reuse is more efficient when the expression will be used several times in a single program.  
- 
- Note：The compiled versions of the most recent patterns passed to re.compile() and the module-level matching functions are cached, so programs that use only a few regular expressions at a time needn’t worry about compiling regular expressions.  
**下面是flag dotall的说明：**
**[python]**[view plain](http://blog.csdn.net/drdairen/article/details/51134816#)[copy](http://blog.csdn.net/drdairen/article/details/51134816#)
- re.DOTALL  
- Make the '.' special character match any character at all, including a newline; without this flag, '.' will match anything except a newline.  
**下面举个栗子进行讲解：**
**[python]**[view plain](http://blog.csdn.net/drdairen/article/details/51134816#)[copy](http://blog.csdn.net/drdairen/article/details/51134816#)
- >>> import re  
- >>> s = "adfad asdfasdf asdfas asdfawef asd adsfas "
- 
- >>> reObj1 = re.compile('((\w+)\s+\w+)')  
- >>> reObj1.findall(s)  
- [('adfad asdfasdf', 'adfad'), ('asdfas asdfawef', 'asdfas'), ('asd adsfas', 'asd')]  
- 
- >>> reObj2 = re.compile('(\w+)\s+\w+')  
- >>> reObj2.findall(s)  
- ['adfad', 'asdfas', 'asd']  
- 
- >>> reObj3 = re.compile('\w+\s+\w+')  
- >>> reObj3.findall(s)  
- ['adfad asdfasdf', 'asdfas asdfawef', 'asd adsfas']  
代码参考下图进行理解：
![](https://img-blog.csdn.net/20160412165147753?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对于上面的代码，我们可以看到：
findall函数返回的总是正则表达式在字符串中所有匹配结果的列表list，此处主要讨论列表中“结果”的展现方式，即findall中返回列表中每个元素包含的信息。
1.当给出的正则表达式中带有多个括号时，列表的元素为多个字符串组成的tuple，tuple中字符串个数与括号对数相同，字符串内容与每个括号内的正则表达式相对应，并且排放顺序是按括号出现的顺序。
2.当给出的正则表达式中带有一个括号时，列表的元素为字符串，此字符串的内容与括号中的正则表达式相对应（不是整个正则表达式的匹配内容）。
3.当给出的正则表达式中不带括号时，列表的元素为字符串，此字符串为整个正则表达式匹配的内容。
《《《《《《《《《《《《《《《《《
对于.re.compile.findall(data)之后的数据，我们可以通过list的offset索引或者str.join()函数来使之变成str字符串，从而进行方便的处理，下面是python3.5中str.join()的文档：
**[python]**[view plain](http://blog.csdn.net/drdairen/article/details/51134816#)[copy](http://blog.csdn.net/drdairen/article/details/51134816#)
- str.join(iterable)  
- Return a string which is the concatenation of the strings in the iterable iterable. A TypeError will be raised if there are any non-string values in iterable, including bytes objects.The separator between elements is the string providing this method.  
