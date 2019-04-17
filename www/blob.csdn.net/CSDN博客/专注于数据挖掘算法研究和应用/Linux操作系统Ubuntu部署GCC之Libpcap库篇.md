# Linux操作系统Ubuntu部署GCC之Libpcap库篇 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年09月10日 11:31:36[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2692








###  Ubuntu操作系统GCC之Libpcap库部署

1．启动虚拟机，进入Ubuntu操作系统，打开firefox浏览器；

2．下载并安装四个组件：

       1）m4源代码：[http://ftp.gnu.org/gnu/m4/](http://ftp.gnu.org/gnu/m4/)

             版本：m4-1.4.16.tar.gz

              解压和安装：

tar -zxvf m4-1.4.16.tar.gz

cd m4-1.4. 16

sudo ./configure

sudo make

sudo make install

    2）flex源代码：[http://prdownloads.sourceforge.net/flex/](http://prdownloads.sourceforge.net/flex/)

版本：flex-2.5.35.tar.bz2

              解压和安装：

tar -zxvf flex-2.5.35.tar.bz2

cd flex-2.5.35

sudo ./configure

sudo make

sudo make install

    3）bison源代码：http://ftp.gnu.org/gnu/bison/ 

版本：bison -2.5.tar.gz

              解压和安装：

tar -zxvf bison -2.5.tar.gz

cd bison -2.5

sudo ./configure

sudo make

sudo make install

    4）libpcap源代码：http://www.tcpdump.org/

版本：libpcap-1.2.0rc1.tar.gz

              解压和安装：

tar -zxvf libpcap-1.2.0rc1.tar.gz

cd libpcap-1.2.0

sudo ./configure

sudo make

sudo make install

3．编译执行下面代码通过：

#include<stdio.h>

#include<pcap.h>

int main()

{

char *dev, errbuf[PCAP_ERRBUF_SIZE];

dev=pcap_lookupdev(errbuf);

if(dev==NULL){

fprintf(stderr,"couldn't find default device: %s\n",errbuf);

return(2);

}

printf("Device: %s\n",dev);

return(0);

}

===============================================================

1）编译这段代码： gcc -o testout test.c -lpcap

   以管理员非root身份执行：./testout

   输出结果：couldn't find default device: no suitable device found；

          需激活root身份并以root身份执行程序；

===============================================================

2）激活命令式root身份，终端窗口执行下面命令：

切换到root身份：sudo -i

设置root身份密码：sudo passwd root

Enter password for root

Confirm password for root 

激活图形化 root 登陆：注销后进入登陆界面，选择“其他”，输入用户名：root，密码为命令式激活时所设置的；

===============================================================

3）以root身份执行成功输出结果：Device: eth0

——执行命令一：sudo su，直接切换到root下的对应目录执行程序：

              fjs@fjs-virtualBox:~$ cd demo

fjs@fjs-virtualBox:~/demo$ sudo -su

[sudo]password for fjs:

              root@fjs-virtualBox:/home/fjs/demo# ./testout

——执行命令二：sudo -i，切换到root后用cd命令定位到/home下的对应目录执行程序：

              fjs@fjs-virtualBox:~$ sudo –i

              root@fjs-virtualBox:~# cd /home/fjs/demo

    root@fjs-virtualBox: /home/fjs/demo# ./testout

===============================================================

4）执行提示：error while loading shared libraries: libpcap.so.1: cannot open shared object file: No such file or directory；

以root身份登陆解决：

——确认libpcap.so.1在/usr/local/lib目录下；

——打开/etc目录下的文件ld.so.conf，添加两行（非root身份不能修改该文件）：

/usr/local/lib

/usr/lib

——在root终端下执行从新加载配置的命令：

加载：ldconfig

显示条件为libpcap的文件：ldconfig –v|grep libpcap

===============================================================
            


