# 每天一个 Linux 命令（37）： date 命令 - 文章 - 伯乐在线
原文出处： [peida](http://www.cnblogs.com/peida/archive/2012/12/13/2815687.html)
在linux环境中，不管是编程还是其他维护，时间是必不可少的，也经常会用到时间的运算，熟练运用date命令来表示自己想要表示的时间，肯定可以给自己的工作带来诸多方便。
1．命令格式：

Shell
```
date [参数]... [+格式]
```
2．命令功能：
date 可以用来显示或设定系统的日期与时间。
3．命令参数：
必要参数:

Shell
```
%H 小时(以00-23来表示)。 
%I 小时(以01-12来表示)。 
%K 小时(以0-23来表示)。 
%l 小时(以0-12来表示)。 
%M 分钟(以00-59来表示)。 
%P AM或PM。 
%r 时间(含时分秒，小时以12小时AM/PM来表示)。 
%s 总秒数。起算时间为1970-01-01 00:00:00 UTC。 
%S 秒(以本地的惯用法来表示)。 
%T 时间(含时分秒，小时以24小时制来表示)。 
%X 时间(以本地的惯用法来表示)。 
%Z 市区。 
%a 星期的缩写。 
%A 星期的完整名称。 
%b 月份英文名的缩写。 
%B 月份的完整英文名称。 
%c 日期与时间。只输入date指令也会显示同样的结果。 
%d 日期(以01-31来表示)。 
%D 日期(含年月日)。 
%j 该年中的第几天。 
%m 月份(以01-12来表示)。 
%U 该年中的周数。 
%w 该周的天数，0代表周日，1代表周一，异词类推。 
%x 日期(以本地的惯用法来表示)。 
%y 年份(以00-99来表示)。 
%Y 年份(以四位数来表示)。 
%n 在显示时，插入新的一行。 
%t 在显示时，插入tab。 
MM 月份(必要) 
DD 日期(必要) 
hh 小时(必要) 
mm 分钟(必要)
ss 秒(选择性)
```
选择参数:

Shell
```
-d<字符串> 　显示字符串所指的日期与时间。字符串前后必须加上双引号。 
-s<字符串> 　根据字符串来设置日期与时间。字符串前后必须加上双引号。 
-u 　显示GMT。 
--help 　在线帮助。 
--version 　显示版本信息
```
4．使用说明：
1.在显示方面，使用者可以设定欲显示的格式，格式设定为一个加号后接数个标记，其中可用的标记列表如下: % :  打印出 %：

Shell
```
%n : 下一行
%t : 跳格
%H : 小时(00..23)
%I : 小时(01..12)
%k : 小时(0..23)
%l : 小时(1..12)
%M : 分钟(00..59)
%p : 显示本地 AM 或 PM
%r : 直接显示时间 (12 小时制，格式为 hh:mm:ss [AP]M)
%s : 从 1970 年 1 月 1 日 00:00:00 UTC 到目前为止的秒数
%S : 秒(00..61)
%T : 直接显示时间 (24 小时制)
%X : 相当于 %H:%M:%S
%Z : 显示时区 %a : 星期几 (Sun..Sat)
%A : 星期几 (Sunday..Saturday)
%b : 月份 (Jan..Dec)
%B : 月份 (January..December)
%c : 直接显示日期与时间
%d : 日 (01..31)
%D : 直接显示日期 (mm/dd/yy)
%h : 同 %b
%j : 一年中的第几天 (001..366)
%m : 月份 (01..12)
%U : 一年中的第几周 (00..53) (以 Sunday 为一周的第一天的情形)
%w : 一周中的第几天 (0..6)
%W : 一年中的第几周 (00..53) (以 Monday 为一周的第一天的情形)
%x : 直接显示日期 (mm/dd/yy)
%y : 年份的最后两位数字 (00.99)
%Y : 完整年份 (0000..9999)
```
2.在设定时间方面：

Shell
```
date -s //设置当前时间，只有root权限才能设置，其他只能查看。
date -s 20080523 //设置成20080523，这样会把具体时间设置成空00:00:00
date -s 01:01:01 //设置具体时间，不会对日期做更改
date -s “01:01:01 2008-05-23″ //这样可以设置全部时间
date -s “01:01:01 20080523″ //这样可以设置全部时间
date -s “2008-05-23 01:01:01″ //这样可以设置全部时间
date -s “20080523 01:01:01″ //这样可以设置全部时间
```
3.加减：

Shell
```
date +%Y%m%d         //显示前天年月日
date +%Y%m%d --date="+1 day"  //显示前一天的日期
date +%Y%m%d --date="-1 day"  //显示后一天的日期
date +%Y%m%d --date="-1 month"  //显示上一月的日期
date +%Y%m%d --date="+1 month"  //显示下一月的日期
date +%Y%m%d --date="-1 year"  //显示前一年的日期
date +%Y%m%d --date="+1 year"  //显示下一年的日期
```
5．使用实例：
实例1：显示当前时间
命令：

Shell
```
date
date '+%c'
date '+%D'
date '+%x'
date '+%T'
date '+%X'
```
输出：

Shell
```
[root@localhost ~]# date
2012年 12月 08日 星期六 08:31:35 CST
[root@localhost ~]# date '+%c'
2012年12月08日 星期六 08时34分44秒
[root@localhost ~]# date '+%D'
12/08/12[root@localhost ~]# date '+%x'
2012年12月08日
[root@localhost ~]# date '+%T'
08:35:36[root@localhost ~]# date '+%X'
08时35分54秒
[root@localhost ~]#
```
说明：
实例2：显示日期和设定时间
命令：

Shell
```
date --date 08:42:00
```
输出：

Shell
```
[root@localhost ~]# date '+%c'
2012年12月08日 星期六 08时41分37秒
[root@localhost ~]# date --date 08:42:00
2012年 12月 08日 星期六 08:42:00 CST
[root@localhost ~]# date '+%c' --date 08:45:00
2012年12月08日 星期六 08时45分00秒
[root@localhost ~]#
```
说明：
实例3：date -d参数使用
命令：
输出：

Shell
```
[root@localhost ~]# date -d "nov 22"
2012年 11月 22日 星期四 00:00:00 CST
[root@localhost ~]# date -d '2 weeks'
2012年 12月 22日 星期六 08:50:21 CST
[root@localhost ~]# date -d 'next monday'
2012年 12月 10日 星期一 00:00:00 CST
[root@localhost ~]#  date -d next-day +%Y%m%d
20121209[root@localhost ~]# date -d tomorrow +%Y%m%d
20121209[root@localhost ~]# date -d last-day +%Y%m%d
20121207[root@localhost ~]# date -d yesterday +%Y%m%d
20121207[root@localhost ~]# date -d last-month +%Y%m
201211[root@localhost ~]# date -d next-month +%Y%m
201301[root@localhost ~]# date -d '30 days ago' 
2012年 11月 08日 星期四 08:51:37 CST
[root@localhost ~]# date -d '-100 days' 
2012年 08月 30日 星期四 08:52:03 CST
[root@localhost ~]#  date -d 'dec 14 -2 weeks'
2012年 11月 30日 星期五 00:00:00 CST
[root@localhost ~]# date -d '50 days'
2013年 01月 27日 星期日 08:52:27 CST
```
说明：
date 命令的另一个扩展是 -d 选项，该选项非常有用。使用这个功能强大的选项，通过将日期作为引号括起来的参数提供，您可以快速地查明一个特定的日期。-d 选项还可以告诉您，相对于当前日期若干天的究竟是哪一天，从现在开始的若干天或若干星期以后，或者以前（过去）。通过将这个相对偏移使用引号括起来，作为 -d 选项的参数，就可以完成这项任务。
具体说明如下：

Shell
```
date -d "nov 22"  今年的 11 月 22 日是星期三
date -d '2 weeks' 2周后的日期
date -d 'next monday' (下周一的日期)
date -d next-day +%Y%m%d（明天的日期）或者：date -d tomorrow +%Y%m%d
date -d last-day +%Y%m%d(昨天的日期) 或者：date -d yesterday +%Y%m%d
date -d last-month +%Y%m(上个月是几月)
date -d next-month +%Y%m(下个月是几月)
使用 ago 指令，您可以得到过去的日期：
date -d '30 days ago' （30天前的日期）
```
使用负数以得到相反的日期：

Shell
```
date -d 'dec 14 -2 weeks' （相对:dec 14这个日期的两周前的日期）
date -d '-100 days' (100天以前的日期)
date -d '50 days'(50天后的日期)
```
实例4：显示月份和日数
命令：

Shell
```
date  '+%B %d'
```
输出：

Shell
```
[root@localhost ~]# date  '+%B %d' 
十二月 08[root@localhost ~]#
```
说明：
实例5：显示时间后跳行，再显示目前日期
命令：

Shell
```
date '+%T%n%D'
```
输出：

Shell
```
[root@localhost ~]# date '+%T%n%D'
09:00:30
12/08/12[root@localhost ~]#
```
说明：
### 本系列文章：
[每天一个 Linux 命令（1）：ls命令](http://blog.jobbole.com/108531/)
[每天一个 Linux 命令（2）：cd命令](http://blog.jobbole.com/108805/)
[每天一个 Linux 命令（3）：pwd命令](http://blog.jobbole.com/108855/)
[每天一个 Linux 命令（4）：mkdir命令](http://blog.jobbole.com/108871/)
[每天一个 Linux 命令（5）：rm 命令](http://blog.jobbole.com/108901/)
[每天一个 Linux 命令（6）：rmdir 命令](http://blog.jobbole.com/108925/)
[每天一个 Linux 命令（7）：mv命令](http://blog.jobbole.com/108983/)
[每天一个 Linux 命令（8）：cp 命令](http://blog.jobbole.com/108988/)
[每天一个 Linux 命令（9）：touch 命令](http://blog.jobbole.com/108989/)
[每天一个 Linux 命令（10）：cat 命令](http://blog.jobbole.com/108990/)
[每天一个 Linux 命令（11）：nl 命令](http://blog.jobbole.com/108991/)
[每天一个 Linux 命令（12）：more 命令](http://blog.jobbole.com/108992/)
[每天一个 Linux 命令（13）：less 命令](http://blog.jobbole.com/108993/)
[每天一个 Linux 命令（14）：head 命令](http://blog.jobbole.com/108994/)
[每天一个 Linux 命令（15）：tail 命令](http://blog.jobbole.com/108995/)
[每天一个 Linux 命令（16）：which命令](http://blog.jobbole.com/109262/)
[每天一个 Linux 命令（17）：whereis 命令](http://blog.jobbole.com/108993/)
[每天一个 Linux 命令（18）：locate 命令](http://blog.jobbole.com/108994/)
[每天一个 Linux 命令（19）：find 命令概览](http://blog.jobbole.com/109265/)
[每天一个 Linux 命令（20）：find命令之exec](http://blog.jobbole.com/108996/)
[每天一个 Linux 命令（21）：find命令之xargs](http://blog.jobbole.com/108997/)
[每天一个 Linux 命令（22）：find 命令的参数详解](http://blog.jobbole.com/109268/)
[每天一个 Linux 命令（23）：Linux 目录结构](http://blog.jobbole.com/109506/)
[每天一个 Linux 命令（24）：Linux 文件类型与扩展名](http://blog.jobbole.com/109254/)
[每天一个 Linux 命令（25）：Linux 文件属性详解](http://blog.jobbole.com/105269)
[每天一个 Linux 命令（26）：用 SecureCRT 来上传和下载文件](http://blog.jobbole.com/109539)
[每天一个 Linux 命令（27）：linux chmod 命令](http://blog.jobbole.com/109614)
[每天一个 Linux 命令（28）：tar 命令](http://blog.jobbole.com/109674/)
[每天一个 Linux 命令（29）: chgrp 命令](http://blog.jobbole.com/109696/)
[每天一个 Linux 命令（30）: chown 命令](http://blog.jobbole.com/109750/)
[每天一个 Linux 命令（31）: /etc/group 文件详解](http://blog.jobbole.com/109781/)
[每天一个 Linux 命令（32）：gzip 命令](http://blog.jobbole.com/109797/)
[每天一个 Linux 命令（33）：df 命令](http://blog.jobbole.com/109827/)
[每天一个 Linux 命令（34）： du 命令](http://blog.jobbole.com/109846/)
[每天一个 Linux 命令（35）： ln 命令](http://blog.jobbole.com/109869/)
[每天一个 Linux 命令（36）： diff 命令](http://blog.jobbole.com/109875/)
