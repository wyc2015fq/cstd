
# Linux下Shell的计时 - Augusdi的专栏 - CSDN博客


2016年08月28日 22:43:46[Augusdi](https://me.csdn.net/Augusdi)阅读数：7409


data命令式linux下的时间显示命令，不是time命令，time命令是对一个command的计时命令：
man date
…
%m month (01..12)
%M minute (00..59)
%n a newline
**%N nanoseconds (000000000..999999999)**
%p locale’s equivalent of either AM or PM; blank if not known
%P like %p, but lower case
%r locale’s 12-hour clock time (e.g., 11:11:04 PM)
%R 24-hour hour and minute; same as %H:%M
**%s seconds since 1970-01-01 00:00:00 UTC**
%S second (00..60)
…
其中，%n表示纳秒数，%s表示是由1970以来的秒数，1毫秒 等于 1000 * 1000 纳秒，1000微秒
start_tm=`date +%s%N`;
sleep 10;
end_tm=`date +%s%N`;
use_tm=`echo $end_tm $start_tm | awk '{ print ($1 - $2) / 1000000000}'`
echo $use_tm



