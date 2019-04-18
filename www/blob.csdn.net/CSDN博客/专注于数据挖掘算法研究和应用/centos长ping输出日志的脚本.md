# centos长ping输出日志的脚本 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月28日 09:16:24[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4787








为监控某服务器的网络情况，制作一个sh脚本，记录ping的长过程，并输出日志以备观察。

1、脚本如下

cat /tmp/ping94.sh

#!/bin/sh

ping 10.121.82.94 -c 172800 | awk '{ print $0"\t" strftime("%H:%M:%S",systime()) }' > /tmp/ping94.log &





2、ping的参数-c，设置持续执行ping动作172800次（60*60*24*2），每次一秒，就是两天。




3、后台执行脚本，nohup需要重定向，这里没用，用&直接后台执行。



