# 【Shell】数组 - zkq_1986的博客 - CSDN博客





2017年12月14日 16:52:16[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：70
个人分类：[Linux](https://blog.csdn.net/zkq_1986/article/category/6407840)









Linux Bash中，数组变量的赋值有两种方法：

　　(1) name = (value1 ... valuen)    // 此时下标从0开始；

　　(2) name[index] = value

　　下面以一个简单的脚本来说明，脚本内容如下：

　　#!/bin/bash

　#定义数组

　　A=(a b c def)

　#把数组按字符串显示输出

　echo ${A[@]}

　　#屏幕显示：a b c def

　#数组的长度表示${#A[*]}

　　len=${#A[*]}

　echo ${#A[*]}

　　#屏幕显示：4

　#改变数组元素的值

　　A[3]='vivian'

　echo ${A[*]}

　　#屏幕显示：a b c vivian

#循环输出数组元素

　　i=0

　　while [ $i -lt $len ]

　　do

 echo ${A[$i]}

 let i++

　　done

　　#屏幕输出：

　　#      a

　　#       b

　　#      c

　　#     vivian



#循环输出数组元素的另一种写法，注意，在条件中，用#a[@]取其个数。

for
 ((i=0;i<${#A[@]};i=$i+1))

do

echo ${A[$i]}
done
　　#循环输出数组元素的另一种写法，注意，在条件中，引用变量一定要用双引号 ，否则报错。

　　for (( j=0; j<"$len"; j=j+1 ))    //len表示数组长度值

　　do

 　echo ${A[$j]}

　　done

　　#循环输出数组元素的另一种写法，注意，${A[*]}不能写成$A ，$A默认是第一个元素。   
 如果A="a b c ded"，就可以写$A，

　for value in ${A[*]}

　　do

 echo $value

　　done

ps：

若a=(1 2 3 4)表示所有元素，则其只能用${a[*]}或者${a[@]}来表示。在a=(1
 2 3 4)中，$a只是表示第一个元素1。

若a="1 2 3 4"表示所有元素，则其可以用${a[*]}或者${a[@]}或者$a来表示。

假如a="1 2 3 4"，则array=($a)就相当于a=(1 2 3 4)，不信你可以试试echo
 ${array[@]}。

上面的例子还可以改写成以下内容：

for value in $a

　　do

 echo $value

　　done

输出：

1

2

3

4








转载自：http://blog.csdn.net/chinawangfei/article/details/42869501



