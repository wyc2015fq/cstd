# linux的shell脚本

## 正则表达式

### grep

主要用于查找文件中包含特定字符的行
```shell
grep 'test' file.txt #查找file.txt文件中包含“test”的行
grep -n -A3 –B2 'test' #查找file.txt文件中包含“test”的行，将所在行的前两行（B2）和后三行（A3）一起找出来。 -A表示after，后续几行；-B表示before，前面几行；-n表示显示出行号。
grep –v 'test' file.txt #查找不包含“test”的行
grep –i 'test' file.txt #查找，不分大小写
grep –n 't[ae]st' file.txt #查找tast或者test。[]表示可选集，只能有一个字符
grep –n '[^t]est' file.txt #查找不以t开头的est字符串
grep –n '[^a-z]est' file.txt #查找不以小写英文字符开头的est字符串。
#也可以写成grep –n '[^[:lower:]]est' file.txt
grep –v '[a-z]est' #包括所有不是[a-z]est的行，比上面的命令包含的范围更大
grep –n '^test' file.txt #查找以test开头的行
grep –n '^[a-z]' file.txt #查找以小写字母开头的行
grep –n '^[^a-z]' file.txt #查找不以小写字母开头的行
#^在[]表示非，在''内表示开头
grep –n '\.$' file.txt #查找以.结尾的行。.需要转义，它表示有任意一个字符
grep –n '^$' file.txt #查找空白行
#.表示一定有一个字符，*表示它前面的那个字符重复0到多次
grep –n 'g..d' file.txt #查找good/gaad/geed等字符
grep –n 'oo*' file.txt #查找包含一个及一个以上的o的字符的行
grep –n 'g.*g' file.txt #其中的.*表示零个或者任意多个字符
#{}限定连续的字符范围，{和}需要用转移字符\
grep –n 'o\{2\}' file.txt #查找两个o的字符串，相当于grep –n'ooo*'
grep –n 'go\{2,5\}g' file.txt #查找g和g之间有2到5个o的字符串
ls | grep –n '^a.*' #查找以a开头的任意文件
```
### sed

主要用于行的操作，替换/删除/增加/插入/打印；用于行内特定字符串的替换
```shell
sed '2,5d' 删除2-5行
sed '2,$d' 删除第二行开始的所有行
sed '2a test' 在第二行后面插入test新行
sed '2i test' 在第二行前面插入test新行
sed '2a test1\
#test2' 在第二行后面插入两行
sed '2,5c hello world' 将第2到5行替换为helloworld
sed -n '2,5p' 将第2到5行打印出来，注意此处必须加上-n（表示只输出sed处理的行）
#相当于 head –n 5 | tail –n 2
部分数据的查找和替换
sed 's/旧字符/新字符/g'
sed 's/^.*addr//g' 删除开头到addr之间的所有字符
sed 's/addr.*$//g' 删除addr到结尾的所有字符
sed 's/#.*$//g' 删除以#开头的注释行
sed –i 's/\.$/\!/g' 将每一行结尾的.替换为！，-i表示直接修改文件的内容
```
### awk

主要用于和print联合，用于对文件的列进行处理
awk '条件1{动作1} 条件2{动作2}'file.txt
```shell
\1.  awk '{print $1 “\t” print $3}' 将文件中每行的第一列和第三列输出，中间用tab隔开

注意，awk必须后跟''，''里面的字符串用””表示

\2.  NF：每一行的字段总数

NR：第几行

FS：使用的分割字符，默认的是空格键

awk '{print $1 “\t lines:” NR “\t columes:” NF}'

\3.  使用条件

awk '{FS=”:”} $3 < 10{print $1 “\T” $3}' 每一行的分割符都是:，第三列的值小于10才打印出这一行的第一列和第三列

注意上面的分割符是:，从第二行才开始有效，第一行的分割符仍然是空格，可以修改为：

awk 'BEGIN{FS=”:”} $3 < 10{print $1 “\T” $3}'

\4.  awk 'NR==1 {printf “%10s %10s%10s”, $1, $2,”total”} NR > 1{total=$1+$2;printf”%10s %10s %10s\n”, $1, $2,total}'

从第二行开始，将第一列和第二列相加作为第三列的值输出。

{}里面有多个命令的时候使用;隔开；awk中的变量可以直接使用，不用加$
```
### 扩展正则表达式

使用方法：grep –E 或者egrep
```shell
 删除空白行和以#开头的行

grep –v '^$'

grep –v '^#'

使用扩展正则表达式，可以写成一行命令：

egrep –v '^$ | ^#'

\2.  Egrep –n 'g(as|ab)d' 查找gasd或者gabd的行

\3.  egrep –n 'A(abc)+C' 查找AabcabcC这样的行，()+表示多个重复的组

\4.  egrep –n 'go?d' ?表示零个或一个前面的字符，+表示一个或一个以上的前面字符
```
五．文件比较工具
```shell
\1.  diff [-bBi] fileA fileB 以行为单位进行比较

-b 忽略多个空白符

-B 忽略空白行

-I 忽略大小写

\2. cmp[-s] fileA fileB 以字节为单位进行比较

\3. patch可以制作补丁文件，用于版本的更新
```













## Shell script

### 一． 规范性

\1.  Shell文件头格式：

\#!/bin/bash

\#Program:

\#  purpose

\#History:

\#2014/2/17 www First release

\2.  输入文件参数个数的判定

if [[ $# -ne 1 ]]; then

echo "Usage: $0 <model_name>" 1>&2

exit 1语法

fi

if [ "$1" == "-h" ] ||[ "$1" == "--help" ];then

 echo "Usage: $0 <model_name>" 1>&2

 exit 1

fi

### 二． 格式

\1.  执行：

source 或者./test.sh

sh test.sh 相当于在子进程中运行，执行完毕以后test.sh里面的变量变为无效

\2.  判断文件是否存在或者目录是否存在：

if [ ! -d${AM} ]; then

 echo "<ERROR>:can not findacoustic model"

 exit 1

fi

if [ ! -f${LM} ]; then

 echo"<ERROR>:can not find langguage model"

 exit 1

 fi

\3.  判断上一步运行是否正确：

  if [ $? != 0 ]; then

   echo "<ERROR>:errorin juicer decoding"

   exit 1

  fi

\4.  条件判断：

if [ $a == 1 ] && [ $b == 2 ]; then

 

elif [ $a == 2 –a $b == 3]; then

 

else

 

fi

\5.  case用法

case $a in

 “1”)

 ;;

 “2”)

 ;;

 *)

 ;;

Esac

\6.  function

function test(){

}

$0 $1 $2可以传参

\7.  while do done

while []

do

 

done

\8.  for do done

for var in $(seg 1 100)#连续

do

done

或者

for ( (i =1; i < 100; i++) )

do

done

### 三． 调试

\1.  在shell脚本中加上 set –x

\2.  sh –x test.sh