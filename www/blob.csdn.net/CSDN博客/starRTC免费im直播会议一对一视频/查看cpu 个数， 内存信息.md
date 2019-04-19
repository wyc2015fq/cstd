# 查看cpu 个数， 内存信息 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年05月11日 09:36:57[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：85
 查看物理CPU个数cat /proc/cpuinfo| grep"physical id"| sort| uniq| wc -l
查看每个物理CPU中core的个数(即核数)cat /proc/cpuinfo| grep"cpu cores"| uniq
查看CPU信息（型号）
cat /proc/cpuinfo | grep name | cut -f2 -d: | uniq -c
查看内 存信息
cat /proc/meminfo
