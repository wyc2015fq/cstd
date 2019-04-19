# shell学习 - sxf_123456的博客 - CSDN博客
2017年09月29日 16:33:41[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：132
个人分类：[shell](https://blog.csdn.net/sxf_123456/article/category/6881619)
                
显示目录下所有文件
#!/bin/bash
for file in `ls /etc`;
do 
echo $file
done 
设置只读变量，在变量前添加readonly就可以定义只读变量,
注意给变量赋值，等号前后不能有空格
#!/bin/bash
myUrl="http://www.w3cschool"
readonly myUrl
myUrl="htpp://www.baidu.com"
删除变量
#!/bin/bash
myUrl="http://www.baidu.com"
unset myUrl
echo $myUrl
拼接字符串(与python不同)
#!/bin/bash
your_name="hello"
greeting="hello,"$your_name"!"
echo $greeting
截取字符串
#!/bin/bash
string="runoob is a great site"
echo ${string:0:4}
shell数组支持一维数组，并且没有限定数组的大小
array_name=(value0 value1 value2 value3)
读取数组
value=${array_name[n]}
使用@可以读取数组中所有元素
echo ${array_name[@]}
for循环
#!/bin/bash
array_value=(1 2 3 4)
for i in 0 1 2 3;
do
echo ${array_value[$i]}
done
while循环
#!/bin/bash
int=1
while(($int<=5))
do
echo $int
let "int++"
done
使用@符号可以获取数组中的所有元素
value=(1 2 3 4)
echo ${value[@]}
获取数组的长度
获取数组元素的个数
length=${#array_name[@]}
或者
length=${#array_name[*]}
获取数组单个元素的长度
length=${#array_name[n]}
#!/bin/bash
value=(1 2 3 4 5)
echo ${#value[@]}
echo ${#value[*]}
echo ${#value[0]}
shell注释
#符号
shell参数传递
在执行shell脚本时，向脚本传递参数，脚内获取参数的格式为$n
#!/bin/bash
echo "执行文件名:$0"
echo "第一个参数:$1"
echo "第二个参数:$2"
echo "第三个参数:$3"
参数传递的特殊字符
$# 传递到脚本的参数个数
$* 以一个单字符串显示所有向脚本传递的参数
$$ 脚本运行的当前进程ID号
$? 显示最后命令的退出状态。0表示没有错误，其他任何值表明有错误
#!/bin/bash
a=10
b=20
value=`expr $a + $b`
echo "a+b:$value"
value=`expr $a \* $b`
echo "a*b:$value"
value=`expr $a / $b`
echo "a/b:$value"
value=`expr $a % $b`
echo "a%b:$value"
if [ $a == $b ]
then
echo "a等于b"
fi
if [ $a != $b ]
then
echo "a不等于b"
fi
关系运算符
-eq 检测两个数是否相等，相等返回true
-ne 检测两个数是否相等，不等返回true
-gt 检测左边大于右边
-lt 检测左边小于右边
-ge 检测左边的数是否大于等于右边
-le 检测左边的数是否小于等于右边
布尔运算符
! 非运算符 [ !false ]返回true
-o 或运算  [ $a -lt 20 -o $b -gt 100 ]
-a 与运算符 [ $a -lt 20 -a $b -gt 100 ]
逻辑运算符
&& 逻辑and
|| 逻辑or
文件测试运算符
-b file 检测文件是否是块设备文件
-c file 检测文件是否是字符设备文件
-d file 检测文件是否是目录
-f file 检测文件是否是普通文件
-p file 检测文件是否是有名管道
-r file 检测文件是否是可读
-w file 检测文件是否是可写
-x file 检测文件是否是可执行
-s file 检测文件是否为空
-e file 检测文件是否存在
#!/bin/bash
file='/home/sxf/1.txt'
if [ -r $file ]
then
echo $file"可读"
fi
if [ -w $file ]
then
echo $file"可写"
fi
if [ -x $file ]
then
echo $file"可执行"
fi
if [ -d $file ]
then
echo $file"目录"
fi
if [ -s $file ]
then
echo $file"为空"
else
echo $file"不为空"
fi
if [ -e $file ]
then
echo $file"存在"
else
echo $file"不存在"
fi
格式化输出
#!/bin/bash
printf "%-10s %-8s %-4s\n" 姓名 性别 体重kg
printf "%-10s %-8s %-4.2f\n" 郭靖 男 66.134
printf "%-10s %-8s %-4.2f\n" 杨过 男 48.666
printf "%-10s %-8s %-4.2f\n" 国富 女 37.1222
if else 语法格式
if condition
then
command1
command2
else
command3
command4
fi
if else-if else 格式
if condition1
then
command1
elif condition2
then
command2
else
commandN
fi
for 循环
for i in 1 2 3 4 5
do
echo $i
done
while循环
while condition
do
command
done
#!/bin/bash
i=1
while(($i<=5))
do
echo $i
let "i++"
done
case语句
case 值 in
模式1)
command1
command2
;;
模式2)
command3
command4
;;
模式3）
command5
command6
;;
esac
#!/bin/bash
echo '输入1到4之间的数字:'
echo '输入的数字为:'
read aNum
case $aNum in
1) echo "选择1"
;;
2) echo "选择2"
;;
3) echo "选择3"
;;
4) echo "选择4"
;;
*) echo "没有输入1到4之间的数字"
;;
esac
跳出循环
break 命令
#!/bin/bash
while:
do
echo -n "输入1到5之间的数字"
read aNum
case $aNum in
1|2|3|4|5) echo "你输入的数字是$aNum"
;;
*) echo "输入的数字不是1到5之间的游戏结束"
break
;;
esac
done

