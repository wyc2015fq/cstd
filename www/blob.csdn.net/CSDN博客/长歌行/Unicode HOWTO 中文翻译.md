# Unicode HOWTO 中文翻译 - 长歌行 - CSDN博客





2017年08月08日 23:15:36[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：620
个人分类：[python](https://blog.csdn.net/iuhsihsow/article/category/5828755)

所属专栏：[Python与GIS](https://blog.csdn.net/column/details/pygis.html)









英文原文: https://docs.python.org/3/howto/unicode.html

最近要用到这方面内容，为了加深理解，以及分享知识，尝试翻译本文。虽然在外企工作，但是第一次翻译英文文章。。耗时两小时，希望能得到支持。

译文如下：




本文讨论了Python对Unicode的支持，并解释了人们在尝试使用Unicode时常遇到的各种问题。



## Unicode简介[](https://docs.python.org/3/howto/unicode.html#introduction-to-unicode)


### 字符代码历史[](https://docs.python.org/3/howto/unicode.html#history-of-character-codes)

1968年，美国信息交换标准代码（American Standard Code for Information Interchange，缩写ASCII）诞生。ASCII定义了各种字符的数字代码，数值从0到127。例如，小写字母“a”被分配97作为其代码值。

ASCII是美国制定的标准，因此它只定义了非重音字符。有'e'，但没有'é'或'Í'。这意味着需要重音字符的语言不能用ASCII表示。（实际上，缺少的口音对于英语也是重要的，其中包含‘naïve’ 和‘café’这样的词语，一些出版物的样式需要拼写成“coöperate”这样子）。

有一段时间，人们的程序中都没有重音字符的表示。在20世纪80年代中期，由法国人编写的Apple II BASIC程序可能会是这样子：





PRINT"MISE A JOUR TERMINEE"PRINT"PARAMETRES ENREGISTRES"


这些消息应该包含口音（terminée, paramètre, enregistrés），但是对于读法语的人来说，这些词都是错误的。

在20世纪80年代，几乎所有的个人计算机都是8位，这意味着字节可以保持从0到255的值。ASCII码只有127个，所以有些机器分配128到255之间的重音符号。不同的机器有不同的代码，但这会导致交换文件的问题。最终出现了128-255范围内各种常用的值集。一些是由国际标准化组织界定的真正标准，另一些约定俗称的规定则是用某个公司或者其他（方面）发明和推动的。

255个字符不是很多。例如，您不能将西欧使用的重音字符和用于俄语的西里尔字母表适用于128-255范围，因为有超过128个这样的字符。

您可以使用不同的代码（所有您的俄语文件（称为KOI8的编码系统中的所有俄语文件，所有法文文件在不同的编码系统中称为Latin1），但如果您想编写引用俄罗斯文本的法文文本呢？在20世纪80年代，人们开始想解决这个问题，开始了Unicode标准化工作。

Unicode开始使用16位字符而不是8位字符。16位表示您有2 ^ 16 = 65,536个不同的值可用，可以从许多不同的字母表示许多不同的字符; 最初的目标是让Unicode包含每一种人类语言的字母。事实证明，即使16位还不足以达到这一目标，而现代的Unicode规范使用更广泛的代码范围，即0到1,114,111（ ```0x10FFFF` in
 base 16）。

ISO 10646则是与之相关的标准. Unicode和ISO 10646最初是分开的，但在1.1版本的Unicode标准修订中合二为一。（这是对Unicode的历史的的高度简化的讨论，精确的历史细节对于了解如何有效地使用Unicode来说不是必需的，如果你好奇的话，请参考参考文献中的Unicode联盟网站或[](https://en.wikipedia.org/wiki/Unicode#History)[Wikipedia
 entry for Unicode](https://en.wikipedia.org/wiki/Unicode#History)以了解更多信息。）




### 定义[](https://docs.python.org/3/howto/unicode.html#definitions)

**字符(character)**是文本中最小的组件。'A'，'B'，'C'等都是不同的字符。“È”和“Í”也是如此。字符是抽象的，根据你所说的语言或语境而有所不同。例如，欧姆（Ω）的符号通常被绘制得很像希腊字母中的大写字母欧米茄（Ω）（它们在某些字体中甚至可能是相同的），但是这两个不同的字符具有不同的含义。

Unicode标准描述了字符如何用码位表示。码位是一个整数值，通常以base 16表示。在标准中，码位使用符号`U+12CA来表示`值为`0x12ca`（十进制为4,810）的字符。Unicode标准包含许多类似如下的表，表中列出了字符及其相应代码：






0061    'a'; LATIN SMALL LETTER A
0062    'b'; LATIN SMALL LETTER B
0063    'c'; LATIN SMALL LETTER C
...
007B    '{'; LEFT CURLY BRACKET






严格地说，这些定义意味着“this is character `U+12CA```” 是没有意义的。 `U+12CA`是一个码位，代表一些特定的字符; 在这种情况下，它代表字符“埃塞俄比亚音节：
 WI”。在非正式情况下，码位和字符之间的区别有时会被遗忘。

一个字符在屏幕上或纸上被一组称为**字形**的图形元素表示。例如，大写字母A的字形是两个对角线笔画和水平笔画，尽管确切的细节将取决于所使用的字体。大多数Python代码不需要担心字形； 找出正确的字形显示通常是GUI工具包或终端的字体渲染器的工作。



### 编码[](https://docs.python.org/3/howto/unicode.html#encodings)

总结上一节：一个Unicode字符串是一个码位序列，它们是从0到`0x10FFFF`（十进制为1,114,111）的数字。该序列需要表示为内存中的一组字节（意为0到255之间的值）。将Unicode字符串转换为字节序列的规则称为**编码**。

最初时候的编码方法是采用一个32位整数的数组。如此，字符串“Python”将如下所示：






   P           y           t           h           o           n
0x50 00 00 00 79 00 00 00 74 00 00 00 68 00 00 00 6f 00 00 00 6e 00 00 00
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23











这种表示是直接的，但使用它却会带来了一些问题。
- 它不是便携式的，不同的处理器不同地排序字节。
- 太浪费空间了。在大多数文本中，大多数码位小于127，或小于255，所以大量的空间占用了`0x00` 字节。与ASCII表示所需的6个字节相比，上述字符串需要24个字节。增加了不必要的RAM使用（桌面计算机有千兆字节的RAM，并且字符串通常不是那么大），但是将磁盘和网络带宽的使用扩展到4倍是不能容忍的。
- 它与现有的C函数不兼容`strlen()`，因此需要使用新的宽字符串函数。
- 许多互联网标准是根据文本数据定义的，不能处理内嵌零字节的内容。

一般来说，人们不会使用这种编码，而是选择更有效和便利的其他编码。UTF-8可能是最常支持的编码； 将在下面讨论。

编码不需要处理每个可能的Unicode字符，大多数编码都不需要。例如，将Unicode字符串转换为ASCII编码的规则很简单；对于每个码位：
- 如果码位<128，则每个字节与码位的值相同。
- 如果码位为128或更大，那么Unicode字符串不能在此编码中表示。（[`UnicodeEncodeError`](https://docs.python.org/3/library/exceptions.html#UnicodeEncodeError)在这种情况下，Python引发了异常。）

Latin-1（Latin：拉丁语），也称为ISO-8859-1，是类似的编码。Unicode码位0-255与Latin-1值相同，因此转换为该编码只需要将码位转换为字节值；如果遇到大于255的码位，则该字符串不能被编码为Latin-1。

编码不一定是简单的如Latin-1一样的一对一映射。考虑在IBM大型机上使用的IBM的EBCDIC。字母值不在一个块中：'a'到'i'的值从129到137，但是'j'到'r'是145到153。如果你想使用EBCDIC作为编码，你可能会使用某种查找表来执行转换，但这只是一个内部细节。

UTF-8是最常用的编码之一。UTF代表“Unicode转换格式（Unicode Transformation Format）”，“8”表示编码中使用8-bit（8位）。（还有一个UTF-16和UTF-32编码，但是它们比UTF-8更少使用。）

**UTF-8使用以下规则：**
- 如果码位<128，则由相应的字节值表示。
- 如果码位是> = 128，则将其转换为两个，三个或四个字节的序列，其中序列的每个字节都在128到255之间。

**UTF-8方便之处：**
- 它可以处理任何Unicode码位。
- Unicode字符串变成一个不包含嵌入零字节的字节序列。这避免了字节排序问题，并且意味着UTF-8字符串可以由C函数处理，例如`strcpy()`通过那些还不能处理零字节的协议发送。
- 一串ASCII文本也是有效的UTF-8文本。
- UTF-8相当紧凑; 大多数常用字符可以用一个或两个字节表示。
- 如果字节损坏或丢失，可以确定下一个UTF-8编码码位的开始并重新同步。随机的8位数据也不太可能看起来像是有效的UTF-8。



### 参考[](https://docs.python.org/3/howto/unicode.html#references)

在[](http://www.unicode.org/)[Unicode
 Consortium site](http://www.unicode.org/)有字符表，一个词汇表和统一规范的PDF版本。~~准备接受阅读挑战吧~~。 Unicode的起源和发展的[年表](http://www.unicode.org/history/)也可在网站上获得。

为了帮助理解这个标准，Jukka Korpela写[了一本介绍指南](https://www.cs.tut.fi/~jkorpela/unicode/guide.html)以更好的阅读Unicode字符表。

另一个[很好的介绍性文章](http://www.joelonsoftware.com/articles/Unicode.html) 是由Joel Spolsky撰写的。如果这个介绍没有让你清楚，你应该尝试阅读这篇替代文章，然后继续。

维基百科条目通常是有帮助的; 请参见“ [字符编码](https://en.wikipedia.org/wiki/Character_encoding) ”和[UTF-8](https://en.wikipedia.org/wiki/UTF-8)的条目。




## Python的Unicode支持[](https://docs.python.org/3/howto/unicode.html#python-s-unicode-support)

现在你已经学习了Unicode的基础，我们可以看看Python的Unicode功能。


### 字符串类型[](https://docs.python.org/3/howto/unicode.html#the-string-type)

因为Python 3.0，语言设有`[str](https://docs.python.org/3/library/stdtypes.html#str)类型`，它包括含Unicode字符，这意味着使用任何字符串的创建都会使用`"unicode rocks!"``'unicode rocks!'`，或三引号字符串语法被存储为Unicode。

Python源代码的默认编码是UTF-8，因此您可以在字符串文字中简单地包含一个Unicode字符：







try:withopen('/tmp/input.txt','r')asf:...exceptOSError:# 'File not found' error message.print("Fichier non trouvé")


您可以将特殊格式的注释作为源代码的第一行或第二行，来使用不同于UTF-8的其他编码：









# -*- coding: <encoding name> -*-




旁注：Python 3还支持在标识符中使用Unicode字符：

répertoire="/tmp/records.log"withopen(répertoire,"w")asf:f.write("test\n")如果您无法在编辑器中输入特定字符，或者由于某种原因希望将源代码保留为ASCII码，则还可以使用字符串文字中的转义序列。（根据您的系统，您可能会看到实际的大写字母
 - 而不是一个u转义。）





>>> "\N{GREEK CAPITAL LETTER DELTA}"# Using the character name'\u0394'>>> "\u0394"# Using a 16-bit hex value'\u0394'>>> "\U00000394"# Using a 32-bit hex value'\u0394'
另外，可以使用以下[`decode()`](https://docs.python.org/3/library/stdtypes.html#bytes.decode)方法 创建一个字符串。此方法接受一个编码参数，如`UTF-8，以及`可选的错误参数。

该误差参数会指定，当输入字符串不能根据编码的规则被转换的响应方式。此参数的合法值是`'strict'`（抛出一个`[UnicodeDecodeError](https://docs.python.org/3/library/exceptions.html#UnicodeDecodeError)异常`）， `'replace'`（使用 `U+FFFD`，替代字符），'ignore'（忽略结果）或者'backslashreplace'（插入一个 \xNN转义序列）。以下示例显示了差异：
>>> b'\x80abc'.decode("utf-8","strict")Traceback (most recent call last):...UnicodeDecodeError: 'utf-8' codec can't decode byte 0x80 in position 0:  invalid start byte>>> b'\x80abc'.decode("utf-8","replace")'\ufffdabc'>>> b'\x80abc'.decode("utf-8","backslashreplace")'\\x80abc'>>> b'\x80abc'.decode("utf-8","ignore")'abc'
编码(Encodings)为包含编码名称的字符串。Python 3.2具有大约100种不同的编码; 请参阅[标准编码中](https://docs.python.org/3/library/codecs.html#standard-encodings)的Python库参考 列表。一些编码有多个名字; 例如，`'latin-1'`，`'iso_8859_1'`和`'8859`'是相同的编码的所有同义词。

也可以使用[`chr()`](https://docs.python.org/3/library/functions.html#chr) 内置函数创建**单字符**Unicode字符串，该函数采用整数，并返回长度为1的包含相应码位的Unicode字符串。反向操作是内置[`ord()`](https://docs.python.org/3/library/functions.html#ord)函数，它使用一个字符的Unicode字符串并返回码位值：
>>> chr()'\ue000'>>> ord('\ue000')57344


### 转换为字节（Bytes）[](https://docs.python.org/3/howto/unicode.html#converting-to-bytes)

`[bytes.decode()](https://docs.python.org/3/library/stdtypes.html#bytes.decode)的`相反方法是[`str.encode()`](https://docs.python.org/3/library/stdtypes.html#str.encode)，返回`[bytes](https://docs.python.org/3/library/stdtypes.html#bytes)`表示的使用以输入的编码方式编码的Unicode字符串。（which
 returns a [`bytes`](https://docs.python.org/3/library/stdtypes.html#bytes)representation
 of the Unicode string, encoded in the requested encoding.）

该错误的参数和 [`decode()`](https://docs.python.org/3/library/stdtypes.html#bytes.decode)的方法相同，但支持了几个可能的情况。如`'strict'`，`'ignore'`和`'replace'`（在这种情况下，插入一个问号代替unencodable字符），也有`'xmlcharrefreplace'`（插入一个XML字符引用）， `backslashreplace`（插入一个`\uNNNN`转义序列）和 `namereplace`（插入一个`\N{...}`转义序列）。

以下示例显示了不同的结果：


>>>>>> u=chr()+'abcd'+chr()>>> u.encode('utf-8')b'\xea\x80\x80abcd\xde\xb4'>>> u.encode('ascii')Traceback (most recent call last):...UnicodeEncodeError: 'ascii' codec can't encode character '\ua000' in  position 0: ordinal not in range(128)>>> u.encode('ascii','ignore')b'abcd'>>> u.encode('ascii','replace')b'?abcd?'>>> u.encode('ascii','xmlcharrefreplace')b'ꀀabcd޴'>>> u.encode('ascii','backslashreplace')b'\\ua000abcd\\u07b4'>>> u.encode('ascii','namereplace')b'\\N{YI SYLLABLE IT}abcd\\u07b4'






用于注册和访问可用编码的底层例程在[`codecs`](https://docs.python.org/3/library/codecs.html#module-codecs)模块中找到。实现新的编码也需要了解[`codecs`](https://docs.python.org/3/library/codecs.html#module-codecs)模块。但是，该模块返回的编码和解码功能通常更底层，不好用。因此该模块将不在本HOWTO中。



### Python中的Unicode文字源代码[](https://docs.python.org/3/howto/unicode.html#unicode-literals-in-python-source-code)

在Python源代码中，可以使用`\u`转义序列来编写特定的Unicode码位 ，后面是四个十六进制数字（four
 hex digits）来提供码位。`\U`转义序列是相似的，但预计8个十六进制数字，而不是4：






>>>>>> s="a\xac\u1234\u20ac\U00008000"... #     ^^^^ two-digit hex escape... #         ^^^^^^ four-digit Unicode escape... #                     ^^^^^^^^^^ eight-digit Unicode escape>>> [ord(c)forcins][97, 172, 4660, 8364, 32768]




对于大于127的码位，使用转义序列作为码位是可行的，但是如果您使用多个重音字符，就像使用法语或其他重音语言的程序一样，会变得烦恼。您也可以使用[`chr()`](https://docs.python.org/3/library/functions.html#chr)内置功能组合字符串，但这更加没有意义。

理想情况下，您希望能够用自己的语言编写文字。然后，您可以使用自己喜欢的编辑器编辑Python源代码，该编辑器将自然显示重音字符，并在运行时使用正确的字符。

Python支持默认在UTF-8中编写源代码，但如果声明使用的编码，则可以使用几乎任何编码。这是通过将特殊注释作为源文件的第一行或第二行来完成的：




#!/usr/bin/env python# -*- coding: latin-1 -*-u='abcdé'print(ord(u[-]))






该灵感来自Emacs用于指定文件本地变量的符号。Emacs支持许多不同的变量，但Python只支持“编码”。该`-*-`符号表示Emacs的该评论是特殊的; 它们对Python没有意义，但是它是一个惯例。Python会在注释中查询`coding: name或者``coding=name`

如果不包括这样的注释，则使用的默认编码将是UTF-8，如前所述。也可以看看[**PEP 263**](https://www.python.org/dev/peps/pep-0263)获取更多信息。



### Unicode属性[](https://docs.python.org/3/howto/unicode.html#unicode-properties)

Unicode规范包括有关码位的信息的数据库。对于每个定义的码位，信息包括字符的名称，其类别，数值（如果适用的话）（Unicode具有表示罗马数字和分数的字符，如三分之一和四分之五）。还有与码位在双向文本和其他显示相关属性中的使用相关的属性。

以下程序显示有关几个字符的一些信息，并打印一个特定字符的数值：






importunicodedatau=chr()+chr(0x0bf2)+chr()+chr()+chr()fori,cinenumerate(u):print(i,'%04x'%ord(c),unicodedata.category(c),end=" ")print(unicodedata.name(c))# Get numeric value of second characterprint(unicodedata.numeric(u[]))




运行时，打印结果如下：






0 00e9 Ll LATIN SMALL LETTER E WITH ACUTE
1 0bf2 No TAMIL NUMBER ONE THOUSAND
2 0f84 Mn TIBETAN MARK HALANTA
3 1770 Lo TAGBANWA LETTER SA
4 33af So SQUARE RAD OVER S SQUARED
1000.0







类别代码是描述字符性质的缩写。这些被分为类别，例如“Letter”, “Number”, “Punctuation”, 或 “Symbol”，这些分类又分为子类别。要从上面的输出中取代码，`'Ll'`意思是“Letter,
 lowecase”，`'No'表示"number, other`”，`'Mn'`是“Mark，nonspacing”，`'So'`是“symbol,
 other”。有关类别代码列表，请参阅 [Unicode字符数据库文档的“常规类别值”部分](http://www.unicode.org/reports/tr44/#General_Category_Values)。



### Unicode正则表达式[](https://docs.python.org/3/howto/unicode.html#unicode-regular-expressions)

```[re](https://docs.python.org/3/library/re.html#module-re)`模块支持的正则表达式可以处理字节或字符串。一些特殊字符的序列，例如 `\d`与`\w`具有取决于匹配模板是否被作为字节或字符串。例如， `\d`将以`[0-9]`字节匹配字符，但对于字符串将匹配`'Nd'`类别中的任何字符。

这个例子中的字符串数字是用泰语和阿拉伯数字写成的：






importrep=re.compile('\d+')s="Over \u0e55\u0e57 57 flavours"m=p.search(s)print(repr(m.group()))




执行时，`\d+`将匹配泰国数字并打印出来。如果在`[compile()](https://docs.python.org/3/library/re.html#re.compile)中`提供[`re.ASCII`](https://docs.python.org/3/library/re.html#re.ASCII)标志，`\d+`则会匹配子串“57”。

类似地，`\w`匹配各种Unicode字符，但只能针对字节形式或标识了`[re.ASCII](https://docs.python.org/3/library/re.html#re.ASCII)`的`[a-zA-Z0-9_]，``\s`将匹配Unicode空格字符或`[ \t\n\r\f\v]。`





### 参考文献[](https://docs.python.org/3/howto/unicode.html#id2)

Python的Unicode支持的一些很好的替代性讨论是：
- [Processing Text Files in Python 3](http://python-notes.curiousefficiency.org/en/latest/python3/text_file_processing.html), by Nick Coghlan.
- [Pragmatic Unicode](http://nedbatchelder.com/text/unipain.html),Ned
 Batchelder的PyCon 2012演示文稿。



该[`str`](https://docs.python.org/3/library/stdtypes.html#str)类型在[文本序列类型
 - str](https://docs.python.org/3/library/stdtypes.html#textseq)的Python库引用中描述 。

[`unicodedata`](https://docs.python.org/3/library/unicodedata.html#module-unicodedata)模块的文档。

[`codecs`](https://docs.python.org/3/library/codecs.html#module-codecs)模块的文档。

Marc-AndréLemburg 在EuroPython 2002上发表[了](https://downloads.egenix.com/python/Unicode-EPC2002-Talk.pdf)[a
 presentation titled “Python and Unicode” (PDF slides)](https://downloads.egenix.com/python/Unicode-EPC2002-Talk.pdf)。幻灯片是对Python
 2的Unicode功能（调用Unicode字符串类型`unicode`和文字开头`u`）的设计的一个很好的概述。





## 读写 Unicode 数据[](https://docs.python.org/3/howto/unicode.html#reading-and-writing-unicode-data)



一旦编写了与Unicode数据一起使用的代码，下一个问题就是输入/输出。如何将Unicode字符串转换到程序中，以及如何将Unicode转换为适合存储或传输的形式？

根据您的输入源和输出目的地，您可能不需要执行任何操作; 您应该检查应用程序中使用的库本身是否支持Unicode。例如，XML解析器通常返回Unicode数据。许多关系数据库还支持Unicode，并可以从SQL查询返回Unicode值。

Unicode数据通常在将其写入磁盘或通过套接字发送之前转换为特定的编码。可以自己做所有的工作：打开一个文件，从它读取一个8位字节的对象，并转换字节`bytes.decode(encoding)`。因此，不推荐手动方法。

一个问题是编码的多字节性质; 一个Unicode字符可以由几个字节表示。如果要以任意大小的块（比如1024或4096字节）读取文件，则需要编写错误处理代码，来捕获某一块最后的单个unicode字符引起的异常。另一个解决方案是将整个文件读入内存，然后执行解码，但这阻止您处理非常大的文件; 如果您需要读取2个GiB文件，则需要2 GiB的RAM。（更确切地说，因为至少有一段时间你需要在内存中同时拥有编码的字符串和Unicode版本。）

该解决方案使用低级解码接口捕获部分编码序列的情况。你已经做了该部分：内置[`open()`](https://docs.python.org/3/library/functions.html#open)功能可以返回假设该文件的内容是在一个指定的编码，并接受统一参数的方法，如类似文件的对象[`read()`](https://docs.python.org/3/library/io.html#io.TextIOBase.read)和 [`write()`](https://docs.python.org/3/library/io.html#io.TextIOBase.write)。这是通过[`open()`](https://docs.python.org/3/library/functions.html#open)的编码和 错误被解释就像那些在参数[`str.encode()`](https://docs.python.org/3/library/stdtypes.html#str.encode) 和[`bytes.decode()`](https://docs.python.org/3/library/stdtypes.html#bytes.decode)。

因此，从文件读取Unicode很简单：




withopen('unicode.txt',encoding='utf-8')asf:forlineinf:print(repr(line))




也可以在更新模式下打开文件，允许读写：






withopen('test',encoding='utf-8',mode='w+')asf:f.write('\u4500 blah blah blah\n')f.seek()print(repr(f.readline()[:]))




Unicode字符`U+FEFF`用作字节顺序标记（BOM），并且通常被写为文件的第一个字符，以帮助自动检测文件的字节顺序。一些编码，如UTF-16，假设BOM在文件的开始出现; 当使用这样的编码时，BOM将自动写入第一个字符，并且在读取文件时将被静默地删除。有这些编码的变体，例如“utf-16-le”和“utf-16-be”用于小端和大端编码，它们指定一个特定字节排序，不跳过BOM。

在某些方面，在UTF-8编码文件的开头使用“BOM”也是惯例。该名称具有误导性，因为UTF-8不是字节顺序的。标记只是宣布文件以UTF-8编码。使用'utf-8-sig'编解码器自动跳过标记，如果存在读取这些文件。




### Unicode文件名[](https://docs.python.org/3/howto/unicode.html#unicode-filenames)

目前大多数常用的操作系统支持包含任意Unicode字符的文件名。通常这是通过将Unicode字符串转换为根据系统而变化的某些编码来实现的。例如，Mac OS X使用UTF-8，而Windows使用可配置的编码; 在Windows上，Python使用名称“mbcs”来引用当前配置的编码。在Unix系统上，如果已设置`LANG`或`LC_CTYPE`环境变量，则只有文件系统编码; 如果没有，则默认编码为UTF-8。

该[`sys.getfilesystemencoding()`](https://docs.python.org/3/library/sys.html#sys.getfilesystemencoding)函数返回要在当前系统上使用的编码，除非你想手动做一些编码工作，否则没有必要修改这部分。当打开一个文件进行阅读或写入时，通常只需要提供Unicode字符串作为文件名，它将自动转换为正确的编码：




filename='filename\u4500abc'withopen(filename,'w')asf:f.write('blah\n')




`[os](https://docs.python.org/3/library/os.html#module-os)`模块中的函数[`os.stat()`](https://docs.python.org/3/library/os.html#os.stat)也接受Unicode文件名。

该[`os.listdir()`](https://docs.python.org/3/library/os.html#os.listdir)函数返回文件名并引发问题：它应该返回Unicode版本的文件名，还是应该返回包含编码版本的字节？ [`os.listdir()`](https://docs.python.org/3/library/os.html#os.listdir)将根据您是否将目录路径提供为字节还是Unicode字符串来分别应对。如果将Unicode字符串作为路径传递，则文件名将使用文件系统的编码进行解码，并返回Unicode字符串列表，而传递字节路径将返回字节形式的文件名。例如，假设默认文件系统编码为UTF-8，运行以下程序：




fn='filename\u4500abc'f=open(fn,'w')f.close()importosprint(os.listdir(b'.'))print(os.listdir('.'))




将产生以下输出：






amk:~$ python t.py
[b'filename\xe4\x94\x80abc', ...]['filename\u4500abc', ...]






第一个列表包含UTF-8编码的文件名，第二个列表包含Unicode版本。

请注意，在大多数情况下，应使用Unicode API。字节API只能在可以存在不可解析文件名的系统上使用，即Unix系统。



### 编写Unicode感知程序的提示[](https://docs.python.org/3/howto/unicode.html#tips-for-writing-unicode-aware-programs)

本节提供了一些关于编写处理Unicode的软件的建议。

最重要的提示是：

> 软件只能在内部使用Unicode字符串，尽快对输入数据进行解码，并在最后编码输出。





> Software should only work with Unicode strings internally, decoding the input data as soon as possible and encoding the output only at the end.

如果您尝试编写接受Unicode和字节字符串的处理函数，那么您将发现程序在无论何种组合两种不同类型的字符串的情况下都会受到漏洞的攻击。没有自动编码或解码：如果你这样做:`str + bytes`,
 将会出现一个`[TypeError](https://docs.python.org/3/library/exceptions.html#TypeError)错误。`

当使用来自Web浏览器或其他不可信源的数据时，常用的技术是在生成的命令行中使用字符串或将其存储在数据库中之前检查字符串中的非法字符。如果你这样做，请小心检查解码的字符串，而不是编码的字节数据; 一些编码可能具有有趣的属性，例如不是双射或不完全符合ASCII的。如果输入数据也指定了编码，这是特别真实的，因为攻击者可以选择一种聪明的方法来隐藏编码的字节流中的恶意文本。


#### 转换文件编码之间[](https://docs.python.org/3/howto/unicode.html#converting-between-file-encodings)

[`StreamRecoder`](https://docs.python.org/3/library/codecs.html#codecs.StreamRecoder)类可以编码之间透明转换，同时，在编码＃1返回数据的流和行为像在编码＃2返回数据流。

例如，如果您的输入文件f是Latin-1，则可以使用[`StreamRecoder`](https://docs.python.org/3/library/codecs.html#codecs.StreamRecoder)来返回以UTF-8编码的字节：




new_f=codecs.StreamRecoder(f,# en/decoder: used by read() to encode its results and# by write() to decode its input.codecs.getencoder('utf-8'),codecs.getdecoder('utf-8'),# reader/writer: used to read and write to the stream.codecs.getreader('latin-1'),codecs.getwriter('latin-1'))




#### 未知编码中的文件[](https://docs.python.org/3/howto/unicode.html#files-in-an-unknown-encoding)

如果您需要更改文件，但不知道文件的编码，您可以做些什么？如果您知道编码与ASCII兼容，并且只想检查或修改ASCII部分，则可以使用`surrogateescape`错误处理程序打开该文件：




withopen(fname,'r',encoding="ascii",errors="surrogateescape")asf:data=f.read()# make changes to the string 'data'withopen(fname+'.new','w',encoding="ascii",errors="surrogateescape")asf:f.write(data)


该`surrogateescape`错误处理程序将所有非ASCII字节解码为Unicode的专用使用区域范围从U
 + DC80到U + DCFF码位。`surrogateescape`当编码数据并将其写回时，使用错误处理程序时，这些专用码位将被转回相同的字节。




### 参考
[](https://docs.python.org/3/howto/unicode.html#id3)


[掌握Python 3的输入/输出](http://pyvideo.org/video/289/pycon-2010--mastering-python-3-i-o)，一个PYCON 2010通话由戴维·比兹利，讨论的文本处理和二进制数据处理。

[Marc-AndréLemburg的演示文稿“在Python中编写Unicode感知应用程序”](https://downloads.egenix.com/python/LSM2005-Developing-Unicode-aware-applications-in-Python.pdf)的[PDF幻灯片](https://downloads.egenix.com/python/LSM2005-Developing-Unicode-aware-applications-in-Python.pdf) 讨论了字符编码的问题以及如何将应用程序国际化和本地化。这些幻灯片只覆盖Python
 2.x。

[Python中的Unicode的胆量](http://pyvideo.org/video/1768/the-guts-of-unicode-in-python) 是Benjamin Peterson发表的一篇PyCon 2013演讲，讨论了Python 3.3中的内部Unicode表示形式。






## 致谢




本文件的初稿由Andrew Kuchling撰写。此后，Alexander Belopolsky，Georg Brandl，Andrew Kuchling和Ezio Melotti进一步修订。

感谢以下提到错误或对此文章提出建议的人：ÉricAraujo，Nicholas Bastin，Nick Coghlan，Marius Gedminas，Kent Johnson，Ken Krugler，Marc-AndréLemburg，Martin vonLöwis，Terry J. Reedy，Chad Whitacre 。




