# sed&awk - xcw0754 - 博客园
# [sed&awk](https://www.cnblogs.com/xcw0754/p/10046413.html)
我的理解，sed是匹配&替换，awk是匹配&过滤。这`匹配`的意思就是模式串的匹配，这`替换`就是堆匹配到的串进行替换操作，这`过滤`就是筛选出指定的字段出来。
手册把sed称为stream editor，即流编辑器，重点是流。而awk则被称为pattern scanning and text processing language，即模式搜索与文本处理的语言，记住它是门语言。
### awk
awk命令模式:
`awk 'pattern { action }' input`
pattern字段是可选的，用于执行逻辑判断。action字段必要的，用于操作，支持的内容就多了，可以打印，根据参数作运算等等。默认是根据空格或`\t`来分列的。
测试输入样本:
```
Beth    4.00    0
Dan 3.75    0
kathy   4.00    10
Mark    5.00    20
Mary    5.50    22
Susie   4.25    18
```
打印首列:
`awk '{print $1}'  input`
支持数字(整型、浮点型之间相互)比较:
`awk '$3 <= 0 {print $1}'  input`
支持字符串比较:
`awk '$1 == "Mary" {print $1}'  input`
支持`||`和`&&`:
```
awk '$3 <= 0 || $3 >= 22 {print $1}'  input
awk '!($3 >= 0 && $3 < 22) {print $1}'  input
```
`pattern`字段支持运算:
`awk '$3 * $3 > 9 {print $1}'  input`
以两个`/`括起来的`pattern`支持正则:
`awk '/Ma.k/ {print $1}'  input`
支持逻辑取反符号:
`awk '!/Ma.k/ {print $1}'  input`
支持多个输入文件:
`awk '{print $1}'  input1 input2`
支持从文件读取命令:
`awk  -f executefile  input`
打印整行:
```
awk '{print $0}'  input
awk '{print}'  input
```
不清楚有多少列，打印最后一列:
`awk '{print $NF}'  input`
打印列数:
`awk '{print NF}'  input`
支持计算:
`awk '{print  $2 * $2 * $3}'  input`
`,`号会被输出成空格，打印多列需用`,`分隔:
`awk '{print  $2,$3}'  input`
打印行号:
`awk '{print  NR}'  input`
插入常量字符串:
`awk { print "total pay for", $1, "is", $2 * $3 } input`
使用`printf`进行格式化输出:
`awk '{ printf("total pay for %s is %.2f\n", $1, $2 * $3) }'  input`
改变分隔符(-F须放在前面):
```
awk -F ">"   '{print  $3}'  input
awk -F ","   '{print  $3}'  input
```
支持以`[]`和`:`隔开多个分隔符(不能有空格，顺序无关):
`awk -F [",":"|"] '{print $3}' input`
对某一列进行累加
`awk '{sum += $2}END{print sum/107}'  input`
### sed
sed命令模式
`sed  'pattern'  input`
这个pattern没有很直观，都是以指令驱动的，指令可以在前面(如s)，也可以在末尾(如p)。常见的需求就是增/删/查/改，`增`就是在匹配行的前/后n行插入新行，`删`就是删除匹配的行，`查`其实可以用grep替代，`改`就是找出匹配的行进行修改(有上下文关系的，比如匹配多行就比较麻烦了)。
先来了解如何指定输入的行号。
打印文件内容
`sed -n 'p'  input`
打印文件行号(行号是新起一行的)
`sed -n '=;p'  input`
只打印第2行
`sed -n '2p'  input`
打印2~4行
`sed -n '2,4p'  input`
从第4行开始输出到结束
`sed -n '2,$p'  input`
打印2~4行且打印行号
`sed -n '2,4{=;p}'  input`
打印含有abc的行
`sed -n '/abc/p'  input`
打印1~4行中含有Mem的行
`sed -n '/Mem/,{1,4p}'  input`
打印除了1~4行的内容
`sed -n '1,4!p'  input`
打印匹配Mem至匹配Swap的行
···
sed -n '/Mem/,/Swap/p' input
···
简单全局替换(不修改源文件，直接输出到终端)
`sed 's/str1/str2/g' input`
全局替换(修改源文件)
`sed -i 's/str1/str2/g' input`
指定其他分隔符，可避免频繁输出转义符号(下例把`/`替换成`|`)
`sed -i 's:/:|:g' input`
从指定文件中执行命令
`sed  -f cmd  input`
只输出匹配的行(-n搭配p，默认是全输出)
`sed -n '$p' input`
优质文章：[http://blog.jobbole.com/109088/#toc_1](http://blog.jobbole.com/109088/#toc_1)

