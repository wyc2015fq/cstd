# 基于UDT connect连接通信以及文件传输--客户端 - DoubleLi - 博客园






上面一篇文章中提出了服务端的，其实这里没有严格意义的服务端和客户端之分，因为我在代码中是基于UDP的，不存在服务端与客户端，两个都是对等的，只是我这里进行一下简单的区分而已。在这里，客户端所进行的主要操作就是首先给服务端发送文件名，然后将文件直接通过fopen(),fread()等一系列的操作将文件发送过去。

        与客户端相对应的源码在另一篇博文中：[http://blog.csdn.net/pingd/article/details/16341467](http://blog.csdn.net/pingd/article/details/16341467)

##         CLIENT端




**[cpp]**[view plain](http://blog.csdn.net/pingd/article/details/16344187#)[copy](http://blog.csdn.net/pingd/article/details/16344187#)



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
- if ((argc != 5))  
-      {  
-         cout<<"Usage: appclient.exe client_port server_ip server_port local_filename"<<endl;  
- return 0;  
-      }  
- 
- //startup  
- if (UDT::ERROR == UDT::startup())  
-      {  
-         cout<<"startup: "<<UDT::getlasterror().getErrorMessage()<<endl;  
-      }else{  
-         cout<<"startup suc..."<<endl;  
-      }  
- 
- //Initialize the UDT library  
-     UDTSOCKET client = UDT::socket(AF_INET, SOCK_DGRAM, 0);  
- if (UDT::ERROR == client)  
-     {  
-         cout<<"socket: "<<UDT::getlasterror().getErrorMessage()<<endl;  
-     }else{  
-         cout<<"client suc..."<<endl;  
-     }  
- 
- //声明udp socket  
- int clientsocket = socket(AF_INET,SOCK_DGRAM,0);  
- if (SOCKET_ERROR == clientsocket)  
-     {  
-         cout<<"udp socket error!"<<endl;  
-     }else{  
-         cout<<"clientsocket suc..."<<endl;  
-     }  
- 
-      sockaddr_in serv_addr,my_addr;  
-      serv_addr.sin_family = AF_INET;  
-      serv_addr.sin_port = htons(atoi(argv[3]));  
-      serv_addr.sin_addr.s_addr = inet_addr(argv[2]);  
-      memset(&(serv_addr.sin_zero), '\0', 8);  
- 
-      my_addr.sin_family = AF_INET;  
-      my_addr.sin_port = htons(atoi(argv[1]));  
-      my_addr.sin_addr.s_addr = INADDR_ANY;  
-      memset(&(my_addr.sin_zero), '\0', 8);  
-      bind(clientsocket,(struct sockaddr*)&my_addr,sizeof(my_addr));  
- 
- int mss = 1052;//最大传输单位  
- //设置收发缓冲区大小 接收限时  和地址重用  
- if(   !( UDT::ERROR != (UDT::setsockopt(client, 0, UDT_SNDBUF, new int(32000), sizeof(int)))  
-         && UDT::ERROR != (UDT::setsockopt(client, 0, UDP_RCVBUF, new int(32000), sizeof(int)))  
-         && UDT::ERROR != (UDT::setsockopt(client,0,UDT_REUSEADDR,new int(1),sizeof(int)))  
-         && UDT::ERROR != (UDT::setsockopt(client, 0, UDT_RENDEZVOUS, new bool(true), sizeof(bool))))  
-         && UDT::ERROR != (UDT::setsockopt(client, 0, UDT_MSS, &mss, sizeof(int))))  
-     {  
-         cout<<"udt socket: "<<UDT::getlasterror().getErrorMessage()<<endl;  
-         UDT::close(client);  
- return 0;  
-     }  
- 
- if (UDT::ERROR == UDT::bind2(client,clientsocket))  
-      {  
-         cout<<"udt bind2:"<<UDT::getlasterror().getErrorMessage()<<endl;  
- return 0;  
-      }else{  
-         cout<<"bind2 suc"<<endl;  
-      }  
- 
- // connect to the server, implict bind  
- if (UDT::ERROR == UDT::connect(client, (sockaddr*)&serv_addr, sizeof(serv_addr)))  
-      {  
-        cout << "connect: " << UDT::getlasterror().getErrorMessage();  
-        UDT::close(client);  
- return 0;  
-      }else{  
-         cout<<"connect suc"<<endl;  
-      }  
- 
- char* hello = argv[4];  
- if (UDT::ERROR == UDT::sendmsg(client, hello, strlen(hello) + 1,-1,true))  
-      {  
-        cout << "send: " << UDT::getlasterror().getErrorMessage();  
- return 0;  
-      }  
- 
- FILE *fp;  
-      fp = fopen(hello,"rb");  
-      fseek(fp,0,SEEK_END);  
- int filesize = ftell(fp);  
- //rewind(fp);//移动到头部  
-      fseek(fp,0,SEEK_SET);  
- static int filepos = 0;//记录文件偏移量  
-      cout<<"filesize = "<<filesize<<endl;  
- 
- char data[MAXLEN+1];  
- int len=0,package=0;  
-      UDT::TRACEINFO trace;  
-      UDT::perfmon(client,&trace);  
- while(1)  
-      {  
-         memset(data,0,sizeof(data));  
-         fread(data,MAXLEN,1,fp);  
- if(filesize>=MAXLEN)  
-         {  
-             len = UDT::sendmsg(client,data,MAXLEN,-1,true);  
- if (len<0)  
-             {  
-                 cout<<"send failure!!\n"<<endl;  
- break;  
-             }  
-             filesize -= MAXLEN;  
-             package ++;//record send all the packages  
-         }else  
-         {  
-             len = UDT::sendmsg(client,data,filesize,-1,true);  
- if (len<0)  
-             {  
-                 cout<<"send failure!!\n"<<endl;  
- break;  
-             }  
-             package ++;//record recv all the packages  
- char *quit = "quit";  
- if (UDT::ERROR == UDT::sendmsg(client,quit,strlen(quit)+1,-1,true))  
-             {  
-                 cout<<"send quit error"<<endl;  
-                 fclose(fp);  
- break;  
-             }  
- break;  
-         }  
-      }  
-      UDT::perfmon(client,&trace);  
-      cout << "speed = " << trace.mbpsSendRate << "Mbits/sec" << endl;  
-      cout<<"send all the packages: "<<package<<endl;  
- 
-      fclose(fp);  
-      UDT::close(client);  
-      UDT::close(clientsocket);  
-      UDT::cleanup();  
- 
- return 1;  
-  }  


运行效果图：

![](http://img.blog.csdn.net/20131115191007031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGluZ0Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



注：要想运行这两个程序直接把源代码复制下来，然后去运行肯定是不行的，这个是需要UDT其它类库的支持的，也就是说，需要你将UDT的源代码文件拷贝进你的项目文件中才行。源代码下载地址：[http://blog.csdn.net/pingd/article/details/14519881](http://blog.csdn.net/pingd/article/details/14519881)

简单截图如下：

![](http://img.blog.csdn.net/20131115191655843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGluZ0Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)









