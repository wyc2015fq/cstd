# FreeBSD下取得昨天日期的方法 - ljx0305的专栏 - CSDN博客
2009年06月18日 11:17:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：762标签：[freebsd																[date																[timezone																[shell																[数据库																[dst](https://so.csdn.net/so/search/s.do?q=dst&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=timezone&t=blog)](https://so.csdn.net/so/search/s.do?q=date&t=blog)](https://so.csdn.net/so/search/s.do?q=freebsd&t=blog)
个人分类：[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)
经常看到有人问如何得到昨天的日期，尤其是备份日志、数据库的时候都需要，我对shell不熟，原来都是发贴问的，好歹算是能满足需要了，现在正在学shell，今天把我现在所知道的方法给总结一下。
方法一：
linux下的date加参数直接就可以得到昨天的日期，FreeBSD也可以的。
date -v -1d +%Y%m%d
将得到昨天的日期，显示年月日
具体可以man date
方法二：
#!/bin/sh
#昨天日期shell版
now=$(date +%Y%m%d%H%M)
echo $now
yesterday=`TZ=GMT+16;export TZ;date +%Y%m%d%H%M`
echo $yesterday
注：此处的16是24-8得来的，8是当前时区与GMT的差值，北京时间是8
这个方法似乎只能得到7天前的日期，再多就不行了，我实验了半天，好像最多最多能到167，也就是说只能得到7天前7小时的时间。大于167得到的时间就跟现在一模一样了。
QUOTE:
这个方法实际上是“歪打正着”，本来是用来取得其它某个时区的当前时间的方法，却被用来获得本地之前的某个时间了。全球的时区一共才24个，也就是说，如果使用这种方法的话，其实“小时”的数值只有这么一点点合法的范围。
【POSIX】：
5860   offset Indicates the value added to the local time to arrive at
5861          Coordinated Universal Time. The offset has the form:
5862                  hh[:mm[:ss]]
5863          The minutes (mm) and seconds (ss) are optional. The hour (hh)
5864          shall be required and may be a single digit. The offset following
5865          std shall be required. If no offset follows dst, the alternative time
5866          is assumed to be one hour ahead of standard time. One or more
5867          digits may be used; the value is always interpreted as a decimal
5868          number. The hour shall be between zero and 24, and the minutes
5869          (and seconds)—if present—between zero and 59. The result of
5870          using values outside of this range is unspecified. If preceded by
5871          a '-', the timezone shall be east of the Prime Meridian;
5872          otherwise, it shall be west (which may be indicated by an
5873          optional preceding '+').
按照上面的说法，使用超范围数值的结果是未定义的。在实验中之所以还能撑到167个小时，应该是和具体实现有关了，但这个数值早就已经不合法了。
方法三：
用gawk的strftime函数，这个方法可以得到很多天前的，不像方法一的那么多限制，但是首先需要安装gawk
cd /usr/ports/japanese/gawk
make install clean
rehash
gawk 'BEGIN{print strftime("%Y%m%d%H%M",systime());print strftime("%Y%m%d%H%M",systime()-1*24*3600)}'
会返回当前的时间和昨天日期的时间
当然你可以把systime()-1*24*3600改成你希望的天数，只要改那个1就成了。
方法四：
#!/bin/sh
# 取得昨天的日期
YEAR=`date +%Y`
DAY=`date +%d`
MONTH=`date +%m`
DAY=`expr $DAY - 1`
if [ $DAY -eq 0 ]; then
MONTH=`expr $MONTH - 1`
if [ $MONTH -eq 0 ]; then
MONTH=12
YEAR=`expr $YEAR - 1`
fi
LASTLINE=`cal $MONTH $YEAR | grep "[0-9]" | tail -1`
DAY=`echo $LASTLINE | sed "s/.* /([0-9][0-9]*/)//1/"`
fi
if [ $DAY -lt 10 ]; then
DAY=0$DAY
fi
YESTERDAY="$YEAR$MONTH$DAY"
echo $YESTERDAY
方法五：
来源于[http://bbs.chinaunix.net/viewthread.php?tid=743902&extra=page%3D1](http://bbs.chinaunix.net/viewthread.php?tid=743902&extra=page%3D1)
#!/bin/sh
#昨天日期shell版
yy=`date +%Y`;mm=`date +%m`;dd=`date +%d`
d0=`echo "$dd"|awk '{printf"%02d/n",$1-1}'`
m0=`echo "$mm"|awk '{printf"%02d/n",$1-1}'`
y0=`echo "$yy"|awk '{printf"%02d/n",$1-1}'`
case $d0 in
00) if [ $m0 = 00 ]; then 
    ym=`cal 12 $y0|xargs|awk '{print $NF}'`
    y0=$y0;m0=12;d0=$ym
    else
    ym=`cal $m0 $yy|xargs|awk '{print $NF}'`
    d0=$ym;m0=$m0;y0=$yy
    fi
    ;;
*) d0=$d0;m0=$mm;y0=$yy;;
esac 
echo "$y0$m0$d0"
