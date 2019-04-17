# Linux后台开发常用工具 - DoubleLi - 博客园






内存分析工具
valgrind


valgrind辅助工具
qcachegrind 可视化查看valgrind结果


淘宝DBA团队发布的监控脚本，使用perl开发，可以完成对linux系统和MySql相关指标的实时监控
orzdba 


取指定进程名的pid
pidof 进程名


性能瓶颈查看：
perf top -p pid


查看调用栈：
pstack pid


https://www.percona.com/
查询程序执行聚合的GDB堆栈跟踪，先进性堆栈跟踪，然后将跟踪信息汇总：
pt-pmp -p pid


格式化explain出来的执行计划按照tree方式输出，方便阅读：
pt-visual-explain


从log文件中读取插叙语句，并用explain分析他们是如何利用索引，完成分析之后会生成一份关于索引没有被查询使用过的报告：
pt-index-usage


其它：
vmstat
tcpdump 网络数据包分析器
objdump
nm
ldd
strings
iostat 输入/输出统计
ifstat 网络流量实时监控工具
vmstat 虚拟内存统计
sar （System Activity Reporter系统活动情况报告，最为全面的系统性能分析工具之一）
iptraf 实时IP局域网监控
iftop 网络带宽监控
htop 进程监控
iotop 磁盘I/O监测工具
fuser 使用文件或文件结构识别进程
lsof 打开文件列表
dmesg
slabtop
free
slurm 查看网络流量
byobu 类似于screen
tmux 终端复用工具，类似于screen
screen 在多个进程之间多路复用一个物理终端的窗口管理器
dtach 用来模拟screen的detach的功能的小工具
dstat 可以取代vmstat,iostat,netstat和ifstat这些命令的多功能产品
NetHogs 监视每个进程的网络带宽
MultiTail 同时监控多个文档、类似tail
Monitorix 系统和网络监控
Arpwatch 以太网活动监控器
Suricata 网络安全监控
Nagios 网络/服务器监控
Collectl 一体化性能检测工具
mtr 网络连通性判断工具，集成了traceroute和ping
socat 多功能的网络工具（Socket CAT，netcat加强版）
netpipes socket操作
ab
wget
curl
tsung 压力测试工具
siege 压力测试和评测工具
nmon 监控Linux系统性能
psacct 监视用户活动
ncdu 基于ncurses库的磁盘使用分析器
slurm 实时网络流量监控
findmnt 查找已经被挂载的文件系统
saidar 系统数据监控和统计工具
ss 可以替代netstat的网络连接查看工具(socket statistics)
ccze 用不同颜色高亮日志协助管理员进行区分和查看分析
netstat 网络统计
ifconfig (ifup ifdown)









