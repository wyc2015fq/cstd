# Shell 基本操作 - zkq_1986的博客 - CSDN博客





2016年09月08日 12:58:54[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：148








## 1 awk总结

```
line="2016-08-29 12:05:00|asd|sdf|asd0||1.000|0|4.76|1.000|0"
#ret=$(echo $line|cut -d "|" -f 1,6,15,16,29)
ret=$(echo $line|awk -F "|" '{print $1,$6,$15,$16}')
echo $ret
```

```
一、显示特定行

1.awk 'NR%2==1' file / /显示奇数行
2.sed -n '1,$n;p' file / /显示偶数行
3.sed -n '1,$p;n' file / /显示奇数行
4.奇数行:sed '2~2d' file / /显示奇数行
5.偶数行:sed '1~2d' file/ /显示偶数行

二、修改Unix时间
linux获得 Unix时间戳方法：awk 'BEGIN{print systime()}'
date 转换法：date -d @1328075260
awk方法：awk '{$1=strftime("%Y-%m-%d-%H:%M:%S",$1); print $0}
三、特定匹配

匹配第七个字段以mp4结尾的第三个域
awk '$7~/mp4$/ {print $3}'

$ awk '$1 ~/[0-9][0-9]$/(print $1}' test-----如果第一个域以两个数字结束就打印这个记录。

$ awk '$1 != 10' test-----如果第一个域不等于10就打印该行

$ awk '$1 == 100 || $2 < 50' test-----如果第一个或等于100或者第二个域小于50，则打印该行。

$ awk '/^root/,/^mysql/' test----打印以正则表达式root开头的记录到以正则表达式mysql开头的记录范围内的所有记录。如果找到一个新的正则表达式root开头的记录，则继续打印直到下一个以正则表达式mysql开头的记录为止，或到文件末尾。

四、BEGIN&END

对某一列求和awk 'BEGIN{sum=0} {sum=$9+sum} END {print "sum " sum}' data 
                        awk '{num+=$1} END {print unm}'
求平均数 awk 'BEGIN{sum=0} {sum=$9+sum} END {print "average " sum / FNR}' access_log 
从第6个字段开始输出awk '{for(i=6; i<NF; ++i){printf "%s,",$i};printf "%s\n", $NF}' access_log
从第6个字段开始输出awk '{for(i=6; i= 6 && NR <=100) printf "%d: %s\n", NR, $7}' access_log

五、IF条件和循环
awk '{if ($NF>3) print $0}'   xxx.txt   最后一列大于3行打印
awk '{if (length($2)>3) print $0}'  xxx.txt       打印第二个字段长度大于3的行
$awk '{if($2 ~ /^baidu$/) print $0}'  xxx.txt  打印第二列为百度的字段  $awk '$2 ~ /^baidu$/ {print $0}' xxx.txt 
awk有三种循环:while循环；for循环；special for循环。
$ awk '{ i = 1; while ( i <= NF ) { print NF,$i; i++}}' test。变量的初始值为1，若i小于可等于NF(记录中域的个数),则执行打印语句，且i增加1。直到i的值大于NF.
$ awk '{for (i = 1; i<NF; i++) print NF,$i}' test。作用同上。
breadkcontinue语句。break用于在满足条件的情况下跳出循环；continue用于在满足条件的情况下忽略后面的语句，直接返回循环的顶端。如：
{for ( x=3; x<=NF; x++) 
            if ($x<0){print "Bottomed out!"; break}}
{for ( x=3; x<=NF; x++)
            if ($x==0){print "Get next item"; continue}}
next语句从输入文件中读取一行，然后从头开始执行awk脚本。如：
{if ($1 ~/test/){next}
    else {print}
}

六、数组
awk '{a[$（NF-2）]++}END{for(i in a) print i,a[i]}'   xxx.txt       统计倒数第2个字段出现的次数（字段相同+1）

七、变量
$ awk '$1 ~/test/{count = $2 + $3; print count}' test,上式的作用是,awk先扫描第一个域，一旦test匹配，就把第二个域的值加上第三个域的值，并把结果赋值给变量count，最后打印出来。
awk可以在命令行中给变量赋值，然后将这个变量传输给awk脚本。如$ awk -F: -f awkscript month=4 year=2004 test，上式的month和year都是自定义变量，分别被赋值为4和2004。在awk脚本中，这些变量使用起来就象是在脚本中建立的一样。注意，如 果参数前面出现test，那么在BEGIN语句中的变量就不能被使用。
域变量也可被赋值和修改，如$ awk '{$2 = 100 + $1; print }' test,上式表示，如果第二个域不存在，awk将计算表达式100加$1的值，并将其赋值给$2，如果第二个域存在，则用表达式的值覆盖$2原来的值。 再例如：$ awk '$1 == "root"{$1 ="test";print}' test，如果第一个域的值是“root”，则把它赋值为“test”，注意，字符串一定要用双引号。
内建变量的使用。变量列表在前面已列出，现在举个例子说明一下。$ awk -F: '{IGNORECASE=1; $1 == "MARY"{print NR,$1,$2,$NF}'test，把IGNORECASE设为1代表忽略大小写，打印第一个域是mary的记录数、第一个域、第二个域和最后一个 域。

八、重定向和管道
$ awk 'BEGIN{ "date" | getline d; print d}' test。执行linux的date命令，并通过管道输出给getline，然后再把输出赋值给自定义变量d，并打印它。
$ awk 'BEGIN{"date" | getline d; split(d,mon); print mon[2]}' test。执行shell的date命令，并通过管道输出给getline，然后getline从管道中读取并将输入赋值给d，split函数把变量d转化成数组mon，然后打印数组mon的第二个元素。
$ awk 'BEGIN{while( "ls" | getline) print}'，命令ls的输出传递给geline作为输入，循环使getline从ls的输出中读取一行，并把它打印到屏幕。这里没有输入文件，因为BEGIN块在打开输入文件前执行，所以可以忽略输入文件。

$ awk 'BEGIN{printf "What is your name?"; getline name < "/dev/tty" } $1 ~name {print "Found" name on line ", NR "."} END{print "See you," name "."} test。在屏幕上打印”What is your name?",并等待用户应答。当一行输入完毕后，getline函数从终端接收该行输入，并把它储存在自定义变量name中。如果第一个域匹配变量name的值，print函数就被执行，END块打印See you和name的值。
$ awk 'BEGIN{while (getline < "/etc/passwd" > 0) lc++; print lc}'。awk将逐行读取文件/etc/passwd的内容，在到达文件末尾前，计数器lc一直增加，当到末尾时，打印lc的值。注意，如果文件不存在，getline返回-1，如果到达文件的末尾就返回0，如果读到一行，就返回1，所以命令 while (getline < "/etc/passwd")在文件不存在的情况下将陷入无限循环，因为返回-1表示逻辑真。
可以在awk中打开一个管道，且同一时刻只能有一个管道存在。通过close()可关闭管道。如：$ awk '{print $1, $2 | "sort" }' test END {close("sort")}。awd把print语句的输出通过管道作为linux命令sort的输入,END块执行关闭管道操作。
system函数可以在awk中执行linux的命令。如：$ awk 'BEGIN{system("clear")'。

1.3.2 打印指定的匹配行数
l -m NUM, --max-count=NUM
在找到NUM个匹配的行之后，不再读这个文件。如果输入是来自一个普通文件的标准输入，并且已经输出了NUM个匹配的行，grep保证标准输入被定位于退出时的最后一次匹配的行之后，不管是否指定了要输出紧随的下文的行。grep在NUM个匹配的行之后停止，它会输出任何紧随的下文的行。
1.3.3 递归模式匹配查找
l -R/-r 递归地读每一目录下的所有文件；
l -R/-r --include=PATTERN 仅仅在搜索匹配PATTERN的文件时在目录中递归搜索；
l -R/-r --exclude=PATTERN 在目录中递归搜索，但是跳过匹配PATTERN的文件。
示例：grep忽略svn文件
1、使用管道进行双层“过滤”，其中第二次grep使用了-v选项，即逆向匹配，打印出不匹配的行,如下：
$grep -r 'function_name' * | grep -v '.svn'
2、直接使用--exclude-dir选项，即指定排除目录，注意svn前的 \.如下：
$grep -r --exclude =\.svn 'function_name' *
1.3.3.1 颜色标识查找结果
l --color：将找到的字符串以特殊颜色显示
```

### 例子

丢弃第一列数据，保留剩下的列

```
awk -F " " '{\
   idx = 2
   for (i=2;i<NF;++i) 
   { 
     printf $i
     if($idx=1)
        printf " "
     else
       idx = 1
   }
   printf "\r\n" 
  }' $1 > clean.txt
```

在shell中输入：./data_clean.sh test.txt

## 2 sed总结

### 1 数据的搜寻并替换

```bash
sed 's/要被取代的字串/新的字串/g'
```



