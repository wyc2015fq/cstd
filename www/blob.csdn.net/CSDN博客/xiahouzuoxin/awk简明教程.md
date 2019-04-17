# awk简明教程 - xiahouzuoxin - CSDN博客





2013年11月26日 09:42:48[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：1234







有一些网友看了前两天的《[Linux下应该知道的技巧](http://coolshell.cn/articles/8883.html)》希望我能教教他们用awk和sed，所以，出现了这篇文章。我估计这些80后的年轻朋友可能对awk/sed这类上古神器有点陌生了，所以需要我这个老家伙来炒炒冷饭。况且，AWK是贝尔实验室1977年搞出来的文本出现神器，今年是蛇年，是AWK的本命年，而且年纪和我相仿，所以非常有必要为他写篇文章。

之所以叫AWK是因为其取了三位创始人 [Alfred Aho](http://en.wikipedia.org/wiki/Alfred_Aho)，[Peter
 Weinberger](http://en.wikipedia.org/wiki/Peter_J._Weinberger), 和 [Brian Kernighan](http://en.wikipedia.org/wiki/Brian_Kernighan) 的Family Name的首字符。要学AWK，就得提一提AWK的一本相当经典的书《[The
 AWK Programming Language](http://plan9.bell-labs.com/cm/cs/awkbook/)》，它在[豆瓣上的评分](http://book.douban.com/subject/1876898/)是9.4分！在[亚马逊上居然卖1022.30元](http://www.amazon.cn/mn/detailApp/?asin=020107981X)。


我在这里的教程并不想面面俱到，本文和我之前的[Go语言简介](http://coolshell.cn/articles/8460.html)一样，全是示例，基本无废话。


我只想达到两个目的：


1）你可以在乘坐公交地铁上下班，或是在坐马桶拉大便时读完（保证是一泡大便的工夫）。


2）我只想让这篇博文像一个火辣的脱衣舞女挑起你的兴趣，然后还要你自己去下工夫去撸。


废话少说，我们开始脱吧（注：这里只是topless）。

#### 起步上台


我从netstat命令中提取了如下信息作为用例：





|1234567891011121314151617181920|$catnetstat.txtProto Recv-Q Send-Q Local-Address          Foreign-Address             Statetcp        0      0 0.0.0.0:3306           0.0.0.0:*                   LISTENtcp        0      0 0.0.0.0:80             0.0.0.0:*                   LISTENtcp        0      0 127.0.0.1:9000         0.0.0.0:*                   LISTENtcp        0      0 coolshell.cn:80        124.205.5.146:18245         TIME_WAITtcp        0      0 coolshell.cn:80        61.140.101.185:37538        FIN_WAIT2tcp        0      0 coolshell.cn:80        110.194.134.189:1032        ESTABLISHEDtcp        0      0 coolshell.cn:80        123.169.124.111:49809       ESTABLISHEDtcp        0      0 coolshell.cn:80        116.234.127.77:11502        FIN_WAIT2tcp        0      0 coolshell.cn:80        123.169.124.111:49829       ESTABLISHEDtcp        0      0 coolshell.cn:80        183.60.215.36:36970         TIME_WAITtcp        0   4166 coolshell.cn:80        61.148.242.38:30901         ESTABLISHEDtcp        0      1 coolshell.cn:80        124.152.181.209:26825       FIN_WAIT1tcp        0      0 coolshell.cn:80        110.194.134.189:4796        ESTABLISHEDtcp        0      0 coolshell.cn:80        183.60.212.163:51082        TIME_WAITtcp        0      1 coolshell.cn:80        208.115.113.92:50601        LAST_ACKtcp        0      0 coolshell.cn:80        123.169.124.111:49840       ESTABLISHEDtcp        0      0 coolshell.cn:80        117.136.20.85:50025         FIN_WAIT2tcp        0      0 :::22                  :::*                        LISTEN|
|----|----|




下面是最简单最常用的awk示例，其输出第1列和第4例，
- 
其中单引号中的被大括号括着的就是awk的语句，注意，其只能被单引号包含。
- 
其中的$1..$n表示第几例。注：$0表示整个行。


|1234567891011121314151617181920|$awk'{print $1, $4}'netstat.txtProto Local-Addresstcp 0.0.0.0:3306tcp 0.0.0.0:80tcp 127.0.0.1:9000tcp coolshell.cn:80tcp coolshell.cn:80tcp coolshell.cn:80tcp coolshell.cn:80tcp coolshell.cn:80tcp coolshell.cn:80tcp coolshell.cn:80tcp coolshell.cn:80tcp coolshell.cn:80tcp coolshell.cn:80tcp coolshell.cn:80tcp coolshell.cn:80tcp coolshell.cn:80tcp coolshell.cn:80tcp :::22|
|----|----|




我们再来看看awk的格式化输出，和C语言的printf没什么两样：


|1234567891011121314151617181920|$awk'{printf "%-8s %-8s %-8s %-18s %-22s %-15s\n",$1,$2,$3,$4,$5,$6}'netstat.txtProto    Recv-Q   Send-Q   Local-Address      Foreign-Address        Statetcp      0        0        0.0.0.0:3306       0.0.0.0:*              LISTENtcp      0        0        0.0.0.0:80         0.0.0.0:*              LISTENtcp      0        0        127.0.0.1:9000     0.0.0.0:*              LISTENtcp      0        0        coolshell.cn:80    124.205.5.146:18245    TIME_WAITtcp      0        0        coolshell.cn:80    61.140.101.185:37538   FIN_WAIT2tcp      0        0        coolshell.cn:80    110.194.134.189:1032   ESTABLISHEDtcp      0        0        coolshell.cn:80    123.169.124.111:49809  ESTABLISHEDtcp      0        0        coolshell.cn:80    116.234.127.77:11502   FIN_WAIT2tcp      0        0        coolshell.cn:80    123.169.124.111:49829  ESTABLISHEDtcp      0        0        coolshell.cn:80    183.60.215.36:36970    TIME_WAITtcp      0        4166     coolshell.cn:80    61.148.242.38:30901    ESTABLISHEDtcp      0        1        coolshell.cn:80    124.152.181.209:26825  FIN_WAIT1tcp      0        0        coolshell.cn:80    110.194.134.189:4796   ESTABLISHEDtcp      0        0        coolshell.cn:80    183.60.212.163:51082   TIME_WAITtcp      0        1        coolshell.cn:80    208.115.113.92:50601   LAST_ACKtcp      0        0        coolshell.cn:80    123.169.124.111:49840  ESTABLISHEDtcp      0        0        coolshell.cn:80    117.136.20.85:50025    FIN_WAIT2tcp      0        0        :::22              :::*                   LISTEN|
|----|----|



#### 脱掉外套
过滤记录

我们再来看看如何过滤记录（下面过滤条件为：第三列的值为0 && 第6列的值为LISTEN）


|12345|$awk'$3==0 && $6=="LISTEN" 'netstat.txttcp        0      0 0.0.0.0:3306               0.0.0.0:*              LISTENtcp        0      0 0.0.0.0:80                 0.0.0.0:*              LISTENtcp        0      0 127.0.0.1:9000             0.0.0.0:*              LISTENtcp        0      0 :::22                      :::*                   LISTEN|
|----|----|




其中的“==”为比较运算符。其他比较运算符：!=, >, <, >=, <=


我们来看看各种过滤记录的方式：


|12345|$awk' $3>0 {print $0}'netstat.txtProto Recv-Q Send-Q Local-Address          Foreign-Address             Statetcp        0   4166 coolshell.cn:80        61.148.242.38:30901         ESTABLISHEDtcp        0      1 coolshell.cn:80        124.152.181.209:26825       FIN_WAIT1tcp        0      1 coolshell.cn:80        208.115.113.92:50601        LAST_ACK|
|----|----|




如果我们需要表头的话，我们可以引入内建变量NR：


|123456|$awk'$3==0 && $6=="LISTEN" || NR==1 'netstat.txtProto Recv-Q Send-Q Local-Address          Foreign-Address             Statetcp        0      0 0.0.0.0:3306           0.0.0.0:*                   LISTENtcp        0      0 0.0.0.0:80             0.0.0.0:*                   LISTENtcp        0      0 127.0.0.1:9000         0.0.0.0:*                   LISTENtcp        0      0 :::22                  :::*                        LISTEN|
|----|----|




再加上格式化输出：


|123456|$awk'$3==0 && $6=="LISTEN" || NR==1 {printf "%-20s %-20s %s\n",$4,$5,$6}'netstat.txtLocal-Address        Foreign-Address      State0.0.0.0:3306         0.0.0.0:*            LISTEN0.0.0.0:80           0.0.0.0:*            LISTEN127.0.0.1:9000       0.0.0.0:*            LISTEN:::22                :::*                 LISTEN|
|----|----|


内建变量

说到了内建变量，我们可以来看看awk的一些内建变量：
|$0|当前记录（这个变量中存放着整个行的内容）|
|----|----|
|$1~$n|当前记录的第n个字段，字段间由FS分隔|
|FS|输入字段分隔符 默认是空格或Tab|
|NF|当前记录中的字段个数，就是有多少列|
|NR|已经读出的记录数，就是行号，从1开始，如果有多个文件话，这个值也是不断累加中。|
|FNR|当前记录数，与NR不同的是，这个值会是各个文件自己的行号|
|RS|输入的记录分隔符， 默认为换行符|
|OFS|输出字段分隔符， 默认也是空格|
|ORS|输出的记录分隔符，默认为换行符|
|FILENAME|当前输入文件的名字|


怎么使用呢，比如：我们如果要输出行号：


|1234567|$awk'$3==0 && $6=="ESTABLISHED" || NR==1 {printf "%02s %s %-20s %-20s %s\n",NR, FNR, $4,$5,$6}'netstat.txt01 1 Local-Address        Foreign-Address      State07 7 coolshell.cn:80      110.194.134.189:1032 ESTABLISHED08 8 coolshell.cn:80      123.169.124.111:49809 ESTABLISHED10 10 coolshell.cn:80      123.169.124.111:49829 ESTABLISHED14 14 coolshell.cn:80      110.194.134.189:4796 ESTABLISHED17 17 coolshell.cn:80      123.169.124.111:49840 ESTABLISHED|
|----|----|


指定分隔符

|123456789|$ awk 'BEGIN{FS=":"} {print $1,$3,$6}'/etc/passwdroot 0/rootbin 1/bindaemon 2/sbinadm 3/var/admlp 4/var/spool/lpdsync5/sbinshutdown6/sbinhalt 7/sbin|
|----|----|




上面的命令也等价于：（-F的意思就是指定分隔符）


|1|$awk -F:'{print $1,$3,$6}'/etc/passwd|
|----|----|




注：如果你要指定多个分隔符，你可以这样来：


|1|awk-F'[;:]'|
|----|----|




再来看一个以\t作为分隔符输出的例子（下面使用了/etc/passwd文件，这个文件是以:分隔的）：


|1234567|$awk -F:'{print $1,$3,$6}'OFS="\t"/etc/passwdroot    0      /rootbin     1      /bindaemon  2      /sbinadm     3      /var/admlp      4      /var/spool/lpdsync   5      /sbin|
|----|----|



#### 脱掉衬衫
字符串匹配

我们再来看几个字符串匹配的示例：


|12345678910111213141516|$awk'$6 ~ /FIN/ || NR==1 {print NR,$4,$5,$6}'OFS="\t"netstat.txt1       Local-Address   Foreign-Address State6       coolshell.cn:80 61.140.101.185:37538    FIN_WAIT29       coolshell.cn:80 116.234.127.77:11502    FIN_WAIT213      coolshell.cn:80 124.152.181.209:26825   FIN_WAIT118      coolshell.cn:80 117.136.20.85:50025     FIN_WAIT2$ $awk'$6 ~ /WAIT/ || NR==1 {print NR,$4,$5,$6}'OFS="\t"netstat.txt1       Local-Address   Foreign-Address State5       coolshell.cn:80 124.205.5.146:18245     TIME_WAIT6       coolshell.cn:80 61.140.101.185:37538    FIN_WAIT29       coolshell.cn:80 116.234.127.77:11502    FIN_WAIT211      coolshell.cn:80 183.60.215.36:36970     TIME_WAIT13      coolshell.cn:80 124.152.181.209:26825   FIN_WAIT115      coolshell.cn:80 183.60.212.163:51082    TIME_WAIT18      coolshell.cn:80 117.136.20.85:50025     FIN_WAIT2|
|----|----|




上面的第一个示例匹配FIN状态， 第二个示例匹配WAIT字样的状态。其实 ~ 表示模式开始。/ /中是模式。这就是一个正则表达式的匹配。


其实awk可以像grep一样的去匹配第一行，就像这样：


|12345|$awk'/LISTEN/'netstat.txttcp        0      0 0.0.0.0:3306            0.0.0.0:*               LISTENtcp        0      0 0.0.0.0:80              0.0.0.0:*               LISTENtcp        0      0 127.0.0.1:9000          0.0.0.0:*               LISTENtcp        0      0 :::22                   :::*                    LISTEN|
|----|----|




我们可以使用 “/FIN|TIME/” 来匹配 FIN 或者 TIME :


|123456789|$awk'$6 ~ /FIN|TIME/ || NR==1 {print NR,$4,$5,$6}'OFS="\t"netstat.txt1       Local-Address   Foreign-Address State5       coolshell.cn:80 124.205.5.146:18245     TIME_WAIT6       coolshell.cn:80 61.140.101.185:37538    FIN_WAIT29       coolshell.cn:80 116.234.127.77:11502    FIN_WAIT211      coolshell.cn:80 183.60.215.36:36970     TIME_WAIT13      coolshell.cn:80 124.152.181.209:26825   FIN_WAIT115      coolshell.cn:80 183.60.212.163:51082    TIME_WAIT18      coolshell.cn:80 117.136.20.85:50025     FIN_WAIT2|
|----|----|




再来看看模式取反的例子：


|12345678910111213|$awk'$6 !~ /WAIT/ || NR==1 {print NR,$4,$5,$6}'OFS="\t"netstat.txt1       Local-Address   Foreign-Address State2       0.0.0.0:3306    0.0.0.0:*       LISTEN3       0.0.0.0:80      0.0.0.0:*       LISTEN4       127.0.0.1:9000  0.0.0.0:*       LISTEN7       coolshell.cn:80 110.194.134.189:1032    ESTABLISHED8       coolshell.cn:80 123.169.124.111:49809   ESTABLISHED10      coolshell.cn:80 123.169.124.111:49829   ESTABLISHED12      coolshell.cn:80 61.148.242.38:30901     ESTABLISHED14      coolshell.cn:80 110.194.134.189:4796    ESTABLISHED16      coolshell.cn:80 208.115.113.92:50601    LAST_ACK17      coolshell.cn:80 123.169.124.111:49840   ESTABLISHED19      :::22   :::*    LISTEN|
|----|----|




或是：


|1|awk'!/WAIT/'netstat.txt|
|----|----|




折分文件


awk拆分文件很简单，使用重定向就好了。下面这个例子，是按第6例分隔文件，相当的简单（其中的NR!=1表示不处理表头）。


|12345678910111213141516171819202122232425262728293031323334|$awk'NR!=1{print > $6}'netstat.txt$lsESTABLISHED  FIN_WAIT1  FIN_WAIT2  LAST_ACK  LISTEN netstat.txt  TIME_WAIT$catESTABLISHEDtcp        0      0 coolshell.cn:80        110.194.134.189:1032        ESTABLISHEDtcp        0      0 coolshell.cn:80        123.169.124.111:49809       ESTABLISHEDtcp        0      0 coolshell.cn:80        123.169.124.111:49829       ESTABLISHEDtcp        0   4166 coolshell.cn:80        61.148.242.38:30901         ESTABLISHEDtcp        0      0 coolshell.cn:80        110.194.134.189:4796        ESTABLISHEDtcp        0      0 coolshell.cn:80        123.169.124.111:49840       ESTABLISHED$catFIN_WAIT1tcp        0      1 coolshell.cn:80        124.152.181.209:26825       FIN_WAIT1$catFIN_WAIT2tcp        0      0 coolshell.cn:80        61.140.101.185:37538        FIN_WAIT2tcp        0      0 coolshell.cn:80        116.234.127.77:11502        FIN_WAIT2tcp        0      0 coolshell.cn:80        117.136.20.85:50025         FIN_WAIT2$catLAST_ACKtcp        0      1 coolshell.cn:80        208.115.113.92:50601        LAST_ACK$catLISTENtcp        0      0 0.0.0.0:3306           0.0.0.0:*                   LISTENtcp        0      0 0.0.0.0:80             0.0.0.0:*                   LISTENtcp        0      0 127.0.0.1:9000         0.0.0.0:*                   LISTENtcp        0      0 :::22                  :::*                        LISTEN$catTIME_WAITtcp        0      0 coolshell.cn:80        124.205.5.146:18245         TIME_WAITtcp        0      0 coolshell.cn:80        183.60.215.36:36970         TIME_WAITtcp        0      0 coolshell.cn:80        183.60.212.163:51082        TIME_WAIT|
|----|----|




你也可以把指定的列输出到文件：


|1|awk'NR!=1{print $4,$5 > $6}'netstat.txt|
|----|----|




再复杂一点：（注意其中的if-else-if语句，可见awk其实是个脚本解释器）


|123456789101112131415161718192021222324252627282930|$awk'NR!=1{if($6 ~/TIME|ESTABLISHED/) print >"1.txt";elseif($6 ~/LISTEN/) print >"2.txt";elseprint >"3.txt"}'netstat.txt$ls?.txt1.txt  2.txt  3.txt$cat1.txttcp        0      0 coolshell.cn:80        124.205.5.146:18245         TIME_WAITtcp        0      0 coolshell.cn:80        110.194.134.189:1032        ESTABLISHEDtcp        0      0 coolshell.cn:80        123.169.124.111:49809       ESTABLISHEDtcp        0      0 coolshell.cn:80        123.169.124.111:49829       ESTABLISHEDtcp        0      0 coolshell.cn:80        183.60.215.36:36970         TIME_WAITtcp        0   4166 coolshell.cn:80        61.148.242.38:30901         ESTABLISHEDtcp        0      0 coolshell.cn:80        110.194.134.189:4796        ESTABLISHEDtcp        0      0 coolshell.cn:80        183.60.212.163:51082        TIME_WAITtcp        0      0 coolshell.cn:80        123.169.124.111:49840       ESTABLISHED$cat2.txttcp        0      0 0.0.0.0:3306           0.0.0.0:*                   LISTENtcp        0      0 0.0.0.0:80             0.0.0.0:*                   LISTENtcp        0      0 127.0.0.1:9000         0.0.0.0:*                   LISTENtcp        0      0 :::22                  :::*                        LISTEN$cat3.txttcp        0      0 coolshell.cn:80        61.140.101.185:37538        FIN_WAIT2tcp        0      0 coolshell.cn:80        116.234.127.77:11502        FIN_WAIT2tcp        0      1 coolshell.cn:80        124.152.181.209:26825       FIN_WAIT1tcp        0      1 coolshell.cn:80        208.115.113.92:50601        LAST_ACKtcp        0      0 coolshell.cn:80        117.136.20.85:50025         FIN_WAIT2|
|----|----|


统计

下面的命令计算所有的C文件，CPP文件和H文件的文件大小总和。


|12|$ls-l  *.cpp *.c *.h |awk'{sum+=$5} END {print sum}'2511401|
|----|----|




我们再来看一个统计各个connection状态的用法：（我们可以看到一些编程的影子了，大家都是程序员我就不解释了。注意其中的数组的用法）


|1234567|$awk'NR!=1{a[$6]++;} END {for (i in a) print i ", " a[i];}'netstat.txtTIME_WAIT, 3FIN_WAIT1, 1ESTABLISHED, 6FIN_WAIT2, 3LAST_ACK, 1LISTEN, 4|
|----|----|




再来看看统计每个用户的进程的占了多少内存（注：sum的RSS那一列）


|123456|$psaux |awk'NR!=1{a[$1]+=$6;} END { for(i in a) print i ", " a[i]"KB";}'dbus, 540KBmysql, 99928KBwww, 3264924KBroot, 63644KBhchen, 6020KB|
|----|----|



#### 脱掉内衣
awk脚本

在上面我们可以看到一个END关键字。END的意思是“处理完所有的行的标识”，即然说到了END就有必要介绍一下BEGIN，这两个关键字意味着执行前和执行后的意思，语法如下：
- 
BEGIN{ 这里面放的是执行前的语句 }
- 
END {这里面放的是处理完所有的行后要执行的语句 }
- 
{这里面放的是处理每一行时要执行的语句}


为了说清楚这个事，我们来看看下面的示例：


假设有这么一个文件（学生成绩表）：


|123456|$catscore.txtMarry   2143 78 84 77Jack    2321 66 78 45Tom     2122 48 77 71Mike    2537 87 97 95Bob     2415 40 57 62|
|----|----|




我们的awk脚本如下（我没有写有命令行上是因为命令行上不易读，另外也在介绍另一种用法）：


|123456789101112131415161718192021222324|$catcal.awk#!/bin/awk -f#运行前BEGIN {    math = 0    english = 0    computer = 0    printf"NAME    NO.   MATH  ENGLISH  COMPUTER   TOTAL\n"    printf"---------------------------------------------\n"}#运行中{    math+=$3    english+=$4    computer+=$5    printf"%-6s %-6s %4d %8d %8d %8d\n", $1, $2, $3,$4,$5, $3+$4+$5}#运行后END {    printf"---------------------------------------------\n"    printf"  TOTAL:%10d %8d %8d \n", math, english, computer    printf"AVERAGE:%10.2f %8.2f %8.2f\n", math/NR, english/NR, computer/NR}|
|----|----|




我们来看一下执行结果：（也可以这样运行 ./cal.awk score.txt）


|1234567891011|$awk-fcal.awkscore.txtNAME    NO.   MATH  ENGLISH  COMPUTER   TOTAL---------------------------------------------Marry  2143     78       84       77      239Jack   2321     66       78       45      189Tom    2122     48       77       71      196Mike   2537     87       97       95      279Bob    2415     40       57       62      159---------------------------------------------  TOTAL:       319      393      350AVERAGE:     63.80    78.60    70.00|
|----|----|


环境变量

即然说到了脚本，我们来看看怎么和环境变量交互：（使用-v参数和ENVIRON，使用ENVIRON的环境变量需要export）


|1234567891011121314|$ x=5$ y=10$exporty$echo$x $y5 10$awk-vval=$x'{print $1, $2, $3, $4+val, $5+ENVIRON["y"]}'OFS="\t"score.txtMarry   2143    78      89      87Jack    2321    66      83      55Tom     2122    48      82      81Mike    2537    87      102     105Bob     2415    40      62      72|
|----|----|



#### 几个花活


最后，我们再来看几个小例子：


|12345678|#从file文件中找出长度大于80的行awk'length>80'file#按连接数查看客户端IPnetstat-ntu |awk'{print $5}'|cut-d: -f1 |sort|uniq-c |sort-nr#打印99乘法表seq9 |sed'H;g'|awk-vRS='''{for(i=1;i<=NF;i++)printf("%dx%d=%d%s", i, NR, i*NR, i==NR?"\n":"\t")}'|
|----|----|



#### 自己撸吧


关于其中的一些知识点可以参看[gawk的手册](http://www.gnu.org/software/gawk/manual/gawk.html)：
- 
内建变量，参看：[http://www.gnu.org/software/gawk/manual/gawk.html#Built_002din-Variables](http://www.gnu.org/software/gawk/manual/gawk.html#Built_002din-Variables)
- 
流控方面，参看：[http://www.gnu.org/software/gawk/manual/gawk.html#Statements](http://www.gnu.org/software/gawk/manual/gawk.html#Statements)
- 
内建函数，参看：[http://www.gnu.org/software/gawk/manual/gawk.html#Built_002din](http://www.gnu.org/software/gawk/manual/gawk.html#Built_002din)
- 
正则表达式，参看：[http://www.gnu.org/software/gawk/manual/gawk.html#Regexp](http://www.gnu.org/software/gawk/manual/gawk.html#Regexp)


（全文完）



（转载本站文章请注明作者和出处 [酷壳 – CoolShell.cn](http://coolshell.cn/) ，请勿用于任何商业用途）




