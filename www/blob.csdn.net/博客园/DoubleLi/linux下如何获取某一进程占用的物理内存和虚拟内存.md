# linux下如何获取某一进程占用的物理内存和虚拟内存 - DoubleLi - 博客园






首先，ps -A查看你所查看进程的进程号

ps -A

加入进程号为pid

那么使用如下脚本，可以打印该进程使用的虚拟内存和物理内存：

root@Storage:/mnt/mtd# cat rss.sh 
#!/bin/sh

while true 
do 
 cat /proc/pid/stat | awk -F" " '{print "virt:"$23}'
 cat /proc/pid/stat | awk -F" " '{print "rss:"$24}'
 sleep 1
done









