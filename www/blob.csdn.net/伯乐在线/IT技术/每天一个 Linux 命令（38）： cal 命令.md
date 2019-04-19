# 每天一个 Linux 命令（38）： cal 命令 - 文章 - 伯乐在线
原文出处： [peida](http://www.cnblogs.com/peida/archive/2012/12/14/2817473.html)
cal命令可以用来显示公历（阳历）日历。公历是现在国际通用的历法，又称格列历，通称阳历。“阳历”又名“太阳历”，系以地球绕行太阳一周为一年，为西方各国所通用，故又名“西历”。
1．命令格式：

Shell
```
cal [参数][月份][年份]
```
2．命令功能：
用于查看日历等时间信息，如只有一个参数，则表示年份(1-9999)，如有两个参数，则表示月份和年份
3．命令参数：

Shell
```
-1 显示一个月的月历
-3 显示系统前一个月，当前月，下一个月的月历
-s  显示星期天为一个星期的第一天，默认的格式
-m 显示星期一为一个星期的第一天
-j  显示在当年中的第几天（一年日期按天算，从1月1号算起，默认显示当前月在一年中的天数）
-y  显示当前年份的日历
```
4．使用实例：
实例1：显示当前月份日历
命令：

Shell
```
cal
```
输出：

Shell
```
[root@localhost ~]# cal
   十二月 2012     
日 一 二 三 四 五 六
                   1
 2  3  4  5  6  7  8
 9 10 11 12 13 14 15
16 17 18 19 20 21 22
23 24 25 26 27 28 29
30 31[root@localhost ~]#
```
实例2：显示指定月份的日历
命令：

Shell
```
cal 9 2012
```
输出：

Shell
```
[root@localhost ~]# cal 9 2012
     九月 2012      
日 一 二 三 四 五 六
                   1
 2  3  4  5  6  7  8
 9 10 11 12 13 14 15
16 17 18 19 20 21 22
23 24 25 26 27 28 29
30
```
实例3：显示2013年日历
命令：

Shell
```
cal -y 2013 
cal 2013
```
输出：
![2013cal](http://jbcdn2.b0.upaiyun.com/2017/01/e8436c8c5c60a705140ed9e9a3822a1d.png)
实例4：显示自1月1日的天数
命令：

Shell
```
cal -j
```
输出：

Shell
```
[root@localhost ~]# cal -j
        十二月 2012        
  日   一   二   三   四   五   六
                        336
337 338 339 340 341 342 343
344 345 346 347 348 349 350
351 352 353 354 355 356 357
358 359 360 361 362 363 364
365 366[root@localhost ~]#
```
实例5：星期一显示在第一列
命令：

Shell
```
cal -m
```
输出：

Shell
```
[root@localhost ~]# cal -m
    十二月 2012     
一 二 三 四 五 六 日
                1  2
 3  4  5  6  7  8  9
10 11 12 13 14 15 16
17 18 19 20 21 22 23
24 25 26 27 28 29 30
31[root@localhost ~]#
```
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
[每天一个 Linux 命令（37）： date 命令](http://blog.jobbole.com/109881/)
