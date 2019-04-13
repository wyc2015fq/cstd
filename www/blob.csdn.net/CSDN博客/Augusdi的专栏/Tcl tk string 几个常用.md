
# Tcl-tk string 几个常用 - Augusdi的专栏 - CSDN博客


2016年10月24日 14:16:22[Augusdi](https://me.csdn.net/Augusdi)阅读数：2743


﻿﻿
这里对string命令中的几个子命令使用实例进行一些解释，以便于更加容易理解string命令中的各个子命令，本文仅对以下几个string命令进行实例解析。分别是repeat、replace、reverse、tolower、totitle、toupper、trim、trimleft、trimright、wordend和wordstart几个子命令。
**string repeat***string count*
非常简单，返回一个把string重复count次的字符串。
% string repeat "This" 3
ThisThisThis

**string replace***string first last*?*newstring*?
也很简单，使用newstring替换string中的first到last的字符串，如果没有newstring，就是使用空代替。
% string replace "This is a tcltk example" 10 14 TCLTK
This is a TCLTK example
如果没有newstring：
% string replace "This is a tcltk example" 10 14
This is a  example
**string reverse***string*
返回string的反序字符串：
% string reverse "This is a tcltk example"
elpmaxe ktlct a si sihT
**string tolower***string*?*first*? ?*last*?
**string totitle***string*?*first*? ?*last*?
**string toupper***string*?*first*? ?*last*?
这三个命令放在一起，是因为三个命令的格式完全相同，只是实现了不同的操作。
将一个字符串全部变为小写形式：
% string tolower "This is a tcltk example"
this is a tcltk example
将一个字符串全部变为大写形式：
% string toupper "This is a tcltk example"
THIS IS A TCLTK EXAMPLE
将一个字符串里面开头的第一个字母转换为大写形式，其他字符转化为小写形式：
% string totitle "this is a TCLTK example"
This is a tcltk example
first和last指定了转换的范围，操作与上述完全相同，只是对字符串的作用范围不同。
**string trim***string*?*chars*?
**string trimleft***string*?*chars*?
**string trimright***string*?*chars*?
这三个命令实现的功能类似，都是去掉chars字符，只是操作的位置有所区别。如果没有指定chars字符，则去掉空白符（包括空格符、制表符、换行符、回车符）。trim对字符串开头和结尾都操作，trimleft只对字符串开头操作，trimright只对字符串结尾操作。
% string trim "!!This is a tcltk example!!" !
This is a tcltk example
% string trimleft "!!This is a tcltk example!!" !
This is a tcltk example!!
% string trimright "!!This is a tcltk example!!" !
!!This is a tcltk example
**string wordend***string charIndex*
**string wordstart***string charIndex*
这两个命令类似，wordend是找出给定索引的字符所在的单词的下一个单词的第一个字符的索引，wordstart是找出给定索引的字符所在的单词的第一个字符的索引。用语言描述比较难理解，下面举例说明就非常清晰了：
% string wordend "This is a tcltk example" 12
15
12索引为tcltk中的l字符，那么返回的结果就是l所在的词tcltk的下一个词example中的第一个字符e的索引，即15。
% string wordstart "This is a tcltk example" 12
10
12索引为tcltk中的l字符，那么返回的结果就是l所在的词的第一个字符t的索引，即10。

|命令
|说明
|
|string bytelength str
|返回用于存储字符串的字节数，由于|UTF8|编码的原因，这个长度可能与|string length|返回长度不一样
|
|string compare ?-nocase? ?-length len? Str1 str2
|根据词典顺序比较两个字符串，|nocase|表示忽略大小写，|length|表示比较前|n|个字符，如果相同返回值为|0|，如果|str1|靠前就返回|-1|，对于其他情况返回|1
|
|string equal ? –nocase? Str1 str2
|比较字符串，如果相同返回|1|，否则|-1|，使用|nocase|来表示忽略大小写
|
|string first str1 str2
|返回|str2|中|str1|第一次出现的位置，如果没有的话，就返回|-1|。
|
|string is class ?-strict? ?-failindex varname? string
|如果|string|属于某个|class|就返回，如果指定了|strict|，那么就不匹配空字符串，否则总是要匹配，如果指定了|failindex|，就会将在|string|中阻止其称为|class|一员的字符串索引赋给|varname|，
|
|string last str1 str2
|返回|str2|中|str1|最后一次出现的位置，如果没有出现就返回|-1
|
|string length str
|返回|string|中的字符个数
|
|string map ?-nocase? charMap string
|返回一个根据|charmap|中输入输出列表将|string|中的字符进行映射后产生的字符串。
|
|string match pattern str
|如果|str|匹配|pattern|就返回|1|，否则返回|0|，
|
|string ranger str i j
|返回字符串中从|i|到|j|的部分。
|
|string repeat str count
|返回将|str|重复|count|次的字符串
|
|string replace str first last ?newstr?
|返回一个通过把从|first|到|last|字符串替换为|newstr|的新字符串，或是返回空
|
|string tolower string ?first? ?last?
|返回|string|的小写形式，|first|和|last|决定了字符串位置
|
|string totitle string ?first? ?last?
|将第一个字符替换为大写，其他为小写，|first|和|last|决定了字符串位置
|
|string toupper string ?first? ?last?
|返回|string|的大写格式，|first|和|last|决定了字符串位置
|
|string trim string ?chars?
|从|string|两端除去|chars|中指定的字符，|chars|默认空
|
|string trimleft string ?chars?
|从|string|的左端除去|chars|中指定的字符，|chars|默认为空
|
|string trimright string ?chars?
|从|string|的右端除去|chars|指定的字符，|chars|默认为空
|
|string wordend str ix
|返回|str|中在索引|ix|位置包含的字符的单词之后的字符的索引位置
|
|string wordstart str ix
|返回|str|中在索引|ix|位置包含字符串的单词中第一个字符的索引位置。
|

