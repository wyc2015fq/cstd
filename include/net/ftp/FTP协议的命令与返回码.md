# FTP协议的命令与返回码



FTP命令

| **命令**         | **描述**                                           |
| ---------------- | -------------------------------------------------- |
| ABOR             | 中断数据连接程序                                   |
| ACCT <account>   | 系统特权帐号                                       |
| ALLO <bytes>     | 为服务器上的文件存储器分配字节                     |
| APPE <filename>  | 添加文件到服务器同名文件                           |
| CDUP <dir path>  | 改变服务器上的父目录                               |
| CWD <dir path>   | 改变服务器上的工作目录                             |
| DELE <filename>  | 删除服务器上的指定文件                             |
| HELP <command>   | 返回指定命令信息                                   |
| LIST <name>      | 如果是文件名列出文件信息，如果是目录则列出文件列表 |
| MODE <mode>      | 传输模式（S=流模式，B=块模式，C=压缩模式）         |
| MKD <directory>  | 在服务器上建立指定目录                             |
| NLST <directory> | 列出指定目录内容                                   |
| NOOP             | 无动作，除了来自服务器上的承认                     |
| PASS <password>  | 系统登录密码                                       |
| PASV             | 请求服务器等待数据连接                             |
| PORT <address>   | IP 地址和两字节的端口 ID                           |
| PWD              | 显示当前工作目录                                   |
| QUIT             | 从 FTP 服务器上退出登录                            |
| REIN             | 重新初始化登录状态连接                             |
| REST <offset>    | 由特定偏移量重启文件传递                           |
| RETR <filename>  | 从服务器上找回（复制）文件                         |
| RMD <directory>  | 在服务器上删除指定目录                             |
| RNFR <old path>  | 对旧路径重命名                                     |
| RNTO <new path>  | 对新路径重命名                                     |
| SITE <params>    | 由服务器提供的站点特殊参数                         |
| SMNT <pathname>  | 挂载指定文件结构                                   |
| STAT <directory> | 在当前程序或目录上返回信息                         |
| STOR <filename>  | 储存（复制）文件到服务器上                         |
| STOU <filename>  | 储存文件到服务器名称上                             |
| STRU <type>      | 数据结构（F=文件，R=记录，P=页面）                 |
| SYST             | 返回服务器使用的操作系统                           |
| TYPE <data type> | 数据类型（A=ASCII，E=EBCDIC，I=binary）            |
| USER <username>> | 系统登录的用户名                                   |

 

FTP响应码

 

| **响应代码** | **解释说明**                     |
| ------------ | -------------------------------- |
| 110          | 新文件指示器上的重启标记         |
| 120          | 服务器准备就绪的时间（分钟数）   |
| 125          | 打开数据连接，开始传输           |
| 150          | 打开连接                         |
| 200          | 成功                             |
| 202          | 命令没有执行                     |
| 211          | 系统状态回复                     |
| 212          | 目录状态回复                     |
| 213          | 文件状态回复                     |
| 214          | 帮助信息回复                     |
| 215          | 系统类型回复                     |
| 220          | 服务就绪                         |
| 221          | 退出网络                         |
| 225          | 打开数据连接                     |
| 226          | 结束数据连接                     |
| 227          | 进入被动模式（IP 地址、ID 端口） |
| 230          | 登录因特网                       |
| 250          | 文件行为完成                     |
| 257          | 路径名建立                       |
| 331          | 要求密码                         |
| 332          | 要求帐号                         |
| 350          | 文件行为暂停                     |
| 421          | 服务关闭                         |
| 425          | 无法打开数据连接                 |
| 426          | 结束连接                         |
| 450          | 文件不可用                       |
| 451          | 遇到本地错误                     |
| 452          | 磁盘空间不足                     |
| 500          | 无效命令                         |
| 501          | 错误参数                         |
| 502          | 命令没有执行                     |
| 503          | 错误指令序列                     |
| 504          | 无效命令参数                     |
| 530          | 未登录网络                       |
| 532          | 存储文件需要帐号                 |
| 550          | 文件不可用                       |
| 551          | 不知道的页类型                   |
| 552          | 超过存储分配                     |
| 553          | 文件名不允许                     |

转自<http://blog.csdn.net/jinhao2003/archive/2008/03/03/2141378.aspx>





# FTP返回码表示的含义

2015年09月30日 13:50:01 [wjacketcn](https://me.csdn.net/wjacketcn) 阅读数：1598

原创 SU整理（FTP)收藏 

整理如下： 

1开头-成功 

2开头-成功 

3开头-权限问题 

4开头-文件问题 

5开头-服务器问题 

150 FILE: %s 

150 Opening %s mode data connection for %s (%I64d Bytes). 

150 Opening %s mode data connection for %s. 

150 Opening ASCII mode data connection for /bin/ls. 

150 Opening BINARY mode data connection for MLSD. 

200 (end of 'index %s') 

200 Command channel switched to clear-text. 

200 Command OK, %I64d Bytes available on %s 

200 Command okay. 

200 EXEC command successful (TID=%u). 

200 Message received okay. 

200 MLST OPTS 

200 MODE S ok. 

200 MODE Z LEVEL set to %d. 

200 MODE Z ok. 

200 Noted. 

200 Opened OK (but not really). 

200 PBSZ command OK. Protection buffer size set to 0. 

200 PBSZ=0 

200 PORT Command successful. 

200 PROT command OK. Using clear data connection. 

200 PROT command OK. Using private data connection. 

200 SSCN:CLIENT METHOD 

200 SSCN:SERVER METHOD 

200 STRU F ok. 

200 STRU R ok. 

200 Transfer progress notification is off. 

200 Transfer progress notification is on. 

200 Type set to A. 

200 Type set to I. 

202 Command not implemented, superfluous at this site. 

202 Command not implemented, superfluous at this site. 

210 UTC%+ld 

211 End 

211 End of status. 

211-Extension supported 

211-Status for user %Name from %IP: 

213 %I64d 

213 %s 

214 Direct comments or bugs to bugs@bugs.com. 

214- The following commands are recognized (* => unimplemented). 

215 UNIX Type: L8 

220 Service ready for new user. 

220 Serv-U FTP Server v%s for WinSock ready... 

221 Goodbye! 

226 ABOR command successful. 

226 Transfer complete. 

226 Transfer complete. 

226 Transfer complete. 

227 Entering Passive Mode (%d,%d,%d,%d,%d,%d) 

230 Password changed okay. 

230 Switching to SYSTEM MAINTENANCE mode. 

230 User logged in, proceed. 

230 User logged in. 

231 Switching to GROUP MAINTENANCE mode. 

232 Switching to DOMAIN MAINTENANCE mode. 

233 Switching to READ-ONLY MAINTENANCE mode. 

233 User logged in, PASSWORD EXPIRED - please change before proceeding. 

234 AUTH command OK. Already have SSL connection. 

234 AUTH command OK. Initializing SSL connection. 

250 %X 

250 DELE command successful. 

250 Directory changed to %s 

250 End. 

250 RMD command successful. 

250 RNTO command successful. 

250-Listing %s 

253 Attributes changed okay. 

253 Date/time changed okay. 

257 "%s" directory created. 

257 "%s" is current directory. 

321 Transferred %I64d Bytes. 

331 Response to otp-md%d %u %s required for skey. 

331 User name okay, need password. 

331 User name okay, please send complete E-mail address as password. 

350 File or directory exists, ready for destination name 

350 Restarting at %I64d. Send STORE or RETRIEVE. 

421 Data connection closed (SSL/TLS negotiation failed). 

421 Service closing control connection. 

425 Cannot open data connection. 

425 Cannot open data connection. 

425 Cannot open data connection. 

425 Try later, data connection in use. 

425 Try later, data connection in use. 

425 Try later, data connection in use. 

425 Try later, out of network sockets. 

425 Try later, out of network sockets. 

425 Try later, out of network sockets. 

425 Unable to listen, too many pending PASV requests from same client IP. 

425 Unable to set up passive listening socket. 

426 Data connection closed, cannot read file %s. 

426 Data connection closed, cannot write file %s - disk full? 

426 Data connection closed, error compressing data stream. 

426 Data connection closed, error decompressing data stream. 

426 Data connection closed, file transfer %s aborted by client. 

426 Data connection closed, file transfer %s aborted. 

426 Data connection closed, receive file %s aborted. 

426 Data connection closed, transfer aborted. 

426 Transfer aborted. 

431 Account temporarily locked for other user, please try later. 

431 Password not changed, no match for old password. 

431 Unable to negotiate secure command connection. 

432 Password not changed, new password should be at least %d characters in length. 

432 Password not changed, new password should contain at least one upper-case character and symbol 

432 Password not changed, old and new passwords cannot be identical. 

450 Cannot open file %s for reading. 

452 Sorry, insufficient disk quota - receive file %s aborted. 

500 '%s': command not understood. 

501 %s: Not a directory. 

501 Cannot EXEC command line (error=%u). 

501 Invalid MLST options 

501 Invalid option. 

501 Not enough space, only %I64d Bytes available on drive %s 

501 Permission denied. 

501 SITE option not supported. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Syntax error in parameters or arguments. 

501 Wildcards not allowed. 

502 Syntax error - use SITE PSWD "" "". 

503 Bad sequence of commands. 

503 Bad sequence of commands. 

503 Secure command connection required prior to PBSZ command. 

503 Secure command connection required prior to PROT command. 

504 Command not implemented for that parameter. 

504 Command not implemented for that parameter. 

504 Command not implemented for that parameter. 

504 Unknown security mechanism. 

510 Cannot determine server time zone. 

521 Data connection cannot be opened with this PROT setting. 

522 SSL/TLS lib refuses to initiate secure data connection. 

530 Cannot change Anonymous password. 

530 Cannot change password. 

530 Not logged in, cannot find home directory. 

530 Not logged in, home directory does not exist. 

530 Not logged in, need secure connection. 

530 Not logged in, only %d sessions from same IP allowed concurrently. 

530 Not logged in, only one session from same IP allowed at a time. 

530 Not logged in, too many tries with incorrect password. 

530 Not logged in, unauthorized IP address. 

530 Not logged in, use real E-mail address as password. 

530 Not logged in. 

530 Not logged in. 

530 Not logged in. 

530 Only client IP address allowed for PORT command. 

530 Only client IP address allowed to connect to PASV socket. 

530 PASSWORD EXPIRED - please change password before proceeding. 

530 Port number has to be between 1024 and 65535. 

530 Sorry, no ANONYMOUS access allowed. 

531 SSL/TLS lib refuses to initiate secure command connection. 

533 SSL command connection required prior to CCC command. 

534 Invalid option. Secure data connection required for implicit SFTP. 

534 Invalid option. Secure data connection required for implicit SFTP. 

550 %s: Cannot create file. 

550 %s: Cannot delete directory. 

550 %s: Cannot delete file. 

550 %s: Cannot determine date/time. 

550 %s: Failed to create. 

550 %s: No such file or directory. 

550 %s: No such file or directory. 

550 %s: No such file or directory. 

550 %s: No such file or directory. 

550 %s: No such file or directory. 

550 %s: No such file or directory. 

550 %s: No such file or directory. 

550 %s: No such file or directory. 

550 %s: No such file. 

550 %s: No such file. 

550 %s: Permission denied. 

550 %s: Permission denied. 

550 %s: Permission denied. 

550 Cannot change path to a port. 

550 Cannot create a port. 

550 Cannot create INDEX for ports. 

550 Cannot create unique file name. 

550 Cannot delete a port. 

550 Cannot delete account home directory. 

550 Cannot determine date/time for a port. 

550 Cannot determine size of a port. 

550 Cannot EXEC a port. 

550 Cannot list port. 

550 Cannot position file at requested starting point. 

550 Cannot read requested number of bytes from file. 

550 Cannot remove a port. 

550 Cannot rename to a port. 

550 Cannot RESTart at any position while APPEnding to a file. 

550 Cannot RESTart beyond end-of-file. 

550 Cannot RESTart in a (new) unique zero-length file. 

550 Cannot RESTart in a new (zero-length) file. 

550 File date/time cannot be set to future. 

550 File transfer not allowed, server stop is pending. 

550 File transfers not allowed, server stop is pending. 

550 MDTM cannot change date/time on this server. 

550 No files found. 

550 No maintenance manager client. 

550 Passive mode not allowed on this server. 

550 Permission denied 

550 Permission denied. 

550 Permission denied. 

550 Permission denied. 

550 Permission denied. 

550 Permission denied. 

550 Permission denied. 

550 Permission denied. 

550 Permission denied. 

550 Permission denied. 

550 Permission denied. 

550 Sorry, insufficient credit for download - upload first. 

550 Sorry, maximum number of GETs reached for this session. 

550 Sorry, maximum number of PUTs reached for this session. 

553 %s: Cannot change attributes. 

553 %s: Cannot change date/time. 

553 %s: Unable to rename file or directory. 

553 Server Edition does not support remote administration.

 