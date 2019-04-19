# windows与linux之间文件的传输方式总结 - 零度的博客专栏 - CSDN博客
2016年07月18日 17:20:23[零度anngle](https://me.csdn.net/zmx729618)阅读数：3374
                
当然，windows与linux之间文件的传输的两种方式有很多，这里就仅仅列出工作中遇到的，作为笔记；
**方法一：安装SSH Secure Shell Client客户端**
安装即可登录直接拖拉到linux，如果从linux下导出文件则需要如此操作
![](http://images.cnitblog.com/blog/418191/201306/05134336-1a61880519404a32810b1c63579a9a05.png)
文件自动回到windows窗口的当前目录；
**方法2：通过windows cmd窗口命令**
Microsoft Windows XP [版本 5.1.2600]
(C) 版权所有 1985-2001 Microsoft Corp.
C:\Documents and Settings\Administrator>d:
D:\>e:
step1:ftp登录
E:\>ftp 128.8.28.212
Connected to 128.8.28.212.
220 (vsFTPd 2.2.2)
User (128.8.28.212:(none)): oracle
331 Please specify the password.
Password:
230 Login successful.
step2:查看目录中的文件列表
ftp> cd wangxj
250 Directory successfully changed.
ftp> ls
200 PORT command successful. Consider using PASV.
150 Here comes the directory listing.
enfo_ods.dmp
enfo_ods.log
tsubject.dmp
tsubject.log
226 Directory send OK.
ftp: 收到 56 字节，用时 0.00Seconds 56000.00Kbytes*step3:从linux当前目录下载文*ftp> get enfo_ods.dmp200 PORT command successful. Consider using
 PASV150 Opening BINARY mode data connection for enfo_ods.dmp (325029888 bytes)226 Transfer complete.ftp: 收到 325029888 字节，用时 27.39Seconds 11866.74Kbytes/sec.
ftp> get enfo_ods.log
200 PORT command successful. Consider using PASV.
150 Opening BINARY mode data connection for enfo_ods.log (13704 bytes).
226 Transfer complete.
ftp: 收到 13704 字节，用时 0.00Seconds 13704000.00Kbytes/sec.
*step4:从windows上传文件到linux*
ftp> put test.ktr;
test.ktr;: File not found
//退出ftp 或者用bye
ftp> quit
E:\>ftp 128.8.28.212
*Connected to 128.8.28.212.220 (vsFTPd 2.2.2)User (128.8.28.212:(none)): oracle331 Please specify the password.Password:230 Login successful.ftp> put E:\test.ktr200 PORT command successful. Consider using PASV.150 Ok to send data.226 Transfer complete.ftp: 发送 23410 字节，用时 0.00Seconds 23410000.00Kbytes/sec.ftp>*
*备注：*
  ftp是通用协议，不区分系统的。
<span id="__mceDel"><em>知道linux机器的IP即可。
开始 -> 运行，输入cmd并回车，键入以下命令（前面的>为命令提示符，不用输入）：
> ftp Linux机器的IP地址
> binary
> put windows系统本地文件全路径 /linux系统上的目标路径
> bye
  
<span style="color:#ff00;line-height: 1.5;">其中，如果是文本文件，可以不用binary设置为二进制传输模式（默认文本模式，会自动转换文本格式）。如果是非文本文件（例如可执行文件），建议一定要设置为 binary 模式。</span></em></span>
