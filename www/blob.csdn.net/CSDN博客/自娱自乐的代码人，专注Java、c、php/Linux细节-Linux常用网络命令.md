# Linux细节 - Linux常用网络命令 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月22日 16:00:22[initphp](https://me.csdn.net/initphp)阅读数：962








**Linux常用网络命令**
|**命令**|**说明**|**使用**|
|----|----|----|
|telnet|用telnet协议与另外主机通信|telnet 127.0.0.1telnet locahost 80|
|ssh|使用 SSH 协议登录远程主机的客户端|ssh root@127.0.0.1|
|scp|基于 SSH 协议在本地主机和远程主机之间复制文件|scp localfile osmond@192.168.0.100:~/remotefile|
|ftp|ftp方式登录网站|ftp 127.0.0.1|
|wget|使用 HTTP 和 FTP 协议，支持代理服务器和断点续传，是基于控制台最强大的下载工具。|wget [http://initphp.com/down.php?id=1](http://initphp.com/down.php?id=1)  下载文件|

****

**FTP操作命令**
|**命令**|**说明**|**使用**|
|----|----|----|
|？|用来列出ftp子命令||
|pwd|显示远程主机上的当前目录。||
|ls|使用UNIX命令列出当前远程目录的内容。||
|cd|在远程主机中切换目录。||
|lcd|在本地主机中切换目录。||
|get|从远程主机当前目录下下传一个文件。||
|mget|从远程主机当前目录下下传多个文件（文件名中可包含通配符）。||
|put|上传一个文件到远程主机的当前目录。||
|mput|上传多个文件到远程主机的当前目录。||
|mkdir|在远程主机上创建目录。||
|rmdir|删除远程主机上的目录。||
|chmod|修改远程主机上文件或目录的权限。||
|open|打开一个新的FTP连接。||
|close|关闭FTP连接。||
|bye|断开与远程主机的连接。||







