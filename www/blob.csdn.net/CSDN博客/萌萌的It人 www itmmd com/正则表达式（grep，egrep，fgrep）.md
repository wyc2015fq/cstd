
# 正则表达式（grep，egrep，fgrep） - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月19日 17:15:09[Jlins](https://me.csdn.net/dyllove98)阅读数：8530标签：[grep																](https://so.csdn.net/so/search/s.do?q=grep&t=blog)[egrep																](https://so.csdn.net/so/search/s.do?q=egrep&t=blog)[fgrep																](https://so.csdn.net/so/search/s.do?q=fgrep&t=blog)[正则表达式																](https://so.csdn.net/so/search/s.do?q=正则表达式&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=fgrep&t=blog)[
																															](https://so.csdn.net/so/search/s.do?q=egrep&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=grep&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=grep&t=blog)

1.正则表达式概述
正则表达式又称正规表示式、正规表示法、正规表达式、规则表达式、常规表示法（英语：Regular Expression，在代码中常简写为regex、regexp或RE），计算机科学的一个概念。正则表达式使用单个字符串来描述、匹配一系列符合某个句法规则的字符串。在很多文本编辑器里，正则表达式通常被用来检索、替换那些符合某个模式的文本。
许多程序设计语言都支持利用正则表达式进行字符串操作。正则表达式这个概念最初是由Unix中的工具软件（例如sed和grep）普及开的。正则表达式通常缩写成“regex”，单数有regexp、regex，复数有regexps、regexes、regexen。

2.基本语法
一个正则表达式通常被称为一个模式（pattern），为用来描述或者匹配一系列符合某个句法规则的字符串。例如：Handel、Hndel和Haendel这三个字符串，都可以由“H(a||ae)ndel”这个模式来描述。大部分正则表达式的形式都有如下的结构：
选择
|竖直分隔符代表选择。例如“gray|grey”可以匹配grey或gray。
数量限定
某个字符后的数量限定符用来限定前面这个字符允许出现的个数。最常见的数量限定符包括“+”、“?”和“*”（不加数量限定则代表出现一次且仅出现一次）：
+加号代表前面的字符必须至少出现一次。（1次、或多次）。例如，“goo+gle”可以匹配google、gooogle、goooogle等;
?问号代表前面的字符最多只可以出现一次。（0次、或1次）。例如，“colou?r”可以匹配color或者colour;
*星号代表前面的字符可以不出现，也可以出现一次或者多次。（0次、或1次、或多次）。例如，“0*42”可以匹配42、042、0042、00042等。
匹配
圆括号可以用来定义操作符的范围和优先度。例如，“gr(a|e)y”等价于“gray|grey”，“(grand)?father”匹配father和grandfather。
上述这些构造子都可以自由组合，因此，“H(ae?|)ndel”和“H(a|ae|)ndel”是相同的。
精确的语法可能因不同的工具或程序而异。
3.正则表达式基础
基本概念
3.1.字符串组成
对于字符串“a5”，是由两个字符“a”、“5”以及三个位置组成的，这一点对于正则表达式的匹配原理理解很重要。
[            ](http://s3.51cto.com/wyfs02/M01/53/6F/wKioL1RnTiuzR0aXAAA3MNshD7c746.jpg)![wKioL1RnTiuzR0aXAAA3MNshD7c746.jpg](http://s3.51cto.com/wyfs02/M01/53/6F/wKioL1RnTiuzR0aXAAA3MNshD7c746.jpg)

3.2.占有字符和零宽度
正则表达式匹配过程中，如果子表达式匹配到的是字符内容，而非位置，并被保存到最终的匹配结果中，那么就认为这个子表达式是占有字符的；如果子表达式匹配的仅仅是位置，或者匹配的内容并不保存到最终的匹配结果中，那么就认为这个子表达式是零宽度的。
占有字符还是零宽度，是针对匹配的内容是否保存到最终的匹配结果中而言的。
占有字符是互斥的，零宽度是非互斥的。也就是一个字符，同一时间只能由一个子表达式匹配，而一个位置，却可以同时由多个零宽度的子表达式匹配。
由一类字符书写的模式，其中有些字符不表示字符字面意义，而是表达控制或通配的功能。
3.3.正则表达式构成
正则表达式由两种字符构成。一种是在正则表达式中具体特殊意义的“元字符”，另一种是普通的“文本字符”。
元字符可以是一个字符，如“^”，也可以是一个字符序列，如“\w”。
我们建立一个文件来说明元字符。
|1
|2
|3
|4
|5
|6
|\#cat test.txt
|ab
|cb
|abx
|aaaaaab
|aaaaaaaaaaaaaab
|


元字符（Meta Character）
1.[…]字符组(CharacterClasses)
字符组可以匹配[ ]中包含的任意一个字符。虽然可以是任意一个，但只能是一个。
字符组支持由连字符“-”来表示一个范围。当“-”前后构成范围时，要求前面字符的码位小于后面字符的码位。
[^…]排除型字符组。排除型字符组表示任意一个未列出的字符，同样只能是一个。排除型字符组同样支持由连字符“-”来表示一个范围。
字符匹配：字符通配
.：匹配任意单个字符。
![wKiom1RnTn_Rp7g5AAAomnZ5mCA249.jpg](http://s3.51cto.com/wyfs02/M01/53/71/wKiom1RnTn_Rp7g5AAAomnZ5mCA249.jpg)
[]：匹配指定集合中的任意单个字符。
![wKiom1RnTpPQqsW_AAAnujMptpw616.jpg](http://s3.51cto.com/wyfs02/M00/53/71/wKiom1RnTpPQqsW_AAAnujMptpw616.jpg)
[^]：匹配指定集合外的任意单个字符
![wKioL1RnTx-QtIdGAAAna4_Q5x0980.jpg](http://s3.51cto.com/wyfs02/M01/53/6F/wKioL1RnTx-QtIdGAAAna4_Q5x0980.jpg)
2.量词（Quantifier）
量词表示一个子表达式可以匹配的次数。量词可以用来修饰一个字符、字符组，或是用()括起来的子表达式。一些常用的量词被定义成独立的元字符。
*：表达式匹配前面任意次，0,1或者多次，前面字符出现与否都匹配，相当于{0,}。
![wKioL1RnT0eDQe9pAAAtH3eOhk4676.jpg](http://s3.51cto.com/wyfs02/M02/53/6F/wKioL1RnT0eDQe9pAAAtH3eOhk4676.jpg)
\+：表达式匹配1次或意多次，至少1次，相当于{1,}
![wKiom1RnTvug_4crAAAlXhEw4k8295.jpg](http://s3.51cto.com/wyfs02/M02/53/71/wKiom1RnTvug_4crAAAlXhEw4k8295.jpg)
\?：表达式匹配0次或1次，相当于{0,1}
![wKioL1RnT3yhOXWyAAAvBnQaKAg880.jpg](http://s3.51cto.com/wyfs02/M00/53/6F/wKioL1RnT3yhOXWyAAAvBnQaKAg880.jpg)
\{m\}：表达式匹配其前面的字符m次
![wKiom1RnTxyCfELkAAApFTxoBKA845.jpg](http://s3.51cto.com/wyfs02/M00/53/71/wKiom1RnTxyCfELkAAApFTxoBKA845.jpg)
\{m,n\}：表达式匹配最少m次，最多n次
![wKioL1RnT6LDlpnGAAAul5zJI1I115.jpg](http://s3.51cto.com/wyfs02/M01/53/6F/wKioL1RnT6LDlpnGAAAul5zJI1I115.jpg)
注意：在不是动态生成的正则表达式中，不要出现“{1}”这样的量词，如“\w{1}”在结果上等价于“\w”，但是会降低匹配效率和可读性，属于画蛇添足的做法。
.*：匹配任意长度的任意字符
![wKioL1RnT8rD6YUxAAAp79LSNM0841.jpg](http://s3.51cto.com/wyfs02/M02/53/6F/wKioL1RnT8rD6YUxAAAp79LSNM0841.jpg)
3.位置锚定
^：行首锚定，出现最左侧
![wKioL1RnT-GyXbWuAAAV7nj3kVo244.jpg](http://s3.51cto.com/wyfs02/M02/53/6F/wKioL1RnT-GyXbWuAAAV7nj3kVo244.jpg)
$：行尾锚定，出现最右侧
![wKioL1RnUAbQbbm-AAAbFsE9-dM551.jpg](http://s3.51cto.com/wyfs02/M00/53/6F/wKioL1RnUAbQbbm-AAAbFsE9-dM551.jpg)
^$：表示空白行
![wKiom1RnT6ngV2b1AAASf_tIVCU573.jpg](http://s3.51cto.com/wyfs02/M00/53/71/wKiom1RnT6ngV2b1AAASf_tIVCU573.jpg)
\<：词首锚定，在查找的单词模式的左侧；\<char
![wKioL1RnUCyA6CYAAAAbHGVVLHk174.jpg](http://s3.51cto.com/wyfs02/M00/53/6F/wKioL1RnUCyA6CYAAAAbHGVVLHk174.jpg)
\>：词尾锚定，在查找的单词模式的右侧；char\>
![wKioL1RnUDzxzXI5AAAkvsSJjOc442.jpg](http://s3.51cto.com/wyfs02/M01/53/6F/wKioL1RnUDzxzXI5AAAkvsSJjOc442.jpg)
\<pattern\>：匹配单词
![wKiom1RnT9_BY_LuAAAZgk_KUco462.jpg](http://s3.51cto.com/wyfs02/M00/53/71/wKiom1RnT9_BY_LuAAAZgk_KUco462.jpg)
4.捕获组（Capture Group）
捕获组就是把正则表达式中子表达式匹配的内容，保存到内存中以数字编号或手动命名的组里，以供后面引用。
(Expression)：普通捕获组，将子表达式Expression匹配的内容保存到以数字编号的组里
(?<name> Expression)：命名捕获组，将子表达式Expression匹配的内容保存到以name命名的组里
普通捕获组（在不产生歧义的情况下，简称捕获组）是以数字进行编号的，编号规则是以“(”从左到右出现的顺序，从1开始进行编号。通常情况下，编号为0的组表示整个表达式匹配的内容。
命名捕获组可以通过捕获组名，而不是序号对捕获内容进行引用，提供了更便捷的引用方式，不用关注捕获组的序号，也不用担心表达式部分变更会导致引用错误的捕获组。
非捕获组：一些表达式中，不得不使用( )，但又不需要保存( )中子表达式匹配的内容，这时可以用非捕获组来抵消使用( )带来的副作用。
(?:Expression)：进行子表达式Expression的匹配，并将匹配内容保存到最终的整个表达式的区配结果中，但Expression匹配的内容不单独保存到一个组内
反向引用：捕获组匹配的内容，可以在正则表达式的外部程序中进行引用，也可以在表达式中进行引用，表达式中引用的方式就是反向引用。
反向引用通常用来查找重复的子串，或是限定某一子串成对出现。
\1，\2：对序号为1和2的捕获组的反向引用
\k<name>：对命名为name的捕获组的反向引用
举例：
“(a|b)\1”在匹配“abaa”时，匹配成功，匹配到的结果是“aa”。“(a|b)”在尝试匹配时，虽然既可以匹配“a”，也可以匹配“b”，但是在进行反向引用时，对应()中匹配的内容已经是固定的了。
我们来实验一下：
|1
|2
|3
|4
|5
|6
|7
|\# vi test3.txt
|aaaaaaaaaax
|abbbbbbbbbx
|abababababx
|abbx
|abx
|abbbx
|

我们要限定只找限定ab出现1-3次的：
![wKiom1RsOoCDrvTnAAA3lTf9J0s598.jpg](http://s3.51cto.com/wyfs02/M01/53/9C/wKiom1RsOoCDrvTnAAA3lTf9J0s598.jpg)
|1
|2
|3
|4
|5
|6
|7
|\# cat test2.txt
|acbhelloacb123
|aabhiacb123
|abaworldabb123
|acbbadacb234
|adbcloseadb234
|adbcloseasb2345
|

我们要求找到以a开始中间跟任意字符，再以b结尾的行，这个行后面再一次出现同样条件的行。
下面的例子能实现匹配以a开头以b结尾，但是中间的字符不一致也匹配。
![wKiom1RnT_rib9Q2AAB9WPWCg0c859.jpg](http://s3.51cto.com/wyfs02/M01/53/71/wKiom1RnT_rib9Q2AAB9WPWCg0c859.jpg)
我们使用分组的反向引用来实现：
![wKioL1RnUICT64auAABvMy0mAmc382.jpg](http://s3.51cto.com/wyfs02/M02/53/6F/wKioL1RnUICT64auAABvMy0mAmc382.jpg)

贪婪与懒惰
当正则表达式中包含能接受重复的限定符时，通常的行为是（在使整个表达式能得到匹配的前提下）匹配尽可能多的字符。以这个表达式为例：a.*b，它将会匹配最长的以a开始，以b结束的字符串。如果用它来搜索aabab的话，它会匹配整个字符串aabab。这被称为贪婪匹配。
有时，我们更需要懒惰匹配，也就是匹配尽可能少的字符。前面给出的限定符都可以被转化为懒惰匹配模式，只要在它后面加上一个问号?。这样.*?就意味着匹配任意数量的重复，但是在能使整个匹配成功的前提下使用最少的重复。现在看看懒惰版的例子吧：
a.*?b匹配最短的，以a开始，以b结束的字符串。如果把它应用于aabab的话，它会匹配aab（第一到第三个字符）和ab（第四到第五个字符）。
懒惰限定符
*?重复任意次，但尽可能少重复
+?重复1次或更多次，但尽可能少重复
??重复0次或1次，但尽可能少重复
{n,m}?重复n到m次，但尽可能少重复
{n,}?重复n次以上，但尽可能少重复

5.grep,egrep,fgrep介绍
Unix的grep家族包括grep、egrep和fgrep。egrep和fgrep的命令只跟grep有很小不同。egrep是grep的扩展，支持更多的re元字符，fgrep就是fixed
 grep或fast grep，它们把所有的字母都看作单词，也就是说，正则表达式中的元字符表示回其自身的字面意义，不再特殊。linux使用GNU版本的grep。它功能更强，可以通过-G、-E、-F命令行选项来使用egrep和fgrep的功能
grep：Global searchRegular expression and Print out the line
egrep：Extended Globalsearch Regular expression and Print out the line
fgrep：Fast Global searchRegular expression and Print out the line

Linux grep是基础的正则表达式，命令用于在文件内查找指定的字符串。
grep (globalsearch regular expression(RE) and print out the line,全面搜索正则表达式并把行打印出来)是一种强大的文本搜索工具，它能使用正则表达式搜索文本，并把匹配的行打印出来。
grep[-acinv] [--color=auto] '搜寻字符串' filename
选项与参数：
-a：将binary文件以text文件的方式搜寻数据
-c：计算找到'搜寻字符串'的次数
-i：忽略大小写的不同，所以大小写视为相同
-n：顺便输出行号
-v：反向选择，亦即显示出没有'搜寻字符串'内容的那一行！
--color=auto：可以将找到的关键词部分加上颜色的显示喔！
-B ：附带显示前面两行
-A ：附带显示后面两行
-C ：附带显示上下两行一起
Linux egrep是扩展的正则表达式，命令用于在文件内查找指定的字符串。
egrep执行效果与"grep-E"相似，使用的语法及参数可参照grep指令，与grep的不同点在于解读字符串的方法。
egrep是用extendedregular expression语法来解读的，而grep则用basic regular expression语法解读，extended
 regularexpression比basic regular expression的表达更规范。
扩展正则表达式的元字符：和基础的没啥区别，就是有些字符不需要\了。
字符匹配
.：匹配任意单个字符
[]：指定集合中的任意单个字符
[^]：匹配指定集合外的任意单个字符
+  ：匹配前面的字符出现至少1次
匹配次数限定
*：匹配前面任意次，0，1或者多次，前面字符出现与否都匹配
?：匹配其前面的字符0次或1次
{m}：匹配其前面的字符m次
{m,n}：匹配其前面的字符至少m次，至多n次，{m,} 表示至少m，至多不限制，{0,n}表示至少0次，至多n次
.*：匹配任意长度的任意字符
字符锚定
^：行首锚定，出现最左侧
$：行尾锚定，出现最右侧
^$：表示空白行
\<：词首锚定，在查找的单词模式的左侧；\<char
\>：词尾锚定，在查找的单词模式的右侧；char\>
\<pattern\>：匹配单词
捕获组的反向引用和grep差不多
(a.b).*\1模式从左到右，引用第\#个左括号以及与其匹配右括号之间的模式匹配到的内容，
\1 , \2
a|b  a或者b，ab|cd就是ab或者cd。
Linux fgrep不使用正则表达式，查询速度比grep命令快，但是不够灵活：它只能找固定的文本，而不是规则表达式。
如果你想在一个文件或者输出中找到包含星号字符的行
[    ](http://s3.51cto.com/wyfs02/M02/53/6F/wKioL1RnTHTC1pptAABiPVZ2KFk533.jpg)![wKioL1RnTHTC1pptAABiPVZ2KFk533.jpg](http://s3.51cto.com/wyfs02/M02/53/6F/wKioL1RnTHTC1pptAABiPVZ2KFk533.jpg)
[    ](http://s3.51cto.com/wyfs02/M02/53/71/wKiom1RnTAOQOKqGAABhey5yZtU879.jpg)![wKiom1RnTAOQOKqGAABhey5yZtU879.jpg](http://s3.51cto.com/wyfs02/M02/53/71/wKiom1RnTAOQOKqGAABhey5yZtU879.jpg)
个人觉得捕获组的概念有些困难，本文对egrep没有实例说明，但是可以根据上文中的grep的例子做实验对比，效果更好，不明白可以google一下。

本文并非所有都原创，有些概念摘自网上博文，下面贴出，以表谢意：
中文wiki：http://zh.wikipedia.org/wiki/%E6%AD%A3%E5%88%99%E8%A1%A8%E8%BE%BE%E5%BC%8F
正则表达式学习参考：[http://blog.csdn.net/lxcnn/article/details/4268033](http://blog.csdn.net/lxcnn/article/details/4268033)
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

