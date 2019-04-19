# 你可能不知道的51个Linux经典命令！ - fanyun的博客 - CSDN博客
2016年04月08日 13:50:02[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：6765
所属专栏：[Linux系统开发维护实战](https://blog.csdn.net/column/details/linux-01.html)
**1.删除0字节文件**
代码如下:
find -type f -size 0 -exec rm -rf {} \;
**2.查看进程**
按内存从大到小排列
代码如下:
ps -e -o "%C : %p : %z : %a"|sort-k5 -nr
**3.按cpu利用率从大到小排列**
代码如下:
ps -e -o "%C : %p : %z : %a"|sort-nr
**4.打印说cache里的URL**
代码如下:
grep -r -a jpg /data/cache/* | strings |grep "http:" | awk -F'http:' '{print "http:"$2;}'
**5.查看http的并发请求数及其TCP连接状态：**
代码如下:
netstat -n | awk '/^tcp/ {++S[$NF]} END{for(a in S) print a, S[a]}'
6. 在sed -i'/Root/s/no/yes/' /etc/ssh/sshd_config   sed在sshd_config这个文件里Root的一行，匹配Root一行，将no替换成yes.
# sed -i '/Root/s/no/yes/'/etc/ssh/sshd_config 
**7.如何杀掉mysql进程：**
代码如下:
ps aux |grep mysql |grep -v grep |awk'{print $2}' |xargs kill -9 (从中了解到awk的用途)
killall -TERM mysqld
kill -9 `cat /usr/local/apache2/logs/httpd.pid` 试试查杀进程PID
**8.显示运行3级别开启的服务:**
代码如下:
ls /etc/rc3.d/S* |cut -c 15- (从中了解到cut的用途，截取数据)
**9.如何在编写SHELL显示多个信息，用EOF代码如下:**
cat << EOF
+--------------------------------------------------------------+
| === Welcome to Tunoff services === |
+--------------------------------------------------------------+
EOF
**10.for 的巧用(如给mysql建软链接)：**
代码如下:
cd /usr/local/mysql/bin
for i in *
do ln /usr/local/mysql/bin/$i /usr/bin/$i
done
**11. 取IP地址：**
代码如下:
ifconfig eth0 |grep "inet addr:"|awk '{print $2}'|cut -c 6- 
或者
ifconfig | grep 'inet addr:'| grep -v '127.0.0.1' | cut -d: -f2 | awk '{ print$1}'
**12.内存的大小:**
代码如下:
free -m |grep "Mem" | awk '{print$2}'
**13.查看连接某服务端口最多的的IP地址**：
代码如下:
netstat -an -t | grep ":80" |grep ESTABLISHED | awk '{printf "%s %s\n",$5,$6}' | sort
**14.查看Apache的并发请求数及其TCP连接状态：**
代码如下:
netstat -n | awk '/^tcp/ {++S[$NF]} END{for(a in S) print a, S[a]}'
**15.因为同事要统计一下服务器下面所有的jpg的文件的大小,写了个shell给他来统计.原来用xargs实现,但他一次处理一部分,搞的有多个总和....,下面的命令就能解决：**
代码如下:
find / -name *.jpg -exec wc -c {} \;|awk'{print $1}'|awk '{a+=$1}END{print a}'
CPU的数量（多核算多个CPU，cat /proc/cpuinfo |grep -c processor）越多，系统负载越低，每秒能处理的请求数也越多。
**16.CPU负载   ：**
# cat /proc/loadavg
检查前三个输出值是否超过了系统逻辑CPU的4倍。  
**18.CPU负载  ：**
 #mpstat 1 1
检查%idle是否过低(比如小于5%)
**19.内存空间 **：
 #free
检查free值是否过低   也可以用 # cat /proc/meminfo
**20.swap空间 ：  **
# free
检查swap used值是否过高   如果swap used值过高，进一步检查swap动作是否频繁：
# vmstat 1 5
观察si和so值是否较大
**21.磁盘空间  ：**
# df -h
检查是否有分区使用率(Use%)过高(比如超过90%)   如发现某个分区空间接近用尽，可以进入该分区的挂载点，用以下命令找出占用空间最多的文件或目录：
代码如下:
# du -cks * | sort -rn | head -n 10
**22.磁盘I/O负载  # iostat -x 1 2**：
检查I/O使用率(%util)是否超过100%
rrqm/s队列中每秒钟合并的读请求数量 
wrqm/s队列中每秒钟合并的写请求数量 
r/s每秒钟完成的读请求数量 
w/s每秒钟完成的写请求数量 
rsec/s每秒钟读取的扇区数量 
wsec/s每秒钟写入的扇区数量 
avgrq-sz平均请求扇区的大小 
avgqu-sz平均请求队列的长度 
await平均每次请求的等待时间 
svctm平均每次请求的服务时间 
util设备的利用率
**23.网络负载：  **
 #sar -n DEV
检查网络流量(rxbyt/s, txbyt/s)是否过高
**24.网络错误 ：  **
# netstat -i
检查是否有网络错误(drop fifo colls carrier)   也可以用命令：# cat/proc/net/dev
**25.网络连接数目 ：**
# netstat -an | grep -E “^(tcp)” | cut -c68- | sort | uniq -c | sort -n
**26.进程总数  ：**
 # psaux | wc -l
检查进程个数是否正常 (比如超过250)
**27.可运行进程数目 ： **
# vmwtat 1 5
列给出的是可运行进程的数目，检查其是否超过系统逻辑CPU的4倍
**28.进程  ： **
# top -id 1
观察是否有异常进程出现
**29.网络状态   检查DNS, 网关等是否可以正常连通：**
**30.用户：   **
# who | wc -l
检查登录用户是否过多 (比如超过50个)   也可以用命令：# uptime
**31.系统日志 ： **
 #cat /var/log/rflogview/*errors
检查是否有异常错误记录   也可以搜寻一些异常关键字，例如：
代码如下:
# grep -i error /var/log/messages
# grep -i fail /var/log/messages
**32.核心日志  ：**
 # dmesg
检查是否有异常错误记录
**33.系统时间：   **
# date
检查系统时间是否正确
**34.打开文件数目：   **
# lsof | wc -l
检查打开文件总数是否过多
**35.日志：   **
# logwatch –print   配置/etc/log.d/logwatch.conf，将 Mailto 设置为自己的email 地址，启动mail服务(sendmail或者postfix)，这样就可以每天收到日志报告了。
缺省logwatch只报告昨天的日志，可以用# logwatch –print –range all 获得所有的日志分析结果。
可以用# logwatch –print –detail high 获得更具体的日志分析结果(而不仅仅是出错日志)。
**36.杀掉80端口相关的进程：**
代码如下:
lsof -i :80|grep -v "PID"|awk'{print "kill -9",$2}'|sh
**37.清除僵死进程：**
代码如下:
ps -eal | awk '{ if ($2 == "Z"){print $4}}' | kill -9
**38.tcpdump抓包 ，用来防止80端口被人攻击时可以分析数据：**
代码如下:
# tcpdump -c 10000 -i eth0 -n dst port 80> /root/pkts
**39.然后检查IP的重复数 并从小到大排序 注意 "-t\ +0"   中间是两个空格：**
代码如下:
# less pkts | awk {'printf$3"\n"'} | cut -d. -f 1-4 | sort | uniq -c | awk {'printf $1""$2"\n"'} | sort -n -t\ +0
**40.查看有多少个活动的php-cgi进程：**
代码如下:
netstat -anp | grep php-cgi | grep ^tcp |wc -l
chkconfig --list | awk '{if ($5=="3:on") print $1}'
**41.kudzu查看网卡型号：**
代码如下:
kudzu --probe --class=network
**42.常用正则表达式：**
匹配中文字符的正则表达式： [\u4e00-\u9fa5]
评注：匹配中文还真是个头疼的事，有了这个表达式就好办了
匹配双字节字符(包括汉字在内)：[^\x00-\xff]
评注：可以用来计算字符串的长度（一个双字节字符长度计2，ASCII字符计1）
匹配空白行的正则表达式：\n\s*\r
评注：可以用来删除空白行
匹配HTML标记的正则表达式：<(\S*?)[^>]*>.*?</\1>|<.*? />
评注：网上流传的版本太糟糕，上面这个也仅仅能匹配部分，对于复杂的嵌套标记依旧无能为力
匹配首尾空白字符的正则表达式：^\s*|\s*$
评注：可以用来删除行首行尾的空白字符(包括空格、制表符、换页符等等)，非常有用的表达式
匹配Email地址的正则表达式：\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*
评注：表单验证时很实用
匹配网址URL的正则表达式：[a-zA-z]+://[^\s]*
评注：网上流传的版本功能很有限，上面这个基本可以满足需求
匹配帐号是否合法(字母开头，允许5-16字节，允许字母数字下划线)：^[a-zA-Z][a-zA-Z0-9_]{4,15}$
评注：表单验证时很实用
匹配国内电话号码：\d{3}-\d{8}|\d{4}-\d{7}
评注：匹配形式如 0511-4405222 或 021-87888822
匹配腾讯QQ号：[1-9][0-9]{4,}
评注：腾讯QQ号从10000开始
匹配中国邮政编码：[1-9]\d{5}(?!\d)
评注：中国邮政编码为6位数字
匹配身份证：\d{15}|\d{18}
评注：中国的身份证为15位或18位
匹配ip地址：\d+\.\d+\.\d+\.\d+
评注：提取ip地址时有用
匹配特定数字：
代码如下:
^[1-9]\d*$　 　 //匹配正整数
^-[1-9]\d*$ 　 //匹配负整数
^-?[1-9]\d*$　　 //匹配整数
^[1-9]\d*|0$　 //匹配非负整数（正整数 + 0）
^-[1-9]\d*|0$　　 //匹配非正整数（负整数 + 0）
^[1-9]\d*\.\d*|0\.\d*[1-9]\d*$　　 //匹配正浮点数
^-([1-9]\d*\.\d*|0\.\d*[1-9]\d*)$　 //匹配负浮点数
^-?([1-9]\d*\.\d*|0\.\d*[1-9]\d*|0?\.0+|0)$　 //匹配浮点数
^[1-9]\d*\.\d*|0\.\d*[1-9]\d*|0?\.0+|0$　　 //匹配非负浮点数（正浮点数+ 0）
^(-([1-9]\d*\.\d*|0\.\d*[1-9]\d*))|0?\.0+|0$　　//匹配非正浮点数（负浮点数+ 0）
评注：处理大量数据时有用，具体应用时注意修正
匹配特定字符串：
代码如下:
^[A-Za-z]+$　　//匹配由26个英文字母组成的字符串
^[A-Z]+$　　//匹配由26个英文字母的大写组成的字符串
^[a-z]+$　　//匹配由26个英文字母的小写组成的字符串
^[A-Za-z0-9]+$　　//匹配由数字和26个英文字母组成的字符串
^\w+$　　//匹配由数字、26个英文字母或者下划线组成的字符串
**43.     有文件file:**
a)   查询file里面空行的所在行号
awk‘{if($0~/^$/)printNR}’ file
or
grep -n ^$ file |awk ‘BEGIN{FS=”:”}{print $1}’
b)   查询file以abc结尾的行
grep abc$ file
c)   打印出file文件第1到第3行
sed -n ’1,3p’ file
head -3 file
**44.     如何将本地80端口的请求转发到8080端口，当前主机IP为192.168.2.1:**
-APREROUTING -d 124.42.60.109 -p tcp -m tcp –dport 80 -j DNAT–to-destination 10.0.0.18:9000
**45.     crontab:**
在11月份内，每天的早上6点到12点中，每隔2小时执行一次/usr/bin/httpd.sh怎么实现
0 6-12/2 * 11*/usr/bin/httpd.sh
**46.     编写个shell脚本将/usr/local/test 目录下大于100K的文件转移到/tmp目录下:**
#!/bin/bash
for file in `ls /root`
do
       if [ -f $file ]; then
             if[ `ls -l $file|awk '{print $5}'` -gt 10000 ];  then
                    mv$file /tmp/
             fi
       fi
done
**47.     read 命令5秒后自动退出 :**
[root@localhost bin]# read -t 5 
**48.     自动ftp上传 :**
#!/bin/sh 
ftp -n<<END_FTP 
open 192.168.1.4 
user codfei duibuqi //用户名codfei 密码duibuqi 
binary 
prompt off //关闭提示
mput test //上传test 
close 
bye 
END_FTP 
自动ssh登陆从A到B然后再到c 
#!/usr/bin/expect -f 
set timeout 30 
spawn ssh codfei@B 
expect "password:" 
send "pppppp/r" 
expect "]*" 
send "ssh codfei@C/r" 
expect "password:" 
send "pppppp/r" 
interact 
#打印第一个域
[root@localhost bin]# cat 3 
eqeqedadasdD 
eqeqdadfdfDD 
fdsfdsfQWEDD 
DSADASDSADSA 
[root@localhost bin]# 
[root@localhost bin]# 
[root@localhost bin]# awk -F "" '{print $1}' 3 
e 
e 
f 
D 
实现字符串翻转
[root@localhost bin]# cat 8 
qweqewqedadaddas 
[root@localhost bin]# rev 8 
saddadadeqweqewq 
**49.利用iptables对应简单攻击**
netstat -an | grep -v LISTEN | awk ‗{print$5}‘ |grep -v 127.0.0.1|grep -v 本机ip|sed 
―s/::ffff://g‖|awk ‗BEGIN { FS=‖:‖ } { Num[$1]++ } END { for(i in Num) if(Num>8)
{ print i} }’ |grep‘[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}’|  xargs -i[]
iptables -I INPUT -s [] -j DROP 
Num>8
部分设定值为阀值这条句子会自动将
netstat -an 中查到的来自同一的超过一定量的连接的列入禁止范围。本机
ip
改成你的服务器的
ip
地址
选择性的删除某些行
# 删除所有空白行 类似于 "grep '.'"
awk NF
awk '/./'
# 删除重复连续的行 模拟 "uniq"
awk 'a !~ $0; {a=$0}'
# 删除重复的、非连续的行
awk '! a[$0]++'                     # 最简练
awk '!($0 in a) {a[$0];print}'      # 最有效
查询系统状态的指令集
cat 文件名        一屏查看文件内容
more 文件名        分页查看文件内容
less 文件名        可控分页查看文件内容
grep -l -r 字符串 路径    显示包含字符串的文件名
grep -L -r 字符串 路径    显示不包含字符串的文件名
lsof -p           进程号例如lsof  -p 2428查看进程打开的文件
lsof abc.txt       显示开启文件abc.txt的进程
lsof -i :22        显示22端口现在运行什么程序
lsof -c nsd        显示nsd进程现在打开的文件
nohup 程序 &    在后台运行程序退出登录后并不结束程序
strace -f -F -o outfile <cmd>    详细显示程序的运行信息
arping IP地址        根据IP查网卡地址
nmblookup -A IP地址    根据IP查电脑名
linux删除特殊文件名的文件
    假设Linux系统中有一个文件名叫―-ee‖如果我们想对它进行操作例如要删除
它按照一般的删除方法在命令行中输入rm -ee命令界面会提示我们是―无效选
项‖(invalid option)原来由于文件名的第一个字符为―-‖Linux把文件名当作选项
了我们可以使用―–‖符号来解决这个问题输入―rm — -ee‖命令便可顺利删除名为
―-ee‖的文件。如果是其他特殊字符的话可以在特殊字符前加一个―‖符号或者用双
引号把整个文件名括起来。
    如/usr/lcoal/目录下有个--exclude文件通过命令rm -- --exclude
删除此文件
一句话快速查找PHP木马的方法
find ./ -name "*.php" -type f-print0|xargs -0 egrep 
"(phpspy|c99sh|milw0rm|eval\(base64_decode|eval\(gzinflate\(base64_decode|eval\(gzinf
late\(str_rot13\(base64_decode|spider_bc)"|awk-F: '{print $1}'|sort|uniq
如何删去重复行并保持顺序不变
awk '{ if (!seen[$0]++) { print $0; } }'$file_path
perl -lne 'print unless $seen{$_}++ '$file_path
**50. 查看某文件的一部分**
如果你只想看文件的前5 行可以使用head命令
如head -5 /etc/passwd
如果你想查看文件的后10行可以使用tail命令
如tail -10 /etc/passwd
查看文件中间一段可以使用sed命令
如:sed –n '5,10p' /etc/passwd 
这样你就可以只查看文件的第5行到第10行
**51.将file.txt 里的123改为456方法 1 **
sed 's/123/456/g' file.txt >file.txt.new 修改的保存到其它文件 
sed -i 's/123/456/g' file.txt 直接修改原文件  
方法2 vi file.txt  输入命令 :%s/123/456/g
注意如果替换的文件有特殊符号如/就要用来取消。
例sed -i 's//usr/local/apache2/htdocs//var/www/html/g' 
/usr/local/apache2/conf/httpd.conf 如果只是下原有的行后添加就用& 例
sed -i 's/DirectoryIndex index.htmlindex.html.var/& index.htm index.php /g' /usr/local/apache2/conf/httpd.conf
