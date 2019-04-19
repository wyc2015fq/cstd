# FTP命令 - xqhrs232的专栏 - CSDN博客
2017年12月23日 19:27:49[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：180
原文地址::[https://www.cnblogs.com/wangyuqin/p/6099242.html](https://www.cnblogs.com/wangyuqin/p/6099242.html)
相关文章
1、FTP协议中的RETR和STOR命令----[http://blog.csdn.net/petershina/article/details/8237301](http://blog.csdn.net/petershina/article/details/8237301)
主要用到的 FTP 命令
FTP 每个命令都有 3 到 4 个字母组成，命令后面跟参数，用空格分开。每个命令都以 "\r\n"结束。
要下载或上传一个文件，首先要登入 FTP 服务器，然后发送命令，最后退出。这个过程中，主要用到的命令有 USER、PASS、SIZE、REST、CWD、RETR、PASV、PORT、QUIT。
USER: 指定用户名。通常是控制连接后第一个发出的命令。“USER gaoleyi\r\n”： 用户名为gaoleyi 登录。
PASS: 指定用户密码。该命令紧跟 USER 命令后。“PASS gaoleyi\r\n”：密码为 gaoleyi。
SIZE: 从服务器上返回指定文件的大小。“SIZE file.txt\r\n”：如果 file.txt 文件存在，则返回该文件的大小。
CWD: 改变工作目录。如：“CWD dirname\r\n”。
PASV: 让服务器在数据端口监听，进入被动模式。如：“PASV\r\n”。
PORT: 告诉 FTP 服务器客户端监听的端口号，让 FTP 服务器采用主动模式连接客户端。如：“PORT h1,h2,h3,h4,p1,p2”。
RETR: 下载文件。“RETR file.txt \r\n”：下载文件 file.txt。
STOR: 上传文件。“STOR file.txt\r\n”：上传文件 file.txt。
REST: 该命令并不传送文件，而是略过指定点后的数据。此命令后应该跟其它要求文件传输的 FTP 命令。“REST 100\r\n”：重新指定文件传送的偏移量为 100 字节。
QUIT: 关闭与服务器的连接。
