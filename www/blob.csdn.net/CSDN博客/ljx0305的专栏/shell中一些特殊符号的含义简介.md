# shell中一些特殊符号的含义简介 - ljx0305的专栏 - CSDN博客
2012年11月07日 16:28:18[ljx0305](https://me.csdn.net/ljx0305)阅读数：4747
在shell中常用的特殊符号罗列如下：
# ;    ;;       .       ,        /        /        'string'|        !    $    ${}    $?       $$    $*        "string"*      **    ?       :        ^       $#    $@        `command`{}     []       [[]]      ()     (())         ||    &&        {xx,yy,zz,...}~    ~+    ~-     &    /<.../>    +        -         %=    ==    !=                    
输出/输入重导向
>       >>    <    <<    : >    &>    2&>    2<>>&    >&2                         
# 井号 (comments)
这几乎是个满场都有的符号，除了先前已经提过的"第一行"
#!/bin/bash
井号也常出现在一行的开头，或者位于完整指令之后，这类情况表示符号后面的是注解文字，不会被执行。
# This line is comments.
echo "a = $a" # a = 0 
由于这个特性，当临时不想执行某行指令时，只需在该行开头加上 # 就行了。这常用在撰写过程中。
#echo "a = $a" # a = 0
如果被用在指令中，或者引号双引号括住的话，或者在倒斜线的后面，那他就变成一般符号，不具上述的特殊功能。
~ 帐户的 home 目录~~~~~~
算是个常见的符号，代表使用者的 home 目录：cd ~；也可以直接在符号后加上某帐户的名称：cd ~user
或者当成是路径的一部份：~/bin；~+ 当前的工作目录，这个符号代表当前的工作目录，她和内建指令 pwd 的作用是相同的。
# echo ~+/var/log 
~- 上次的工作目录，这个符号代表上次的工作目录。
# echo ~-/etc/httpd/logs
; 分号 (Command separator)
在 shell 中，担任"连续指令"功能的符号就是"分号"。譬如以下的例子：cd ~/backup ; mkdir startup ; cp ~/.* startup/.
;; 连续分号 (Terminator)
专用在 case 的选项，担任 Terminator 的角色。
case "$fop" inhelp) echo "Usage: Command -help -version filename" ;;version) echo "version 0.1" ;;esac
. 逗号 (dot)
在 shell 中，使用者应该都清楚，一个 dot 代表当前目录，两个 dot 代表上层目录。
CDPATH=.:~:/home:/home/web:/var:/usr/local
在上行 CDPATH 的设定中，等号后的 dot 代表的就是当前目录的意思。
如果档案名称以 dot 开头，该档案就属特殊档案，用 ls 指令必须加上 -a 选项才会显示。除此之外，在 regular expression 中，一个 dot 代表匹配一个字元。
'string' 单引号 (single quote)
被单引号用括住的内容，将被视为单一字串。在引号内的代表变数的$符号，没有作用，也就是说，他被视为一般符号处理，防止任何变量替换。
heyyou=homeecho '$heyyou' # We get $heyyou
"string" 双引号 (double quote)
被双引号用括住的内容，将被视为单一字串。它防止通配符扩展，但允许变量扩展。这点与单引数的处理方式不同。
heyyou=homeecho "$heyyou" # We get home
`command` 倒引号 (backticks)
在前面的单双引号，括住的是字串，但如果该字串是一列命令列，会怎样？答案是不会执行。要处理这种情况，我们得用倒单引号来做。
fdv=`date +%F`echo "Today $fdv"
在倒引号内的 date +%F 会被视为指令，执行的结果会带入 fdv 变数中。
, 逗点 (comma)
这个符号常运用在运算当中当做"区隔"用途。如下例
#!/bin/bashlet "t1 = ((a = 5 + 3, b = 7 - 1, c = 15 / 3))"echo "t1 = $t1, a = $a, b = $b"
/ 斜线 (forward slash)
在路径表示时，代表目录。
cd /etc/rc.dcd ../..cd /
通常单一的 / 代表 root 根目录的意思；在四则运算中，代表除法的符号。
文章出处：http://www.diybl.com/course/3_program/shell/shelljs/2008614/125554.htmllet "num1 = ((a = 10 / 2, b = 25 / 5))"
/ 倒斜线 ()
在交互模式下的escape 字元，有几个作用；放在指令前，有取消 aliases 的作用；放在特殊符号前，则该特殊符号的作用消失；放在指令的最末端，表示指令连接下一行。
# type rmrm is aliased to `rm -i'# /rm ./*.log
上例，我在 rm 指令前加上 escape 字元，作用是暂时取消别名的功能，将 rm 指令还原。
# bkdir=/home# echo "Backup dir, /$bkdir = $bkdir"Backup dir, $bkdir = /home
上例 echo 内的 /$bkdir，escape 将 $ 变数的功能取消了，因此，会输出 $bkdir，而第二个 $bkdir 则会输出变数的内容 /home。
| 管道 (pipeline)
pipeline 是 UNIX 系统，基础且重要的观念。连结上个指令的标准输出，做为下个指令的标准输入。
who | wc -l
善用这个观念，对精简 script 有相当的帮助。
! 惊叹号(negate or reverse)
通常它代表反逻辑的作用，譬如条件侦测中，用 != 来代表"不等于"
if [ "$?" != 0 ]thenecho "Executes error"exit 1fi
在规则表达式中她担任 "反逻辑" 的角色
ls a[!0-9]
上例，代表显示除了a0, a1 .... a9 这几个文件的其他文件。
: 冒号
在 bash 中，这是一个内建指令："什么事都不干"，但返回状态值 0。
:
echo $? # 回应为 0
: > f.$$
上面这一行，相当于 cat /dev/null > f.$$。不仅写法简短了，而且执行效率也好上许多。
有时，也会出现以下这类的用法
: ${HOSTNAME?} ${USER?} ${MAIL?}
这行的作用是，检查这些环境变数是否已设置，没有设置的将会以标准错误显示错误讯息。像这种检查如果使用类似 test 或 if 这类的做法，基本上也可以处理，但都比不上上例的简洁与效率。
除了上述之外，还有一个地方必须使用冒号
PATH=$PATH:$HOME/fbin:$HOME/fperl:/usr/local/mozilla
在使用者自己的HOME 目录下的 .bash_profile 或任何功能相似的档案中，设定关于"路径"的场合中，我们都使用冒号，来做区隔。
? 问号 (wild card)
在文件名扩展(Filename expansion)上扮演的角色是匹配一个任意的字元，但不包含 null 字元。
# ls a?a1
善用她的特点，可以做比较精确的档名匹配。
* 星号 (wild card)
相当常用的符号。在文件名扩展(Filename expansion)上，她用来代表任何字元，包含 null 字元。
# ls a*a a1 access_log
在运算时，它则代表 "乘法"。
let "fmult=2*3"
除了内建指令 let，还有一个关于运算的指令 expr，星号在这里也担任"乘法"的角色。不过在使用上得小心，他的前面必须加上escape 字元。
** 次方运算
两个星号在运算时代表 "次方" 的意思。
let "sus=2**3"echo "sus = $sus" # sus = 8
$ 钱号(dollar sign)
变量替换(Variable Substitution)的代表符号。
vrs=123echo "vrs = $vrs" # vrs = 123
另外，在 Regular Expressions 里被定义为 "行" 的最末端 (end-of-line)。这个常用在 grep、sed、awk 以及 vim(vi) 当中。
${} 变量的正规表达式
bash 对 ${} 定义了不少用法。以下是取自线上说明的表列
    ${parameter:-word}    ${parameter:=word}    ${parameter:?word}    ${parameter:+word}    ${parameter:offset}    ${parameter:offset:length}    ${!prefix*}    ${#parameter}    ${parameter#word}    ${parameter##word}    ${parameter%word}    ${parameter%%word}    ${parameter/pattern/string}    ${parameter//pattern/string}
文章出处：http://www.diybl.com/course/3_program/shell/shelljs/2008614/125554_2.html$*引用script 的执行引用变量，引用参数的算法与一般指令相同，指令本身为0，其后为1，然后依此类推。引用变量的代表方式如下：
$0, $1, $2, $3, $4, $5, $6, $7, $8, $9, ${10}, ${11}.....
个位数的，可直接使用数字，但两位数以上，则必须使用 {} 符号来括住。
$* 则是代表所有引用变量的符号。使用时，得视情况加上双引号。
echo "$*"
还有一个与 $* 具有相同作用的符号，但效用与处理方式略为不同的符号。
$@
$@ 与 $* 具有相同作用的符号，不过她们两者有一个不同点。
符号 $* 将所有的引用变量视为一个整体。但符号 $@ 则仍旧保留每个引用变量的区段观念。
$#
这也是与引用变量相关的符号，她的作用是告诉你，引用变量的总数量是多少。
echo "$#"
$? 状态值 (status variable)
一般来说，UNIX(linux) 系统的进程以执行系统调用exit() 来结束的。这个回传值就是status值。回传给父进程，用来检查子进程的执行状态。
一般指令程序倘若执行成功，其回传值为 0；失败为 1。
tar cvfz dfbackup.tar.gz /home/user > /dev/nullecho "$?"$$
由于进程的ID是唯一的，所以在同一个时间，不可能有重复性的 PID。有时，script 会需要产生临时文件，用来存放必要的资料。而此script 亦有可能在同一时间被使用者们使用。在这种情况下，固定文件名在写法上就显的不可靠。唯有产生动态文件名，才能符合需要。符号$$ 或许可以符合这种需求。它代表当前shell 的 PID。
echo "$HOSTNAME, $USER, $MAIL" > ftmp.$$
使用它来作为文件名的一部份，可以避免在同一时间，产生相同文件名的覆盖现象。
ps: 基本上，系统会回收执行完毕的 PID，然后再次依需要分配使用。所以 script 即使临时文件是使用动态档名的写法，如果 script 执行完毕后仍不加以清除，会产生其他问题。
() 指令群组 (command group)
用括号将一串连续指令括起来，这种用法对 shell 来说，称为指令群组。如下面的例子：(cd ~ ; vcgh=`pwd` ; echo $vcgh)，指令群组有一个特性，shell会以产生 subshell 来执行这组指令。因此，在其中所定义的变数，仅作用于指令群组本身。我们来看个例子
# cat ftmp-01#!/bin/basha=fsh(a=incg ; echo -e "/n $a /n")echo $a# ./ftmp-01incgfsh
除了上述的指令群组，括号也用在 array 变数的定义上；另外也应用在其他可能需要加上escape 字元才能使用的场合，如运算式。
(())
这组符号的作用与 let 指令相似，用在算数运算上，是 bash 的内建功能。所以，在执行效率上会比使用 let 指令要好许多。
#!/bin/bash(( a = 10 ))echo -e "inital value, a = $a/n"(( a++ ))echo "after a++, a = $a"
{} 大括号 (Block of code)
有时候 script 当中会出现，大括号中会夹着一段或几段以"分号"做结尾的指令或变数设定。
# cat ftmp-02#!/bin/basha=fsh{a=inbc ; echo -e "/n $a /n"}echo $a# ./ftmp-02inbcinbc
这种用法与上面介绍的指令群组非常相似，但有个不同点，它在当前的 shell 执行，不会产生 subshell。
大括号也被运用在 "函数" 的功能上。广义地说，单纯只使用大括号时，作用就像是个没有指定名称的函数一般。因此，这样写 script 也是相当好的一件事。尤其对输出输入的重导向上，这个做法可精简 script 的复杂度。
此外，大括号还有另一种用法，如下
{xx,yy,zz,...}
这种大括号的组合，常用在字串的组合上，来看个例子
mkdir {userA,userB,userC}-{home,bin,data}
我们得到 userA-home, userA-bin, userA-data, userB-home, userB-bin, userB-data, userC-home, userC-bin, userC-data，这几个目录。这组符号在适用性上相当广泛。能加以善用的话，回报是精简与效率。像下面的例子
chown root /usr/{ucb/{ex,edit},lib/{ex?.?*,how_ex}}
如果不是因为支援这种用法，我们得写几行重复几次呀！
[] 中括号
常出现在流程控制中，扮演括住判断式的作用。if [ "$?" != 0 ]thenecho "Executes error"exit 1fi
这个符号在正则表达式中担任类似 "范围" 或 "集合" 的角色
rm -r 200[1234]
上例，代表删除 2001, 2002, 2003, 2004 等目录的意思。
[[]]
这组符号与先前的 [] 符号，基本上作用相同，但她允许在其中直接使用 || 与 && 逻辑等符号。
#!/bin/bashread akif [[ $ak > 5 || $ak < 9 ]]thenecho $akfi
|| 逻辑符号
这个会时常看到，代表 or 逻辑的符号。
&& 逻辑符号
这个也会常看到，代表 and 逻辑的符号。
& 后台工作
单一个& 符号，且放在完整指令列的最后端，即表示将该指令列放入后台中工作。
tar cvfz data.tar.gz data > /dev/null &
/<.../> 单字边界
文章出处：http://www.diybl.com/course/3_program/shell/shelljs/2008614/125554_3.html这组符号在规则表达式中，被定义为"边界"的意思。譬如，当我们想找寻 the 这个单字时，如果我们用
grep the FileA
你将会发现，像 there 这类的单字，也会被当成是匹配的单字。因为 the 正巧是 there 的一部份。如果我们要必免这种情况，就得加上 "边界" 的符号
grep '/' FileA
+ 加号 (plus)
在运算式中，她用来表示 "加法"。
expr 1 + 2 + 3
此外在规则表达式中，用来表示"很多个"的前面字元的意思。
# grep '10/+9' fileB109100910000910000931010009#这个符号在使用时，前面必须加上 escape 字元。
- 减号 (dash)
在运算式中，她用来表示 "减法"。
expr 10 - 2
此外也是系统指令的选项符号。
ls -expr 10 - 2
在 GNU 指令中，如果单独使用 - 符号，不加任何该加的文件名称时，代表"标准输入"的意思。这是 GNU 指令的共通选项。譬如下例
tar xpvf -
这里的 - 符号，既代表从标准输入读取资料。
不过，在 cd 指令中则比较特别
cd -
这代表变更工作目录到"上一次"工作目录。
% 除法 (Modulo)
在运算式中，用来表示 "除法"。
expr 10 % 2
此外，也被运用在关于变量的规则表达式当中的下列
${parameter%word}${parameter%%word}
一个 % 表示最短的 word 匹配，两个表示最长的 word 匹配。 
= 等号 (Equals)
常在设定变数时看到的符号。
vara=123echo " vara = $vara"
或者像是 PATH 的设定，甚至应用在运算或判断式等此类用途上。
== 等号 (Equals)
常在条件判断式中看到，代表 "等于" 的意思。
if [ $vara == $varb ]
...下略
!= 不等于
常在条件判断式中看到，代表 "不等于" 的意思。
if [ $vara != $varb ]
...下略
^
这个符号在规则表达式中，代表行的 "开头" 位置。 
文章出处：http://www.diybl.com/course/3_program/shell/shelljs/2008614/125554_4.html${ }的用法
假设我们定义了一个变量为：
file=/dir1/dir2/dir3/my.file.txt
我们可以用 ${ } 分別替换获得不同的值：
${file#*/}：拿掉第一条 / 及其左边的字串：dir1/dir2/dir3/my.file.txt
${file##*/}：拿掉最后一条 / 及其左边的字串：my.file.txt
${file#*.}：拿掉第一个 . 及其左边的字串：file.txt
${file##*.}：拿掉最后一个 . 及其左边的字串：txt
${file%/*}：拿掉最后一条 / 及其右边的字串：/dir1/dir2/dir3
${file%%/*}：拿掉第一条 / 及其右边的字串： （空啦）
${file%.*}：拿掉最后一个 . 及其右边的字串：/dir1/dir2/dir3/my.file
${file%%.*}：拿掉第一个 . 及其右边的字串：/dir1/dir2/dir3/my
记忆的方法为：
# 是去掉左边的字符；在键盘上 # 在 $ 之左边;
% 是去掉右边的字符；在键盘上 % 在 $ 之右边;
单一符号是最小匹配﹔两个符号是最大匹配。 
PS：${file#/}（不加*号）表示只去掉最左边的一个“/”
+++其实上面的语法，不只可以处理 . 和 / ，而是可以处理任意字符。勤奋的朋友可以自己编一个string=daabbababac，处理一下试试──binbindatou注+++
${file:0:5}：提取最左边的 5 个字符：/dir1
${file:5:5}：提取第 5 个字符右边的连续 5 个字符：/dir2
我们也可以对变量值里的字串作替换：
${file/dir/path}：将第一個 dir 提换为 path：/path1/dir2/dir3/my.file.txt
${file//dir/path}：将全部 dir 提换为 path：/path1/path2/path3/my.file.txt
+++这个语法跟vim里的替换语法相似，unix工具都是”相通“的──binbindatou注+++
利用 ${ } 还可针对不同的变数状态赋值(未设定、空值、非空值)： 
${file-my.file.txt} ：假如 $file 没有设定，则使用 my.file.txt 作返回值。(空值及非空值时不作处理) 
${file:-my.file.txt} ：假如 $file 没有设定或为空值，则使用 my.file.txt 作返回值。 (非空值时不作处理)
${file+my.file.txt} ：假如 $file 设为空值或非空值，均使用 my.file.txt 作返回值。(没有设定时不作处理)
${file:+my.file.txt} ：若 $file 为非空值，则使用 my.file.txt 作返回值。 (没设定及空值时不作处理)
${file=my.file.txt} ：若 $file 没设定，则使用 my.file.txt 作返回值，同时将 $file 赋值为 my.file.txt 。 (空值及非空值时不作处理)
${file:=my.file.txt} ：若 $file 没设定或为空值，则使用 my.file.txt 作返回值，同时将 $file 赋值为 my.file.txt 。 (非空值时不作处理)
${file?my.file.txt} ：若 $file 没设定，则将 my.file.txt 输出至 STDERR。 (空值及非空值时不作处理)
${file:?my.file.txt} ：若 $file 没设定或为空值，则将 my.file.txt 输出至 STDERR。 (非空值时不作处理)
tips:
以上的理解在于, 你一定要分清楚 unset 与 null 及 non-null 这三种赋值状态.
一般而言, : 与 null 有关, 若不带 : 的话, null 不受影响, 若带 : 则连 null 也受影响.
还有哦，${#var} 可计算出变量值的长度：
${#file} 可得到 27 ，因为 /dir1/dir2/dir3/my.file.txt 刚好是 27 个字符...
接下来，再为大家介绍一下 bash 的数组(array)处理方法。
一般而言，A="a b c def" 这样的变量只是将 $A 替换为一个单一的字符串，
但是改为 A=(a b c def) ，则是将 $A 定义为数组...
bash 的数组替换方法可参考如下方法：
${A[@]} 或 ${A[*]} 可得到 a b c def (全部元素)
${A[0]} 可得到 a (第一个元素)，${A[1]} 则为第二个元素...
${#A[@]} 或 ${#A[*]} 可得到 4 (所包含的元素数)
${#A[0]}可得到 1 (即第一个元素(a)的长度)，${#A[3]} 可得到 3 (第四个元素(def)的长度)
A[3]=xyz 则是将第四个元素重新定义为 xyz ...
诸如此类的.... 
文章出处：DIY部落(http://www.diybl.com/course/3_program/shell/shelljs/20090307/159094.html) 引号的作用 
1 双引号（“”）
1）使用””可引用除字符$(美元符号)、`（反引号）、/（反斜线）外的任意字符或字符串。双引号不会阻止shell对这三个字符做特殊处理（标示变量名、命令替换、反斜线转义）。
  Eg：name=gezn; echo “User name:$name”//将打印User name :gezn
     Echo “The date is:`date +date-%d-%m-%Y`”//将打印The date is: 03-05-2009
     Echo –e “$USER/t$UID”  //将打印gezn  500
2）如果要查新包含空格的字符串经常用到双引号
2单引号（’’）
1)      如果用单引号把字符串括起来，则dayi9nhao内字符串中的任何特殊字符的特殊含义均被屏蔽。
2)      举例：echo –e ‘$USER/t$UID’//将打印$USER $UID(没有屏蔽/t,是因为选项“-e”的缘故)
          echo ‘USER/t$UID’ //将打印$USER/t$UID
3反引号（``）
1)      shell将反引号中的内容作为一个系统命令，并执行其内容。使用这种方法可以替换输出为一个变量
2)      举例：a=`date + date-%d-%m-%Y`  //将打印The date is: 03-05-2009
4.反斜线（/）
1）如果下一个字符有特殊含义，反斜线防止shell误解其含义，即屏蔽其特殊含义。
2）下属字符包含有特殊含义：& * + $ ` “ |  ?
3) 在打印字符串时要加入八进制字符（ASCII相应字符）时，必须在前面加反斜线，否则shell作普通数字处。
举例： bj=Beijing; echo ”variable/$bj=$bj”//将打印variable $bj = beijing
文章出处：DIY部落(http://www.diybl.com/course/3_program/shell/shelljs/20090307/159100.html) 在shell脚本的实际编写中，有一些特殊的变量十分有用：
1. $# 传递到脚本的参数个数
　　2. $* 以一个单字符串显示所有向脚本传递的参数。与位置变量不同，此选项参数可超过9个
　　3. $$ 脚本运行的当前进程ID号
　　4. $! 后台运行的最后一个进程的进程ID号
　　5. $@ 与$#相同，但是使用时加引号，并在引号中返回每个参数
　　6. $- 显示shell使用的当前选项，与set命令功能相同
　　7. $? 显示最后命令的退出状态。0表示没有错误，其他任何值表明有错误。
文章出处：DIY部落(http://www.diybl.com/course/3_program/shell/shelljs/20090319/163415.html) unix shell中的日期格式转换
$ t_t="Jul  1 21:29"
$ date "+%G-%m-%d %H:%M:%S" -d "$t_t" # $t_t的格式是比较随意的
2008-07-01 21:29:00
$ date +%b/%d/%G -d "2008-07-01"
Jul/01/2008 
unix shell中的日期之间间隔的天数
$ expr '(' $(date +%s -d "2008-07-02") - $(date +%s -d "2008-05-30") ')' / 86400
33
##存在bcdate的话，可以直接使用。 
shell中日期加减指定间隔单位
增加36小时：
$ a=`date +%Y-%m-%d`
$ b=`date +%Y-%m-%d -d "$a +36 hours"`
10天前：
$ date -d "$a -10 days"
Sun Jun 22 00:00:00 CST 2008 
以指定格式显示文件更改后最后日期，如yyyy-mm-dd hh24:mi:ss
$ date "+%Y-%m-%d %H:%M:%S" -r test.bak
2008-07-01 21:28:55 
文章出处：http://www.diybl.com/course/3_program/shell/shelljs/200876/130763.html一、用户登陆进入系统后的系统环境变量： 
$HOME 使用者自己的目录 
$PATH 执行命令时所搜寻的目录 
$TZ 时区 
$MAILCHECK 每隔多少秒检查是否有新的信件 
$PS1 在命令列时的提示号 
$PS2 当命令尚未打完时，Shell 要求再输入时的提示号 
$MANPATH man 指令的搜寻路径 
二、特殊变量： 
$0 这个程序的执行名字 
$n 这个程序的第n个参数值，n=1..9 
$* 这个程序的所有参数 
$# 这个程序的参数个数 
$$ 这个程序的PID 
$! 执行上一个指令的PID 
$? 执行上一个指令的返回值 
三、shell中的变元： 
* 任意字符串 
? 一个任意字符 
[abc] a, b, c三者中之一 
[a-n] 从a到n的任一字符 
四、几个特殊字符表示 
/b 退回 
/c 打印一行时没有换行符 这个我们经常会用到 
/f 换页 
/r 回车 
/t 制表 
/v 垂直制表 
// 反斜线本身 
五、判断文件的属性 
格式：-操作符 filename 
-e 文件存在返回1， 否则返回0 
-r 文件可读返回1,否则返回0 
-w 文件可写返回1,否则返回0 
-x 文件可执行返回1,否则返回0 
-o 文件属于用户本人返回1, 否则返回0 
-z 文件长度为0返回1, 否则返回0. 
-f 文件为普通文件返回1, 否则返回0 
-d 文件为目录文件时返回1, 否则返回0 
六、测试字符串 
字符串1 = 字符串2　当两个字串相等时为真 
字符串1 != 字符串2 当两个字串不等时为真 
-n 字符串　 　　　 当字符串的长度大于0时为真 
-z 字符串　　　　　 当字符串的长度为0时为真 
字符串　　　　　　 当串字符串为非空时为真 
七、测试两个整数关系 
数字1 -eq 数字2　　　　 两数相等为真 
数字1 -ne 数字2　　　　 两数不等为真 
数字1 -gt 数字2　　　　 数字1大于数字2为真 
数字1 -ge 数字2 　　　 数字1大于等于数字2为真 
数字1 -lt 数字2　　　　 数字1小于数字2为真 
数字1 -le 数字2　　　　 数字1小于等于数字2为真 
八、逻辑测试 
-a 　 　　　　　 与 
-o　　　　　　　 或 
!　　　　　　　　非 
今天介绍shell特殊字符的引用 
=============================== 
shell中的特殊字符有 
1、$ 美元符 
2、/ 反斜杠 
3、` 反引号 
4、"　双引号 
5、< ,>,*,?,[,] 
下面我一一举列说明 
一、$符号 
1、echo $? 显示的是上一条指令退出状态 
2、echo "$?" 效果同上 
3、echo '$?' 显示的是$? 
4、echo /$? 显示的是$? 
5、echo "/$?" 显示的是$? 
　　大家可能已经看出　$符号在双引号中具有特殊意义　双引号对$符号不起作用 
而单引号可以将特殊字符的的特殊意义屏蔽掉，使其能显示为字符本身，反斜 
杠也可以将特殊字符的特殊含义屏蔽掉，使特殊字符失去特殊含义。 
二、/　反斜杠 
　　反斜杠的作用是将特殊符号字符的特殊含义屏蔽掉，使其还是原字符 
A＝1234 
echo /$A 显示为$A 如果不加/将显示为1234 
echo /` 显示为` 
echo /" 显示为双引号 
echo // 显示为/ 
三、` 反引号 
　　反引号的功能是命令替换，将反引号中的字符串做为命令来执行，我们在用shell编程时经常用的到　将系统命令的执行结果赋给一个变量 
A=`date` 
echo $A 显示的不是date而是当时的时间串 
比如有一文件A的内容如下　 
ABCDEFG 
1234456 
abcdefg 
B=`cat A|grep 234` # 检索文件A中含有字符串234的行 
echo $B 将显示为1234456 
echo "$B" 将显示为什么？ 
echo "/$B" 将显示为什么？读者自己试试 
四、" 双引号 
　　在系统中有些特殊字符，为避免引用这些特殊字符　往往用双引号或单引号将这些特殊字符引起来，使其不具有特殊含义。 
　　但有一部分特殊字符在引号中还是具有特殊含义，用双引号引起来是不起作用的。本文中所列的前四个特殊字符在双引号中还是特殊字符。为了使其不具有特殊含义一是用单引号引进来二是用/反斜线使其失去作用。 
　　比如我们想原样输出这些特殊字符 
echo """ 
echo "$" 
echo "/" 
echo "`" 
　　　以上不是你所期望的结果，因为双引号对它们不起作用，你只能这样才能输出这些特殊字符的原形 
echo '"' 
echo '$' 
echo '/' 
echo '`' 
或 
echo "/"" 
echo "/$" 
echo "//" 
echo "/`" 
将分别显示为　" $ / ` 
五、其它特殊字符 
　　大家注意到　除了前四个特殊字符外　我将其它的特殊字符都放在一块，这是因为前四个特殊字符在双引号中还是具有特殊含义，所以单独拿出来讲，除此以外的特殊字符如果你要输出这些特殊字符的原形，你就可以用双引号或单引号引起来使其失去特殊含义。 
< ,>,*,?,[,]对shell有特殊含义　但你可以用双引号引起来输入这些原形 
　　讲了这么多大家是不是已经注意到所有的特殊字符在单引号中失去特殊含义，如果你要输出特殊字符原形但又记不清那些特殊字符在双引号中不能输出原形，建议你干脆用单引号引起来。
今天介绍条件测试语句 
一、if 条件语句　 
格式： 
if 条件表达式 
then #当条件为真时执行以下语句 
命令列表 
else #为假时执行以下语句 
命令列表 
fi 
if　语句也可以嵌套使用 
if 条件表达式1 
then 
if 条件表达式2 
then 
命令列表 
else 
if 条件表达式3 
then 
命令列表 
else 
命令列表 
fi 
fi 
else 
命令列表 
fi 
你可以进行多层嵌套　一个if语句一定要跟一个fi　表示该层条件结束　　否则会造成语法错误 
结合前面讲的　举例如下： 
这里先讲一个条件语句中用到的命令test　表示测试test后面的条件是否为真 
if test -f "$1" 
then 
lpr $1 
else 
if test -d "$1" 
then 
cd $1 
lpr $1 
else 
echo "$1不是文件或目录" 
fi 
fi 
以上的例子还可以改成如下所示 
if test -f "$1" 
then 
lpr $1 
elif test -d "$1" #elif　同else if 
then 
(cd $1;lpr $1) 
else 
echo "$1不是文件或目录" 
fi 
以上的例子不知您是否看懂是什么意思吗？ 
假如我们现在将这个例子保存为prfile 
chmod +x prfile 
执行刚才的程序 
./prfile aaa 
这个例子是检查你的输入的参数是否是一个文件　如果是就打印　如果是一个目录　先转目录再打印　如果即不是文件也不是目录给出提示 
二、多重条件测试语句case 
格式： 
case 字串　in 
模式) 命令列表;; 
模式) 命令列表;; 
.... 
esac 
多重条件语句是以case 开始以esac结束　中间可以有多个条件列表　功能是测试字串和和里面的模式有没有匹配的,有就执行里面的命令列表　模式也可以是*号　表示任意字串，每个模式里面的最后要心;;双引号结束，否则会发生语法错误。 
现举例如下： 
case $1 in 
*.c) 
cc $1 
;; 
*.txt) 
lpr $1 
;; 
*) 
echo "未知的类型" 
esac 
假如将以上内容保存在文件abc中 
chmod +x abc 
执行　./abc a.c 　 将会对文件a.c进行编译 
执行 ./abc readme.txt　将会把文件通过打印机 
假如我将以上内容改一下，你是否会知道它的执行结果？ 
case $1 in 
*) 
cc $1 
;; 
*.txt) 
lpr $1 
;; 
*.c) 
echo "未知的类型" 
esac 
今天介绍循环语句 
一. while 循环 
while 命令格式 
while 条件表 
do 
命令表 
done 
执行过程 
shell首先执行条件表,如果条件表的最后一条语句的退出状态为零,则执行盾环体内的命令 
表,执行完后,再检查条件表,如果退出状态为零将继续执行,如此循环往复直到条件表的 
最后一条语句的退出状态非零. 退出状态为零就是条件为真True. 
举例说明 假如shell文件的内容如下: 
Sum=0 
i=0 
while true #true是系统的关键词 表示真 
do 
i=`expr $i + 1` 
Sum=`expr $Sum + $i` 
if [ $i = "100" ] 
then 
break; 
fi 
done 
echo $i $Sum 
最后这个程序显示的是 100 5050 
这个程序的运算就是将1到100加起来 
下面将这个程序再改动一下 
Sum=0 
i=0 
while [ $i != "100" ] 
do 
i=`expr $i + 1` 
Sum=`expr $Sum + $i` 
done 
echo $i $Sum 
改动后的程序运算结果和上面是一样 但程序比上面的要简练 
在这个循环中还可以以until做为测试条件 它正好与while测试的条件相反,也就是当条件为假时将继续执行循环体内的语句,否则就退出循环体,下面还用这个例子. 
Sum=0 
i=0 
until [ $i = "100" ] 
do 
i=`expr $i + 1` 
Sum=`expr $Sum + $i` 
done 
echo $i $Sum 
当i不等于100时循环 就是当条件为假时循环,否则就退出,而第一个例子是当i不等于100 
时循环,也就是测试条件为真时循环. 
二.for 循环 
命令格式: 
for 变量 in 名字列表 
do 
命令列表 
done 
这里的名字列表是一个由空格分隔的字符串列表,shell在执行for循环时每次依次从名字表 
中取出一个字符串赋给循环变量作为变量的值. 
在写for语句时,也可以省略in 名字列表部分,这表示用当前的位置参数来代替这时的名 
字列表. 
下面举个例子 
比如在你的电脑中有两个目录,一个是aa,一个是bb在这两个目录中有5个相同的文件,但其 
中一个目录中的一个或多个文件刚刚修改过,现在我忘记刚才改的是那几个文件 了,那么我靠梢员冉弦幌抡饬礁瞿柯嫉奈募椭懒?程序如下: 
for File in a1 a2 a3 a4 a5 
do 
diff aa/$File bb/$File 
done 
下面再举一个不带名字列表的例子 
for File 
do 
echo $Filw 
done 
文件内容保存在a.sh中 并可执行 
我们在执行这个shell程序时命令行如下: 
a.sh a1 a2 a3 a4 a5 
执行结果如下: 
a1 
a2 
a3 
a4 
a5 
大家从这个例子中可以看到命令行的参数被逐一读入一次 
三.循环控制语句 
break 命令不执行当前循环体内break下面的语句从当前循环退出. 
continue 命令是程序在本循体内忽略下面的语句,从循环头开始执行. 
一,命令组合:圆括号和花括号 
shell中有两种方法将命令组合在一起:圆括号和花括号.圆括号使shell创建一个子shell 
来读取并执行括起来的名命令.左括号和右括号不论出现在命令行中的什么位置,shell都会 
认为它们具有特殊的组合意义的.只有用双引号将它们括起来引用,才表示圆括号或花括号 
的原义.例如: 
echo a(b) 
将出现语法上的错误,要想输出a(b)字符串 只能括起来 
echo "a(b)" 
或echo a"("b")" 
这样才能被shell正确解释. 
利用组合命令有什么作用呢? 
一,用圆括号组合命令 
圆括号的组合命令可以创建子进程运行组合程序,建立子进程的功能是很有用的,因为 
子shell在组合命令中的种种操作都不会影响到当前shell的各变量的值. 
例如: 
子进程在执行组合命令时改变了工作目录,并在新的工作目录下执行一系例命令,执行 
完后它可以不必返回原工作目录,因为子进程工作目录的改变不会影响到当前工作目录. 
创建子进程后将当前的环境也同样传给子shell,当前shell中用export输出到环境中的 
各变量在子shell中同样有效. 
花括号也可以将命令组合在一起.左 右花括号只有作为一条命令的第一个字出现时, 
shell才它们含有特殊含义. 
与圆括号不同的是花括号并不创建子shell,只是由当前的shell来读取并执行括起来的 
命令.有时用户希望使用一组命令的顺序输出作为另一组命令的输入,此时用花括号是很方 
便的. 
不论是用圆括号不是花括号,退出状态都是等于最后一条括起来的命令的退出状态. 
二,可以在当前shell中执行的命令 
用户在使用shell时一定要了解那些是可以在当前shell中执行的命令 那些不可以 
可以在当前shell中执行的命令有: 
break case cd continue 
echo eval exec exit 
export for if read 
readonly return set shift 
test times trap umask 
until wait while 
: {} 
文章出处：http://www.diybl.com/course/3_program/shell/shellxl/2008911/142059.html
