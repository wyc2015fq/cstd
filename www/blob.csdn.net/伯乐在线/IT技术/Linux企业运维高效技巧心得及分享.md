# Linux企业运维高效技巧心得及分享 - 文章 - 伯乐在线
原文出处： [wgkgood](http://wgkgood.blog.51cto.com/1192594/1641247)
随着Linux在企业中的不断飞速的应用，为了企业中更好的运维，熟悉日常运维的技巧能更好的满足企业的发展，同时让我们的运维更加轻松，不再觉得运维是苦逼的活，真正去锻炼去成长去磨练。
让我们的运维更有”韵味”,让我们的生活更加美好。 通过本次课程的学习，我会把我在日常运维中的心得和体会分享给大家，让大家得到真正的知识，然后应用在自己的运维工作中。
那我们需要注意什么呢：
**1) Linux行业目标**
我们要明白学习Linux运维的目的，相信大家都是为了能找到一份非常好的工作，一个高薪的工作，不断的练习，不断的成长。
通过工作，让我们的生活更加的完整和充实。
**2) Linux学习路线**
在明白自己的大的目标之后，我们需要分解大目标，接下来就是真正去行动，去朝着小目标努力，有哪些小目标呢？
计算机基础知识—>硬件认识—>windows系统日常操作—>Linux系统入门—>Linux目录及权限学习—>linux必备20个命令(ls pwd cd cat useradd groupadd rm cp chown chmod vi find grep ps free top sed awk if for case wc yum rpm tar unzip more head tail等)
—>Linux简单服务器搭建（掌握tar常见文件解压方式，掌握安装软件的方法：
yum install方式安装；源码编译安装三步，configure、make、make install）
—>Apache服务构建—>Mysql服务搭建—>PHP服务器搭建—>LAMP架构整合discuz论坛—>Kickstart自动化系统安装—>cacti监控部署—>Shell脚本编程（包括各种语句的学习，if for awk for while sed等）—>Linux高级服务器搭建—>Nginx WEB服务器搭建—>Tomcat服务器搭建—>resin服务器搭建—>Nginx均衡java服务器—>LNMP架构搭建（yum/源码）—>Nginx动静分离—>LVS+Keepalived负载均衡部署—>LVS+Keepalived+Nginx+Tomcat均衡架构部署—>高级Shell编写—>自动化运维学习（KVM、Puppet、ZABBIX、Ansible、Mysql+DRBD等）
**3) 编辑器命令技巧**
熟悉命令行及vi编辑器的查找，匹配删除、跳转等等，例如在shell命令行里ctrl +a跳转到最前，ctrl +e跳转到最末尾。
![wKiom1VDoRHgNS13AAEhDdvOO3M965.jpg](http://jbcdn2.b0.upaiyun.com/2016/01/2c68b486f7e32fef802ac8ffe206fed1.jpg)
在vi编辑器里面：
Shift + ^跳转到开头，shift + $跳转到末尾。
匹配/word字符，删除光标所在字符按x即可，跳转到文本最末行按G，跳转到文本首行按gg。
同时删除光标行至文本最后一行：dG
删除光标行至文本第一行：dgg
**4) 系统运行状态监测**
可以使用free m查看内存剩余大小，通常看

Shell
```
-/+ buffers/cache:        881        112（该值大约为真实内存值）
```
![wKioL1VDopCBAhrbAADtx2lCdOM632.jpg](http://jbcdn2.b0.upaiyun.com/2016/01/b7141928b01033a6494da03e7eb2f0f0.jpg)
可以使用df h查看到tmpfs内存文件系统，加速静态文件及图片：
![wKiom1VDoTKCBokwAAHFsP5LxiI921.jpg](http://jbcdn2.b0.upaiyun.com/2016/01/4fe22a1c1bf27f5b3f94599fdcf50621.jpg)
查看本地网卡流量技巧：

Shell
```
yum install iftop -y
```
iftop  -i eth0查看结果如下图：
![wKioL1VDoqWgx2j-AAE6U3o_2FA299.jpg](http://jbcdn2.b0.upaiyun.com/2016/01/44d3c3c5463c60248b56519e22d4fd13.jpg)

Shell
```
中间这两个左右箭头，表示的是流量的方向。
TX：发送流量。
RX：接收流量。
TOTAL：总流量。
Cumm：运行iftop到目前时间的总流量。
peak：流量峰值。
rates：分别表示过去 2s 10s 40s 的平均流量。
```
查看磁盘IO负载技巧：
vmstat 1 5 (每秒输出结果，总共输出5次)
![wKiom1VDoU3gnsrHAAHXSEB9e0I420.jpg](http://jbcdn2.b0.upaiyun.com/2016/01/b6f71c6da5953b2914241aeb35e78473.jpg)
r: 运行队列中进程数量
b: 等待IO的进程数量
Memory（内存）：
bi: 每秒读取的块数
bo: 每秒写入的块数
wa: 等待IO时间
注意*一般判断系统负载是否过高，IO磁盘读写是否超高，我们可以查看r、b和wa的时间，当然是越小表是性能资源还有很多剩余，如果过大，我们就需要查看是由于什么操作导致的。
可以结合iostat查看更容易判断是不是磁盘读写导致IO很高。
![wKioL1VDosejDwYNAALXr5Bcbec129.jpg](http://jbcdn2.b0.upaiyun.com/2016/01/69494363f2e2921ea5c0d20640d9c67a.jpg)
一般判断%util的值，如果持续超过75%以上就需要注意了，检查相关服务的访问是否异常，然后去一一解决。
服务后台启动：
常见的程序放在后台运行方法主要有：
screen 后台运行：
在命令行执行screen 回车，进入一个随机的screen后台，可以输入命令，然后按ctrl +a+d保存退出即可，这时程序已经在后台运行。
Screen ls可以查看当前运行screen后台列表,执行screen r 加PID可以进入相应的后台，再次退出还需要按ctrl+a+d
![wKiom1VDoWugoW-xAAD8tvvgwck824.jpg](http://jbcdn2.b0.upaiyun.com/2016/01/183458121c94366aa3d52fcddaee778a.jpg)
如何想要删除screen，需要执行kill -9 3215 ，然后执行screen -wipe 即可删除。
![wKioL1VDot2Q-lERAALDhEzUPjU835.jpg](http://jbcdn2.b0.upaiyun.com/2016/01/45c1f769fe825bd80c7221bfef39a7b4.jpg)
除了screen之外，我们还可以使用nohup来后台运行程序：

Shell
```
nohup  sh  auto_nginx.sh
```
即程序已经在后台运行，可以在当前目录查看tail fn 10 nohup.out可以看到程序执行的相关信息，如果需要结束就直接kill 进程就OK。
当然了还有更多的技巧，后期会给大家一起交流和探讨，继续分享。
