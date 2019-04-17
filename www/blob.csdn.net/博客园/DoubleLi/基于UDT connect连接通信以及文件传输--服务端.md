# 基于UDT connect连接通信以及文件传输--服务端 - DoubleLi - 博客园






网上与UDT相关的资料不多，与UDT相关的源码例子更少。最近在接触UDT，也是因为缺少相关的资料，导致学习起来甚感痛苦。下面将我自己这两天弄出来的代码贴出来，希望对在寻找相关资料的童鞋有一定的帮助。与服务端相对应的客户端在另一篇博文中



**[cpp]**[view plain](http://blog.csdn.net/pingd/article/details/16341467#)[copy](http://blog.csdn.net/pingd/article/details/16341467#)



- 


##         SERVER端




**[cpp]**[view plain](http://blog.csdn.net/pingd/article/details/16341467#)[copy](http://blog.csdn.net/pingd/article/details/16341467#)



- #include <iostream>  
- #include "udt.h"  
- #include <io.h>  
- 
- #pragma comment(lib,"ws2_32.lib")  
- 
- using namespace std;  
- 
- #define MAXLEN 4096  
- 
- int main(int argc,char *argv[])  
-  {     
- if ((argc != 4))  
-      {  
-         cout<<"Use: appserver.exe server_port client_ip client_port"<<endl;  
- return 0;  
-      }  
- 
- //startup  
- //这里是对UDT的启动记性初始化操作  
- if (UDT::ERROR == UDT::startup())  
-      {  
-          cout<<"startup: "<<UDT::getlasterror().getErrorMessage()<<endl;  
-      }else{  
-         cout<<"startup suc..."<<endl;  
-      }  
- 
- //socket  
- //像声明一个普通的socket一样声明一个UDTSOCKET  
-      UDTSOCKET serv = UDT::socket(AF_INET, SOCK_DGRAM, 0);  
- if (UDT::ERROR == serv)  
-     {  
-         cout<<"socket: "<<UDT::getlasterror().getErrorMessage()<<endl;  
-     }else{  
-         cout<<"client suc..."<<endl;  
-     }  
- 
- //声明udp socket，这里是udp的哈，不是udt  
- int sersocket = socket(AF_INET,SOCK_DGRAM,0);  
- if (SOCKET_ERROR == sersocket)  
-     {  
-         cout<<"udp socket error!"<<endl;  
-     }else{  
-         cout<<"clientsocket suc..."<<endl;  
-     }  
- 
- //为了能够在局域网中直接进行处理，先默认设置两个  
-      sockaddr_in my_addr,client_addr;  
-      my_addr.sin_family = AF_INET;  
-      my_addr.sin_port = htons(atoi(argv[1]));  
-      my_addr.sin_addr.s_addr = INADDR_ANY;  
-      memset(&(my_addr.sin_zero), '\0', 8);  
-      bind(sersocket,(struct sockaddr*)&my_addr,sizeof(my_addr));  
- 
-      client_addr.sin_family = AF_INET;  
-      client_addr.sin_port = htons(atoi(argv[3]));  
-      client_addr.sin_addr.s_addr = inet_addr(argv[2]);  
- //client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  
-      memset(&(client_addr.sin_zero), '\0', 8);  
- 
- int mss = 1052;//最大传输单位  
- //设置收发缓冲区大小 接收限时  和地址重用  
- if(   !( UDT::ERROR != (UDT::setsockopt(serv, 0, UDT_SNDBUF, new int(32000), sizeof(int)))  
-         && UDT::ERROR != (UDT::setsockopt(serv, 0, UDP_RCVBUF, new int(32000), sizeof(int)))  
-         && UDT::ERROR != (UDT::setsockopt(serv,0,UDT_REUSEADDR,new int(1),sizeof(int)))  
-         && UDT::ERROR != (UDT::setsockopt(serv, 0, UDT_RENDEZVOUS, new bool(true), sizeof(bool))))  
-         && UDT::ERROR != (UDT::setsockopt(serv, 0, UDT_MSS, &mss, sizeof(int)) ))  
-     {  
-         cout<<"udt socket: "<<UDT::getlasterror().getErrorMessage()<<endl;  
-         UDT::close(serv);  
- return 0;  
-     }  
- //这里是直接将udp的接口绑定在udt的接口之上，如果不这样做的话是没法使用UDT中的SOCK_DGRAM的  
- if (UDT::ERROR == UDT::bind2(serv,sersocket))  
-      {  
-         cout<<"udt bind2:"<<UDT::getlasterror().getErrorMessage()<<endl;  
- return 0;  
-      }else{  
-         cout<<"bind2 suc"<<endl;  
-      }  
- //这里也是关键部分，与client端对应的connect操作，就是相互之间的打洞处理  
- if (UDT::ERROR == UDT::connect(serv, (sockaddr*)&client_addr, sizeof(client_addr)))  
-      {  
-        cout << "connect: " << UDT::getlasterror().getErrorMessage();  
-        UDT::close(serv);  
- return 0;  
-      }else{  
-         cout<<"connetc suc"<<endl;  
-      }  
- //这里已经可以正常接收了，接收从client发过来的filename，目的是用于本地的文件创建  
- char filename[100];  
- if (UDT::ERROR == UDT::recvmsg(serv, filename, 100))  
-      {  
-        cout << "recv:" << UDT::getlasterror().getErrorMessage() << endl;  
- return 0;  
-      }  
-      cout <<"filename: "<< filename <<endl;  
- 
- //使用FILE进行文件操作，关于文件的相关操作这里不详述了，实在不懂的可以留言  
- FILE *fp;  
- char localfile[100];  
-      memset(localfile,0,sizeof(localfile));  
-      strcpy(localfile,"d:\\");  
-      strcat(localfile,filename);  
- if((fp = fopen(localfile,"w+"))==NULL)  
-      {  
-         cout<<filename<<" open failure!"<<endl;  
- return 0;  
-      }  
-      fclose(fp);  
-      fp = fopen(localfile,"wb");  
- 
- char data[MAXLEN];  
- int len=0,package=0,filelen=0;  
-      UDT::TRACEINFO trace;  
-      UDT::perfmon(serv,&trace);    
- while(1)  
-      {  
- //前面部分打开文件后，这里就是循环接收文件并保存  
-         memset(data,0,sizeof(data));  
-         len = UDT::recvmsg(serv, data, MAXLEN);  
-         filelen += len;  
- //cout<<"filelen = "<<filelen<<endl;  
- if (strncmp("quit",data,4)==0)  
-         {  
-             cout<<data<<endl;  
-             fclose(fp);  
- break;  
-         }else  
-         {  
-             package ++;//record recv all the packages  
-         }  
-         fwrite(data,len,1,fp);        
-      }  
-      fclose(fp);  
-      fp = fopen(localfile,"rb");  
-      fseek(fp,0,SEEK_END);//move to the end  
-      filelen = ftell(fp)  
-      fseek(fp,0,SEEK_SET);  
-      cout<<"filesize = "<<filelen<<endl;  
-      UDT::perfmon(serv,&trace);  
-      cout << "speed = " << trace.mbpsRecvRate << "Mbits/sec" << endl;  
-      cout<<"recv all the packages: "<<package<<endl;  
- 
-      fclose(fp);  
-      UDT::close(serv);  
-      UDT::cleanup();  
- 
- return 1;  
-  }   










运行效果截图：

![](http://img.blog.csdn.net/20131115185634203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGluZ0Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




注：要想运行这两个程序直接把源代码复制下来，然后去运行肯定是不行的，这个是需要UDT其它类库的支持的，也就是说，需要你将UDT的源代码文件拷贝进你的项目文件中才行。源代码下载地址：[http://blog.csdn.net/pingd/article/details/14519881](http://blog.csdn.net/pingd/article/details/14519881)

简单截图如下：
![](http://img.blog.csdn.net/20131115191831593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGluZ0Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)










