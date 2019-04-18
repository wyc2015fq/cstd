# python之正则表达式 - nosmatch的专栏 - CSDN博客
2011年07月13日 09:57:12[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：400标签：[python																[正则表达式																[string																[import																[output																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=output&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=正则表达式&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[脚本](https://blog.csdn.net/HDUTigerkin/article/category/845589)
                 1. 正则表达式基础
### 1.1. 简单介绍
正则表达式并不是Python的一部分。正则表达式是用于处理字符串的强大工具，拥有自己独特的语法以及一个独立的处理引擎，效率上可能不如str自带的方法，但功能十分强大。得益于这一点，在提供了正则表达式的语言里，正则表达式的语法都是一样的，区别只在于不同的编程语言实现支持的语法数量不同；但不用担心，不被支持的语法通常是不常用的部分。如果已经在其他语言里使用过正则表达式，只需要简单看一看就可以上手了。
下图展示了使用正则表达式进行匹配的流程： 
![](http://hi.csdn.net/attachment/201107/13/0_1310522302Kqxc.gif)
正则表达式的大致匹配过程是：依次拿出表达式和文本中的字符比较，如果每一个字符都能匹配，则匹配成功；一旦有匹配不成功的字符则匹配失败。如果表达式中有量词或边界，这个过程会稍微有一些不同，但也是很好理解的，看下图中的示例以及自己多使用几次就能明白。
下图列出了Python支持的正则表达式元字符和语法：
![](http://hi.csdn.net/attachment/201107/13/0_1310522241JKJO.gif)
### 1.2. 数量词的贪婪模式与非贪婪模式
正则表达式通常用于在文本中查找匹配的字符串。Python里数量词默认是贪婪的（在少数语言里也可能是默认非贪婪），总是尝试匹配尽可能多的字符；非贪婪的则相反，总是尝试匹配尽可能少的字符。例如：正则表达式"ab*"如果用于查找"abbbc"，将找到"abbb"。而如果使用非贪婪的数量词"ab*?"，将找到"a"。
### 1.3. 反斜杠的困扰
与大多数编程语言相同，正则表达式里使用"\"作为转义字符，这就可能造成反斜杠困扰。假如你需要匹配文本中的字符"\"，那么使用编程语言表示的正则表达式里将需要4个反斜杠"\\\\"：前两个和后两个分别用于在编程语言里转义成反斜杠，转换成两个反斜杠后再在正则表达式里转义成一个反斜杠。Python里的原生字符串很好地解决了这个问题，这个例子中的正则表达式可以使用r"\\"表示。同样，匹配一个数字的"\\d"可以写成r"\d"。有了原生字符串，你再也不用担心是不是漏写了反斜杠，写出来的表达式也更直观。
### 1.4. 匹配模式
正则表达式提供了一些可用的匹配模式，比如忽略大小写、多行匹配等，这部分内容将在Pattern类的工厂方法re.compile(pattern[, flags])中一起介绍。
## 2. re模块
### 2.1. 开始使用re
Python通过re模块提供对正则表达式的支持。使用re的一般步骤是先将正则表达式的字符串形式编译为Pattern实例，然后使用Pattern实例处理文本并获得匹配结果（一个Match实例），最后使用Match实例获得信息，进行其他的操作。
|`01`|```python# encoding: UTF-8```|
|`02`|```pythonimport``````pythonre```|
|`03`|` `|
|`04`|```python# 将正则表达式编译成Pattern对象```|
|`05`|```pythonpattern``````python=``````pythonre.``````pythoncompile``````python(r``````python'hello'``````python)```|
|`06`|` `|
|`07`|```python# 使用Pattern匹配文本，获得匹配结果，无法匹配时将返回None```|
|`08`|```pythonmatch``````python=``````pythonpattern.match(``````python'hello world!'``````python)```|
|`09`|` `|
|`10`|```pythonif``````pythonmatch:```|
|`11`|`    ````python# 使用Match获得分组信息```|
|`12`|`    ````pythonprint``````pythonmatch.group()```|
|`13`|` `|
|`14`|```python### 输出 ###```|
|`15`|```python# hello```|
**re.compile(strPattern[, flag]): **
这个方法是Pattern类的工厂方法，用于将字符串形式的正则表达式编译为Pattern对象。 第二个参数flag是匹配模式，取值可以使用按位或运算符'|'表示同时生效，比如re.I | re.M。另外，你也可以在regex字符串中指定模式，比如re.compile('pattern', re.I | re.M)与re.compile('(?im)pattern')是等价的。 
可选值有： 
- re.**I**(re.IGNORECASE): 忽略大小写（括号内是完整写法，下同） 
- **M**(MULTILINE): 多行模式，改变'^'和'$'的行为（参见上图） 
- **S**(DOTALL): 点任意匹配模式，改变'.'的行为 
- **L**(LOCALE): 使预定字符类 \w \W \b \B \s \S 取决于当前区域设定 
- **U**(UNICODE): 使预定字符类 \w \W \b \B \s \S \d \D 取决于unicode定义的字符属性 
- **X**(VERBOSE): 详细模式。这个模式下正则表达式可以是多行，忽略空白字符，并可以加入注释。以下两个正则表达式是等价的： 
|`1`|```pythona``````python=``````pythonre.``````pythoncompile``````python(r``````python"""\d +  # the integral part```|
|`2`|`                   ````python\.    # the decimal point```|
|`3`|`                   ````python\d *  # some fractional digits"""``````python, re.X)```|
|`4`|```pythonb``````python=``````pythonre.``````pythoncompile``````python(r``````python"\d+\.\d*"``````python)```|
re提供了众多模块方法用于完成正则表达式的功能。这些方法可以使用Pattern实例的相应方法替代，唯一的好处是少写一行re.compile()代码，但同时也无法复用编译后的Pattern对象。这些方法将在Pattern类的实例方法部分一起介绍。如上面这个例子可以简写为：
[view source](#viewSource)[print](#printSource)[?](#about)
|`1`|```pythonm``````python=``````pythonre.match(r``````python'hello'``````python,``````python'hello world!'``````python)```|
|`2`|```pythonprint``````pythonm.group()```|
re模块还提供了一个方法escape(string)，用于将string中的正则表达式元字符如*/+/?等之前加上转义符再返回，在需要大量匹配元字符时有那么一点用。
### 2.2. Match
Match对象是一次匹配的结果，包含了很多关于此次匹配的信息，可以使用Match提供的可读属性或方法来获取这些信息。
属性：
- **string**: 匹配时使用的文本。 
- **re**: 匹配时使用的Pattern对象。 
- **pos**: 文本中正则表达式开始搜索的索引。值与Pattern.match()和Pattern.seach()方法的同名参数相同。 
- **endpos**: 文本中正则表达式结束搜索的索引。值与Pattern.match()和Pattern.seach()方法的同名参数相同。 
- **lastindex**: 最后一个被捕获的分组在文本中的索引。如果没有被捕获的分组，将为None。 
- **lastgroup**: 最后一个被捕获的分组的别名。如果这个分组没有别名或者没有被捕获的分组，将为None。 
方法：
- **group([group1, …]): **
获得一个或多个分组截获的字符串；指定多个参数时将以元组形式返回。group1可以使用编号也可以使用别名；编号0代表整个匹配的子串；不填写参数时，返回group(0)；没有截获字符串的组返回None；截获了多次的组返回最后一次截获的子串。 
- **groups([default]):**
以元组形式返回全部分组截获的字符串。相当于调用group(1,2,…last)。default表示没有截获字符串的组以这个值替代，默认为None。 
- **groupdict([default]): **返回以有别名的组的别名为键、以该组截获的子串为值的字典，没有别名的组不包含在内。default含义同上。 
- **start([group]):**
返回指定的组截获的子串在string中的起始索引（子串第一个字符的索引）。group默认值为0。 
- **end([group]): **返回指定的组截获的子串在string中的结束索引（子串最后一个字符的索引+1）。group默认值为0。 
- **span([group]): **返回(start(group), end(group))。 
- **expand(template):**
将匹配到的分组代入template中然后返回。template中可以使用\id或\g<id>、\g<name>引用分组，但不能使用编号0。\id与\g<id>是等价的；但\10将被认为是第10个分组，如果你想表达\1之后是字符'0'，只能使用\g<1>0。 
|`01`|```pythonimport``````pythonre```|
|`02`|```pythonm``````python=``````pythonre.match(r``````python'(\w+) (\w+)(?P<sign>.*)'``````python,``````python'hello world!'``````python)```|
|`03`|` `|
|`04`|```pythonprint``````python"m.string:"``````python, m.string```|
|`05`|```pythonprint``````python"m.re:"``````python, m.re```|
|`06`|```pythonprint``````python"m.pos:"``````python, m.pos```|
|`07`|```pythonprint``````python"m.endpos:"``````python, m.endpos```|
|`08`|```pythonprint``````python"m.lastindex:"``````python, m.lastindex```|
|`09`|```pythonprint``````python"m.lastgroup:"``````python, m.lastgroup```|
|`10`|` `|
|`11`|```pythonprint``````python"m.group(1,2):"``````python, m.group(``````python1``````python,``````python2``````python)```|
|`12`|```pythonprint``````python"m.groups():"``````python, m.groups()```|
|`13`|```pythonprint``````python"m.groupdict():"``````python, m.groupdict()```|
|`14`|```pythonprint``````python"m.start(2):"``````python, m.start(``````python2``````python)```|
|`15`|```pythonprint``````python"m.end(2):"``````python, m.end(``````python2``````python)```|
|`16`|```pythonprint``````python"m.span(2):"``````python, m.span(``````python2``````python)```|
|`17`|```pythonprint``````pythonr``````python"m.expand(r'\2 \1\3'):"``````python, m.expand(r``````python'\2 \1\3'``````python)```|
|`18`|` `|
|`19`|```python### output ###```|
|`20`|```python# m.string: hello world!```|
|`21`|```python# m.re: <_sre.SRE_Pattern object at 0x016E1A38>```|
|`22`|```python# m.pos: 0```|
|`23`|```python# m.endpos: 12```|
|`24`|```python# m.lastindex: 3```|
|`25`|```python# m.lastgroup: sign```|
|`26`|```python# m.group(1,2): ('hello', 'world')```|
|`27`|```python# m.groups(): ('hello', 'world', '!')```|
|`28`|```python# m.groupdict(): {'sign': '!'}```|
|`29`|```python# m.start(2): 6```|
|`30`|```python# m.end(2): 11```|
|`31`|```python# m.span(2): (6, 11)```|
|`32`|```python# m.expand(r'\2 \1\3'): world hello!```|
2.3. Pattern
Pattern对象是一个编译好的正则表达式，通过Pattern提供的一系列方法可以对文本进行匹配查找。
Pattern不能直接实例化，必须使用re.compile()进行构造。
Pattern提供了几个可读属性用于获取表达式的相关信息：
- pattern: 编译时用的表达式字符串。 
- flags: 编译时用的匹配模式。数字形式。 
- groups: 表达式中分组的数量。 
- groupindex: 以表达式中有别名的组的别名为键、以该组对应的编号为值的字典，没有别名的组不包含在内。 
|`01`|```pythonimport``````pythonre```|
|`02`|```pythonp``````python=``````pythonre.``````pythoncompile``````python(r``````python'(\w+) (\w+)(?P<sign>.*)'``````python, re.DOTALL)```|
|`03`|` `|
|`04`|```pythonprint``````python"p.pattern:"``````python, p.pattern```|
|`05`|```pythonprint``````python"p.flags:"``````python, p.flags```|
|`06`|```pythonprint``````python"p.groups:"``````python, p.groups```|
|`07`|```pythonprint``````python"p.groupindex:"``````python, p.groupindex```|
|`08`|` `|
|`09`|```python### output ###```|
|`10`|```python# p.pattern: (\w+) (\w+)(?P<sign>.*)```|
|`11`|```python# p.flags: 16```|
|`12`|```python# p.groups: 3```|
|`13`|```python# p.groupindex: {'sign': 3}```|
实例方法[ | re模块方法]：
- **match(string[, pos[, endpos]]) | re.match(pattern, string[, flags]): **这个方法将从string的pos下标处起尝试匹配pattern；如果pattern结束时仍可匹配，则返回一个Match对象；如果匹配过程中pattern无法匹配，或者匹配未结束就已到达endpos，则返回None。 
pos和endpos的默认值分别为0和len(string)；re.match()无法指定这两个参数，参数flags用于编译pattern时指定匹配模式。 
注意：这个方法并不是完全匹配。当pattern结束时若string还有剩余字符，仍然视为成功。想要完全匹配，可以在表达式末尾加上边界匹配符'$'。 
示例参见2.1小节。 
- **search(string[, pos[, endpos]]) | re.search(pattern, string[, flags]): **这个方法用于查找字符串中可以匹配成功的子串。从string的pos下标处起尝试匹配pattern，如果pattern结束时仍可匹配，则返回一个Match对象；若无法匹配，则将pos加1后重新尝试匹配；直到pos=endpos时仍无法匹配则返回None。 
pos和endpos的默认值分别为0和len(string))；re.search()无法指定这两个参数，参数flags用于编译pattern时指定匹配模式。 
|`01`|```python# encoding: UTF-8```|
|`02`|```pythonimport``````pythonre```|
|`03`|` `|
|`04`|```python# 将正则表达式编译成Pattern对象```|
|`05`|```pythonpattern``````python=``````pythonre.``````pythoncompile``````python(r``````python'world'``````python)```|
|`06`|` `|
|`07`|```python# 使用search()查找匹配的子串，不存在能匹配的子串时将返回None```|
|`08`|```python# 这个例子中使用match()无法成功匹配```|
|`09`|```pythonmatch``````python=``````pythonpattern.search(``````python'hello world!'``````python)```|
|`10`|` `|
|`11`|```pythonif``````pythonmatch:```|
|`12`|`    ````python# 使用Match获得分组信息```|
|`13`|`    ````pythonprint``````pythonmatch.group()```|
|`14`|` `|
|`15`|```python### 输出 ###```|
|`16`|```python# world```|
**split(string[, maxsplit]) | re.split(pattern, string[, maxsplit]): **按照能够匹配的子串将string分割后返回列表。maxsplit用于指定最大分割次数，不指定将全部分割。 
|`1`|```pythonimport``````pythonre```|
|`2`|` `|
|`3`|```pythonp``````python=``````pythonre.``````pythoncompile``````python(r``````python'\d+'``````python)```|
|`4`|```pythonprint``````pythonp.split(``````python'one1two2three3four4'``````python)```|
|`5`|` `|
|`6`|```python### output ###```|
|`7`|```python# ['one', 'two', 'three', 'four', '']```|
**findall(string[, pos[, endpos]]) | re.findall(pattern, string[, flags]): **搜索string，以列表形式返回全部能匹配的子串。 
|`1`|```pythonimport``````pythonre```|
|`2`|` `|
|`3`|```pythonp``````python=``````pythonre.``````pythoncompile``````python(r``````python'\d+'``````python)```|
|`4`|```pythonprint``````pythonp.findall(``````python'one1two2three3four4'``````python)```|
|`5`|` `|
|`6`|```python### output ###```|
|`7`|```python# ['1', '2', '3', '4']```|
**finditer(string[, pos[, endpos]]) | re.finditer(pattern, string[, flags]): **搜索string，返回一个顺序访问每一个匹配结果（Match对象）的迭代器。 
|`1`|```pythonimport``````pythonre```|
|`2`|` `|
|`3`|```pythonp``````python=``````pythonre.``````pythoncompile``````python(r``````python'\d+'``````python)```|
|`4`|```pythonfor``````pythonm``````pythonin``````pythonp.finditer(``````python'one1two2three3four4'``````python):```|
|`5`|`    ````pythonprint``````pythonm.group(),```|
|`6`|` `|
|`7`|```python### output ###```|
|`8`|```python# 1 2 3 4```|
**sub(repl, string[, count]) | re.sub(pattern, repl, string[, count]): **使用repl替换string中每一个匹配的子串后返回替换后的字符串。 
当repl是一个字符串时，可以使用\id或\g<id>、\g<name>引用分组，但不能使用编号0。 
当repl是一个方法时，这个方法应当只接受一个参数（Match对象），并返回一个字符串用于替换（返回的字符串中不能再引用分组）。 
count用于指定最多替换次数，不指定时全部替换。 
|`01`|```pythonimport``````pythonre```|
|`02`|` `|
|`03`|```pythonp``````python=``````pythonre.``````pythoncompile``````python(r``````python'(\w+) (\w+)'``````python)```|
|`04`|```pythons``````python=``````python'i say, hello world!'```|
|`05`|` `|
|`06`|```pythonprint``````pythonp.sub(r``````python'\2 \1'``````python, s)```|
|`07`|` `|
|`08`|```pythondef``````pythonfunc(m):```|
|`09`|`    ````pythonreturn``````pythonm.group(``````python1``````python).title()``````python+``````python' '``````python+``````pythonm.group(``````python2``````python).title()```|
|`10`|` `|
|`11`|```pythonprint``````pythonp.sub(func, s)```|
|`12`|` `|
|`13`|```python### output ###```|
|`14`|```python# say i, world hello!```|
|`15`|```python# I Say, Hello World!```|
**subn(repl, string[, count]) |re.sub(pattern, repl, string[, count]): **返回 (sub(repl, string[, count]), 替换次数)。 
|`01`|```pythonimport``````pythonre```|
|`02`|` `|
|`03`|```pythonp``````python=``````pythonre.``````pythoncompile``````python(r``````python'(\w+) (\w+)'``````python)```|
|`04`|```pythons``````python=``````python'i say, hello world!'```|
|`05`|` `|
|`06`|```pythonprint``````pythonp.subn(r``````python'\2 \1'``````python, s)```|
|`07`|` `|
|`08`|```pythondef``````pythonfunc(m):```|
|`09`|`    ````pythonreturn``````pythonm.group(``````python1``````python).title()``````python+``````python' '``````python+``````pythonm.group(``````python2``````python).title()```|
|`10`|` `|
|`11`|```pythonprint``````pythonp.subn(func, s)```|
|`12`|` `|
|`13`|```python### output ###```|
|`14`|```python# ('say i, world hello!', 2)```|
|`15`|```python# ('I Say, Hello World!', 2)```|

