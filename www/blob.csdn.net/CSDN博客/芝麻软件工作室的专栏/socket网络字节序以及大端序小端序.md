
# socket网络字节序以及大端序小端序 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月02日 08:05:21[seven-soft](https://me.csdn.net/softn)阅读数：2968


不同CPU中，4字节整数1在内存空间的存储方式是不同的。4字节整数1可用2进制表示如下：00000000 00000000 00000000 00000001
有些CPU以上面的顺序存储到内存，另外一些CPU则以倒序存储，如下所示：00000001 00000000 00000000 00000000
若不考虑这些就收发数据会发生问题，因为保存顺序的不同意味着对接收数据的解析顺序也不同。
## 大端序和小端序
CPU向内存保存数据的方式有两种：大端序（Big Endian）：高位字节存放到低位地址（高位字节在前）。
小端序（Little Endian）：高位字节存放到高位地址（低位字节在前）。
仅凭描述很难解释清楚，不妨来看一个实例。假设在 0x20 号开始的地址中保存4字节 int 型数据 0x12345678，大端序CPU保存方式如下图所示：![](http://c.biancheng.net/cpp/uploads/allimg/151109/1-15110ZSA3309.jpg)
图1：整数 0x12345678 的大端序字节表示
对于大端序，最高位字节 0x12 存放到低位地址，最低位字节 0x78 存放到高位地址。小端序的保存方式如下图所示：![](http://c.biancheng.net/cpp/uploads/allimg/151109/1-15110ZT059560.jpg)
图2：整数 0x12345678 的小端序字节表示
不同CPU保存和解析数据的方式不同（主流的Intel系列CPU为小端序），小端序系统和大端序系统通信时会发生数据解析错误。因此在发送数据前，要将数据转换为统一的格式——网络字节序（Network Byte Order）。网络字节序统一为大端序。
主机A先把数据转换成大端序再进行网络传输，主机B收到数据后先转换为自己的格式再解析。
## 网络字节序转换函数
在《[使用bind()和connect()函数](http://c.biancheng.net/cpp/html/3033.html)》一节中讲解了
 sockaddr_in 结构体，其中就用到了网络字节序转换函数，如下所示：//创建sockaddr_in结构体变量
structsockaddr_inserv_addr;
memset(&serv_addr,0,sizeof(serv_addr));//每个字节都用0填充
serv_addr.sin_family=AF_INET;//使用IPv4地址
serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");//具体的IP地址
serv_addr.sin_port=htons(1234);//端口号

htons() 用来将当前主机字节序转换为网络字节序，其中`h`代表主机（host）字节序，`n`代表网络（network）字节序，`s`代表short，htons
 是 h、to、n、s 的组合，可以理解为”将short型数据从当前主机字节序转换为网络字节序“。
常见的网络字节转换函数有：htons()：host to network short，将short类型数据从主机字节序转换为网络字节序。
ntohs()：network to host short，将short类型数据从网络字节序转换为主机字节序。
htonl()：host to network long，将long类型数据从主机字节序转换为网络字节序。
ntohl()：network to host long，将long类型数据从网络字节序转换为主机字节序。
通常，以`s`为后缀的函数中，`s`代表2个字节short，因此用于端口号转换；以`l`为后缀的函数中，`l`代表4个字节的long，因此用于IP地址转换。
举例说明上述函数的调用过程：\#include<stdio.h>
\#include<stdlib.h>
\#include<WinSock2.h>
\#pragmacomment(lib,"ws2_32.lib")
intmain(){
unsignedshorthost_port=0x1234,net_port;
unsignedlonghost_addr=0x12345678,net_addr;
net_port=htons(host_port);
net_addr=htonl(host_addr);
printf("Host ordered port: %\#x\n",host_port);
printf("Network ordered port: %\#x\n",net_port);
printf("Host ordered address: %\#lx\n",host_addr);
printf("Network ordered address: %\#lx\n",net_addr);
system("pause");
return0;
}

运行结果：
Host ordered port: 0x1234
Network ordered port: 0x3412
Host ordered address: 0x12345678
Network ordered address: 0x78563412
另外需要说明的是，sockaddr_in 中保存IP地址的成员为32位整数，而我们熟悉的是点分十进制表示法，例如 127.0.0.1，它是一个字符串，因此为了分配IP地址，需要将字符串转换为4字节整数。
inet_addr() 函数可以完成这种转换。inet_addr() 除了将字符串转换为32位整数，同时还进行网络字节序转换。请看下面的代码：\#include<stdio.h>
\#include<stdlib.h>
\#include<WinSock2.h>
\#pragmacomment(lib,"ws2_32.lib")
intmain(){
char*addr1="1.2.3.4";
char*addr2="1.2.3.256";
unsignedlongconv_addr=inet_addr(addr1);
if(conv_addr==INADDR_NONE){
puts("Error occured!");
}else{
printf("Network ordered integer addr: %\#lx\n",conv_addr);
}
conv_addr=inet_addr(addr2);
if(conv_addr==INADDR_NONE){
puts("Error occured!");
}else{
printf("Network ordered integer addr: %\#lx\n",conv_addr);
}
system("pause");
return0;
}

运行结果：
Network ordered integer addr: 0x4030201
Error occured!
从运行结果可以看出，inet_addr() 不仅可以把IP地址转换为32位整数，还可以检测无效IP地址。
注意：为 sockaddr_in 成员赋值时需要显式地将主机字节序转换为网络字节序，而通过 write()/send() 发送数据时TCP协议会自动转换为网络字节序，不需要再调用相应的函数。

