# Linux下rz，sz与ssh的配合使用，实现文件传输 - 在思索中前行！ - CSDN博客





2014年09月24日 19:47:16[_Tham](https://me.csdn.net/txl16211)阅读数：925标签：[linux文件传输](https://so.csdn.net/so/search/s.do?q=linux文件传输&t=blog)
个人分类：[linux学习																[电脑杂料](https://blog.csdn.net/txl16211/article/category/2373823)](https://blog.csdn.net/txl16211/article/category/2231071)








   一般来说，[linux](http://www.cnblogs.com/276815076/admin/javascript:;)[服务器](http://www.cnblogs.com/276815076/admin/javascript:;)大多是通过[ssh](http://www.cnblogs.com/276815076/admin/javascript:;)客户端来进行远程的登陆和[管理](http://www.cnblogs.com/276815076/admin/javascript:;)的，使用ssh登陆linux主机以后，如何能够快速的和本地机器进行文件的交互呢，也就是上传和下载文件到服务器和本地；
   与ssh有关的两个命令可以提供很方便的操作：
sz：将选定的文件发送（send）到本地机器
   rz：运行该命令会弹出一个文件选择窗口，从本地选择文件上传到服务器(receive)

rz，sz是便是Linux/Unix同[Windows](http://www.cnblogs.com/276815076/admin/javascript:;)进行ZModem文件传输的命令行工具

windows端需要支持ZModem的telnet/ssh客户端（比如SecureCRT）

运行命令rz，即是接收文件，SecureCRT就会弹出文件选择对话框，选好文件之后关闭对话框，文件就会上传到当前目录

注意：单独用rz会有两个问题：上传中断、上传文件变化（md5不同），解决办法是上传是用rz -be，并且去掉弹出的对话框中“Upload files as ASCII”前的勾选。
-b binary 用binary的方式上传下载，不解释字符为ascii
-e 强制escape 所有控制字符，比如Ctrl+x，DEL等

运行命令sz file1 file2就是发文件到windows上（保存的目录是可以配置） 比ftp命令方便多了，而且服务器不用再开FTP服务了

PS：Linux上rz/sz这两个小工具安装lrzsz-x.x.xx.rpm即可。

当然，还可以设置一下目录了：

在SecureCRT设置一下上传和下载的默认目录
options–>session options–>file transfer 下可以设置上传和下载的目录
剩下的你只要在用SecureCRT登陆linux终端的时候：
发送文件到客户端：sz filename
zmodem接收可以自行启动.
从客户端上传文件到linux服务端：
只要服务端执行 : rz
然后在 SecureCRT 里选文件发送,协议 zmodem




