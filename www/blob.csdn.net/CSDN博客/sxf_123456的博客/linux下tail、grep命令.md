# linux下tail、grep命令 - sxf_123456的博客 - CSDN博客
2018年03月30日 21:01:30[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：770
                1、tail -F /home/sxf/yy/python/exer2/test1.log | grep bb  #查看test1.log中包含bb的数据
2、tail -f 日志文件           # 监控日志文件,再次执行某个程序时，该日志文件能够看到日志信息的输出
3、tail -f /home/sxf/yy/python/exer2/test1.log | awk '{ 
if (match($0, /.*aa.*/)) { print "\033[32;1m"$0"\033[0m" } 
else if (match($0, /.*bb.*/)) { print "\033[36;1m"$0"\033[0m" }
else { print $0 } }' #日志着色脚本,将查询到的数据有颜色显示
/home/sxf/yy/python/exer2/test1.log 文件内容如下：
cc 23 shanghai nanjing
ee 25 beijing nanjing
aa 22 beijing shanghai
cc 20 beijing shanghai
aa 25 shanghai shanghai
aa 24 nanjing beijing
ee 25 nanjing nanjing
bb 23 shanghai nanjing
dd 20 nanjing nanjing
aa 20 nanjing shanghai            
