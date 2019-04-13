
# linux 实用命令 —— cut、tr、sort、touch - Zhang's Wikipedia - CSDN博客


2018年05月17日 22:33:07[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：293



## 0. cut
> cut：remove sections from each line of files。
注意是获取每一行的指定区域；
-c
-c, --characters character-list：只输出在区间character-list内的每个TAB和backspace都占一个字符。
$ cat example
test2
this is test1
$ cut -c1-6 example \#\# print 开头算起前 6 个字元 从1开始计数
test2
this i
-d与-f
-d, --delimiter delim：指定在-f参数中的field-list的分割符（为delim中的第一个字符，缺省为TAB）。
-f, --fields field-list：只输出在field-list内的域。
$ echo 12:00:01 | cut -d ':' -f 1
12
$ echo 12:00:01 | cut -d ':' -f 2
00
$ echo 12:00:01 | cut -d ':' -f 3
01
## 1. tr：字符替换，sed 的精简版
[:alnum:]
tr -cd ‘[:alnum:]’-d：delete；-c：complement；-cd：删除后边的参数以外的；

[:class:]
[:alpah:]：字母，[:digit:]：数字；[:xdigit:]：十六进制字符
[:alnum:]：字母和数字
[:cntrl:]：控制（非打印）字符；[:punct:]：标点；[:print:]：可打印字符
[:space:]：空白字符，不限于空格；
[:lower:]：小写字母，[:upper:]：大写字母tr '[:lower:]' '[:upper:]'：小写转换为大写
tr '[:upper:]' '[:lower:]'：大写转换为小写
uname -s | tr '[:upper:]' '[:lower:]'

## 2. sort
去重
sort -u：排序并去重
sort | uniq -c：排序去重并统计次数；
3. touch
touch - change file timestamps.
touch 最本质的功能是改变文件的时间戳。
常见参数
-a   或–time=atime或–time=access或–time=use 　只更改存取时间。
-c   或–no-create 　不建立任何文档。
-d 　使用指定的日期时间，而非现在的时间。
-f 　此参数将忽略不予处理，仅负责解决BSD版本touch指令的兼容性问题。
-m   或–time=mtime或–time=modify 　只更改变动时间。
-r 　把指定文档或目录的日期时间，统统设成和参考文档或目录的日期时间相同。
-t 　使用指定的日期时间，而非现在的时间。
创建不存在的文件
touch log2012.log log2013.log
更新log.log的时间和log2012.log时间戳相同
touch -r log.log log2012.log
设定文件的时间戳
touch -t 201211142234.50 log.log-t  time 使用指定的时间值 time 作为指定文件相应时间戳记的新值．此处的 time规定为如下形式的十进制数:
[[CC]YY]MMDDhhmm[.SS]这里，CC为年数中的前两位，即”世纪数”；YY为年数的后两位，即某世纪中的年数．如果不给出CC的值，则touch   将把年数CCYY限定在1969–2068之内．MM为月数，DD为天将把年数CCYY限定在1969–2068之内．MM为月数，DD为天数，hh 为小时数(几点)，mm为分钟数，SS为秒数．此处秒的设定范围是0–61，这样可以处理闰秒．这些数字组成的时间是环境变量TZ指定的时区中的一个时 间．由于系统的限制，早于1970年1月1日的时间是错误的。


