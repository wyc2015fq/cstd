# Linux常用指令 - weixin_33985507的博客 - CSDN博客
2015年09月22日 10:24:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2

server将被频繁使用的部署linux，使用互联网的时候非常时刻，检查指导，然后运行之前早已被人遗忘，因此，这篇文章是通常用于记录一些自己的linux指令。以作备忘。并不断加入。
1、文件、文件夹相关
a、cd：切换文件夹 (#cd /sbin....）
b、mkdir：创建文件夹 (#mkdir test)
c、rm：删除文件或者文件夹(#rm -fr test)
d、cp：复制文件或者文件夹(#cp -fr src dst)
e、mv：移动文件或者文件夹（#mv -f src dst）
f、ll/ls：查看当前文件夹的文件内容
g、cat：顺序查看文件内容(倒序查是tac)(#cat filename)
2、系统相关命令
a、查看cpu信息：#/proc/cpuinfo | grep name | cut -f2 -d: | uniq -c
b、查看linux版本号：#lsb_release -a
c、重新启动电脑：#reboot
d、查看进程：#ps -aux
e、杀死进程：#kill pis
f、查看某个程序在什么位置：#whereis exec
g、下载文件：#wget -c url 
h、查看端口情况： #netstat -tlpn
i、监控内存cpu情况：#top
j、查看内存分布：#pmap pid
3、压缩解压
a、tar压缩：#tar xcvf  dst src
b、tar解压：#tar xvf dst 
c、zip压缩：#zip -r dst src
d、unzip解压：#unzip  dst
4、vi/vim经常使用
a、i->切换成输入模式
b、esc->切换到命令模式
c、:q->退出指令
d、:wq->保存并退出
5、打开关闭服务
a、打开防火墙：#service iptables start	
b、关闭防火墙：#service iptables stop
c、打开mysql：#service mysqld start
d、关闭mysql：#service mysqld stop
6、coredump相关
a、改动core文件大小：#vi /etc/security/limits.conf，然后改动soft core的值。并放开凝视
b、查询core文件大小：#ulimit -c
c、调试core文件：#gdb ./program:name core:ame
7、valgrind经常使用启动命令
a、仅仅开日志和内存泄露检查：#valgrind --log-file=valgrind.log --leak-check=full ./program:name
8、动态库文件夹相关
a、加入当前文件夹：#export LD_LIBRARY_PATH=`pwd`
b、查看动态库载入文件夹：#echo $LD_LIBRARY_PATH
c、查询某个档案的动态函式库：#ldd execname
d、查看某个库的符号表：#nm -D execname
9、gdb相关指令
a、ctrl+c：程序暂停
b、c：继续执行
c、bt：打印当前线程的当前的调用栈
d、thread apply all bt full：打印出全部线程的当前的调用栈
e、info local：查看全部本地变量
f、info threads：显示当前可调试的全部线程
g、thread ID：切换当前调试的线程为指定ID的线程
h、p：查看指定变量
i、up：堆栈上移
j、down：堆栈下移
k、step：单步执行
l、attach pid：附加到进程进行调试
m、break file:linenumber：在指定文件指定行加断点
n、delete number：删除指定断点
10、软件安装yum
a、yum list |grep xxx：列出能够安装的安装包
b、yum install xxx：安装上面列出的安装包
版权声明：本文博主原创文章，博客，未经同意不得转载。
