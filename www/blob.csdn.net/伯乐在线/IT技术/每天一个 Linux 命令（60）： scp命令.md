# 每天一个 Linux 命令（60）： scp命令 - 文章 - 伯乐在线
原文出处： [peida](http://www.cnblogs.com/peida/archive/2013/03/15/2960802.html)
scp是secure copy的简写，用于在Linux下进行远程拷贝文件的命令，和它类似的命令有cp，不过cp只是在本机进行拷贝不能跨服务器，而且scp传输是加密的。可能会稍微影响一下速度。当你服务器硬盘变为只读 read only system时，用scp可以帮你把文件移出来。另外，scp还非常不占资源，不会提高多少系统负荷，在这一点上，rsync就远远不及它了。虽然 rsync比scp会快一点，但当小文件众多的情况下，rsync会导致硬盘I/O非常高，而scp基本不影响系统正常使用。
1．命令格式：

Shell
```
scp [参数] [原路径] [目标路径]
```
2．命令功能：
scp是 secure copy的缩写, scp是linux系统下基于ssh登陆进行安全的远程文件拷贝命令。linux的scp命令可以在linux服务器之间复制文件和目录。
3．命令参数：

Shell
```
-1  强制scp命令使用协议ssh1  
-2  强制scp命令使用协议ssh2  
-4  强制scp命令只使用IPv4寻址  
-6  强制scp命令只使用IPv6寻址  
-B  使用批处理模式（传输过程中不询问传输口令或短语）  
-C  允许压缩。（将-C标志传递给ssh，从而打开压缩功能）  
-p 保留原文件的修改时间，访问时间和访问权限。  
-q  不显示传输进度条。  
-r  递归复制整个目录。  
-v 详细方式显示输出。scp和ssh(1)会显示出整个过程的调试信息。这些信息用于调试连接，验证和配置问题。   
-c cipher  以cipher将数据传输进行加密，这个选项将直接传递给ssh。   
-F ssh_config  指定一个替代的ssh配置文件，此参数直接传递给ssh。  
-i identity_file  从指定文件中读取传输时使用的密钥文件，此参数直接传递给ssh。    
-l limit  限定用户所能使用的带宽，以Kbit/s为单位。     
-o ssh_option  如果习惯于使用ssh_config(5)中的参数传递方式，   
-P port  注意是大写的P, port是指定数据传输用到的端口号   
-S program  指定加密传输时所使用的程序。此程序必须能够理解ssh(1)的选项。
```
4．使用实例：
scp命令的实际应用概述：
从本地服务器复制到远程服务器：
(1) 复制文件：
命令格式：

Shell
```
scp local_file remote_username@remote_ip:remote_folder
```
或者

Shell
```
scp local_file remote_username@remote_ip:remote_file
```
或者

Shell
```
scp local_file remote_ip:remote_folder
```
或者

Shell
```
scp local_file remote_ip:remote_file
```
第1,2个指定了用户名，命令执行后需要输入用户密码，第1个仅指定了远程的目录，文件名字不变，第2个指定了文件名
第3,4个没有指定用户名，命令执行后需要输入用户名和密码，第3个仅指定了远程的目录，文件名字不变，第4个指定了文件名
(2) 复制目录：
命令格式：

Shell
```
scp -r local_folder remote_username@remote_ip:remote_folder
```
或者

Shell
```
scp -r local_folder remote_ip:remote_folder
```
第1个指定了用户名，命令执行后需要输入用户密码；
第2个没有指定用户名，命令执行后需要输入用户名和密码；
从远程服务器复制到本地服务器：
从远程复制到本地的scp命令与上面的命令雷同，只要将从本地复制到远程的命令后面2个参数互换顺序就行了。
实例1：从远处复制文件到本地目录
命令：

Shell
```
scp root@192.168.120.204:/opt/soft/nginx-0.5.38.tar.gz /opt/soft/
```
输出：
```
Shell
			
			
```
[root@localhost ~]# cd /opt/soft/
[root@localhost soft]# ll
总计 80072
drwxr-xr-x 12 root root     4096 09-21 18:40 fms3.5
drwxr-xr-x  3 root root     4096 09-21 17:58 fms4.5
drwxr-xr-x 10 root root     4096 10-30 17:15 jdk1.6.0_16
drwxr-xr-x 10 root root     4096 09-17 19:27 jdk1.6.0_16.bak
-rwxr-xr-x  1 root root 81871260 2009-12-21 jdk-6u16-linux-x64.bin
drwxrwxrwx  2 root root     4096 09-21 01:16 mysql
drwxr-xr-x  3 root root     4096 09-21 18:40 setup_file
drwxr-xr-x  9 root root     4096 09-17 19:23 tomcat6.0.32
drwxr-xr-x  9 root root     4096 2012-08-14 tomcat_7.0
[root@localhost soft]# scp root@192.168.120.204:/opt/soft/nginx-0.5.38.tar.gz /opt/soft/
root@192.168.120.204's password:
nginx-0.5.38.tar.gz                                                                               100%  479KB 478.7KB/s   00:00
[root@localhost soft]# ll
总计 80556
drwxr-xr-x 12 root root     4096 09-21 18:40 fms3.5
drwxr-xr-x  3 root root     4096 09-21 17:58 fms4.5
drwxr-xr-x 10 root root     4096 10-30 17:15 jdk1.6.0_16
drwxr-xr-x 10 root root     4096 09-17 19:27 jdk1.6.0_16.bak
-rwxr-xr-x  1 root root 81871260 2009-12-21 jdk-6u16-linux-x64.bin
drwxrwxrwx  2 root root     4096 09-21 01:16 mysql
-rw-r--r--  1 root root   490220 03-15 09:11 nginx-0.5.38.tar.gz
drwxr-xr-x  3 root root     4096 09-21 18:40 setup_file
drwxr-xr-x  9 root root     4096 09-17 19:23 tomcat6.0.32
drwxr-xr-x  9 root root     4096 2012-08-14 tomcat_7.0
[root@localhost soft]#
```
```
说明：
从192.168.120.204机器上的/opt/soft/的目录中下载nginx-0.5.38.tar.gz 文件到本地/opt/soft/目录中
实例2：从远处复制到本地
命令：

Shell
```
scp -r root@192.168.120.204:/opt/soft/mongodb /opt/soft/
```
输出：
```
Shell
			
			
```
[root@localhost soft]# ll
总计 80556
drwxr-xr-x 12 root root     4096 09-21 18:40 fms3.5
drwxr-xr-x  3 root root     4096 09-21 17:58 fms4.5
drwxr-xr-x 10 root root     4096 10-30 17:15 jdk1.6.0_16
drwxr-xr-x 10 root root     4096 09-17 19:27 jdk1.6.0_16.bak
-rwxr-xr-x  1 root root 81871260 2009-12-21 jdk-6u16-linux-x64.bin
drwxrwxrwx  2 root root     4096 09-21 01:16 mysql
-rw-r--r--  1 root root   490220 03-15 09:11 nginx-0.5.38.tar.gz
drwxr-xr-x  3 root root     4096 09-21 18:40 setup_file
drwxr-xr-x  9 root root     4096 09-17 19:23 tomcat6.0.32
drwxr-xr-x  9 root root     4096 2012-08-14 tomcat_7.0
[root@localhost soft]# scp -r root@192.168.120.204:/opt/soft/mongodb /opt/soft/
root@192.168.120.204's password:
mongodb-linux-i686-static-1.8.5.tgz                                                               100%   28MB  28.3MB/s   00:01
README                                                                                            100%  731     0.7KB/s   00:00
THIRD-PARTY-NOTICES                                                                               100% 7866     7.7KB/s   00:00
mongorestore                                                                                      100% 7753KB   7.6MB/s   00:00
mongod                                                                                            100% 7760KB   7.6MB/s   00:01
mongoexport                                                                                       100% 7744KB   7.6MB/s   00:00
bsondump                                                                                          100% 7737KB   7.6MB/s   00:00
mongofiles                                                                                        100% 7748KB   7.6MB/s   00:01
mongostat                                                                                         100% 7808KB   7.6MB/s   00:00
mongos                                                                                            100% 5262KB   5.1MB/s   00:01
mongo                                                                                             100% 3707KB   3.6MB/s   00:00
mongoimport                                                                                       100% 7754KB   7.6MB/s   00:00
mongodump                                                                                         100% 7773KB   7.6MB/s   00:00
GNU-AGPL-3.0                                                                                      100%   34KB  33.7KB/s   00:00
[root@localhost soft]# ll
总计 80560
drwxr-xr-x 12 root root     4096 09-21 18:40 fms3.5
drwxr-xr-x  3 root root     4096 09-21 17:58 fms4.5
drwxr-xr-x 10 root root     4096 10-30 17:15 jdk1.6.0_16
drwxr-xr-x 10 root root     4096 09-17 19:27 jdk1.6.0_16.bak
-rwxr-xr-x  1 root root 81871260 2009-12-21 jdk-6u16-linux-x64.bin
drwxr-xr-x  3 root root     4096 03-15 09:18 mongodb
drwxrwxrwx  2 root root     4096 09-21 01:16 mysql
-rw-r--r--  1 root root   490220 03-15 09:11 nginx-0.5.38.tar.gz
drwxr-xr-x  3 root root     4096 09-21 18:40 setup_file
drwxr-xr-x  9 root root     4096 09-17 19:23 tomcat6.0.32
drwxr-xr-x  9 root root     4096 2012-08-14 tomcat_7.0
[root@localhost soft]#
```
```
说明：
从192.168.120.204机器上的/opt/soft/中下载mongodb 目录到本地的/opt/soft/目录来。
实例3：上传本地文件到远程机器指定目录
命令：

Shell
```
scp /opt/soft/nginx-0.5.38.tar.gz root@192.168.120.204:/opt/soft/scptest
```
输出：
```
Shell
			
			
```
上传前目标机器的目标目录：
[root@localhost soft]# cd scptest/
[root@localhost scptest]# ll
总计 0
[root@localhost scptest]# ll
本地机器上传：
[root@localhost soft]# scp /opt/soft/nginx-0.5.38.tar.gz root@192.168.120.204:/opt/soft/scptest
root@192.168.120.204's password:
nginx-0.5.38.tar.gz                                                                               100%  479KB 478.7KB/s   00:00
[root@localhost soft]#
上传后目标机器的目标目录：
[root@localhost scptest]# ll
总计 484
-rw-r--r-- 1 root root 490220 03-15 09:25 nginx-0.5.38.tar.gz
[root@localhost scptest]#
```
```
说明：
复制本地opt/soft/目录下的文件nginx-0.5.38.tar.gz 到远程机器192.168.120.204的opt/soft/scptest目录
实例4：上传本地目录到远程机器指定目录
命令：

Shell
```
scp -r /opt/soft/mongodb root@192.168.120.204:/opt/soft/scptest
```
输出：
```
Shell
			
			
```
上传前目标机器的目标目录：
[root@localhost ~]# cd /opt/soft/scptest/
[root@localhost scptest]# ll
总计 484
-rw-r--r-- 1 root root 490220 03-15 09:25 nginx-0.5.38.tar.gz
[root@localhost scptest]#
本地机器上传：
[root@localhost ~]# scp -r /opt/soft/mongodb root@192.168.120.204:/opt/soft/scptest
root@192.168.120.204's password:
mongodb-linux-i686-static-1.8.5.tgz                                                               100%   28MB  28.3MB/s   00:01
README                                                                                            100%  731     0.7KB/s   00:00
THIRD-PARTY-NOTICES                                                                               100% 7866     7.7KB/s   00:00
mongorestore                                                                                      100% 7753KB   7.6MB/s   00:00
mongod                                                                                            100% 7760KB   7.6MB/s   00:01
mongoexport                                                                                       100% 7744KB   7.6MB/s   00:00
bsondump                                                                                          100% 7737KB   7.6MB/s   00:00
mongofiles                                                                                        100% 7748KB   7.6MB/s   00:00
mongostat                                                                                         100% 7808KB   7.6MB/s   00:01
mongos                                                                                            100% 5262KB   5.1MB/s   00:00
mongo                                                                                             100% 3707KB   3.6MB/s   00:00
mongoimport                                                                                       100% 7754KB   7.6MB/s   00:01
mongodump                                                                                         100% 7773KB   7.6MB/s   00:00
GNU-AGPL-3.0                                                                                      100%   34KB  33.7KB/s   00:00
[root@localhost ~]#
上传后目标机器的目标目录：
[root@localhost scptest]# ll
总计 488
drwxr-xr-x 3 root root   4096 03-15 09:33 mongodb
-rw-r--r-- 1 root root 490220 03-15 09:25 nginx-0.5.38.tar.gz
[root@localhost scptest]#
```
```
说明：
上传本地目录 /opt/soft/mongodb到远程机器192.168.120.204上/opt/soft/scptest的目录中去
### 本系列文章：
[每天一个 Linux 命令（1）：ls命令](http://blog.jobbole.com/108531/)
[每天一个 Linux 命令（2）：cd命令](http://blog.jobbole.com/108805/)
[每天一个 Linux 命令（3）：pwd命令](http://blog.jobbole.com/108855/)
[每天一个 Linux 命令（4）：mkdir命令](http://blog.jobbole.com/108871/)
[每天一个 Linux 命令（5）：rm 命令](http://blog.jobbole.com/108901/)
[每天一个 Linux 命令（6）：rmdir 命令](http://blog.jobbole.com/108925/)
[每天一个 Linux 命令（7）：mv命令](http://blog.jobbole.com/108983/)
[每天一个 Linux 命令（8）：cp 命令](http://blog.jobbole.com/108988/)
[每天一个 Linux 命令（9）：touch 命令](http://blog.jobbole.com/108989/)
[每天一个 Linux 命令（10）：cat 命令](http://blog.jobbole.com/108990/)
[每天一个 Linux 命令（11）：nl 命令](http://blog.jobbole.com/108991/)
[每天一个 Linux 命令（12）：more 命令](http://blog.jobbole.com/108992/)
[每天一个 Linux 命令（13）：less 命令](http://blog.jobbole.com/108993/)
[每天一个 Linux 命令（14）：head 命令](http://blog.jobbole.com/108994/)
[每天一个 Linux 命令（15）：tail 命令](http://blog.jobbole.com/108995/)
[每天一个 Linux 命令（16）：which命令](http://blog.jobbole.com/109262/)
[每天一个 Linux 命令（17）：whereis 命令](http://blog.jobbole.com/108993/)
[每天一个 Linux 命令（18）：locate 命令](http://blog.jobbole.com/108994/)
[每天一个 Linux 命令（19）：find 命令概览](http://blog.jobbole.com/109265/)
[每天一个 Linux 命令（20）：find命令之exec](http://blog.jobbole.com/108996/)
[每天一个 Linux 命令（21）：find命令之xargs](http://blog.jobbole.com/108997/)
[每天一个 Linux 命令（22）：find 命令的参数详解](http://blog.jobbole.com/109268/)
[每天一个 Linux 命令（23）：Linux 目录结构](http://blog.jobbole.com/109506/)
[每天一个 Linux 命令（24）：Linux 文件类型与扩展名](http://blog.jobbole.com/109254/)
[每天一个 Linux 命令（25）：Linux 文件属性详解](http://blog.jobbole.com/105269)
[每天一个 Linux 命令（26）：用 SecureCRT 来上传和下载文件](http://blog.jobbole.com/109539)
[每天一个 Linux 命令（27）：linux chmod 命令](http://blog.jobbole.com/109614)
[每天一个 Linux 命令（28）：tar 命令](http://blog.jobbole.com/109674/)
[每天一个 Linux 命令（29）: chgrp 命令](http://blog.jobbole.com/109696/)
[每天一个 Linux 命令（30）: chown 命令](http://blog.jobbole.com/109750/)
[每天一个 Linux 命令（31）: /etc/group 文件详解](http://blog.jobbole.com/109781/)
[每天一个 Linux 命令（32）：gzip 命令](http://blog.jobbole.com/109797/)
[每天一个 Linux 命令（33）：df 命令](http://blog.jobbole.com/109827/)
[每天一个 Linux 命令（34）： du 命令](http://blog.jobbole.com/109846/)
[每天一个 Linux 命令（35）： ln 命令](http://blog.jobbole.com/109869/)
[每天一个 Linux 命令（36）： diff 命令](http://blog.jobbole.com/109875/)
[每天一个 Linux 命令（37）： date 命令](http://blog.jobbole.com/109881/)
[每天一个 Linux 命令（38）： cal 命令](http://blog.jobbole.com/109886/)
[每天一个 Linux 命令（39）： grep 命令](http://blog.jobbole.com/109907/)
[每天一个 Linux 命令（40）： wc 命令](http://blog.jobbole.com/109925/)
[每天一个 Linux 命令（41）： ps 命令](http://blog.jobbole.com/109948/)
[每天一个 Linux 命令（44）： top 命令](http://blog.jobbole.com/109951/)
[每天一个 Linux 命令（45）： free 命令](http://blog.jobbole.com/109965/)
[每天一个 Linux 命令（46）： vmstat 命令](http://blog.jobbole.com/109970/)
[每天一个 Linux 命令（47）： iostat 命令](http://blog.jobbole.com/109980/)
[每天一个 Linux 命令（48）： watch 命令](http://blog.jobbole.com/109994/)
[每天一个 Linux 命令（49）： at 命令](http://blog.jobbole.com/110001/)
[每天一个 Linux 命令（50）： crontab 命令](http://blog.jobbole.com/110005/)
[每天一个 Linux 命令（51）： lsof 命令](http://blog.jobbole.com/110011/)
[每天一个 Linux 命令（52）： ifconfig 命令](http://blog.jobbole.com/110016/)
[每天一个 Linux 命令（53）： route 命令](http://blog.jobbole.com/110019/)
[每天一个 Linux 命令（54）： ping 命令](http://blog.jobbole.com/110025/)
[每天一个 Linux 命令（55）： traceroute 命令](http://blog.jobbole.com/110032/)
[每天一个 Linux 命令（56）： netstat 命令](http://blog.jobbole.com/110037/)
[每天一个 Linux 命令（57）： ss 命令](http://blog.jobbole.com/110046/)
[每天一个 Linux 命令（58）： telnet 命令](http://blog.jobbole.com/110058/)
[每天一个 Linux 命令（59）： rcp 命令](http://blog.jobbole.com/110072/)
