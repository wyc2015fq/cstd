# Shell常用命令 - Hxyue2018's Blog - CSDN博客





2018年10月18日 10:14:06[Hxyue2018](https://me.csdn.net/Super_Json)阅读数：28








列出当天访问次数最多的IP

命令：cut -d- -f 1 /usr/local/apache2/logs/access_log |uniq -c | sort -rn | head -20

原理：

       cut

       -d, --delimiter=DELIM

              use DELIM instead of TAB for field delimiter

              表示用-分割，然后-f 1 

       -f, --fields=LIST

              select only these fields;  also print any line that contains  no

              delimiter character, unless the -s option is specified

           表示打印第一部分，就是ip

 uniq 是将重复行去掉， -c表示前面前面加上数目，

       sort -rn 就是按照数字从大到小排序，

       head -20取前面20行


最后打印的结果大概是这样:

   217 192.114.71.13

   116 124.90.132.65

   108 192.114.71.13

   102 194.19.140.96

   101 217.70.34.173

   100 219.235.240.36



以下是其他一些分析日志的shell用法：

1、查看当天有多少个IP访问：
awk '{print $1}' log_file|sort|uniq|wc -l

2、查看某一个页面被访问的次数;
grep "/index.php" log_file | wc -l

3、查看每一个IP访问了多少个页面：
awk '{++S[$1]} END {for (a in S) print a,S[a]}' log_file

4、将每个IP访问的页面数进行从小到大排序：
awk '{++S[$1]} END {for (a in S) print S[a],a}' log_file | sort -n

5、查看某一个IP访问了哪些页面：
grep ^111.111.111.111 log_file| awk '{print $1,$7}'

6、去掉搜索引擎统计当天的页面：
awk '{print $12,$1}' log_file | grep ^\"Mozilla | awk '{print $2}' |sort | uniq | wc -l

7、查看2009年6月21日14时这一个小时内有多少IP访问：
 awk '{print $4,$1}' log_file | grep 21/Jun/2009:14 | awk '{print $2}'| sort | uniq | wc -l


