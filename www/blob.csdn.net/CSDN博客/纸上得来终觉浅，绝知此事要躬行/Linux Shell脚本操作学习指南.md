# Linux Shell脚本操作学习指南 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年09月12日 15:08:24[boonya](https://me.csdn.net/boonya)阅读数：2467








**目录**

[Shell文件的规范](#Shell%E6%96%87%E4%BB%B6%E7%9A%84%E8%A7%84%E8%8C%83)

[Shell文件的创建和运行](#Shell%E6%96%87%E4%BB%B6%E7%9A%84%E5%88%9B%E5%BB%BA%E5%92%8C%E8%BF%90%E8%A1%8C)

[Shell文件语法](#Shell%E6%96%87%E4%BB%B6%E8%AF%AD%E6%B3%95)

[Shell传递参数](#Shell%E4%BC%A0%E9%80%92%E5%8F%82%E6%95%B0)

[Shell参数校验](#Shell%E5%8F%82%E6%95%B0%E6%A0%A1%E9%AA%8C)

[Shell数组](#Shell%E6%95%B0%E7%BB%84)

[Shell运算符](#Shell%E8%BF%90%E7%AE%97%E7%AC%A6)

[Shell输出指令echo和printf](#Shell%E8%BE%93%E5%87%BA%E6%8C%87%E4%BB%A4echo%E5%92%8Cprintf)

[Shell test 命令](#Shell%20test%20%E5%91%BD%E4%BB%A4)

[Shell 流程控制](#Shell%20%E6%B5%81%E7%A8%8B%E6%8E%A7%E5%88%B6)

[Shell 函数](#Shell%20%E5%87%BD%E6%95%B0)

参考地址：[http://www.runoob.com/linux/linux-shell.html](http://www.runoob.com/linux/linux-shell.html)

## Shell文件的规范

文件开头第一行解析头标识：

#！/bin/bash  或#！/bin/sh

注意：

（1）在Shell中如果一行的第一个字母是#,则是注释，但是上面两个是写在第一行，所以不是脚本注释行，如果写在某个命令之后，则变成注释行。

（2）sh为bash的软链接，大多数情况下，脚本的开头使用“#!/bin/bash”和“#!/bin/sh”是没有区别的，但更规范的写法是在脚本的开头使用“#!/bin/bash”。

## Shell文件的创建和运行

1、vi创建一个test.sh文件

  vi test.sh

2、文件没有任何权限运行

  bash test.sh

  . test.sh

3、赋予文件权限运行

chmod a+x test.sh

./test.sh

## Shell文件语法

1、=====变量=====

#变量类型

#    运行shell时，会同时存在三种变量：

#    1) 局部变量 局部变量在脚本或命令中定义，仅在当前shell实例中有效，其他shell启动的程序不能访问局部变量。

#    2) 环境变量 所有的程序，包括shell启动的程序，都能访问环境变量，有些程序需要环境变量来保证其正常运行。必要的时候shell脚本也可以定义环境变量。

#    3) shell变量 shell变量是由shell程序设置的特殊变量。shell变量中有一部分是环境变量，有一部分是局部变量，这些变量保证了shell的正常运行

# 声明变量

var

username

# 变量赋值

username="boonya"

username="boonyatouch"

# 使用变量

$var $username 或${var} ${username}

# 只读变量

readonly host

host="www.boonya.cn";

# 删除变量 变量被删除后不能再次使用。unset 命令不能删除只读变量

unset username

2、=====字符串拼接及单双引号问题=====

name="boonya"

# 使用双引号拼接

greeting="hello, "$name" !"

greeting_new="hello, ${name} !"

echo $greeting  $greeting_new

# 使用单引号拼接

greeting_2='hello, '$your_name' !'

greeting_3='hello, ${your_name} !'

echo $greeting_2  $greeting_3

3、=====获取字符串长度===============

string="abcd"

echo ${#string} #输出 4

4、=====提取子字符串=================

以下实例从字符串第 2 个字符开始截取 4 个字符：

string="boonya is a great site"

echo ${string:1:4} # 输出 unoo



5、==查找子字符串 ==================

查找字符 i 或 o 的位置(哪个字母先出现就计算哪个)：

string="boonya is a great site"

echo `expr index "$string" io`  # 输出 4

6、=====数组=====================

数组名=(值1 值2 ... 值n)

示例：

array_values=(value0 value1 value2 value3)

7、==== 读取数组================

读取数组元素值的一般格式是：

${数组名[下标]}

例如：

valuen=${array_name[n]}

${array_name[index]}

8、=======@遍历功能===================

使用 @ 符号可以获取数组中的所有元素，例如：

echo ${array_name[@]}

9、 获取数组的长度

获取数组长度的方法与获取字符串长度的方法相同，例如：

# 取得数组元素的个数

length=${#array_name[@]}

# 或者

length=${#array_name[*]}

# 取得数组单个元素的长度

lengthn=${#array_name[n]}

10、=========Shell 注释=================

以 # 开头的行就是注释，会被解释器忽略。通过每一行加一个 # 号设置多行注释，像这样：

#--------------------------------------------

# 这是一个注释

# author：boonya

# site：www.boonya.cn

# slogan：学的不仅是技术，更是梦想！

#--------------------------------------------

##### 用户配置区 开始 #####

#

#

# 这里可以添加脚本描述信息

#

#

##### 用户配置区 结束  #####

11、=======多行注释==========================

多行注释还可以使用以下格式：

:<<EOF

注释内容...

注释内容...

注释内容...

EOF

EOF 也可以使用其他符号:

:<<'

注释内容...

注释内容...

注释内容...

'

:<<!

注释内容...

注释内容...

注释内容...

!

## Shell传递参数

参数处理     说明

$#     传递到脚本的参数个数

$*     以一个单字符串显示所有向脚本传递的参数。

如"$*"用「"」括起来的情况、以"$1 $2 … $n"的形式输出所有参数。

$$     脚本运行的当前进程ID号

$!     后台运行的最后一个进程的ID号

$@     与$*相同，但是使用时加引号，并在引号中返回每个参数。

如"$@"用「"」括起来的情况、以"$1" "$2" … "$n" 的形式输出所有参数。

$-     显示Shell使用的当前选项，与set命令功能相同。

$?     显示最后命令的退出状态。0表示没有错误，其他任何值表明有错误。

我们可以在执行 Shell 脚本时，向脚本传递参数，脚本内获取参数的格式为：$n。n 代表一个数字，1 为执行脚本的第一个参数，2 为执行脚本的第二个参数，以此类推……，$0为执行的文件名。

#!/bin/bash

# author:boonya

# url:www.boonya.cn

echo "Shell 传递参数实例！";

echo "执行的文件名：$0";

echo "第一个参数Host：$1";

echo "第二个参数Port：$2";

echo "第三个参数System：$3";

为脚本设置可执行权限，并执行脚本，输出结果如下所示：

$ chmod +x test.sh

$ ./param.sh localhost 8090 linux

Shell 传递参数实例！

执行的文件名：-bash

第一个参数Host：localhost

第二个参数Port：8090

第三个参数System：linux

## Shell参数校验

if [ -n "$1" ]; then

    echo "包含第一个参数"

else

    echo "没有包含第一参数"

fi

## Shell数组

Bash Shell 只支持一维数组（不支持多维数组）

array_name=(value1 ... valuen)

====获取单个数组元素

my_array=(A B "C" D)

echo "第一个元素为: ${my_array[0]}"

echo "第二个元素为: ${my_array[1]}"

echo "第三个元素为: ${my_array[2]}"

echo "第四个元素为: ${my_array[3]}"

==获取数组中的所有元素

使用@ 或 * 可以获取数组中的所有元素，例如：

#!/bin/bash

# author:boonya

# url:www.boonya.cn

my_array[0]=A

my_array[1]=B

my_array[2]=C

my_array[3]=D

echo "数组的元素为: ${my_array[*]}"

echo "数组的元素为: ${my_array[@]}"

==获取数组的长度

获取数组长度的方法与获取字符串长度的方法相同，例如：

#!/bin/bash

# author:boonya

# url:www.boonya.cn

my_array[0]=A

my_array[1]=B

my_array[2]=C

my_array[3]=D

echo "数组元素个数为: ${#my_array[*]}"

echo "数组元素个数为: ${#my_array[@]}"

## Shell运算符

1、基本运算符

Shell 和其他编程语言一样，支持多种运算符，包括：

    算数运算符

    关系运算符

    布尔运算符

    字符串运算符

    文件测试运算符

原生bash不支持简单的数学运算，但是可以通过其他命令来实现，例如 awk 和 expr，expr 最常用。

expr 是一款表达式计算工具，使用它能完成表达式的求值操作。

#!/bin/bash

val=`expr 2 + 2`

echo "两数之和为 : $val"

2、算术运算符

下表列出了常用的算术运算符，假定变量 a 为 10，变量 b 为 20：

运算符     说明     举例

+     加法     `expr $a + $b` 结果为 30。

-     减法     `expr $a - $b` 结果为 -10。

*     乘法     `expr $a \* $b` 结果为  200。

/     除法     `expr $b / $a` 结果为 2。

%     取余     `expr $b % $a` 结果为 0。

=     赋值     a=$b 将把变量 b 的值赋给 a。

==     相等。用于比较两个数字，相同则返回 true。     [ $a == $b ] 返回 false。

!=     不相等。用于比较两个数字，不相同则返回 true。     [ $a != $b ] 返回 true。

注意：条件表达式要放在方括号之间，并且要有空格，例如: [$a==$b] 是错误的，必须写成 [ $a == $b ]。

3、关系运算符

关系运算符只支持数字，不支持字符串，除非字符串的值是数字。

下表列出了常用的关系运算符，假定变量 a 为 10，变量 b 为 20：

运算符     说明     举例

-eq     检测两个数是否相等，相等返回 true。     [ $a -eq $b ] 返回 false。

-ne     检测两个数是否不相等，不相等返回 true。     [ $a -ne $b ] 返回 true。

-gt     检测左边的数是否大于右边的，如果是，则返回 true。     [ $a -gt $b ] 返回 false。

-lt     检测左边的数是否小于右边的，如果是，则返回 true。     [ $a -lt $b ] 返回 true。

-ge     检测左边的数是否大于等于右边的，如果是，则返回 true。     [ $a -ge $b ] 返回 false。

-le     检测左边的数是否小于等于右边的，如果是，则返回 true。     [ $a -le $b ] 返回 true。

示例：

#!/bin/bash

# author:boonya

# url:www.boonya.cn

a=10

b=20

if [ $a -eq $b ]

then

   echo "$a -eq $b : a 等于 b"

else

   echo "$a -eq $b: a 不等于 b"

fi

if [ $a -ne $b ]

then

   echo "$a -ne $b: a 不等于 b"

else

   echo "$a -ne $b : a 等于 b"

fi

if [ $a -gt $b ]

then

   echo "$a -gt $b: a 大于 b"

else

   echo "$a -gt $b: a 不大于 b"

fi

if [ $a -lt $b ]

then

   echo "$a -lt $b: a 小于 b"

else

   echo "$a -lt $b: a 不小于 b"

fi

if [ $a -ge $b ]

then

   echo "$a -ge $b: a 大于或等于 b"

else

   echo "$a -ge $b: a 小于 b"

fi

if [ $a -le $b ]

then

   echo "$a -le $b: a 小于或等于 b"

else

   echo "$a -le $b: a 大于 b"

fi

4、布尔运算符

下表列出了常用的布尔运算符，假定变量 a 为 10，变量 b 为 20：

运算符     说明     举例

!     非运算，表达式为 true 则返回 false，否则返回 true。     [ ! false ] 返回 true。

-o     或运算，有一个表达式为 true 则返回 true。     [ $a -lt 20 -o $b -gt 100 ] 返回 true。

-a     与运算，两个表达式都为 true 才返回 true。     [ $a -lt 20 -a $b -gt 100 ] 返回 false。

示例：

#!/bin/bash

# author:boonya

# url:www.boonya.cn

a=10

b=20

if [ $a != $b ]

then

   echo "$a != $b : a 不等于 b"

else

   echo "$a != $b: a 等于 b"

fi

if [ $a -lt 100 -a $b -gt 15 ]

then

   echo "$a 小于 100 且 $b 大于 15 : 返回 true"

else

   echo "$a 小于 100 且 $b 大于 15 : 返回 false"

fi

if [ $a -lt 100 -o $b -gt 100 ]

then

   echo "$a 小于 100 或 $b 大于 100 : 返回 true"

else

   echo "$a 小于 100 或 $b 大于 100 : 返回 false"

fi

if [ $a -lt 5 -o $b -gt 100 ]

then

   echo "$a 小于 5 或 $b 大于 100 : 返回 true"

else

   echo "$a 小于 5 或 $b 大于 100 : 返回 false"

fi

5、逻辑运算符

以下介绍 Shell 的逻辑运算符，假定变量 a 为 10，变量 b 为 20:

运算符     说明     举例

&&     逻辑的 AND     [[ $a -lt 100 && $b -gt 100 ]] 返回 false

||     逻辑的 OR     [[ $a -lt 100 || $b -gt 100 ]] 返回 true

示例：

#!/bin/bash

# author:boonya

# url:www.boonya.cn

a=10

b=20

if [[ $a -lt 100 && $b -gt 100 ]]

then

   echo "返回 true"

else

   echo "返回 false"

fi

if [[ $a -lt 100 || $b -gt 100 ]]

then

   echo "返回 true"

else

   echo "返回 false"

fi

6、字符串运算符

下表列出了常用的字符串运算符，假定变量 a 为 "abc"，变量 b 为 "efg"：

运算符     说明     举例

=     检测两个字符串是否相等，相等返回 true。     [ $a = $b ] 返回 false。

!=     检测两个字符串是否相等，不相等返回 true。     [ $a != $b ] 返回 true。

-z     检测字符串长度是否为0，为0返回 true。     [ -z $a ] 返回 false。

-n     检测字符串长度是否为0，不为0返回 true。     [ -n "$a" ] 返回 true。

str     检测字符串是否为空，不为空返回 true。     [ $a ] 返回 true。

示例：

#!/bin/bash

# author:boonya

# url:www.boonya.cn

a="abc"

b="efg"

if [ $a = $b ]

then

   echo "$a = $b : a 等于 b"

else

   echo "$a = $b: a 不等于 b"

fi

if [ $a != $b ]

then

   echo "$a != $b : a 不等于 b"

else

   echo "$a != $b: a 等于 b"

fi

if [ -z $a ]

then

   echo "-z $a : 字符串长度为 0"

else

   echo "-z $a : 字符串长度不为 0"

fi

if [ -n "$a" ]

then

   echo "-n $a : 字符串长度不为 0"

else

   echo "-n $a : 字符串长度为 0"

fi

if [ $a ]

then

   echo "$a : 字符串不为空"

else

   echo "$a : 字符串为空"

fi

7、文件测试运算符

文件测试运算符用于检测 Unix 文件的各种属性。

属性检测描述如下：

操作符     说明     举例

-b file     检测文件是否是块设备文件，如果是，则返回 true。     [ -b $file ] 返回 false。

-c file     检测文件是否是字符设备文件，如果是，则返回 true。     [ -c $file ] 返回 false。

-d file     检测文件是否是目录，如果是，则返回 true。     [ -d $file ] 返回 false。

-f file     检测文件是否是普通文件（既不是目录，也不是设备文件），如果是，则返回 true。     [ -f $file ] 返回 true。

-g file     检测文件是否设置了 SGID 位，如果是，则返回 true。     [ -g $file ] 返回 false。

-k file     检测文件是否设置了粘着位(Sticky Bit)，如果是，则返回 true。     [ -k $file ] 返回 false。

-p file     检测文件是否是有名管道，如果是，则返回 true。     [ -p $file ] 返回 false。

-u file     检测文件是否设置了 SUID 位，如果是，则返回 true。     [ -u $file ] 返回 false。

-r file     检测文件是否可读，如果是，则返回 true。     [ -r $file ] 返回 true。

-w file     检测文件是否可写，如果是，则返回 true。     [ -w $file ] 返回 true。

-x file     检测文件是否可执行，如果是，则返回 true。     [ -x $file ] 返回 true。

-s file     检测文件是否为空（文件大小是否大于0），不为空返回 true。     [ -s $file ] 返回 true。

-e file     检测文件（包括目录）是否存在，如果是，则返回 true。     [ -e $file ] 返回 true。

示例：

变量 file 表示文件"/var/www/boonya/test.sh"，它的大小为100字节，具有 rwx 权限。下面的代码，将检测该文件的各种属性：

#!/bin/bash

# author:boonya

# url:www.boonya.cn

file="/var/www/boonya/test.sh"

if [ -r $file ]

then

   echo "文件可读"

else

   echo "文件不可读"

fi

if [ -w $file ]

then

   echo "文件可写"

else

   echo "文件不可写"

fi

if [ -x $file ]

then

   echo "文件可执行"

else

   echo "文件不可执行"

fi

if [ -f $file ]

then

   echo "文件为普通文件"

else

   echo "文件为特殊文件"

fi

if [ -d $file ]

then

   echo "文件是个目录"

else

   echo "文件不是个目录"

fi

if [ -s $file ]

then

   echo "文件不为空"

else

   echo "文件为空"

fi

if [ -e $file ]

then

   echo "文件存在"

else

   echo "文件不存在"

fi

## Shell输出指令echo和printf

1、echo

echo "content"

2、printf

printf 命令模仿 C 程序库（library）里的 printf() 程序。

printf 由 POSIX 标准所定义，因此使用 printf 的脚本比使用 echo 移植性好。

printf 使用引用文本或空格分隔的参数，外面可以在 printf 中使用格式化字符串，还可以制定字符串的宽度、左右对齐方式等。默认 printf 不会像 echo 自动添加换行符，我们可以手动添加 \n。

printf  format-string  [arguments...]

参数说明：

    format-string: 为格式控制字符串

    arguments: 为参数列表。

参考示例：

#! /bin/bash

$ echo "Hello, Shell"

Hello, Shell

$ printf "Hello, Shell\n"

Hello, Shell

$

3、printf的转义序列

序列    说明

\a    警告字符，通常为ASCII的BEL字符

\b    后退

\c    抑制（不显示）输出结果中任何结尾的换行字符（只在%b格式指示符控制下的参数字符串中有效），而且，任何留在参数里的字符、任何接下来的参数以及任何留在格式字符串中的字符，都被忽略

\f    换页（formfeed）

\n     换行

\r    回车（Carriage return）

\t    水平制表符

\v    垂直制表符

\\    一个字面上的反斜杠字符

\ddd     表示1到3位数八进制值的字符。仅在格式字符串中有效

\0ddd    表示1到3位的八进制值字符

## Shell test 命令

Shell中的 test 命令用于检查某个条件是否成立，它可以进行数值、字符和文件三个方面的测试。

1、数值测试

参数     说明

-eq     等于则为真

-ne     不等于则为真

-gt     大于则为真

-ge     大于等于则为真

-lt     小于则为真

-le     小于等于则为真

示例：

num1=100

num2=100

if test $[num1] -eq $[num2]

then

    echo '两个数相等！'

else

    echo '两个数不相等！'

fi

2、字符串测试

参数     说明

=     等于则为真

!=     不相等则为真

-z 字符串     字符串的长度为零则为真

-n 字符串     字符串的长度不为零则为真

示例：

num1="ru1noob"

num2="boonya"

if test $num1 = $num2

then

    echo '两个字符串相等!'

else

    echo '两个字符串不相等!'

fi

3、文件测试

参数     说明

-e 文件名     如果文件存在则为真

-r 文件名     如果文件存在且可读则为真

-w 文件名     如果文件存在且可写则为真

-x 文件名     如果文件存在且可执行则为真

-s 文件名     如果文件存在且至少有一个字符则为真

-d 文件名     如果文件存在且为目录则为真

-f 文件名     如果文件存在且为普通文件则为真

-c 文件名     如果文件存在且为字符型特殊文件则为真

-b 文件名     如果文件存在且为块特殊文件则为真

示例：

#！/bin/bash

cd /bin

if test -e ./bash

then

    echo '文件已存在!'

else

    echo '文件不存在!'

fi

另外，Shell还提供了与( -a )、或( -o )、非( ! )三个逻辑操作符用于将测试条件连接起来，其优先级为："!"最高，"-a"次之，"-o"最低。例如：

#！/bin/bash

cd /bin

if test -e ./notFile -o -e ./bash

then

    echo '至少有一个文件存在!'

else

    echo '两个文件都不存在'

fi

## Shell 流程控制

1、if else-if else

示例1：

if condition

then

    command1

    command2

    ...

    commandN

else

    command

fi

示例2：

if condition1

then

    command1

elif condition2

then

    command2

else

    commandN

fi

示例3：

a=10

b=20

if [ $a == $b ]

then

   echo "a 等于 b"

elif [ $a -gt $b ]

then

   echo "a 大于 b"

elif [ $a -lt $b ]

then

   echo "a 小于 b"

else

   echo "没有符合的条件"

fi

2、for循环

for var in item1 item2 ... itemN

do

    command1

    command2

    ...

    commandN

done

写成一行;for var in item1 item2 ... itemN; do command1; command2… done;

3、 while 语句

while循环用于不断执行一系列命令，也用于从输入文件中读取数据；命令通常为测试条件。其格式为：

while condition

do

    command

done

 ======无限循环===

无限循环语法格式：

while :

do

    command

done

或者

while true

do

    command

done

或者

for (( ; ; ))

4、 until 循环

until 循环执行一系列命令直至条件为 true 时停止。

until 循环与 while 循环在处理方式上刚好相反。

一般 while 循环优于 until 循环，但在某些时候—也只是极少数情况下，until 循环更加有用。

until 语法格式:

until condition

do

    command

done

示例：

#!/bin/bash

a=0

until [ ! $a -lt 10 ]

do

   echo $a

   a=`expr $a + 1`

done

5、 case

Shell case语句为多选择语句。可以用case语句匹配一个值与一个模式，如果匹配成功，执行相匹配的命令。case语句格式如下：

case 值 in

模式1)

    command1

    command2

    ...

    commandN

    ;;

模式2）

    command1

    command2

    ...

    commandN

    ;;

esac

case工作方式如上所示。取值后面必须为单词in，每一模式必须以右括号结束。取值可以为变量或常数。匹配发现取值符合某一模式后，其间所有命令开始执行直至 ;;。

取值将检测匹配的每一个模式。一旦模式匹配，则执行完匹配模式相应命令后不再继续其他模式。如果无一匹配模式，使用星号 * 捕获该值，再执行后面的命令。

下面的脚本提示输入1到4，与每一种模式进行匹配：



echo '输入 1 到 4 之间的数字:'

echo '你输入的数字为:'

read aNum

case $aNum in

    1)  echo '你选择了 1'

    ;;

    2)  echo '你选择了 2'

    ;;

    3)  echo '你选择了 3'

    ;;

    4)  echo '你选择了 4'

    ;;

    *)  echo '你没有输入 1 到 4 之间的数字'

    ;;

esac

6、break continue esac

 break命令：break命令允许跳出所有循环（终止执行后面的所有循环）。

示例：

#!/bin/bash

while :

do

    echo -n "输入 1 到 5 之间的数字:"

    read aNum

    case $aNum in

        1|2|3|4|5) echo "你输入的数字为 $aNum!"

        ;;

        *) echo "你输入的数字不是 1 到 5 之间的! 游戏结束"

            break

        ;;

    esac

done

 continue：continue命令与break命令类似，只有一点差别，它不会跳出所有循环，仅仅跳出当前循环。

示例：

#!/bin/bash

while :

do

    echo -n "输入 1 到 5 之间的数字: "

    read aNum

    case $aNum in

        1|2|3|4|5) echo "你输入的数字为 $aNum!"

        ;;

        *) echo "你输入的数字不是 1 到 5 之间的!"

            continue

            echo "游戏结束"

        ;;

    esac

done

 esac：case的语法和C family语言差别很大，它需要一个esac（就是case反过来）作为结束标记，每个case分支用右圆括号，用两个分号表示break。

## Shell 函数

1、普通函数

linux shell 可以用户定义函数，然后在shell脚本中可以随便调用。

[ function ] funname [()]

{

    action;

    [return int;]

}

说明：

    1、可以带function fun() 定义，也可以直接fun() 定义,不带任何参数。

    2、参数返回，可以显示加：return 返回，如果不加，将以最后一条命令运行结果，作为返回值。 return后跟数值n(0-255 ）。

示例：

#!/bin/bash

# author:boonya

# url:www.boonya.cn

demo(){

    echo "这是我的第一个 shell 函数!"

}

echo "-----函数开始执行-----"

   demo

echo "-----函数执行完毕-----"

2、函数参数

在Shell中，调用函数时可以向其传递参数。在函数体内部，通过 $n 的形式来获取参数的值，例如，$1表示第一个参数，$2表示第二个参数...

带参数的函数示例：

#!/bin/bash

# author:boonya

# url:www.boonya.cn

funWithParam(){

    echo "第一个参数为 $1 !"

    echo "第二个参数为 $2 !"

    echo "第十个参数为 $10 !"

    echo "第十个参数为 ${10} !"

    echo "第十一个参数为 ${11} !"

    echo "参数总数有 $# 个!"

    echo "作为一个字符串输出所有参数 $* !"

}

funWithParam 1 2 3 4 5 6 7 8 9 34 73

注意，$10 不能获取第十个参数，获取第十个参数需要${10}。当n>=10时，需要使用${n}来获取参数。

另外，还有几个特殊字符用来处理参数：

参数处理     说明

$#     传递到脚本的参数个数

$*     以一个单字符串显示所有向脚本传递的参数

$$     脚本运行的当前进程ID号

$!     后台运行的最后一个进程的ID号

$@     与$*相同，但是使用时加引号，并在引号中返回每个参数。

$-     显示Shell使用的当前选项，与set命令功能相同。

$?     显示最后命令的退出状态。0表示没有错误，其他任何值表明有错误。





