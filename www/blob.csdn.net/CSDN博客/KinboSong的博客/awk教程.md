# awk教程 - KinboSong的博客 - CSDN博客
2018年03月22日 10:30:35[songjinbo3](https://me.csdn.net/KinboSong)阅读数：96
## **简介**
awk是一个强大的文本分析工具，相对于grep的查找，sed的编辑，awk在其对数据分析并生成报告时，显得尤为强大。简单来说awk就是把文件逐行的读入，以空格为默认分隔符将每行切片，切开的部分再进行各种分析处理。
awk有3个不同版本: awk、nawk和gawk，未作特别说明，一般指gawk，gawk 是 AWK 的 GNU 版本。
awk其名称得自于它的创始人 Alfred Aho 、Peter Weinberger 和 Brian Kernighan 姓氏的首个字母。实际上 AWK 的确拥有自己的语言： AWK 程序设计语言 ， 三位创建者已将它正式定义为“样式扫描和处理语言”。它允许您创建简短的程序，这些程序读取输入文件、为数据排序、处理数据、对输入执行计算以及生成报 表，还有无数其他的功能。
## **使用方法**
```
awk '{pattern + action}' {filenames}
```
尽管操作可能会很复杂，但语法总是这样，其中 pattern 表示 AWK 在数据中查找的内容，而 action 是在找到匹配内容时所执行的一系列命令。花括号（{}）不需要在程序中始终出现，但它们用于根据特定的模式对一系列指令进行分组。 pattern就是要表示的正则表达式，用斜杠括起来。
awk语言的最基本功能是在文件或者字符串中基于指定规则浏览和抽取信息，awk抽取信息后，才能进行其他文本操作。完整的awk脚本通常用来格式化文本文件中的信息。
通常，awk是以文件的一行为处理单位的。awk每接收文件的一行，然后执行相应的命令，来处理文本。
## **调用awk**
有三种方式调用awk
```
1.命令行方式
awk [-F  field-separator]  'commands'  input-file(s)
其中，commands 是真正awk命令，[-F域分隔符]是可选的。 input-file(s) 是待处理的文件。
在awk中，文件的每一行中，由域分隔符分开的每一项称为一个域。通常，在不指名-F域分隔符的情况下，默认的域分隔符是空格。
2.shell脚本方式
将所有的awk命令插入一个文件，并使awk程序可执行，然后awk命令解释器作为脚本的首行，一遍通过键入脚本名称来调用。
相当于shell脚本首行的：#!/bin/sh
可以换成：#!/bin/awk
3.将所有的awk命令插入一个单独文件，然后调用：
awk -f awk-script-file input-file(s)
其中，-f选项加载awk-script-file中的awk脚本，input-file(s)跟上面的是一样的。

本章重点介绍命令行方式。
 
## **入门实例**
假设last -n 5的输出如下
```
[root@www ~]# last -n 5 <==仅取出前五行
root     pts/1   192.168.1.100  Tue Feb 10 11:21   still logged in
root     pts/1   192.168.1.100  Tue Feb 10 00:46 - 02:28  (01:41)
root     pts/1   192.168.1.100  Mon Feb  9 11:41 - 18:30  (06:48)
dmtsai   pts/1   192.168.1.100  Mon Feb  9 11:41 - 11:41  (00:00)
root     tty1                   Fri Sep  5 14:09 - 14:10  (00:01)
如果只是显示最近登录的5个帐号
```
#last -n 5 | awk  '{print $1}'
root
root
root
dmtsai
root
awk工作流程是这样的：读入有’\n’换行符分割的一条记录，然后将记录按指定的域分隔符划分域，填充域，$0则表示所有域,$1表示第一个域,$n表示第n个域。默认域分隔符是"空白键" 或 "[tab]键",所以$1表示登录用户，$3表示登录用户ip,以此类推。
 
如果只是显示/etc/passwd的账户
```
#cat /etc/passwd |awk  -F ':'  '{print $1}'  
root
daemon
bin
sys
```
这种是awk+action的示例，每行都会执行action{print $1}。
-F指定域分隔符为’:’。
如果只是显示/etc/passwd的账户和账户对应的shell,而账户与shell之间以tab键分割
```
#cat /etc/passwd |awk  -F ':'  '{print $1"\t"$7}'
root    /bin/bash
daemon  /bin/sh
bin     /bin/sh
sys     /bin/sh
```
如果只是显示/etc/passwd的账户和账户对应的shell,而账户与shell之间以逗号分割,而且在所有行添加列名name,shell,在最后一行添加”blue,/bin/nosh”。
```
cat /etc/passwd |awk  -F ':'  'BEGIN {print "name,shell"}  {print $1","$7} END {print "blue,/bin/nosh"}'
name,shell
root,/bin/bash
daemon,/bin/sh
bin,/bin/sh
sys,/bin/sh
....
blue,/bin/nosh
awk工作流程是这样的：先执行BEGIN，然后读取文件，读入有/n换行符分割的一条记录，然后将记录按指定的域分隔符划分域，填充域，$0则表示所有域,$1表示第一个域,$n表示第n个域,随后开始执行模式所对应的动作action。接着开始读入第二条记录······直到所有的记录都读完，最后执行END操作。
 
搜索/etc/passwd有root关键字的所有行
```
#awk -F: '/root/' /etc/passwd
root:x:0:0:root:/root:/bin/bash
```
这种是pattern的使用示例，匹配了pattern(这里是root)的行才会执行action(没有指定action，默认输出每行的内容)。
搜索支持正则，例如找root开头的: awk -F: ‘/^root/’ /etc/passwd
搜索/etc/passwd有root关键字的所有行，并显示对应的shell
```
# awk -F: '/root/{print $7}' /etc/passwd             
/bin/bash
```
这里指定了action{print $7}
## **awk内置变量**
awk有许多内置变量用来设置环境信息，这些变量可以被改变，下面给出了最常用的一些变量。
```
ARGC               命令行参数个数
ARGV               命令行参数排列
ENVIRON            支持队列中系统环境变量的使用
FILENAME           awk浏览的文件名
FNR                浏览文件的记录数
FS                 设置输入域分隔符，等价于命令行 -F选项
NF                 浏览记录的域的个数
NR                 已读的记录数
OFS                输出域分隔符
ORS                输出记录分隔符
RS                 控制记录分隔符
此外,$0变量是指整条记录。$1表示当前行的第一个域,$2表示当前行的第二个域,……以此类推。
 
统计/etc/passwd:文件名，每行的行号，每行的列数，对应的完整行内容:
```
#awk  -F ':'  '{print "filename:" FILENAME ",linenumber:" NR ",columns:" NF ",linecontent:"$0}' /etc/passwd
filename:/etc/passwd,linenumber:1,columns:7,linecontent:root:x:0:0:root:/root:/bin/bash
filename:/etc/passwd,linenumber:2,columns:7,linecontent:daemon:x:1:1:daemon:/usr/sbin:/bin/sh
filename:/etc/passwd,linenumber:3,columns:7,linecontent:bin:x:2:2:bin:/bin:/bin/sh
filename:/etc/passwd,linenumber:4,columns:7,linecontent:sys:x:3:3:sys:/dev:/bin/sh
```
使用printf替代print,可以让代码更加简洁，易读
```
awk  -F ':'  '{printf("filename:%10s,linenumber:%s,columns:%s,linecontent:%s\n",FILENAME,NR,NF,$0)}' /etc/passwd
```
## **print和printf**
awk中同时提供了print和printf两种打印输出的函数。
其中print函数的参数可以是变量、数值或者字符串。**字符串必须用双引号引用**，**参数用逗号分隔**。如果没有逗号，参数就串联在一起而无法区分。这里，逗号的作用与输出文件的分隔符的作用是一样的，只是后者是空格而已。
printf函数，其用法和c语言中printf基本相似,可以格式化字符串,输出复杂时，printf更加好用，代码更易懂。
## ** awk编程**
** 变量和赋值**
除了awk的内置变量，awk还可以自定义变量。
下面统计/etc/passwd的账户人数
```
awk '{count++;print $0;} END{print "user count is ", count}' /etc/passwd
root:x:0:0:root:/root:/bin/bash
......
user count is  40
```
count是自定义变量。之前的action{}里都是只有一个print,其实print只是一个语句，而**action{}可以有多个语句，以;号隔开**。
这里没有初始化count，虽然默认是0，但是妥当的做法还是初始化为0:
```
awk 'BEGIN {count=0;print "[start]user count is ", count} {count=count+1;print $0;} END{print "[end]user count is ", count}' /etc/passwd
[start]user count is  0
root:x:0:0:root:/root:/bin/bash
...
[end]user count is  40
```
统计某个文件夹下的文件占用的字节数
```
ls -l |awk 'BEGIN {size=0;} {size=size+$5;} END{print "[end]size is ", size}'
[end]size is  8657198
```
如果以M为单位显示:
```
ls -l |awk 'BEGIN {size=0;} {size=size+$5;} END{print "[end]size is ", size/1024/1024,"M"}' 
[end]size is  8.25889 M
```
注意，统计不包括文件夹的子目录。
**条件语句**
awk中的条件语句是从C语言中借鉴来的，见如下声明方式：
```
if (expression) {
    statement;
    statement;
    ... ...
}
if (expression) {
    statement;
} else {
    statement2;
}
if (expression) {
    statement1;
} else if (expression1) {
    statement2;
} else {
    statement3;
}
 
统计某个文件夹下的文件占用的字节数,过滤4096大小的文件(一般都是文件夹):
```
ls -l |awk 'BEGIN {size=0;print "[start]size is ", size} {if($5!=4096){size=size+$5;}} END{print "[end]size is ", size/1024/1024,"M"}' 
[end]size is  8.22339 M
```
**循环语句**
awk中的循环语句同样借鉴于C语言，支持while、do/while、for、break、continue，这些关键字的语义和C语言中的语义完全相同。
**数组**
因为awk中数组的下标可以是数字和字母，数组的下标通常被称为关键字(key)。值和关键字都存储在内部的一张针对key/value应用hash的表格里。由于hash不是顺序存储，因此在显示数组内容时会发现，它们并不是按照你预料的顺序显示出来的。数组和变量一样，都是在使用时自动创建的，awk也同样会自动判断其存储的是数字还是字符串。一般而言，awk中的数组用来从记录中收集信息，可以用于计算总和、统计单词以及跟踪模板被匹配的次数等等。
显示/etc/passwd的账户
```
awk -F ':' 'BEGIN {count=0;} {name[count] = $1;count++;}; END{for (i = 0; i < NR; i++) print i, name[i]}' /etc/passwd
0 root
1 daemon
2 bin
3 sys
4 sync
5 games
......
这里使用for循环遍历数组
 
awk编程的内容极多，这里只罗列简单常用的用法，更多请参考 [http://www.gnu.org/software/gawk/manual/gawk.html](http://www.gnu.org/software/gawk/manual/gawk.html)
http://www.cnblogs.com/ggjucheng/archive/2013/01/13/2858470.html
 
- awk脚本是由模式和操作组成的：
pattern {action} 如$ awk '/root/' test，或$ awk ‘$3 < 100’ test。
两者是可选的，如果**没有模式，则action应用到全部记录**，如果**没有action，则输出匹配全部记录**。默认情况下，每一个输入行都是一条记录，但用户可通过RS变量指定不同的分隔符进行分隔。
### 3.1. 模式
模式可以是以下任意一个：
- 
/正则表达式/：使用通配符的扩展集。
- 
关系表达式：可以用下面运算符表中的关系运算符进行操作，可以是字符串或数字的比较，如$2>%1选择第二个字段比第一个字段长的行。
- 
模式匹配表达式：用运算符~(匹配)和~!(不匹配)。
- 
模式，模式：**指定一个行的范围**。该语法不能包括BEGIN和END模式。
- 
BEGIN：让用户指定**在第一条输入记录被处理之前所发生的动作**，通常可在这里设置全局变量。
- 
END：让用户在最后一条输入记录被**读取之后发生**的动作。
### 3.2. 操作
操作由一人或多个命令、函数、表达式组成，之间由换行符或分号隔开，并**位于大括号内**。主要有四部份：
- 
变量或数组赋值
- 
输出命令
- 
内置函数
- 
控制流命令

 
## 4. awk的环境变量
**Table 1. awk的环境变量**
|变量|描述|
|----|----|
|$n|当前记录的第n个字段，字段间由FS分隔。|
|$0|完整的输入记录。|
|ARGC|命令行参数的数目。|
|ARGIND|命令行中当前文件的位置(从0开始算)。|
|ARGV|包含命令行参数的数组。|
|CONVFMT|数字转换格式(默认值为%.6g)|
|ENVIRON|环境变量关联数组。|
|ERRNO|最后一个系统错误的描述。|
|FIELDWIDTHS|字段宽度列表(用空格键分隔)。|
|FILENAME|当前文件名。|
|FNR|同NR，但相对于当前文件。|
|FS|字段分隔符(默认是任何空格)。|
|IGNORECASE|如果为真，则进行忽略大小写的匹配。|
|NF|当前记录中的字段数。|
|NR|当前记录数。|
|OFMT|数字的输出格式(默认值是%.6g)。|
|OFS|输出字段分隔符(默认值是一个空格)。|
|ORS|输出记录分隔符(默认值是一个换行符)。|
|RLENGTH|由match函数所匹配的字符串的长度。|
|RS|记录分隔符(默认是一个换行符)。|
|RSTART|由match函数所匹配的字符串的第一个位置。|
|SUBSEP|数组下标分隔符(默认值是\034)。|
## 5. awk运算符
**Table 2. 运算符**
|运算符|描述|
|----|----|
|= += -= *= /= %= ^= **=|赋值|
|?:|C条件表达式|
||||逻辑或|
|&&|逻辑与|
|~ ~!|匹配正则表达式和不匹配正则表达式|
|< <= > >= != ==|关系运算符|
|空格|连接|
|+ -|加，减|
|* / &|乘，除与求余|
|+ - !|一元加，减和逻辑非|
|^ ***|求幂|
|++ –|增加或减少，作为前缀或后缀|
|$|字段引用|
|in|数组成员|
## 6. 记录和域
### 6.1. 记录
awk把**每一个以换行符结束的行称为一个记录**。
记录分隔符：默认的输入和输出的分隔符都是**回车**，保存在内建变量ORS和RS中。
$0变量：它指的是整条记录。如$ awk ‘{print $0}’ test将输出test文件中的所有记录。
变量NR：一个计数器，每处理完一条记录，NR的值就增加1。如$ awk '{print NR,$0}’ test将输出test文件中所有记录，并在记录前显示记录号。

### 6.2. 域
**记录中每个单词称做“域”**，默认情况下以空格或tab分隔。awk可跟踪域的个数，并在内建变量NF中保存该值。如$ awk '{print $1,$3}’ test将打印test文件中第一和第三个以空格分开的列(域)。

### 6.3. 域分隔符
内建变量FS保存输入域分隔符的值，默认是空格或tab。我们可以通过-F命令行选项修改FS的值。如$ awk -F: '{print $1,$5}’ test将打印以冒号为分隔符的第一，第五列的内容。
可以同时使用多个域分隔符，这时应该把分隔符写成放到方括号中，如$awk -F'[:\t]' '{print $1,$3}’ test，表示以空格、冒号和tab作为分隔符。
输出域的分隔符默认是一个空格，保存在OFS中。如$ awk -F: '{print $1,$5}' test，$1和$5间的逗号就是OFS的值。
 
## 7. gawk专用正则表达式元字符
以下几个是gawk专用的，不适合unix版本的awk。
- \Y
匹配一个单词开头或者末尾的空字符串。
- \B
匹配单词内的空字符串。
- \<
匹配一个单词的开头的空字符串，锚定开始。
- \>
匹配一个单词的末尾的空字符串，锚定末尾。
- \w
匹配一个字母数字组成的单词。
- \W
匹配一个非字母数字组成的单词。
- \‘
匹配字符串开头的一个空字符串。
- \’
匹配字符串末尾的一个空字符串。
## 8. POSIX字符集
 

## 9. 匹配操作符(~)
用来在记录或者域内匹配正则表达式。如$ awk '$1 ~/^root/’ test将显示test文件第一列中以root开头的行。

## 10. 比较表达式
conditional expression1 ? expression2: expression3，例如：$ awk '{max = {$1 > $3} ? $1: $3: print max}' test。如果第一个域大于第三个域，$1就赋值给max，否则$3就赋值给max。
$ awk '$1 + $2 < 100’ test。如果第一和第二个域相加大于100，则打印这些行。
$ awk '$1 > 5 && $2 < 10’ test,如果第一个域大于5，并且第二个域小于10，则打印这些行。

## 11. 范围模板
范围模板匹配从第一个模板的第一次出现到第二个模板的第一次出现之间所有行。如果有一个模板没出现，则匹配到开头或末尾。如$ awk ‘/root/,/mysql/’ test将显示root第一次出现到mysql第一次出现之间的所有行。

## 12. 一个验证passwd文件有效性的例子
$ cat /etc/passwd | awk -F: '\<br>NF != 7{\<br>printf("line %d,does not have 7 fields:%s\n",NR,$0)}\<br>$1 !~ /[A-Za-z0-9]/{printf(“line %d,non alpha and numeric user id:%d: %s\n,NR,$0)}\
$2 == "*" {printf("line %d, no password: %s\n",NR,$0)}’
|![1](https://www.cnblogs.com/emanlee/admin/images/callouts/1.png)|cat把结果输出给awk，awk把域之间的分隔符设为冒号。|
|----|----|
|![2](https://www.cnblogs.com/emanlee/admin/images/callouts/2.png)|如果域的数量(NF)不等于7，就执行下面的程序。|
|![3](https://www.cnblogs.com/emanlee/admin/images/callouts/3.png)|printf打印字符串”line ?? does not have 7 fields”，并显示该条记录。|
|![4](https://www.cnblogs.com/emanlee/admin/images/callouts/4.png)|如果第一个域没有包含任何字母和数字，printf打印“no alpha and numeric user id” ，并显示记录数和记录。|
|![5](https://www.cnblogs.com/emanlee/admin/images/callouts/5.png)|如果第二个域是一个星号，就打印字符串“no passwd”，紧跟着显示记录数和记录本身。|


## 13. 几个实例

- 
$ awk ‘/^(no|so)/’ test—–打印所有以模式no或so开头的行。
- 
$ awk '/^[ns]/{print $1}’ test—–如果记录以n或s开头，就打印这个记录。
- 
$ awk '$1 ~/[0-9][0-9]$/(print $1}’ test—–如果第一个域以两个数字结束就打印这个记录。
- 
$ awk '$1 == 100 || $2 < 50’ test—–如果第一个或等于100或者第二个域小于50，则打印该行。
- 
$ awk '$1 != 10’ test—–如果第一个域不等于10就打印该行。
- 
$ awk '/test/{print $1 + 10}’ test—–如果记录包含正则表达式test，则第一个域加10并打印出来。
- 
$ awk '{print ($1 > 5 ? "ok "$1: "error"$1)}’ test—–如果第一个域大于5则打印问号后面的表达式值，否则打印冒号后面的表达式值。
- 
$ awk ‘/^root/,/^mysql/’ test—-打印以正则表达式root开头的记录到以正则表达式mysql开头的记录范围内的所有记录。如果找到一个新的正则表达式root开头的记录，则继续打印直到下一个以正则表达式mysql开头的记录为止，或到文件末尾。

## 14. awk编程

### 14.1. 变量

- 
在awk中，变量不需要定义就可以直接使用，变量类型可以是数字或字符串。
- 
赋值格式：Variable = expression，如$ awk '$1 ~/test/{count = $2 + $3; print count}’ test,上式的作用是,awk先扫描第一个域，一旦test匹配，就把第二个域的值加上第三个域的值，并把结果赋值给变量count，最后打印出来。
- 
awk可以在命令行中给变量赋值，然后将这个变量传输给awk脚本。如$ awk -F: -f awkscript month=4 year=2004 test，上式的month和year都是自定义变量，分别被赋值为4和2004。在awk脚本中，这些变量使用起来就象是在脚本中建立的一样。注意，如果参数前面出现test，那么在BEGIN语句中的变量就不能被使用。
- 
域变量也可被赋值和修改，如$ awk '{$2 = 100 + $1; print }' test,上式表示，如果第二个域不存在，awk将计算表达式100加$1的值，并将其赋值给$2，如果第二个域存在，则用表达式的值覆盖$2原来的值。再例如：$ awk '$1 == “root”{$1 =”test”;print}’ test，如果第一个域的值是“root”，则把它赋值为“test”，注意，字符串一定要用双引号。
- 
内建变量的使用。变量列表在前面已列出，现在举个例子说明一下。$ awk -F: '{IGNORECASE=1; $1 == "MARY"{print NR,$1,$2,$NF}’test，把IGNORECASE设为1代表忽略大小写，打印第一个域是mary的记录数、第一个域、第二个域和最后一个域。

### 14.2. BEGIN模块

BEGIN模块后紧跟着动作块，这个动作块在awk处理任何输入文件之前执行。所以它可以在没有任何输入的情况下进行测试。它通常用来改变内建变量的值，如OFS,RS和FS等，以及打印标题。如：$ awk 'BEGIN{FS=":"; OFS="\t"; ORS="\n\n"}{print $1,$2,$3} test。上式表示，在处理输入文件以前，域分隔符(FS)被设为冒号，输出文件分隔符(OFS)被设置为制表符，输出记录分隔符(ORS)被设置为两个换行符。$ awk ‘BEGIN{print “TITLE TEST”}只打印标题。

### 14.3. END模块

END不匹配任何的输入文件，但是执行动作块中的所有动作，它在整个输入文件处理完成后被执行。如$ awk ‘END{print “The number of records is” NR}’ test，上式将打印所有被处理的记录数。

### 14.4. 重定向和管道

- 
awk可使用shell的重定向符进行重定向输出，如：$ awk '$1 = 100 {print $1 > “output_file” }’ test。上式表示如果第一个域的值等于100，则把它输出到output_file中。也可以用>>来重定向输出，但不清空文件，只做追加操作。
- 
输出重定向需用到**getline**函数。**getline**从标准输入、管道或者当前正在处理的文件之外的其他输入文件获得输入。它负责从输入获得下一行的内容，并给NF,NR和FNR等内建变量赋值。如果得到一条记录，**getline**函数返回1，如果到达文件的末尾就返回0，如果出现错误，例如打开文件失败，就返回-1。如：
$ awk ‘BEGIN{ “date” | **getline** d; print d}’ test。执行linux的date命令，并通过管道输出给**getline**，然后再把输出赋值给自定义变量d，并打印它。
$ awk ‘BEGIN{“date” | **getline** d; split(d,mon); print mon[2]}’ test。执行shell的date命令，并通过管道输出给**getline**，然后getline从管道中读取并将输入赋值给d，split函数把变量d转化成数组mon，然后打印数组mon的第二个元素。
$ awk ‘BEGIN{while( “ls” | **getline**) print}’，命令ls的输出传递给geline作为输入，循环使getline从ls的输出中读取一行，并把它打印到屏幕。这里没有输入文件，因为BEGIN块在打开输入文件前执行，所以可以忽略输入文件。
$ awk 'BEGIN{printf "What is your name?"; getline name < "/dev/tty" } $1 ~name {print “Found” name on line “, NR “.”} END{print “See you,” name “.”} test。在屏幕上打印”What is your name?”,并等待用户应答。当一行输入完毕后，getline函数从终端接收该行输入，并把它储存在自定义变量name中。如果第一个域匹配变量name的值，print函数就被执行，END块打印See you和name的值。
$ awk ‘BEGIN{while (getline < “/etc/passwd” > 0) lc++; print lc}’。awk将逐行读取文件/etc/passwd的内容，在到达文件末尾前，计数器lc一直增加，当到末尾时，打印lc的值。注意，如果文件不存在，getline返回-1，如果到达文件的末尾就返回0，如果读到一行，就返回1，所以命令 while (getline < “/etc/passwd”)在文件不存在的情况下将陷入无限循环，因为返回-1表示逻辑真。
- 
可以在awk中打开一个管道，且同一时刻只能有一个管道存在。通过close()可关闭管道。如：$ awk '{print $1, $2 | “sort” }’ test END {close(“sort”)}。awd把print语句的输出通过管道作为linux命令sort的输入,END块执行关闭管道操作。
- 
system函数可以在awk中执行linux的命令。如：$ awk ‘BEGIN{system(“clear”)’。
- 
fflush函数用以刷新输出缓冲区，如果没有参数，就刷新标准输出的缓冲区，如果以空字符串为参数，如fflush(“”),则刷新所有文件和管道的输出缓冲区。

### 14.5. 条件语句

awk中的条件语句是从C语言中借鉴过来的，可控制程序的流程。
#### 14.5.1. if 语句
格式：
        {if (expression){
                   statement; statement; ...
                     }
        }
$ awk '{if ($1 <$2) print $2 “too high”}’ test。如果第一个域小于第二个域则打印。
$ awk '{if ($1 < $2) {count++; print “ok”}}’ test.如果第一个域小于第二个域，则count加一，并打印ok。
#### 14.5.2. if/else语句，用于双重判断
格式：
        {if (expression){
                   statement; statement; ...
                       }
        else{
                   statement; statement; ...
                       }
        }
$ awk '{if ($1 > 100) print $1 "bad" ; else print "ok"}' test。如果$1大于100则打印$1 bad,否则打印ok。
$ awk '{if ($1 > 100){ count++; print $1} else {count--; print $2}' test。如果$1大于100，则count加一，并打印$1，否则count减一，并打印$1。
#### 14.5.3. if/else else if语句，用于多重判断。
格式：
        {if (expression){
                    statement; statement; ...
                   }
        else if (expression){
                    statement; statement; ...
                   }
        else if (expression){
                    statement; statement; ...
                   }
        else {
                   statement; statement; ...
             }
        }
### 14.6. 循环
- 
awk有三种循环: **while**循环；**for**循环；**special for**循环。
- 
$ awk '{ i = 1; while ( i <= NF ) { print NF,$i; i++}}’ test。变量的初始值为1，若i小于可等于NF(记录中域的个数),则执行打印语句，且i增加1。直到i的值大于NF.
- 
$ awk '{for (i = 1; i<NF; i++) print NF,$i}’ test。作用同上。
- 
**bread** **continue** 语句。break用于在满足条件的情况下跳出循环；continue用于在满足条件的情况下忽略后面的语句，直接返回循环的顶端。如：
{for ( x=3; x<=NF; x++) 
            if ($x<0){print "Bottomed out!"; break}}
{for ( x=3; x<=NF; x++)
            if ($x==0){print "Get next item"; continue}}
      
- 
**next** 语句从输入文件中读取一行，然后从头开始执行awk脚本。如：
{if ($1 ~/test/){next}
    else {print}
}
     
- 
**exit** 语句用于结束awk程序，但不会略过END块。退出状态为0代表成功，非零值表示出错。
### 14.7. 数组
awk中的数组的下标可以是数字和字母，称为关联数组。
#### 14.7.1. 下标与关联数组
- 
用变量作为数组下标。如：$ awk {name[x++]=$2};END{for(i=0;i<NR;i++) print i,name[i]}’ test。数组name中的下标是一个自定义变量x，awk初始化x的值为0，在每次使用后增加1。第二个域的值被赋给name数组的各个元素。在END模块中，for循环被用于循环整个数组，从下标为0的元素开始，打印那些存储在数组中的值。因为下标是关健字，所以它不一定从0开始，可以从任何值开始。
- 
special for 循环用于读取关联数组中的元素。格式如下：
{for (item in arrayname){
         print arrayname[item]
         }
}
$ awk '/^tom/{name[NR]=$1}; END{for(i in name){print name[i]}}’ test。打印有值的数组元素。打印的顺序是随机的。
- 
用字符串作为下标。如：count[“test”]
- 
用域值作为数组的下标。一种新的for循环方式，for (index_value in array) statement。如:$ awk '{count[$1]++} END{for(name in count) print name,count[name]}' test。该语句将打印$1中字符串出现的次数。它首先以第一个域作数组count的下标，第一个域变化，索引就变化。
- 
delete 函数用于删除数组元素。如：$ awk '{line[x++]=$1} END{for(x in line) delete(line[x])}’ test。分配给数组line的是第一个域的值，所有记录处理完成后，special for循环将删除每一个元素。
 
 
 
### 14.8. awk的内建函数
#### 14.8.1. 字符串函数
- 
**sub** 函数匹配记录中最大、最靠左边的子字符串的正则表达式，并用替换字符串替换这些字符串。如果没有指定目标字符串就默认使用整个记录。替换只发生在第一次匹配的时候。格式如下：
            sub (regular expression, substitution string):
            sub (regular expression, substitution string, target string)
实例：
            $ awk '{ sub(/test/, "mytest"); print }' testfile
            $ awk '{ sub(/test/, "mytest"); $1}; print }' testfile
第一个例子在整个记录中匹配，替换只发生在第一次匹配发生的时候。如要在整个文件中进行匹配需要用到gsub
第二个例子在整个记录的第一个域中进行匹配，替换只发生在第一次匹配发生的时候。
- 
**gsub** 函数作用如sub，但它在整个文档中进行匹配。格式如下：
            gsub (regular expression, substitution string)
            gsub (regular expression, substitution string, target string)
实例：
            $ awk '{ gsub(/test/, "mytest"); print }' testfile
            $ awk '{ gsub(/test/, "mytest" , $1) }; print }' testfile
第一个例子在整个文档中匹配test，匹配的都被替换成mytest。
第二个例子在整个文档的第一个域中匹配，所有匹配的都被替换成mytest。
- 
**index** 函数返回子字符串第一次被匹配的位置，偏移量从位置1开始。格式如下：
          index(string, substring)
实例：
            $ awk '{ print index("test", "mytest") }' testfile
实例返回test在mytest的位置，结果应该是3。
- 
**length** 函数返回记录的字符数。格式如下：
            length( string )
            length
实例：
            $ awk '{ print length( "test" ) }' 
            $ awk '{ print length }' testfile
第一个实例返回test字符串的长度。
第二个实例返回testfile文件中第条记录的字符数。
- 
**substr** 函数返回从位置1开始的子字符串，如果指定长度超过实际长度，就返回整个字符串。格式如下：
            substr( string, starting position )
            substr( string, starting position, length of string )
实例：
            $ awk '{ print substr( "hello world", 7,11 ) }' 
上例截取了world子字符串。
- 
**match** 函数返回在字符串中正则表达式位置的索引，如果找不到指定的正则表达式则返回0。match函数会设置内建变量RSTART为字符串中子字符串的开始位置，RLENGTH为到子字符串末尾的字符个数。substr可利于这些变量来截取字符串。函数格式如下：
            match( string, regular expression )
实例：
            $ awk '{start=match("this is a test",/[a-z]+$/); print start}'
            $ awk '{start=match("this is a test",/[a-z]+$/); print start, RSTART, RLENGTH }'
第一个实例打印以连续小写字符结尾的开始位置，这里是11。
第二个实例还打印RSTART和RLENGTH变量，这里是11(start)，11(RSTART)，4(RLENGTH)。
- 
**toupper** 和**tolower** 函数可用于字符串大小间的转换，该功能只在gawk中有效。格式如下：
            toupper( string )
            tolower( string )
实例：
            $ awk '{ print toupper("test"), tolower("TEST") }'
- 
**split** 函数可按给定的分隔符把字符串分割为一个数组。如果分隔符没提供，则按当前FS值进行分割。格式如下：
            split( string, array, field separator )
            split( string, array )
实例：
            $ awk '{ split( "20:18:00", time, ":" ); print time[2] }'
上例把时间按冒号分割到time数组内，并显示第二个数组元素18。
#### 14.8.2. 时间函数
- 
systime函数返回从1970年1月1日开始到当前时间(不计闰年)的整秒数。格式如下：
            systime()
实例：
            $ awk '{ now = systime(); print now }'
- 
strftime函数使用C库中的strftime函数格式化时间。格式如下：
            systime( [format specification][,timestamp] )
**Table 3. 日期和时间格式说明符**
|格式|描述|
|----|----|
|%a|星期几的缩写(Sun)|
|%A|星期几的完整写法(Sunday)|
|%b|月名的缩写(Oct)|
|%B|月名的完整写法(October)|
|%c|本地日期和时间|
|%d|十进制日期|
|%D|日期 08/20/99|
|%e|日期，如果只有一位会补上一个空格|
|%H|用十进制表示24小时格式的小时|
|%I|用十进制表示12小时格式的小时|
|%j|从1月1日起一年中的第几天|
|%m|十进制表示的月份|
|%M|十进制表示的分钟|
|%p|12小时表示法(AM/PM)|
|%S|十进制表示的秒|
|%U|十进制表示的一年中的第几个星期(星期天作为一个星期的开始)|
|%w|十进制表示的星期几(星期天是0)|
|%W|十进制表示的一年中的第几个星期(星期一作为一个星期的开始)|
|%x|重新设置本地日期(08/20/99)|
|%X|重新设置本地时间(12：00：00)|
|%y|两位数字表示的年(99)|
|%Y|当前月份|
|%Z|时区(PDT)|
|%%|百分号(%)|
实例：
            $ awk '{ now=strftime( "%D", systime() ); print now }'
            $ awk '{ now=strftime("%m/%d/%y"); print now }'
 
 
#### 14.8.3. 内建数学函数
**Table 4. **
|函数名称|返回值|
|----|----|
|atan2(x,y)|y,x范围内的余切|
|cos(x)|余弦函数|
|exp(x)|求幂|
|int(x)|取整|
|log(x)|自然对数|
|rand()|随机数|
|sin(x)|正弦|
|sqrt(x)|平方根|
|srand(x)|x是rand()函数的种子|
|int(x)|取整，过程没有舍入|
|rand()|产生一个大于等于0而小于1的随机数|
#### 14.8.4. 自定义函数
在awk中还可自定义函数，格式如下：
        function name ( parameter, parameter, parameter, ... ) {
                        statements
                        return expression                  # the return statement and expression are optional
        }
 
 
## 15. How-to
- 
如何把一行竖排的数据转换成横排？
awk ‘{printf(“%s,”,$1)}’ filename
 
## awk中使用NR和FNR的一些例子
http://blog.sina.com.cn/s/blog_5a3640220100b7c8.html
http://www.linuxidc.com/Linux/2012-05/61174.htm
一般在awk里面输入文件是多个时，NR==FNR才有意义，如果这个值为true，表示还在处理第一个文件。
NR==FNR 這個一般用於讀取兩個或者兩個以上的文件中，用於判斷是在讀取第一個文件。。
test.txt 10行内容
test2.txt 4行内容
awk ‘{print NR,FNR}’ test.txt test2.txt
1 1
2 2
3 3
4 4
5 5
6 6
7 7
8 8
9 9
10 10
11 1
12 2
13 3
14 4
 
现在有两个文件格式如下：
#cat account
张三|000001
李四|000002
#cat cdr
000001|10
000001|20
000002|30
000002|15
想要得到的结果是将用户名，帐号和金额在同一行打印出来,如下:
张三|000001|10
张三|000001|20
李四|000002|30
李四|000002|15
执行如下代码
#awk -F \| ‘NR==FNR{a[$2]=$0;next}{print a[$1]"|"$2}’ account cdr
注释:
由NR=FNR为真时,判断当前读入的是第一个文件account,然后使用{a[$2]=$0;next}循环将account文件的每行记录都存入数组a,并使用$2第2个字段作为下标引用.
由NR=FNR为假时,判断当前读入了第二个文件cdr,然后跳过{a[$2]=$0;next},对第二个文件cdr的每一行都无条件执行{print  a[$1]"|"$2},此时变量$1为第二个文件的第一个字段,与读入第一个文件时,采用第一个文件第二个字段$2为数组下标相同.因此可以在此使用a[$1]引用数组。
 
 
awk ‘{gsub(/$/,”“);gsub(/,/,”“);
if ($1>=0.1 && $1<0.2) c1+=1;
else if ($1>=0.2 && $1<0.3) c2+=1;
else if ($1>=0.3 && $1<0.4) c3+=1;
else if ($1>=0.4 && $1<0.5) c4+=1;
else if ($1>=0.5 && $1<0.6) c5+=1;
else if ($1>=0.6 && $1<0.7) c6+=1;
else if ($1>=0.7 && $1<0.8) c7+=1;
else if ($1>=0.8 && $1<0.9) c8+=1;
else if ($1>=0.9 ) c9+=1;
else c10+=1; }
END {printf  “%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t”,c1,c2,c3,c4,c5,c6,c7,c8,c9,c10} ’ /NEW
 
**示例/例子**：
awk ‘{if($0~/^>.*$/) {tmp=$0; getline; if( length($0)>=200) {print  tmp”\n”$0; } }}’  filename
awk ‘{if($0~/^>.*$/) {IGNORECASE=1; if($0~/PREDICTED/) {getline;} else {print $0; getline;  print $0; } }}’ filename
awk ‘{if($0~/^>.*$/) {IGNORECASE=1; if($0~/mRNA/) {print $0; getline;  print $0; } else {getline;} }}’  filename
awk ‘{ temp=$0; getline; if($0~/unavailable/) {;} else {print temp”\n”$0;} }’ filename
 
substr($4,20)     —>  表示是从第4个字段里的第20个字符开始，一直到设定的分隔符”,”结束.
substr($3,12,8)  —>  表示是从第3个字段里的第12个字符开始，截取8个字符结束.
 
一、awk**字符串转数字**
$ awk 'BEGIN{a="100";b="10test10";print (a+b+0);}' <br>110 <br>只需要将变量通过”+”连接运算。自动强制将字符串转为整型。非数字变成0，发现第一个非数字字符，后面自动忽略。 <br>二、awk<strong>数字转为字符串</strong> <br>$ awk ‘BEGIN{a=100;b=100;c=(a”“b);print c}’      
100100 
只需要将变量与””符号连接起来运算即可。 
三、awk**字符串连接操作(字符串连接；链接；串联)** 
$ awk 'BEGIN{a="a";b="b";c=(a""b);print c}'      <br>ab <br>$ awk ‘BEGIN{a=”a”;b=”b”;c=(a+b);print c}’  
0 
 
**把文件中的各行串联起来：**
awk ‘BEGIN{xxxx=”“;}{xxxx=(xxxx”“$0);}END{print xxxx}' temp.txt<br>awk 'BEGIN{xxxx="";}{xxxx=(xxxx"\",\""$0);}END{print xxxx}’ temp.txt
 
 
来源：[](http://www.cnblogs.com/emanlee/p/3327576.html)[http://www.cnblogs.com/emanlee/p/3327576.html](http://www.cnblogs.com/emanlee/p/3327576.html)
```
```
```
```
```
```
```
