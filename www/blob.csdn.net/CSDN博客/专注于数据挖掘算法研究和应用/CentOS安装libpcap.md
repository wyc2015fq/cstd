# CentOS安装libpcap - 专注于数据挖掘算法研究和应用 - CSDN博客





2012年01月12日 14:51:27[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：12578








1.安装GCC：

  yum -y install gcc-c++

2.安装flex： 

  yum -y install flex 

  没有flex，直接安装libpcap会提示"Your operating system's lex is insufficient to compile libpcap"错误；


3.安装bison

  yum -y install bison

  前面安装的是flex，就需要搭配bison，如不会提示"don't have both flex and bison;reverting to lex/yacc"错误；

4.安装 libpcap

  下载地址：[http://www.tcpdump.org/](http://www.tcpdump.org/)

  下载版本：libpcap-1.2.1.tar.gz

   命令如下：

   ./configure

   make

   make install 

5.简单的例子测试一下libpcap：

  //device.c 

  #include <stdio.h>

  #include <pcap.h>

  int main(int argc,char *argv[]){

      char *dev, errbuf[PCAP_ERRBUF_SIZE];

      dev=pcap_lookupdev(errbuf);

      if(dev==NULL){

         fprintf(stderr,"couldn't find default device: %s\n",errbuf);

         return(2);

      }

      printf("Device: %s\n",dev);

      return(0);

  }

  编译指令：gcc -o device device.c -lpcap

  备注：编译时要使用libpcap的参数-lpcap，否则会提示“pcap_lookupdev 未定义的引用”的错误；

  运行指令：./device 



