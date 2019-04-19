# 【linux指令】sed指令 - Koma Hub - CSDN博客
2019年01月09日 21:23:45[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：54
原文地址：[https://www.cnblogs.com/maxincai/p/5146338.html](https://www.cnblogs.com/maxincai/p/5146338.html)
**目录**
[sed的选项、命令、替换标记](#sed%E7%9A%84%E9%80%89%E9%A1%B9%E5%91%BD%E4%BB%A4%E6%9B%BF%E6%8D%A2%E6%A0%87%E8%AE%B0)
[命令格式](#%E5%91%BD%E4%BB%A4%E6%A0%BC%E5%BC%8F)
[选项](#%E9%80%89%E9%A1%B9)
[参数](#%E5%8F%82%E6%95%B0)
[sed命令](#sed%E5%91%BD%E4%BB%A4)
[sed替换标记](#sed%E6%9B%BF%E6%8D%A2%E6%A0%87%E8%AE%B0)
[sed元字符集](#sed%E5%85%83%E5%AD%97%E7%AC%A6%E9%9B%86)
[sed用法实例](#sed%E7%94%A8%E6%B3%95%E5%AE%9E%E4%BE%8B)
[我们先准备一个测试文件](#%E6%88%91%E4%BB%AC%E5%85%88%E5%87%86%E5%A4%87%E4%B8%80%E4%B8%AA%E6%B5%8B%E8%AF%95%E6%96%87%E4%BB%B6)
[替换操作：s命令](#%E6%9B%BF%E6%8D%A2%E6%93%8D%E4%BD%9C%EF%BC%9As%E5%91%BD%E4%BB%A4)
[替换文本中的字符串：](#%E6%9B%BF%E6%8D%A2%E6%96%87%E6%9C%AC%E4%B8%AD%E7%9A%84%E5%AD%97%E7%AC%A6%E4%B8%B2%EF%BC%9A)
[-n选项和p命令一起使用表示只打印那些发生替换的行：](#-n%E9%80%89%E9%A1%B9%E5%92%8Cp%E5%91%BD%E4%BB%A4%E4%B8%80%E8%B5%B7%E4%BD%BF%E7%94%A8%E8%A1%A8%E7%A4%BA%E5%8F%AA%E6%89%93%E5%8D%B0%E9%82%A3%E4%BA%9B%E5%8F%91%E7%94%9F%E6%9B%BF%E6%8D%A2%E7%9A%84%E8%A1%8C%EF%BC%9A)
[全面替换标记g](#%E5%85%A8%E9%9D%A2%E6%9B%BF%E6%8D%A2%E6%A0%87%E8%AE%B0g)
[当需要从第N处匹配开始替换时，可以使用/Ng:](#%E5%BD%93%E9%9C%80%E8%A6%81%E4%BB%8E%E7%AC%ACN%E5%A4%84%E5%8C%B9%E9%85%8D%E5%BC%80%E5%A7%8B%E6%9B%BF%E6%8D%A2%E6%97%B6%EF%BC%8C%E5%8F%AF%E4%BB%A5%E4%BD%BF%E7%94%A8%2FNg%3A)
[定界符](#%E5%AE%9A%E7%95%8C%E7%AC%A6)
[删除操作：d命令](#%E5%88%A0%E9%99%A4%E6%93%8D%E4%BD%9C%EF%BC%9Ad%E5%91%BD%E4%BB%A4)
[删除空白行：](#%E5%88%A0%E9%99%A4%E7%A9%BA%E7%99%BD%E8%A1%8C%EF%BC%9A)
[删除文件的第2行：](#%E5%88%A0%E9%99%A4%E6%96%87%E4%BB%B6%E7%9A%84%E7%AC%AC2%E8%A1%8C%EF%BC%9A)
[删除文件的第2行到末尾所有行：](#%E5%88%A0%E9%99%A4%E6%96%87%E4%BB%B6%E7%9A%84%E7%AC%AC2%E8%A1%8C%E5%88%B0%E6%9C%AB%E5%B0%BE%E6%89%80%E6%9C%89%E8%A1%8C%EF%BC%9A)
[删除文件最后一行：](#%E5%88%A0%E9%99%A4%E6%96%87%E4%BB%B6%E6%9C%80%E5%90%8E%E4%B8%80%E8%A1%8C%EF%BC%9A)
[删除文件中所有以my开头的行：](#%E5%88%A0%E9%99%A4%E6%96%87%E4%BB%B6%E4%B8%AD%E6%89%80%E6%9C%89%E4%BB%A5my%E5%BC%80%E5%A4%B4%E7%9A%84%E8%A1%8C%EF%BC%9A)
[已匹配字符串标记&](#%E5%B7%B2%E5%8C%B9%E9%85%8D%E5%AD%97%E7%AC%A6%E4%B8%B2%E6%A0%87%E8%AE%B0%26)
[子串匹配标记\1](#%E5%AD%90%E4%B8%B2%E5%8C%B9%E9%85%8D%E6%A0%87%E8%AE%B0%5C1)
[组合多个表达式](#%E7%BB%84%E5%90%88%E5%A4%9A%E4%B8%AA%E8%A1%A8%E8%BE%BE%E5%BC%8F)
[引用](#%E5%BC%95%E7%94%A8)
[选定行的范围：,(逗号)](#%E9%80%89%E5%AE%9A%E8%A1%8C%E7%9A%84%E8%8C%83%E5%9B%B4%EF%BC%9A%2C(%E9%80%97%E5%8F%B7))
[多点编辑：e命令](#%E5%A4%9A%E7%82%B9%E7%BC%96%E8%BE%91%EF%BC%9Ae%E5%91%BD%E4%BB%A4)
[从文件读入：r命令](#%E4%BB%8E%E6%96%87%E4%BB%B6%E8%AF%BB%E5%85%A5%EF%BC%9Ar%E5%91%BD%E4%BB%A4)
[写入文件：w命令](#%E5%86%99%E5%85%A5%E6%96%87%E4%BB%B6%EF%BC%9Aw%E5%91%BD%E4%BB%A4)
[追加（行下）：a\命令](#%E8%BF%BD%E5%8A%A0%EF%BC%88%E8%A1%8C%E4%B8%8B%EF%BC%89%EF%BC%9Aa%5C%E5%91%BD%E4%BB%A4)
[插入（行上）：i\命令](#%E6%8F%92%E5%85%A5%EF%BC%88%E8%A1%8C%E4%B8%8A%EF%BC%89%EF%BC%9Ai%5C%E5%91%BD%E4%BB%A4)
[下一个：n命令](#%E4%B8%8B%E4%B8%80%E4%B8%AA%EF%BC%9An%E5%91%BD%E4%BB%A4)
[变形：y命令](#%E5%8F%98%E5%BD%A2%EF%BC%9Ay%E5%91%BD%E4%BB%A4)
[退出：q命令](#%E9%80%80%E5%87%BA%EF%BC%9Aq%E5%91%BD%E4%BB%A4)
[打印奇数行或偶数行](#%E6%89%93%E5%8D%B0%E5%A5%87%E6%95%B0%E8%A1%8C%E6%88%96%E5%81%B6%E6%95%B0%E8%A1%8C)
[奇数行](#%E5%A5%87%E6%95%B0%E8%A1%8C)
[偶数行](#%E5%81%B6%E6%95%B0%E8%A1%8C)
sed是一种流编辑器，它是文本处理中非常有用的工具，能够完美的配合正则表达式使用，功能不同凡响。处理时，把当前处理的行存储在临时缓冲区中，称为『模式空间』（pattern space），接着用sed命令处理缓冲区中的内容，处理完成后，把缓冲区的内容送往屏幕。接着处理下一行，这样不断重复，直到文件末尾。文件内容并没有改变，除非你使用重定向存储输出。sed主要用来自动编辑一个或多个文件，简化对文件的反复操作，编写转换程序等。
# sed的选项、命令、替换标记
## `命令格式`
```
sed [options] 'command' file(s)
sed [options] -f scriptfile file(s)
```
## `选项`
|参数|完整参数|说明|
|----|----|----|
|-e script|--expression=script|以选项中的指定的script来处理输入的文本文件|
|-f script|--files=script|以选项中的指定的script文件来处理输入的文本文件|
|-h|--help|显示帮助|
|-n|--quiet --silent|仅显示script处理后的结果|
|-V|--version|显示版本信息|
## `参数`
文件：指定待处理的文本文件列表
# `sed命令`
|命令|说明|
|----|----|
|d|删除，删除选择的行|
|D|删除模板块的第一行|
|s|替换指定字符|
|h|拷贝模板块的内容到内存中的缓冲区|
|H|追加模板块的内容到内存中的缓冲区|
|g|获得内存缓冲区的内容，并替代当前模板块中文本|
|G|获得内存缓冲区的内容，并追加到当前模板块文本的后面|
|l|列表不能打印字符的清单|
|n|读取下一个输入行，用下一个命令处理新的行而不是第一个命令|
|N|追加下一个输入行到模板块后面并在二者间嵌入一个新行，改变当前行号码|
|p|打印模板块的行|
|P|打印模板块的第一行|
|q|退出sed|
|b label|分支到脚本中带有标记的地方，如果分支不存在则分支到脚本的末尾|
|r file|从file中读行|
|t label|if分支，从最后一行开始，条件一旦满足或者T，t命令，将导致分支到带有标号的命令处，或者到脚本的末尾|
|T label|错误分支，从最后一行开始，一旦发生错误或者T，t命令，将导致分支到带有标号的命令处，或者到脚本的末尾|
|w file|写并追加模板块到file末尾|
|W file|写并追加模板块的第一行到file末尾|
|!|表示后面的命令对所有没有被选定的行发生作用|
|=|打印当前行号|
|#|把注释扩展到第一个换行符以前|
# `sed替换标记`
|命令|说明|
|----|----|
|g|表示行内全面替换|
|p|表示打印行|
|w|表示把行写入一个文件|
|x|表示互换模板块中的文本和缓冲区中的文本|
|y|表示把一个字符翻译为另外的字符（但是不用于正则表达式）|
|\1|子串匹配标记|
|&|已匹配字符串标记|
# `sed元字符集`
|命令|说明|
|----|----|
|^|匹配行开始，如：/^sed/匹配所有以sed开头的行。|
|$|匹配行结束，如：/sed$/匹配所有以sed结尾的行。|
|.|匹配一个非换行符的任意字符，如：/s.d/匹配s后接一个任意字符，最后是d。|
|*|匹配0个或多个字符，如：/*sed/匹配所有模板是一个或多个空格后紧跟sed的行。|
|[]|匹配一个指定范围内的字符，如/[sS]ed/匹配sed和Sed。|
|[^]|匹配一个不在指定范围内的字符，如：/[^A-RT-Z]ed/匹配不包含A-R和T-Z的一个字母开头，紧跟ed的行。|
|(..)|匹配子串，保存匹配的字符，如s/(love)able/\1rs，loveable被替换成lovers。|
|&|保存搜索字符用来替换其他字符，如s/love/**&**/，love这成**love**。|
|<|匹配单词的开始，如:/<love/匹配包含以love开头的单词的行。|
|>|匹配单词的结束，如/love>/匹配包含以love结尾的单词的行。|
|x{m}|重复字符x，m次，如：/0{5}/匹配包含5个0的行。|
|x{m,}|重复字符x，至少m次，如：/0{5,}/匹配至少有5个0的行。|
|x{m,n}|重复字符x，至少m次，不多于n次，如：/0{5,10}/匹配5~10个0的行。|
# sed用法实例
## 我们先准备一个测试文件
```
MacBook-Pro:tmp maxincai$ cat test.txt
my cat's name is betty
This is your dog
my dog's name is frank
This is your fish
my fish's name is george
This is your goat
my goat's name is adam
```
## `替换操作：s命令`
### 替换文本中的字符串：
```
MacBook-Pro:tmp maxincai$ sed 's/This/aaa/' test.txt
my cat's name is betty
aaa is your dog
my dog's name is frank
aaa is your fish
my fish's name is george
aaa is your goat
my goat's name is adam
```
### -n选项和p命令一起使用表示只打印那些发生替换的行：
```
MacBook-Pro:tmp maxincai$ sed -n 's/This/aaa/p' test.txt
aaa is your dog
aaa is your fish
aaa is your goat
```
> 
测试过程中发现mac os x和linux还是有点不一样，换回centos 6.5进行测试
直接编辑文件选项-i，会匹配test.txt文件中每一行的第一个This替换为this:
```
[root@vagrant-centos65 workspace]# sed -i 's/This/this/' test.txt
[root@vagrant-centos65 workspace]# cat test.txt
my cat's name is betty
this is your dog
my dog's name is frank
this is your fish
my fish's name is george
this is your goat
my goat's name is adam
```
## `全面替换标记g`
使用后缀/g标记会替换每一行中的所有匹配：
```
[root@vagrant-centos65 workspace]# sed 's/this/This/g' test.txt
my cat's name is betty
This is your This dog
my dog's name is This frank
This is your fish
my fish's name is This george
This is your goat
my goat's name is This adam
```
## 当需要从第N处匹配开始替换时，可以使用/Ng:
```
[root@vagrant-centos65 workspace]# echo sksksksksksk | sed 's/sk/SK/2g'
skSKSKSKSKSK
[root@vagrant-centos65 workspace]# echo sksksksksksk | sed 's/sk/SK/3g'
skskSKSKSKSK
[root@vagrant-centos65 workspace]# echo sksksksksksk | sed 's/sk/SK/4g'
skskskSKSKSK
```
# `定界符`
以上命令中字符 / 在sed中作为定界符使用，也可以使用任意的定界符：
```
[root@vagrant-centos65 workspace]# echo sksksksksksk | sed 's:sk:SK:4g'
skskskSKSKSK
[root@vagrant-centos65 workspace]# echo sksksksksksk | sed 's|sk|SK|4g'
skskskSKSKSK
```
定界符出现在样式内部时，需要进行转义：
```
[root@vagrant-centos65 workspace]# echo '/usr/local/bin' | sed 's/\/usr\/local\/bin/\/USR\/LOCAL\/BIN/g'
/USR/LOCAL/BIN
```
# `删除操作：d命令`
## 删除空白行：
```
[root@vagrant-centos65 workspace]# cat test.txt
my cat's name is betty
this is your this dog
my dog's name is this frank
this is your fish
my fish's name is this george
this is your goat
my goat's name is this adam
[root@vagrant-centos65 workspace]# sed '/^$/d' test.txt
my cat's name is betty
this is your this dog
my dog's name is this frank
this is your fish
my fish's name is this george
this is your goat
my goat's name is this adam
```
## 删除文件的第2行：
```
[root@vagrant-centos65 workspace]# sed '2d' test.txt
my cat's name is betty
my dog's name is this frank
this is your fish
my fish's name is this george
this is your goat
my goat's name is this adam
```
## 删除文件的第2行到末尾所有行：
```
[root@vagrant-centos65 workspace]# sed '2,$d' test.txt
my cat's name is betty
```
## 删除文件最后一行：
```
[root@vagrant-centos65 workspace]# sed '$d' test.txt
my cat's name is betty
this is your this dog
my dog's name is this frank
this is your fish
my fish's name is this george
this is your goat
```
## 删除文件中所有以my开头的行：
```
[root@vagrant-centos65 workspace]# sed '/^my/'d test.txt
this is your this dog
this is your fish
this is your goat
```
# `已匹配字符串标记&`
正则表达式\w\+匹配每一个单词，使用[&]替换它，&对应之前所匹配到的单词：
```
[root@vagrant-centos65 workspace]# echo this is a test line | sed 's/\w\+/[&]/g'
[this] [is] [a] [test] [line]
```
# `子串匹配标记\1`
匹配给定样式的其中一部份：
```
[root@vagrant-centos65 workspace]# echo this is digit 7 in a number | sed 's/digit \([0-9]\)/\1/'
this is 7 in a number
```
命令中digit 7，被替换成7.样式匹配到的子串是7，\(..\)用于匹配子串，对于匹配到的第一个子串标记为\1，依此类推匹配到的第二个结果就是\2,例如：
```
[root@vagrant-centos65 workspace]# echo aaa BBB | sed 's/\([a-z]\+\) \([A-Z]\+\)/\2 \1/'
BBB aaa
```
# `组合多个表达式`
sed '表达式' | sed '表达式'
等价于
sed '表达式; 表达式'
# `引用`
sed表达式可以使用单引号来引用，但是如果表达式内部包含变量字符串，就需要使用双引号。
```
[root@vagrant-centos65 workspace]# test=hello
[root@vagrant-centos65 workspace]# echo hello WORLD | sed "s/$test/HELLO/"
HELLO WORLD
```
# `选定行的范围：,(逗号)`
打印从第5行开始到第一个包含以this开始的行之间的所有行：
```
[root@vagrant-centos65 workspace]# sed -n '5,/^this/p' test.txt
my fish's name is this george
this is your goat
```
# `多点编辑：e命令`
-e选项允许在同一行里执行多条命令：
```
[root@vagrant-centos65 workspace]# sed -e '1,5d' -e 's/my/MY/' test.txt
this is your goat
MY goat's name is this adam
```
上面sed表达式的第一条命令删除1至5行，第二条命令用check替换test。命令的执行顺序对结果有影响。如果两个命令都是替换命令，那么第一个命令将影响第二个命令的结果。
和 -e 等价的命令是 --expression
# `从文件读入：r命令`
file里的内容被读进来，显示在与test匹配的行后面，如果匹配多行，则file的内容将显示在所有匹配行的下面：
```
[root@vagrant-centos65 workspace]# cat test1.txt
aaaaaaaa
[root@vagrant-centos65 workspace]# sed '/my/r test1.txt' test.txt
my cat's name is betty
aaaaaaaa
this is your this dog
my dog's name is this frank
aaaaaaaa
this is your fish
my fish's name is this george
aaaaaaaa
this is your goat
my goat's name is this adam
aaaaaaaa
```
# `写入文件：w命令`
在test.txt中所有包含my的行都被写入test2.txt里：
```
[root@vagrant-centos65 workspace]# sed -n '/my/w test2.txt' test.txt
[root@vagrant-centos65 workspace]# cat test2.txt
my cat's name is betty
my dog's name is this frank
my fish's name is this george
my goat's name is this adam
```
# `追加（行下）：a\命令`
将this is a test line 追加到以my开头的行后面：
```
[root@vagrant-centos65 workspace]# sed '/^my/a\this is a test line' test.txt
my cat's name is betty
this is a test line
this is your this dog
my dog's name is this frank
this is a test line
this is your fish
my fish's name is this george
this is a test line
this is your goat
my goat's name is this adam
this is a test line
```
在text.txt文件第2行之后插入this is a test line:
```
[root@vagrant-centos65 workspace]# sed '2a\this is a test line' test.txt
my cat's name is betty
this is your this dog
this is a test line
my dog's name is this frank
this is your fish
my fish's name is this george
this is your goat
my goat's name is this adam
```
# `插入（行上）：i\命令`
将this is a test line 插入到以my开头的行前面：
```
[root@vagrant-centos65 workspace]# sed '/^my/i\this is a test line' test.txt
this is a test line
my cat's name is betty
this is your this dog
this is a test line
my dog's name is this frank
this is your fish
this is a test line
my fish's name is this george
this is your goat
this is a test line
my goat's name is this adam
```
# `下一个：n命令`
如果my被匹配，则移动到匹配行的下一行，替换这一行的this为This,并打印该行：
```
[root@vagrant-centos65 workspace]# sed '/my/{n; s/this/This/; }' test.txt
my cat's name is betty
This is your this dog
my dog's name is this frank
This is your fish
my fish's name is this george
This is your goat
my goat's name is this adam
```
# `变形：y命令`
把1-10行内所有的abcde转变为大写，注意，正则表达式元字符不能使用这个命令：
```
[root@vagrant-centos65 workspace]# sed '1,10y/abcde/ABCDE/' test.txt
my CAt's nAmE is BEtty
this is your this Dog
my Dog's nAmE is this frAnk
this is your fish
my fish's nAmE is this gEorgE
this is your goAt
my goAt's nAmE is this ADAm
```
# `退出：q命令`
打印完第3行，退出sed
```
[root@vagrant-centos65 workspace]# sed '3q' test.txt
my cat's name is betty
this is your this dog
my dog's name is this frank
```
# `打印奇数行或偶数行`
方法1：
## 奇数行
```
[root@vagrant-centos65 workspace]# sed -n 'p;n' test.txt
my cat's name is betty
my dog's name is this frank
my fish's name is this george
my goat's name is this adam
```
## 偶数行
```
[root@vagrant-centos65 workspace]# sed -n 'n;p' test.txt
this is your this dog
this is your fish
this is your goat
```
方法2：
```
sed -n '1~2p' test.txt
sed -n '2~2p' test.txt
```
更多的需要在以后的工作中慢慢摸索，这里只是一个简单的记录，以后如果有更多经验了再完善一篇sed实战吧。
