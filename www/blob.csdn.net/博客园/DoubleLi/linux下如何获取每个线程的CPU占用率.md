# linux下如何获取每个线程的CPU占用率 - DoubleLi - 博客园






啥也不说，直接上脚本：

root@Storage:/mnt/mtd# cat cpu.sh 
#!/bin/sh

while true
do
        ps -H -eo user,pid,ppid,tid,time,%cpu,cmd --sort=%cpu
        sleep 1
done
root@Storage:/mnt/mtd# 









