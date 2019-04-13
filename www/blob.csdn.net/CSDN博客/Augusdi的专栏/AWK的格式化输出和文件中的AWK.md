
# AWK的格式化输出和文件中的AWK - Augusdi的专栏 - CSDN博客


2015年06月19日 13:26:48[Augusdi](https://me.csdn.net/Augusdi)阅读数：3047


﻿﻿
**一，**Output FroMaT（输出格式）
**1，**
OFMT变量，在OFMT中定义数字的格式；默认为“%.6gd”，只会打印小数点后6位。
**2，**
[root@rhel helinbash]\# df
Filesystem           1K-blocks      Used Available Use% Mounted on
/dev/sda2              9920624   6479488   2929068  69% /
/dev/sda5             26742436   6534784  18827272  26% /u01
/dev/sda1               988088     23460    913624   3% /boot
tmpfs                   517568         0    517568   0% /dev/shm
[root@rhel helinbash]\# df | grep -v 'Available'| gawk '$4 >200000 { OFMT="%.2f"; print $1,$4/1024,"M"}'
/dev/sda2 2860.42 M
/dev/sda5 18386.01 M
/dev/sda1 892.21 M
tmpfs 505.44 M
[root@rhel helinbash]\#
[root@rhel helinbash]\# df | grep -v 'Available'| gawk '$4 >2000000 { OFMT="%.2f"; print $1,$4/1024,"M"}'
/dev/sda2 2860.42 M
/dev/sda5 18386.01 M
[root@rhel helinbash]\#
**3，**
[root@rhel helinbash]\# expr 10/5
10/5
[root@rhel helinbash]\# expr 10 / 5
2
[root@rhel helinbash]\# expr 10 / 3
3
[root@rhel helinbash]\# expr 11 / 3
3
[root@rhel helinbash]\#
**4，**
[root@rhel helinbash]\# echo -e "1000 100\n2000 101"
1000 100
2000 101
[root@rhel helinbash]\# echo -e "1000 100\n2000 101" | gawk '$2 >100'
2000 101
[root@rhel helinbash]\# echo -e "1000 100\n2000 101" | gawk '$2 >100 {print $1 /$2 }'
19.802
[root@rhel helinbash]\# echo -e "1000 100\n2000 101" | gawk '$2 >100 { OFMR="%.2f"; print $1 /$2 }'
19.802
[root@rhel helinbash]\# echo -e "1000 100\n2000 101" | gawk '$2 >100 { OFMT="%.2f"; print $1 /$2 }'
19.80
[root@rhel helinbash]\# echo -e "1000 100\n2000 97" | gawk '$2 <100 { OFMT="%.2f"; print $1 /$2 }'
20.62
[root@rhel helinbash]\# echo -e "1000 100\n2000 97" | gawk '$2 <100 { OFMT="%.4f"; print $1 /$2 }'
20.6186
[root@rhel helinbash]\#
**5，**
printf函数转义字符；printf与C语言中的printf雷同；
**（1）**
转义字符
%c 字符；
printf("The character is %c\n",x)；
%s 字符串
%d 十进制整数
printf("The boy is %d years old\n,50")
%f 浮点数
**（2）**
[root@rhel helinbash]\# gawk '{printf("===");}' names.txt
==================[root@rhel helinbash]\# cat names.txt

Tom Savage                 100
Molly   Lee           200
John  Doe       300
[root@rhel helinbash]\#
[root@rhel helinbash]\#

[root@rhel helinbash]\# gawk '{printf("===");}' names.txt
==================[root@rhel helinbash]\# cat names.txt

Tom Savage                 100
Molly   Lee           200
John  Doe       300
[root@rhel helinbash]\#
[root@rhel helinbash]\# gawk '{printf("===\n\n");}' names.txt
===
===
===
===
===
===
[root@rhel helinbash]\# gawk '{printf("===\n\n",$1);}' names.txt
===
===
===
===
===
===
[root@rhel helinbash]\#
[root@rhel helinbash]\# gawk '{printf("%s===%d\n\n",$1,$3);}' names.txt
===0
===0
Tom===100
Molly===200
John===300
===0
[root@rhel helinbash]\# gawk '{printf("%s\t%s\t%d\n",$1,$2,$3);}' names.txt
0
0
Tom     Savage  100
Molly   Lee     200
John    Doe     300
0
[root@rhel helinbash]\#
**6，**
printf函数修饰符；
打印时需要对齐，下面提供一些打印输出时所用到的修饰符；
-（横杠） 左对齐；
**（1）**
[root@rhel helinbash]\# echo "Bluefox" | gawk '{ printf "|%-15s|\n",$1}'
|Bluefox        |
[root@rhel helinbash]\# echo "Bluefox" | gawk '{ printf "|%15s|\n",$1}'
|        Bluefox|
[root@rhel helinbash]\#
**（2）**
\#（井号）显示8进制时前面加0，显示16进制时加0X
+（加好）显示正负值时正+负-号
0（零）用0对显示值填充空白处
**（3）**
[root@rhel helinbash]\# echo "Bluefox" | gawk '{ printf (" %s \n",$0)}'
Bluefox
[root@rhel helinbash]\# echo "Bluefox" | gawk '{ printf ("| %s | \n",$0)}'
| Bluefox |
[root@rhel helinbash]\#
**（4）**
[root@rhel helinbash]\# gawk '{printf("?\t?\t?\n",$1,$2,$3)}' names.txt
?       ?       ?
?       ?       ?
?       ?       ?
?       ?       ?
?       ?       ?
?       ?       ?
[root@rhel helinbash]\# gawk '{printf("%20s\t%-20s\t%-15d\n",$1,$2,$3)}' names.txt
Tom    Savage                  100
Molly    Lee                     200
John    Doe                     300
yang    wawa                    -121212
[root@rhel helinbash]\# gawk '{printf("%20s\t%-20s\t%+-15d\n",$1,$2,$3)}' names.txt
Tom    Savage                  +100
Molly    Lee                     +200
John    Doe                     +300
yang    wawa                    -121212
[root@rhel helinbash]\#
**（5）**
[root@rhel helinbash]\# gawk 'BEGIN{printf("%d",0x1F)}'
31[root@rhel helinbash]\# gawk 'BEGIN{printf("%d\n",0x1F)}'
31
[root@rhel helinbash]\# gawk 'BEGIN{printf("%d\n",0x20)}'
32
[root@rhel helinbash]\#
[root@rhel helinbash]\# gawk 'BEGIN{printf("%d\n",0xFF)}'
255
[root@rhel helinbash]\#
**二，**文件中的awk命令
**1，**
将awk写入一个文件中，更适合复杂的程序
使用-f选项指定awk的文件名；
awk一次读取一条记录，测试文件中的每一条命令这样循环；
**2，**
**（1）**
[root@rhel helinbash]\# gawk '{printf("%s - %s - %s\n",$1,$2,$3)}' names.txt
Tom - Savage - 100
Molly - Lee - 200
John - Doe - 300
yang - wawa - -121212
[root@rhel helinbash]\# gawk '/^Molly/{printf("%s - %s - %s\n",$1,$2,$3)}' names.txt
Molly - Lee - 200
[root@rhel helinbash]\#
[root@rhel helinbash]\# gawk '/^Molly/{printf("Welcome %s",$1)}' names.txt
Welcome Molly[root@rhel helinbash]\#
**（2）**
[root@rhel helinbash]\# cat names.txt
Tom Savage                 100
Molly   Lee           200
John  Doe       300
yang wawa -121212
[root@rhel helinbash]\# vim my.awk
[root@rhel helinbash]\# gawk -f my.awk names.txt
Tom - Savage - 100
Molly - Lee - 200
Nice to meet you -> Molly
John - Doe - 300
yang - wawa - -121212
[root@rhel helinbash]\# cat my.awk
\#!/bin/gawk

{printf("%s - %s - %d\n",$1,$2,$3)}
/^Mo/ { printf("Nice to meet you -> %s\n",$1) }
[root@rhel helinbash]\#
**（3）**
[root@rhel helinbash]\# vim my.awk
[root@rhel helinbash]\# gawk -f my.awk names.txt
Tom - Savage - 100
Nice to meet you -> Tom
Molly - Lee - 200
Molly   Lee           200
Nice to meet you -> Molly
John - Doe - 300
Nice to meet you -> John
yang - wawa - -121212
Nice to meet you -> yang
[root@rhel helinbash]\# cat my.awk
\#!/bin/gawk
**（4）**
{printf("%s - %s - %d\n",$1,$2,$3)} \#这个规则也是无条件打印
/^Mo/   \#这个仅仅变成了过滤规则
{ printf("Nice to meet you -> %s\n",$1) }   \#没有任何条件就会打印的，默认一行是一条规则。
[root@rhel helinbash]\#
逐条按照脚本里面的规则（这里有3个规则）进行过滤和并打印。
**（5）**
[root@rhel helinbash]\# vim my.awk
[root@rhel helinbash]\# gawk -f my.awk names.txt
Tom - Savage - 100
Molly - Lee - 200
Nice to meet you -> Molly
John - Doe - 300
yang - wawa - -121212
[root@rhel helinbash]\# cat my.awk
\#!/bin/gawk

{printf("%s - %s - %d\n",$1,$2,$3)}
/^Mo/ {
printf("Nice to meet you -> %s\n",$1)
}\#这个就变得正常了！！！！！
[root@rhel helinbash]\#
**（6）**
[root@rhel helinbash]\# gawk -f my.awk names.txt
Tom - Savage - 100
Molly - Lee - 200
Nice to meet you -> Molly
John - Doe - 300
yang - wawa - -121212
[root@rhel helinbash]\# cat my.awk
\#!/bin/gawk

{printf("%s - %s - %d\n",$1,$2,$3)}
/^Mo/    \
{
printf("Nice to meet you -> %s\n",$1)
}
[root@rhel helinbash]\#

[http://blog.itpub.net/29611940/viewspace-1172651/](http://blog.itpub.net/29611940/viewspace-1172651/)

