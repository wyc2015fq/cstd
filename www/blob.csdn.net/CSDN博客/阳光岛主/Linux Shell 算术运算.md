
# Linux Shell 算术运算 - 阳光岛主 - CSDN博客

2018年03月20日 12:13:03[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：2528


**[米扑博客](https://blog.mimvp.com/article/5994.html)**总结了 Bash shell 的算术运算有四种方式:
**1、使用 expr 外部程式**
加法 r=`expr 4 + 5`
echo $r
注意! '4' '+' '5' 这三者之间要有空格
r=`expr 4 * 5` \# 错误
乘法 r=`expr 4 \* 5`
**2、使用 $(( ))**
r=$(( 4 + 5 ))
echo $r
**3、使用 $[ ]**
r=$[ 4 + 5 ]
echo $r
乘法
r=`expr 4 \* 5`
r=$(( 4 * 5 ))
r=$[ 4 * 5 ]
echo $r
除法
r=`expr 40 / 5`
r=$(( 40 / 5 ))
r=$[ 40 / 5 ]
echo $r
减法
r=`expr 40 - 5`
r=$(( 40 - 5 ))
r=$[ 40 - 5 ]
echo $r
求余数
r=$[ 100 % 43 ]
echo $r
乘幂 (如 2 的 3 次方)
r=$(( 2 ** 3 ))
r=$[ 2 ** 3 ]
echo $r
注：expr 沒有乘幂
**4、使用let 命令**
加法：
n=10
let n=n+1
echo $n \#n=11
乘法：
let m=n*10
echo $m
除法：
let r=m/10
echo $r
求余数：
let r=m%7
echo $r
乘冪：
let r=m**2
echo $r
虽然Bash shell 有四种算术运算方法，但并不是每一种都是跨平台的，建议使用expr。
另外，我们在 script 中经常有**加1操作**，以下四法皆可：
m=$[ m + 1]
m=`expr $m + 1`
m=$(($m + 1))
let m=m+1
**Shell awk 循环求和**
**1. 简单求和**
|1
|2
|3
|4
|5
|6
|7
|8
|$|cat|test|.txt
|11
|22
|33
|44
|55
|$|awk|'{sum += $1};END{print sum}'|test|.txt
|165
|

**2. 特定列求和**
|1
|2
|3
|4
|5
|6
|7
|8
|$|cat|test|.txt
|aa      11
|bb      22
|cc      33
|aa      44
|dd|55
|$|awk|'/aa/ {sum += $2};END {print sum}'|test|.txt
|55
|

**3. 求和、平均数、最大值、最小值**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|$|cat|test|.txt
|aa      11
|bb      22
|cc      33
|aa      44
|dd|55
|$|cat|test|.txt ||awk|'{sum += $2};END {print sum}'
|165
|$|cat|test|.txt ||awk|'{sum += $2};END {print sum/NR}'
|33
|$|cat|test|.txt ||awk|'BEGIN {max=0} {if($2>max) max=$2 fi};END {print max}'
|55
|$|cat|test|.txt ||awk|'BEGIN {min=999999999} {if($2<min) min=$2 fi};END {print min}'
|11
|

**awk 浮点运算：**
运算表达式：awk 'BEGIN{printf "%.6f\n", ( 10 / 3)}'
运算结果：3.333333
PS：shell的内部算术运算符无法处理浮点数，所以当需要处理浮点数是，要用到外部工具（如awk）

**参考推荐**：
[Linux 之 shell 算术运算符](https://blog.mimvp.com/article/7419.html)
[Linux 之 shell 比较运算符](https://blog.mimvp.com/article/7415.html)
[Linux Shell 函数返回值](https://blog.mimvp.com/article/11359.html)
[Linux shell脚本通过expect实现自动输入密码](https://blog.mimvp.com/article/21043.html)


