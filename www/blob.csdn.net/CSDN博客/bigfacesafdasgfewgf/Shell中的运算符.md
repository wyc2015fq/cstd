# Shell中的运算符 - bigfacesafdasgfewgf - CSDN博客





2015年05月18日 14:52:53[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：840标签：[Shell																[算术运算																[expr																[let](https://so.csdn.net/so/search/s.do?q=let&t=blog)
个人分类：[Shell](https://blog.csdn.net/puqutogether/article/category/3236387)








    Shell中也可以实现基本算术运算，以及位运算。

    我们总结起来有如下几点：

**    1）基本的算术运算符：+、-、*、/、**都支持，最后的**是幂运算；**

**    2）算术运算要和一些命令结合才能使用，如expr`...`命令，$((...))命令，$[...]命令，let命令，注意：expr后面是反引号；**

**    3）一定要注意所有运算符两边都要有空格，比如result=`expr 2 + 3`就是正确的，如果是result=`expr 2+3`就是错误的！其余的运算符也是需要空格隔开！！**

**    4）运算公式中的括号运算、乘法运算一定要加转移符斜杠，即result=`expr \( 2 + 3 \) \* 5`才正确，除法运算不需要；**

**    5）expr不可以做幂运算；**



```
#-----------------------------/chapter4/ex4-28.sh------------------
#! /bin/sh

#计算2和100的差，即-98
result=`expr 2 - 100`
echo "$result"
#计算2和100的和，即102
result=`expr 2 + 100`
echo "$result"
#计算2和5的乘积，即10,乘号一定要加转移符反斜杠
result=`expr 2 \* 5`
echo "$result"
#计算24和8的商，即3
result=`expr 24 / 8`
echo "$result"
#先计算2和6的差，然后再乘以12，即-48，括号一定要加转移符反斜杠
result=`expr \( 2 - 6 \) \* 12`
echo "$result"
#错误的语法
result=`expr 2+5`
echo "$result"
#错误的语法
result=`expr 2-4*9`
echo "$result"
#错误的语法
result=`expr 1-(4-7)`
echo "$result"
```

**    6）使用$((...))也可以完成算法书运算，而且不需要对运算符和括号做转义处理！也不需要严格规定运算符两端加空格，空格可加可不加。如下：**


```
#-----------------------------/chapter4/ex4-29.sh------------------
#! /bin/sh

#紧凑格式，计算3和6的和
result=$((3+6))
echo "$result"
#松散格式，计算3和9的和
result=$(( 3 + 9 ))
echo "$result"
#计算3和6的乘积
reuslt=$(( 3 * 6 ))
echo "$result"
#计算7和5的商
result=$(( 7 / 5 ))
echo "$result"
#计算8和3的余数
result=$(( 8 % 3 ))
echo "$result"
#复合运算
result=$(( ( 1-4 ) * 5 ))
echo "$result"
```

**    所以，使用$((...))的形式更适合做算术运算。**
**    7）使用$[...]的形式和上面$((...))的形式一样，较常用；**



```
#-----------------------------/chapter4/ex4-30.sh------------------
#! /bin/sh

#加法运算
result=$[4+5]
echo "$result"
#复合运算
result=$[(1+2)*3]
echo "$result"
#幂运算
result=$[ 2 ** 4 ]
echo "$result"
```

**    8）let也可以实现算术运算。let后面直接接运算表达式，运算式中的变量不需要加$，直接使用，如下：**


```
#-----------------------------/chapter4/ex4-31.sh------------------
#! /bin/sh

#定义变量
n=10
#加法运算
let n=n+1
echo "$n"
#乘法运算
let n=n*10
echo "$n"
#幂运算
let n=n**2
echo "$n"
```

**    9）除了基本运算发之外，Shell还支持符合算术符：+=、-=、*=、/=、%=，以及位运算符：<<、>>、&、|、~、^、<<=、>>=、&=、|=、^=，还有自增++、自减--运算；**](https://so.csdn.net/so/search/s.do?q=expr&t=blog)](https://so.csdn.net/so/search/s.do?q=算术运算&t=blog)](https://so.csdn.net/so/search/s.do?q=Shell&t=blog)




