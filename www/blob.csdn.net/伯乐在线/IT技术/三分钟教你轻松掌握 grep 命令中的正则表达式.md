# 三分钟教你轻松掌握 grep 命令中的正则表达式 - 文章 - 伯乐在线
原文出处： [VIVEK GITE](http://www.cyberciti.biz/faq/grep-regular-expressions/)   译文出处：[LinuxStory-天寒](http://www.linuxstory.org/grep-regular-expressions/)
![grep](http://jbcdn2.b0.upaiyun.com/2016/02/171d041fa8c074c89508a634532d6944.jpg)
如何在 Linux 系统和类 Unix 的操作系统中使用带正则表达式的 grep 命令呢？
Linux 系统自带了支持拓展正则表达式的 GNU 版本 grep 工具。所有的 Linux 系统中默认安装的都是 GNU 版 grep 。grep 命令被用来检索一台服务器或工作站上任何位置的文本信息。
# 一、快速了解正则表达式
### 1、如何匹配你要查找的内容？
正则表达式只不过是每个输入行匹配的模式。模式是一个字符序列。下面都是范例：
例如：“^w1”、“w1|w2”、“[^ ]”。
在 ‘/etc/passswd’ 中检索 ‘vivek’ 。

XHTML
```
grep vivek /etc/passwd
```
输出结果案例：

XHTML
```
vivek:x:1000:1000:Vivek Gite,,,:/home/vivek:/bin/bash
vivekgite:x:1001:1001::/home/vivekgite:/bin/sh
gitevivek:x:1002:1002::/home/gitevivek:/bin/sh
```
在任何情况下都搜索 ‘vivek’  (即不区分大小)：

XHTML
```
grep -i -w vivek /etc/passwd
```
不区分大小写地检索 ‘vivek’ 和 ‘raj’ ：

XHTML
```
grep -E -i -w 'vivek|raj' /etc/passwd
```
在最后一个例子中，使用了扩展正则表达式的模式。
固定检索内容的位置：
你可以使用 ^ 和 $ 符号强制一个正则表达式分别匹配一行的开始或结束的位置。下面的示例显示以 ‘vivek’ 开头的文本。

XHTML
```
grep ^vivek /etc/passwd
```
输出结果示例：

XHTML
```
vivek:x:1000:1000:Vivek Gite,,,:/home/vivek:/bin/bash
vivekgite:x:1001:1001::/home/vivekgite:/bin/sh
```
你可以只显示以 vivek 开头的文本行。举例说就是不显示 vivekgite , vivekg 这样单词开头的。

XHTML
```
grep -w ^vivek /etc/passwd
```
检索以 ‘foo’ 结尾的文本格式：

XHTML
```
grep 'foo$' FILENAME
```
你还可以用下面这样的方式搜索空白行：

XHTML
```
grep '^$' FILENAME
```
### 2、如何匹配具体字符？
匹配 ‘Vivek’ 或 ‘vivek’ ：

XHTML
```
grep '[vV]ivek' FILENAME
```
或者可以这样：

XHTML
```
grep '[vV][iI][Vv][Ee][kK]' FILENAME
```
你可以匹配数字（例如匹配 vivek1 或 Vivek2 ）：

XHTML
```
grep -w '[vV]ivek[0-9]' FILENAME
```
你可以匹配两位数（例如匹配 foo11 ， foo12 ）：

XHTML
```
grep 'foo[0-9][0-9]' FILENAME
```
不仅仅是数字，你可以匹配字母：

XHTML
```
grep '[A-Za-z]' FILENAME
```
显示所有包含 “w” 或 “n” 字母的文本行：

XHTML
```
grep [wn] FILENAME
```
在括号内的表达式中，在“ [: ”和“ :] ”中所附的字符类的名称：代表属于该类的所有字符的列表。标准字符类名称：
- **[:alnum:]** – 字母数字字符。
- **[:alpha:]** – 字母顺序
- **[:blank:]** – 空格和制表符。
- **[:digit:]** – 数字： ‘0 1 2 3 4 5 6 7 8 9’。
- **[:lower:]** – 小写字母：‘a b c d e f ‘。
- **[:space:]** – 特殊字符：制表符，换行符，垂直制表符、换页，回车，和空间。
- **[:upper:]** – 大写字母：‘A B C D E F G H I J K L M N O P Q R S T U V W X Y Z’。
在下面这个例子中，匹配所有大写字母：

XHTML
```
grep '[:upper:]' FILENAME
```
### 3、如何使用通配符？
你可以用 “.” 来代替单个字符。在下面的例子中，查询了所有以字母 “b” 开头、字母 “t” 结尾的三个字符的单词。

XHTML
```
grep '\<b.t\>' FILENAME
```
在上面的例子中，
- \< 在单词的开始位置匹配空格字符串
- \> 在单词的结尾匹配空格字符串
检索并输出所有两个字母的结果：

XHTML
```
grep '^..$' FILENAME
```
检索并显示所有以 ‘.’ 和数字开头的结果：

XHTML
```
grep '^\.[0-9]' FILENAME
```
**转义字符’.’**
下面的正则表达式查找 IP 地址 192.168.1.254 将不能获得预期的结果：

XHTML
```
grep '192.168.1.254' /etc/hosts
```
其中三个点都需要被转义：

XHTML
```
grep '192\.168\.1\.254' /etc/hosts
```
以下示例将只匹配一个地址：

XHTML
```
egrep '[[:digit:]]{1,3}\.[[:digit:]]{1,3}\.[[:digit:]]{1,3}\.[[:digit:]]
{1,3}' FILENAME
```
以下将不分大小写地匹配单词 Linux 或 Unix ：

XHTML
```
egrep -i '^(linux|unix)' FILENAME
```
# 二、深入探索 grep 高级查找模式
### 1、如何检索一个具有以 ‘-‘ 开头的的模式？
使用 -e 选项搜索所有匹配 ‘–test–‘ 的结果。grep 会尝试把 ‘–test–‘ 作为一个选项解析：

XHTML
```
grep -e '--test--' FILENAME
```
### 2、如何在grep中使用 OR 的逻辑运算 ？


```
grep -E 'word1|word2' FILENAME
### OR ###
egrep 'word1|word2' FILENAME
```
或者可以这样做

XHTML
```
grep 'word1\|word2' FILENAME
```
### 3、如何在grep中使用 AND 的逻辑运算 ？
按照下面的语法显示所有包含了单词 ‘word1′ 和 ‘word2′ 的结果：

XHTML
```
grep 'word1' FILENAME | grep 'word2'
```
或者可以这样：

XHTML
```
grep 'foo.*bar\|word3.*word4' FILENAME
```
### 4、如何测试序列？
你可以使用下面的语法测试一个字符在序列中的重复的次数：


```
{N}
{N,}
{min,max}
```
匹配包含两个字母 v 的字符串结果：

XHTML
```
egrep "v{2}" FILENAME
```
下面的例子中将检索文件内包含 “col” 和 “cool” 的字符串结果：

XHTML
```
egrep 'co{1,2}l' FILENAME
```
下面的例子中将匹配至少含有3个字母 c 的结果：

XHTML
```
egrep 'c{3,}' FILENAME
```
下面的示例将匹配 “91-1234567890″ 格式的手机号码（即 “两位数字-十位数字”）

XHTML
```
grep "[[:digit:]]\{2\}[ -]\?[[:digit:]]\{10\}" FILENAME
```
### 5、如何使 grep 的输出结果高亮标注？
使用下面例子的语法：

XHTML
```
grep --color regex FILENAME
```
### 6、如何使 grep 的输出只显示匹配的部分而不是整行？
使用下面例子的语法：

XHTML
```
grep -o regex FILENAME
```
# 三、正则表达式操作符总结
|正则表达式操作符|含义|
|----|----|
|.|匹配任何单个字符。|
|?|匹配前一个字符0次或1次。|
|*|匹配前一个字符≥0次。|
|+|匹配前一个字符≥1次。|
|{N}|匹配前一个字符N次。|
|{N,}|匹配前一个字符≥m次。|
|{N,M}|匹配前一个字符 N 到 M次。|
|–|如果在列表中的某个列表或某个范围内的结束点，表示该范围。|
|^|开始标记，表示在开始位置匹配一个空字符串。也表示不在列表的范围内的字符。|
|$|结束标记。匹配一个空的字符串。|
|\b|单词锁定符。在一个单词的边缘位置匹配空字符串。|
|\B|在一个单词的非边缘位置匹配空字符串。|
|\<|匹配单词开始的空字符串。|
|\>|匹配单词结尾的空字符串。|
# 四、关于 grep 和 egrep
egrep 即 grep -E ，它把模式作为一个扩展的正则表达式解释。grep 帮助文档中这样定义：
In basic regular expressions the meta-characters ?, +, {, |, (, and ) lose their special meaning; instead use the backslashed versions \?, \+, \{,\|, \(, and \). Traditional egrep did not support the { meta-character, and some egrep implementations support \{ instead, so portable scripts should avoid { in grep -E patterns and should use [{] to match a literal {. GNU grep -E attempts to support traditional usage by assuming that { is not special if it would be the start of an invalid interval specification. For example, the command grep -E ‘{1’ searches for the two-character string {1 instead of reporting a syntax error in the regular expression. POSIX.2 allows this behavior as an extension, but portable scripts should avoid it.
##### 参考文献：
- grep and regex 的帮助文档
- grep 的 info帮助文档
