# linux 登陆ftp及文件上传及下载 - sxf_123456的博客 - CSDN博客
2018年01月16日 13:02:58[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：132
个人分类：[Linux command](https://blog.csdn.net/sxf_123456/article/category/6748440)
**1.      Linux 终端连接FTP**
[oracle@Dave ~]$ ftp 10.85.7.97
Connected to 10.85.7.97.
220 Serv-U FTP Server v6.4 for WinSock ready...
500 'AUTH': command not understood.
500 'AUTH': command not understood.
KERBEROS_V4 rejected as an authentication type
Name (10.85.7.97:oracle): super
331 User name okay, need password.
Password:   -- Linux 的密码是不回显的
230 User logged in, proceed.
Remote system type is UNIX.
Using binary mode to transfer files.
ftp>
如果FTP 允许匿名用户，那么用户名要输入anonymous,密码任意。不能直接敲回车。
**2.      查看FTP 命令**
ftp> ?
Commands may be abbreviated.  Commands are:
!               cr              mdir            proxy           send
$               delete          mget            sendport        site
account         debug           mkdir           put             size
append          dir             mls             pwd             status
ascii           disconnect      mode            quit            struct
bell            form            modtime         quote           system
binary          get             mput            recv            sunique
bye             glob            newer           reget           tenex
case            hash            nmap            rstatus         trace
ccc             help            nlist           rhelp           type
cd              idle            ntrans          rename          user
cdup            image           open            reset           umask
chmod           lcd             passive         restart         verbose
clear           ls              private         rmdir           ?
close           macdef          prompt          runique
cprotect        mdelete         protect         safe
可以通过help command 查看每个命令的说明
ftp> help put
put             send one file
ftp> help mput
mput            send multiple files
**3.      上传文件**
Put命令：格式：put local-file [remote-file] 将一个文件上传到ftp
Mput命令：格式：mput local-files 将本地主机中一批文件传送至远端主机.
                  注意：mput命令只能将当前本地目录下的文件上传到FTP上的当前目录。比如，在 /root/dave下运行的ftp命令，则只有在/root/dave下的文件linux才会上传到服务器上的当前目录下。
**Put 代码示例：**
ftp> pwd    -- 显示FTP上当前路径
257 "/" is current directory.
ftp> ls   -- 显示当前目录下的文件
227 Entering Passive Mode (10,85,7,97,16,141)
150 Opening ASCII mode data connection for /bin/ls.
drw-rw-rw-   1 user     group           0 Aug 17 16:11 .
drw-rw-rw-   1 user     group           0 Aug 17 16:11 ..
-rw-rw-rw-   1 user     group     1433088 Feb  5  2010 ASPAJAXExtSetup_PConline_PConline.msi
-rw-rw-rw-   1 user     group    197626837 Mar  8 10:00 DelphiV7.0.zip
drw-rw-rw-   1 user     group           0 Jul  9 10:17 HHT
-rw-rw-rw-   1 user     group     9235148 Dec 24  2009 Install_HSKey_V1.3.exe
-rw-rw-rw-   1 user     group    47316992 May 27 16:38 Release_4.rar
ftp> mkdir Dave    -- 创建文件夹Dave
257 "/Dave" directory created.
ftp> cd Dave      -- 进入文件夹Dave
250 Directory changed to /Dave
ftp> pwd        -- 显示当前目录
257 "/Dave" is current directory.
ftp> lcd     -- 显示当前本地的路径，我们可以将这个路径下的这个文件上传到FTP服务器的相关位置
Local directory now /home/oracle
ftp> !      -- 退出当前的窗口，返回Linux 终端，当我们退出终端的时候，又会返回到FTP上。
[oracle@Dave ~]$ ls  -- 显示当前目录下的文件
dead.letter  scripts  sqlnet.log
[oracle@Dave ~]$ vi Dave  -- 创建文件 Dave
[oracle@Dave ~]$ vi DBA   -- 创建文件 DBA
[oracle@Dave ~]$ ls       -- 显示文件夹里的内容，等会我们将这些文件copy到FTP上
Dave  DBA  dead.letter  scripts  sqlnet.log
[oracle@Dave ~]$ exit  -- 退出终端，返回FTP命令行
exit
ftp> lcd
Local directory now /home/oracle
ftp> put DBA DBA    -- 将刚才创建的文件DBA 上传到ftp的当前目录上并命名为DBA。
local: DBA remote: DBA
227 Entering Passive Mode (10,85,7,97,16,156)
150 Opening BINARY mode data connection for DBA.
226 Transfer complete.
5 bytes sent in 3.3e-05 seconds (1.5e+02 Kbytes/s)
ftp> put DBA /Dave/SFDBA -- 将刚才创建的文件DBA 上传到ftp的当前目录上并重命名为SFDBA。
local: DBA remote: /Dave/SFDBA
227 Entering Passive Mode (10,85,7,97,16,158)
150 Opening BINARY mode data connection for SFDBA.
226 Transfer complete.
5 bytes sent in 1.8e-05 seconds (2.7e+02 Kbytes/s)
ftp> put /home/oracle/DBA /test/SFDBA 
local: /home/oracle/DBA remote: /test/SFDBA
227 Entering Passive Mode (10,85,7,97,16,163)
150 Opening BINARY mode data connection for SFDBA.
226 Transfer complete.
5 bytes sent in 2.2e-05 seconds (2.2e+02 Kbytes/s)
ftp> cd test
250 Directory changed to /test
ftp> ls
227 Entering Passive Mode (10,85,7,97,16,169)
150 Opening ASCII mode data connection for /bin/ls.
drw-rw-rw-   1 user     group           0 Aug 17 17:11 .
drw-rw-rw-   1 user     group           0 Aug 17 17:11 ..
-rw-rw-rw-   1 user     group           5 Aug 17 17:11 SFDBA
-rw-rw-rw-   1 user     group        5508 Aug 17 16:21 sqlnet.log
226 Transfer complete.
**Mput 示例代码：**
ftp> cd Dave
250 Directory changed to /Dave
ftp> mput *
mput alert_log.txt?    -- 这里每个文件都要确认，按回车键就可以了
227 Entering Passive Mode (10,85,7,97,16,196)
150 Opening BINARY mode data connection for alert_log.txt.
226 Transfer complete.
712 bytes sent in 2.9e-05 seconds (2.4e+04 Kbytes/s)
mput alert_newccs.hist?
227 Entering Passive Mode (10,85,7,97,16,197)
150 Opening BINARY mode data connection for alert_newccs.hist.
226 Transfer complete.
9321666 bytes sent in 0.78 seconds (1.2e+04 Kbytes/s)
mput alert_newccs.log?
227 Entering Passive Mode (10,85,7,97,16,198)
150 Opening BINARY mode data connection for alert_newccs.log.
226 Transfer complete.
21707 bytes sent in 0.0014 seconds (1.5e+04 Kbytes/s)
ftp> ls       -- 显示目录下的文件
227 Entering Passive Mode (10,85,7,97,16,217)
150 Opening ASCII mode data connection for /bin/ls.
drw-rw-rw-   1 user     group           0 Aug 17 17:22 .
drw-rw-rw-   1 user     group           0 Aug 17 17:22 ..
-rw-rw-rw-   1 user     group           5 Aug 17 17:09 SFDBA
-rw-rw-rw-   1 user     group         712 Aug 17 17:21 alert_log.txt
-rw-rw-rw-   1 user     group     9321666 Aug 17 17:21 alert_newccs.hist
-rw-rw-rw-   1 user     group       21707 Aug 17 17:21 alert_newccs.log
226 Transfer complete.
ftp> delete SFDBA   --删除SFDBA 文件
250 DELE command successful.
ftp> mdelete a*   -- 批量删除文件
mdelete alert_log.txt?  -- 每个文件都要确认
250 DELE command successful.
mdelete alert_newccs.hist?
250 DELE command successful.
mdelete alert_newccs.log?
250 DELE command successful.
**4.下载文件**
同样也有2个命令： get 和 mget。 Mget 用户批量下载。
                   格式：get [remote-file] [local-file]
                              mget [remote-files]
                  同样，mget 是将文件下载到本地的当前目录下。
**Get 示例：**
ftp> get /test/SFDBA /home/SFDBA
local: /home/SFDBA remote: /test/SFDBA
local: /home/SFDBA: Permission denied  --Linux对权限控制的很严格，下载的时候是否有对应文件夹的写权限
ftp>  get /test/SFDBA /home/oracle/SFDBA
local: /home/oracle/SFDBA remote: /test/SFDBA
227 Entering Passive Mode (10,85,7,97,17,0)
150 Opening BINARY mode data connection for SFDBA (5 Bytes).
226 Transfer complete.
5 bytes received in 6.2e-05 seconds (79 Kbytes/s)
ftp> !
[oracle@Dave bdump]$ cd /home/oracle/
[oracle@Dave ~]$ ls
Dave  DBA  dead.letter  scripts  SFDBA  sqlnet.log
**Mget 示例：**
ftp> ls
227 Entering Passive Mode (10,85,7,97,17,10)
150 Opening ASCII mode data connection for /bin/ls.
drw-rw-rw-   1 user     group           0 Aug 17 17:38 .
drw-rw-rw-   1 user     group           0 Aug 17 17:38 ..
-rw-rw-rw-   1 user     group           5 Aug 17 17:38 DBA
-rw-rw-rw-   1 user     group          12 Aug 17 17:38 Dave
-rw-rw-rw-   1 user     group           5 Aug 17 17:11 SFDBA
226 Transfer complete.
ftp> mget *
mget DBA?  -- 每个文件都要确认，按回车即可
227 Entering Passive Mode (10,85,7,97,17,12)
150 Opening BINARY mode data connection for DBA (5 Bytes).
226 Transfer complete.
5 bytes received in 8.1e-05 seconds (60 Kbytes/s)
mget Dave?
227 Entering Passive Mode (10,85,7,97,17,13)
150 Opening BINARY mode data connection for Dave (12 Bytes).
226 Transfer complete.
12 bytes received in 8.3e-05 seconds (1.4e+02 Kbytes/s)
mget SFDBA?
227 Entering Passive Mode (10,85,7,97,17,14)
150 Opening BINARY mode data connection for SFDBA (5 Bytes).
226 Transfer complete.
5 bytes received in 9e-05 seconds (54 Kbytes/s)
ftp> !
[oracle@Dave ~]$ ls
Dave  DBA  dead.letter  scripts  SFDBA  sqlnet.log
说明的地方：FTP 当前目录下的文件下载到本地的当前目录。
**5.   断开FTP 连接**
Bye命令或者quit命令：中断与服务器的连接。
ftp> bye
221 Goodbye!
以上只是Linux 终端访问ftp及上传下载的几个简单示例。 不过一般这几个应该够用了。其他功能感兴趣的自己研究。

------------------------------------------------------------------------------
Blog： http://blog.csdn.net/tianlesoftware
网上资源： http://tianlesoftware.download.csdn.net
