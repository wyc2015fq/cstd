# 修改Linux系统日期与时间date clock - starRTC免费im直播会议一对一视频 - CSDN博客
2017年01月04日 17:46:33[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：285
- 先设置日期
date -s 20080103
- 再设置时间
date -s 18:24:30
为了永久生效，需要将修改的时间写入CMOS。
查看CMOS的时间：
#clock -r 
将当前系统时间写到CMOS中去
clock -w
## 艺搜参考
[http://www.jb51.net/os/RedHat/1136.html](http://www.jb51.net/os/RedHat/1136.html)
