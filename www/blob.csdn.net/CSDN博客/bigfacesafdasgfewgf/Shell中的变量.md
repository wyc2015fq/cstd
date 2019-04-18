# Shell中的变量 - bigfacesafdasgfewgf - CSDN博客





2015年05月17日 15:22:44[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：767











    这篇博文我们首先简单介绍Shell中变量的几个注意点，然后通过实际的例子来学习。

**    关于Shell的变量，总结起来有如下几个注意点：**

**    1）Shell中的变量是不区分类型的，这点和C++、java语言不同。变量统一地按照字符串存储；**

**    2）允许使用declare来改变变量的类型为整型，如declare -i x   这样并不改变x本身的值**

**    3）echo语句中显示变量值，需要使用$x；**

**    4）允许变量执行一些不同的操作，例如字符串的比较和整数的加减等；**

**    5）x=123   x此时是整型变量    x="123"    x此时是字符串类型；**

**    6）关于变量中字符串的删除和替换操作：**

**![](https://img-blog.csdn.net/20150517154725984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



**    7）执行变量加1运算时，x+=1   如果变量x中含有非数字字符，那么x会自动转换为整数0，再加1；如果x中全是数字字符，那么自动转换为相应的整数，再加1；**

**    8）把一个空串赋值给一个变量：x=""**

**    9）没有定义变量，直接$p，认为p变量中是一个空串；**






```
#-----------------------------/chapter3/ex3-1.sh------------------
#! /bin/bash

#定义变量x，并且赋值为123。#此时x是整型变量，123是数字，不是"123"
x=123
#变量x加1，整型相加，支持+=运算符，记得加上双引号
let "x += 1"
#输出变量x的值，记得加上双引号
echo "x = $x"
#显示空行
echo

#替换x中的1为abc，并且将值赋给变量y。关于变量中字符串的替换和删除有表格
#${变量/旧字符串/新字符串}  第一个旧字符串会被新字符串替换
y=${x/1/abc}
#输出变量y的值
echo "y = $y"

#声明变量y，这样并不改变x本身的值
declare -i y
#输出变量y的值，这样并不改变x本身的值
echo "y = $y"
#变量y的值加1，y中含有非数字字符，被自动转换为整数0
let "y += 1"
#输出变量y的值
echo "y = $y"
#显示空行
echo

#将字符串赋给变量z，含有字母的字符串
z=abc22
#输出变量z的值
echo "z = $z"

#替换变量z中的abc为数字11，并且将值赋给变量m，变量的替换
m=${z/abc/11}
#输出变量m的值
echo "m = $m"
#变量m加1，自动转换为整数
let "m += 1"
#输出变量m的值
echo "m = $m"
echo

#将空串赋给变量n
n=""
#输出变量n的值，echo输出的时候，会带来n = ，后面会有$n加上n的内容
echo "n = $n"
#变量n加1
let "n += 1"
echo "n = $n"
echo

#输出空变量p的值，没有定义变量，直接$p，认为p变量中是一个空串
echo "p = $p"
# 变量p加1
let "p += 1"
echo "p = $p"
```





**    10）如果变量的值包含空格，则一定要用双引号括起来赋值；**

**    11）declare除以把x变量声明为整型之外的功能，还可以控制变量的读写属性，具体总结如下：**

**![](https://img-blog.csdn.net/20150517162332952?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**注意：上面的属性前面的-号，可以变成+号，表示相反的意思；**

**    12）如果变量x被声明为整数类型，那么x就可以参与一些算术运算，比如加法，比如除法； **

**    13）如果变量x被声明为整数类型，那么再把字符串赋值给x的时候，x一律被转换为0；**

**    14）bash不内置对浮点数的支持，所以不要把小数赋值；**

**    15）在shell中，为了得到算术式的结果，可以使用方括号或是圆括号，x=$[6/3]和x=$((6/3))**

**    16）在declare -r x之后，x是只读变量，不能对只读变量赋值修改；**






```
#-----------------------------/chapter3/ex3-3.sh------------------
#! /bin/bash

#定义变量x，并将一个算术式赋给它,字符串
x=6/3
echo "$x"
#定义变量x为整数
declare -i x
echo "$x"
#将算术式赋给变量x，此时x是整数类型，可以参与算术运算
x=6/3
echo "$x"

#将字符串赋给变量x
x=hello
echo "$x"
#将浮点数赋给变量x，bash不内置对浮点数的支持，所以不要把小数赋值；
x=3.14
echo "$x"
#取消变量x的整数属性，这个+号很重要
declare +i x
#重新将算术式赋给变量x
x=6/3
echo "$x"
#求表达式的值
x=$[6/3]
echo "$x"
#求表达式的值
x=$((6/3))
echo "$x"
#声明只读变量x
declare -r x
echo "$x"
#尝试为只读变量赋值，不能对只读变量赋值修改
x=5
echo "$x
```




**    17）变量赋值中的单引号''。单引号括起来的字符都做为普通字符，包括特殊字符也被当做普通字符处理；**

**    18）变量赋值中的双引号""。双引号括起来的字符，除了"$"，“\”, "'", """这几个特殊字符可以保留功能，其余都是普通字符；**

**    19）变量赋值中的反引号``。首先执行反引号中的部分，并以标准输出结果展示出来。**



```
#-----------------------------/chapter3/ex3-4.sh------------------
#! /bin/bash

#输出当前目录
echo "current directory is `pwd`"
```

**    20）全局变量。作用域从被定义~shell脚本运行结束或者变量被删除。其不仅可以在脚本中定义，还可以再函数func中定义，而且都是全局变量；注意：函数的参数是局部变量。**




```
#-----------------------------/chapter3/ex3-6.sh------------------
#! /bin/bash

#定义函数
func()
{
   #在函数内部定义变量
   v2=200
}
#调用函数
func
#输出变量的值
echo "$v2"
```

**    21）局部变量。一般使用在函数内部，用local x=100来定义。如果函数内部一个局部变量和一个全局变量同名，局部变量优先处理。**




```
#-----------------------------/chapter3/ex3-8.sh------------------
#! /bin/bash

#定义函数
func()
{
   #输出全局变量v1的值
   echo "global variable v1 is $v1"
   #定义局部变量v1
   local v1=2
   #输出局部变量v1的值
   echo "local variable v1 is $v1"
}
#定义全局变量v1
v1=1
#调用函数
func
#输出全局变量v1的值
echo "global variable v1 is $v1"
```

**    22）系统变量。一般用作脚本和函数的参数，以及脚本和函数的返回值。常用的系统变量总结如下：**


![](https://img-blog.csdn.net/20150517170452924?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**    注意：使用$@取文件每个参数的时候，@只能是一位数0~9，如果要是两位数或者更多，需要使用${@}。**



```
#-----------------------------/chapter3/ex3-9.sh------------------
#! /bin/bash

#输出脚本的参数个数
echo "the number of parameters is $#"
#输出上一个命令的退出状态码
echo "the return code of last command is $?"
#输出当前脚本名称
echo "the script name is $0"
#输出所有的参数
echo "the parameters are $*"
#输出其中的几个参数
echo "\$1=$1;\$2=$2;\$11=$11"
```

**    23）环境变量。环境变量就是所有shell程序都可以直接使用的变量。而且环境变量全是都是大写字母。**


![](https://img-blog.csdn.net/20150517171059458?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**    24）unset清除变量。**

**    25）变量的赋值中，被单引号括起来的是全引用，被双引号括起来的是部分应用，被反引号或是圆括号括起来的是命令替换，反斜杠\是转义。**




    参考：

    《Shell从入门到精通》

    《鸟哥的Linux私房菜》












