
# CentOS6安装telnet - leofionn的博客 - CSDN博客


2018年04月29日 19:52:53[leofionn](https://me.csdn.net/qq_36142114)阅读数：662


一.CentOS下查看系统是否已安装telnet
rpm -qa | grep telnet
telnet-0.17-48.el6.x86_64
telnet-server-0.17-48.el6.x86_64

telnet 是挂在 xinetd 底下的，所以同时查看是否安装了xinetd服务
rpm -qa | grep xinetd
xinetd-2.3.14-39.el6_4.x86_64

两者都没有的安装的话，yum安装以下服务
yum install xinetd telnet telnet-server -y

二.配置telnet
设置开机启动
\#chkconfig telnet on

修改配置文件
vi /etc/xinetd.d/telnet
将”disable= yes”改成” disable=no”

三、激活服务
重启xinetd服务
service xinetd restart
或者/etc/rc.d/init.d/xinetd restart

四.iptables防火墙会阻止telnet，所以需要在iptables允许，用如下命令
当你启动telnet服务后，你可以用netstat –tunlp命令来查看telnet服务所使用的端口，可以发现有23。使用下面命令开启这些端口:
iptables -I INPUT -p tcp --dport 23 -jACCEPT
iptables -I INPUT -p udp --dport 23 -jACCEPT
service iptables save //保存
service iptables restart //重启防火墙
或者来点狠的！！关闭防火墙!
service iptables stop

五.使用telnet命令
格式： telnet ip prot

六.其他常用的telnet命令功能描述：
close  关闭当前连接
logout  强制退出远程用户并关闭连接
display 显示当前操作的参数
mode    试图进入命令行方式或字符方式
open    连接到某一站点
quit    退出
set    设置当前操作的参数
unset  复位当前操作参数
status  打印状态信息
toggle  对操作参数进行开关转换
slc    改变特殊字符的状态
auth    打开/关闭确认功能z挂起
telnetsend    发送特殊字符
telnetenviron 更改环境变量?显示帮助信息

七.错误解决
如果使用telnet命令有以下提示：
-bash: telnet: command not found
请检查是否缺少安装telnet包

八：详细命令
Linux telnet命令用于远端登入。
执行telnet指令开启终端机阶段作业，并登入远端主机。
### 语法
telnet[-8acdEfFKLrx][-b<主机别名>][-e<脱离字符>][-k<域名>][-l<用户名称>][-n<记录文件>][-S<服务类型>][-X<认证形态>][主机名称或IP地址<通信端口>]**参数说明**：
-8 允许使用8位字符资料，包括输入与输出。
-a 尝试自动登入远端系统。
-b<主机别名> 使用别名指定远端主机名称。
-c 不读取用户专属目录里的.telnetrc文件。
-d 启动排错模式。
-e<脱离字符> 设置脱离字符。
-E 滤除脱离字符。
-f 此参数的效果和指定"-F"参数相同。
-F 使用Kerberos V5认证时，加上此参数可把本地主机的认证数据上传到远端主机。
-k<域名> 使用Kerberos认证时，加上此参数让远端主机采用指定的领域名，而非该主机的域名。
-K 不自动登入远端主机。
-l<用户名称> 指定要登入远端主机的用户名称。
-L 允许输出8位字符资料。
-n<记录文件> 指定文件记录相关信息。
-r 使用类似rlogin指令的用户界面。
-S<服务类型> 设置telnet连线所需的IP TOS信息。
-x 假设主机有支持数据加密的功能，就使用它。
-X<认证形态> 关闭指定的认证形态。
《参考菜鸟教程》【来自@若泽大数据】


