# 嵌入式 busybox自带的tftp、telnet、ftp服务器 - DoubleLi - 博客园






**a、tftp server服务器配置与使用**

**方法1:**

**#udpsvd -vE 069 tftpd -c /root&**

**//上面的0表示对所有ip地址都进行侦听**

**方法2:**

**#vi/etc/inetd.conf**

**  69 dgram udp nowait root tftpd tftpd -c /root**

**#inetd**



**测试使用：**

**我们可以通过windows下tftp32软件进行下载、上传文件测试**

**将tftp32设为客户端，设置主机ip，输入待发送文件名点击put或准备接收的文件名点击get，如下图所示：**



**b、tftp client的使用**

**tftp32作为服务器**



**1.将开发板上的hello.c文件put到PC机端tftpd32的根目录下,并且重命名为world.c**

**#tftp -l hello.c  -r  world.c -p  (tftp服务器的ip地址)**



**2.将PC上tftp32目录下的world.c文件get到开发板机端,并且重命名为hello word.c**

**# tftp -l hello_world.c  -r  world.c -g  (tftp服务器的ip地址)**

**3、telnet 服务器: **

**#telnetd //默认的端口为23**

**注意：必须在/dev目录下创建pts目录，并挂载devpts文件系统**



**在windows中开始--》运行--》cmd 启动MS-DOS**

**>telnet10.10.1.216(开发板的IP)**

**输入用户名及密码即可登录**



**如果想开机运行**

**在/etc/init.d/rcS中加入**

**/sbin/telnetd& 即可**









