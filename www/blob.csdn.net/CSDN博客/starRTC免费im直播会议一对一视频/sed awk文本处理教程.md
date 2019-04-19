# sed awk文本处理教程 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年12月11日 09:26:01[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：123
sed全名叫stream editor，流编辑器，用程序的方式来编辑文本，相当的hacker啊。sed基本上就是玩正则模式匹配，所以，玩sed的人，正则表达式一般都比较强。

把my字符串替换成Hao Chen’s
$
sed"s/my/Hao Chen's/g"pets.txt
s表示替换命令，/my/表示匹配my，/Hao
 Chen’s/表示把匹配替换成Hao Chen’s，/g 表示一行上的替换所有的匹配
注意：上面的sed并没有对文件的内容改变，只是把处理过后的内容输出，如果你要写回文件，你可以使用重定向，如：
$
sed"s/my/Hao Chen's/g"pets.txt > hao_pets.txt
或使用 -i 参数直接修改文件内容：
$
sed-i "s/my/Hao Chen's/g"pets.txt
在每一行最前面加点东西：
$
sed's/^/#/g'pets.txt
#This is my cat
#  my cat's name is betty
#This is my dog
#  my dog's name is frank
#This is my fish
#  my fish's name is george
#This is my goat
#  my goat's name is adam
可以用于批量注释

在每一行最后面加点东西：
$
sed's/$/ --- /g'pets.txt
This is my
cat---
my
cat's name is betty ---
This is my dog ---
my dog's name is frank ---
This is my fish ---
my fish's name is george ---
This is my goat ---
my goat's name is adam ---
- ^ 表示一行的开头。如：/^#/ 以#开头的匹配。
- $ 表示一行的结尾。如：/}$/ 以}结尾的匹配。
- \< 表示词首。 如 \<abc 表示以 abc 为首的詞。
- \> 表示词尾。 如 abc\> 表示以 abc 結尾的詞。
- . 表示任何单个字符。
- * 表示某个字符出现了0次或多次。
- [ ] 字符集合。 如：[abc]表示匹配a或b或c，还有[a-zA-Z]表示匹配所有的26个字符。如果其中有^表示反，如[^a]表示非a的字符

参看[sed的手册](http://www.gnu.org/software/sed/manual/sed.html)
==============awk===================
之所以叫AWK是因为其取了三位创始人的首字符。要学AWK，就得提一提AWK的一本相当经典的书《[The
 AWK Programming Language](http://plan9.bell-labs.com/cm/cs/awkbook/)》，它在[豆瓣上的评分](http://book.douban.com/subject/1876898/)是9.4分！在[亚马逊上居然卖1022.30元](http://www.amazon.cn/mn/detailApp/?asin=020107981X)。
示例，输出第1列和第4列
awk'{print $1, $4}'netstat.txt
单引号中的被大括号括着的就是awk的语句，注意，其只能被单引号包含。
- 其中的$1..$n表示第几列。注：$0表示整个行。
awk的格式化输出
awk'{printf "%-8s %-8s %-8s %-18s %-22s %-15s\n",$1,$2,$3,$4,$5,$6}'netstat.txt
和C语言的printf没什么两样
**过滤记录**
**1,**过滤条件：第三列的值为0 && 第6列的值为LISTEN
awk'$3==0 && $6=="LISTEN" 'netstat.txt
awk' $3>0 {print $0}'netstat.txt
awk'$3==0 && $6=="LISTEN" || NR==1 {printf "%-20s %-20s %s\n",$4,$5,$6}'netstat.txt
**内建变量**
NF 多少列
NR 行号 有多个文件话，这个值不断累加
FNR 各个文件自己的行号
如：我们如果要输出行号：
$ awk '$3==0 && $6=="ESTABLISHED" || NR==1 {printf "%02s %s %-20s %-20s %s\n",NR, FNR, $4,$5,$6}' netstat.txt
**指定分隔符**
awk-F: '{print $1,$3,$6}'/etc/passwd （-F的意思就是指定分隔符）
以\t作为分隔符输出的例子（下面使用了/etc/passwd文件，这个文件是以:分隔的）：
awk-F: '{print $1,$3,$6}'OFS="\t"/etc/passwd
**字符串匹配**
匹配FIN状态
awk'$6 ~ /FIN/ || NR==1 {print NR,$4,$5,$6}'OFS="\t"netstat.txt
匹配WAIT字样的状态
awk'$6 ~ /WAIT/ || NR==1 {print NR,$4,$5,$6}'OFS="\t"netstat.txt
 ~ 表示模式开始。/ /中间的内容是模式。这就是一个正则表达式的匹配。

awk可以像grep一样的去匹配第一行，就像这样：
awk'/LISTEN/'netstat.txt
模式取反
awk'!/WAIT/'netstat.txt
**折分文件**
按第6例分隔文件，相当的简单（其中的NR!=1表示不处理表头）。
awk'NR!=1{print > $6}'netstat.txt
ls
ESTABLISHED  FIN_WAIT1  FIN_WAIT2  LAST_ACK  LISTEN 
netstat.txt  TIME_WAIT
也可以把指定的列输出到文件：
awk'NR!=1{print $4,$5 > $6}'netstat.txt

下面的命令计算所有的C文件，CPP文件和H文件的文件大小总和。
ls-l  *.cpp *.c *.h | awk'{sum+=$5} END {print sum}'
统计各个connection状态的用法
awk'NR!=1{a[$6]++;} END {for (i in a) print i ", " a[i];}'netstat.txt
TIME_WAIT, 3
FIN_WAIT1, 1
ESTABLISHED, 6
FIN_WAIT2, 3
LAST_ACK, 1
LISTEN, 4

统计每个用户的进程的占了多少内存（注：sum的RSS那一列）
psaux |
awk'NR!=1{a[$1]+=$6;} END { for(i in a) print i ", " a[i]"KB";}'
dbus, 540KB
mysql, 99928KB
www, 3264924KB
root, 63644KB
hchen, 6020KB

#从file文件中找出长度大于80的行
awk'length>80'file
#按连接数查看客户端IP
netstat-ntu | awk'{print $5}'| cut-d: -f1 | sort| uniq-c | sort-nr
#打印99乘法表
seq9 | sed'H;g'| awk-vRS='''{for(i=1;i<=NF;i++)printf("%dx%d=%d%s", i, NR, i*NR, i==NR?"\n":"\t")}'
参看[gawk的手册](http://www.gnu.org/software/gawk/manual/gawk.html)：
- 内建变量，参看：[http://www.gnu.org/software/gawk/manual/gawk.html#Built_002din-Variables](http://www.gnu.org/software/gawk/manual/gawk.html#Built_002din-Variables)
- 流控方面，参看：[http://www.gnu.org/software/gawk/manual/gawk.html#Statements](http://www.gnu.org/software/gawk/manual/gawk.html#Statements)
- 内建函数，参看：[http://www.gnu.org/software/gawk/manual/gawk.html#Built_002din](http://www.gnu.org/software/gawk/manual/gawk.html#Built_002din)
- 正则表达式，参看：[http://www.gnu.org/software/gawk/manual/gawk.html#Regexp](http://www.gnu.org/software/gawk/manual/gawk.html#Regexp)
- 
[参考：](http://www.gnu.org/software/sed/manual/sed.html)
[http://coolshell.cn/articles/9070.html](http://coolshell.cn/articles/9070.html)
[http://coolshell.cn/articles/9104.html](http://coolshell.cn/articles/9104.html)
