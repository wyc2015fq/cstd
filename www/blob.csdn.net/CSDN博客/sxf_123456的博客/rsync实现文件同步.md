# rsync实现文件同步 - sxf_123456的博客 - CSDN博客
2017年04月24日 21:12:15[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：281
个人分类：[shell](https://blog.csdn.net/sxf_123456/article/category/6881619)

rsync是类unix系统下的数据镜像备份工工具，一般linux系统都自带了 [可以确认一下：shell>rpm -qa|grep rsync]
服务端：192.168.1.2  同步目录：/home/source
客户端：192.168.1.3  同步目录：/home/receive
1、服务端配置
shell>vi /etc/rsyncd.conf   #创建配置文件rsyncd.conf
配置文件内容：
```
uid=nobody //运行rsync守护进程的用户
gid=nobody //运行rsync守护进程的组
use chroot = no //不使用chroot
max connections = 10 //最大连接数，0为不限制
pid file = /var/run/rsyncde.pid //pid文件存放位置
lock file = /var/run/rsync.lock //锁文件存放位置
log file = /var/log/rsyncd.log //日志记录文件的存放位置
//通过该选项可以覆盖客户指定的IP超时时间
//通过该选项可以确保rsync服务器不会永远等待一个崩溃的客户端
//超时单位为秒钟,0表示没有超时定义,这也是默认值
//对于匿名rsync服务器来说,一个理想的数字是600.
Timeout = 300
Log format = %t %a %m %f %b
[backup] //这里是认证的模块名，在client端需要指定
path=/home/source/ //需要做镜像的目录
ignore errors //可以忽略一些无关的IO错误
read only = yes  //该选项设定是否允许客户上传文件
list = no //不允许列文件
auth users = rsync //认证的用户名
secrets file = /etc/rsyncd.secrets //秘密文件(下面定义)  
hosts allow = 192.168.1.3 //允许主机或网段
hosts deny = 0.0.0.0/0 //禁止主机
```
shell>vi etc/rsyncd.secrets #创建密码文件
内容：
```
rsync:rsync //这里用户名和密码都定义为rsync
```
shell>chmod 0600 /etc/rsyncd.secrets  #更改密码文件权限
shell>rsync --daemon;  #启动服务,默认在873端口监听(可以自己修改)
2、客户端
客户端就不用启动rsync服务了
shell>vi /etc/rsyncd.secrets #这里也是创建密码文件，文件名字自定义，但内容只需要填目标服务授权密码
shell>chmod 0600 /etc/rsyncd.secrets #更改文件权限
内容：
```
rsync //这里是服务端rsync服务授权密码
```
测试：(在服务端/home/source/ 先创建test文件 )
shell>rsync -vzrtopg --delete --progress --password-file=/etc/rsyncd.secrets rsync@192.168.1.2::backup /home/receive
v:传输时的进度等信息, z:表示压缩, r:是递归, t:保持文件原有时间, o:保持文件原有属主, P:传输进度, g:保持文件原有用户组
--progress 指显示
--delete 指如果服务器端删除了这一文件，那么客户端也相应把文件删除，保持真正的一致
--password-file=/etc/rsyncd.secrets 认证密码
rsync 认证用户
backup 认证模块
如果客户端/home/receive/下产生了test文件代表同步成功
下一步写一个脚本文件实现真正的同步......
启动脚本：rsync.sh 
shell>vi /home/rsync.sh
`#!/bin/sh`
```
rsync
 -vzrtopg --delete --progress --password-file=/etc/rsyncd.secrets rsync@192.168.1.2::backup /home/receive
```
关闭脚本：killrsync.sh
shell>vi /home/killrsync.sh
```
#!/bin/sh
RSYNC_PID=`ps auxww|grep rsync |grep -v grep|awk '{print $2}'`
kill -9 $RSYNC_PID
```
设置定时任务 (crontab？google can help you！)
shell>crontab -e
```
SHELL=/bin/sh
PATH=/sbin:/bin:/usr/sbin:/usr/bin
MAILTO=root
0 3 * * *  /home/rsync.sh  //每天晚上3点执行一次同步
0 6 * * *  /home/killrsync.sh //每天早上6点强制终止同步(如果还没完成)
```
shell>crontab -l #可以查看任务设置情况
shell>service crond status #查看crond是否已启动，若启动了能看到PID
shell>service crond start #启动crond服务
