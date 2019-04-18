# 纪一次TCP/IP连接关闭全程 - gauss的专栏 - CSDN博客
2015年04月17日 01:39:40[gauss](https://me.csdn.net/mathlmx)阅读数：638
TCP/IP众所周知在连接的时候，需要经历三次握手，而在终止的时候需要经历四次（有某些是以三次实现）握手才能“完美的”终止一次TCP/IP，因为TCP/IP的可靠性导致了一种互不信任的通信模式。故非此周折；
TCP/IP三次握手过程。
![](http://hi.csdn.net/attachment/201005/29/0_1275113181zj1g.gif)
三次握手好理解。下面是TCP/IP关闭的四次握手流程图；
![](http://hi.csdn.net/attachment/201005/29/0_12751131862Z76.gif)
当一方发送close的时候，则向服务器发送一个FIN报文分节。得到服务器的响应，这个时候，客户机无法再进行任何的读写，也许会存在位于客户机套接字发送缓存区的数据没有发送的问题。这个时候，可以选择设置SO_LINGER选项。将close进行限时阻塞。这个时候服务器如果进行读写的话，则读会读取到流结束符，写会导致异常抛出。
C++服务器代码：
- #define LINKQ 200  
- 
- int main(int args,char *argc[]){  
-     int listenfd;  
- 
-     listenfd=socket(AF_INET,SOCK_STREAM,0);  
- 
-     /**display the sock send buf size*/  
- 
-     int sndbufold;  
- 
-     socklen_t sndbufoldsize=sizeof(sndbufold);  
- 
-     getsockopt(listenfd,SOL_SOCKET,SO_SNDBUF,&sndbufold,&sndbufoldsize);  
- 
-     cout << "--" << sndbufold << "--" << endl;  
- 
-     sndbufold=1000;  
- 
-     setsockopt(listenfd,SOL_SOCKET,SO_SNDBUF,&sndbufold,sndbufoldsize);  
- 
- 
-     int sndbufold2;  
- 
-     socklen_t sndbufoldsize2=sizeof(sndbufold2);  
- 
- 
-     getsockopt(listenfd,SOL_SOCKET,SO_SNDBUF,&sndbufold2,&sndbufoldsize2);  
- 
-     cout << "--" << sndbufold2 << "--" << endl;  
- 
- 
-     /**display the sock send buf size*/  
- 
- 
-     struct sockaddr_in addr;  
-     bzero(&addr,sizeof(addr));  
-     addr.sin_family=AF_INET;  
-     addr.sin_port=htons(10355);  
-     addr.sin_addr.s_addr=htonl(INADDR_ANY);  
-     if((bind(listenfd,(struct sockaddr *)&addr,sizeof(addr)))<0){  
-         perror("error:");  
-     }  
-     listen(listenfd,LINKQ);  
- 
-     struct sockaddr_in clientaddr;  
- 
-     socklen_t clientlen;  
- 
- 
-     int clientfd=accept(listenfd,(struct sockaddr *)&clientaddr,&clientlen);  
- 
-     int sndbufold3=200;  
- 
-     socklen_t sndbufoldsize3=sizeof(sndbufold3);  
- 
-     setsockopt(clientfd,SOL_SOCKET,SO_SNDBUF,&sndbufold3,sndbufoldsize3);  
- 
-     int sndbufold4;  
- 
-     socklen_t sndbufoldsize4=sizeof(sndbufold4);  
- 
- 
-     getsockopt(listenfd,SOL_SOCKET,SO_SNDBUF,&sndbufold4,&sndbufoldsize4);  
- 
-     cout << "client:" << sndbufold4 <<endl;  
- 
-     char res[5000];  
- 
-     memset(res,'a',5000);  
- 
- 
-     int wlen;  
- 
-     for(;;){  
-         cout << "go to read" << endl <<flush;  
-         int readsize=read(clientfd,res,100);  
-         cout << "read back:" << readsize << endl << flush;  
-     }  
- 
- }  
服务器主要是读从客户端中读取字节。
C++客户端：
- int main(int args,char *argc[]){  
- 
-          int listenfd;  
- 
-          listenfd=socket(AF_INET,SOCK_STREAM,0);  
- 
- 
- 
-          /**display the sock send buf size*/  
- 
- 
- 
-          int sndbufold;  
- 
- 
- 
-          socklen_t sndbufoldsize=sizeof(sndbufold);  
- 
- 
- 
-          getsockopt(listenfd,SOL_SOCKET,SO_RCVBUF,&sndbufold,&sndbufoldsize);  
- 
- 
- 
-          cout << "--" << sndbufold << "--" << endl;  
- 
- 
- 
-          sndbufold=1033;  
- 
- 
- 
-          setsockopt(listenfd,SOL_SOCKET,SO_RCVBUF,&sndbufold,sndbufoldsize);  
- 
- 
- 
- 
- 
-     int sndbufold2;  
- 
- 
- 
-          socklen_t sndbufoldsize2=sizeof(sndbufold2);  
- 
- 
- 
- 
- 
-          getsockopt(listenfd,SOL_SOCKET,SO_RCVBUF,&sndbufold2,&sndbufoldsize2);  
- 
- 
- 
-          cout << "--" << sndbufold2 << "--" << endl;  
- 
- 
- 
- 
- 
-          /**display the sock send buf size*/  
- 
- 
- 
- 
- 
-          struct sockaddr_in addr;  
- 
-          bzero(&addr,sizeof(addr));  
- 
-          addr.sin_family=AF_INET;  
- 
-          addr.sin_port=htons(10355);  
- 
-          struct in_addr host;  
- 
- 
- 
-          inet_pton(AF_INET,"192.168.1.101",(void *)&(addr.sin_addr));  
- 
- 
- 
-          connect(listenfd,(struct sockaddr *)&addr,sizeof(addr));  
- 
- 
- 
-          int canread;  
- 
- 
- 
-          char getc[]="coreymylife";  
- 
- 
- 
- 
- 
- 
- 
-          for(int i=0;i<300;i++){  
- 
-                    write(listenfd,getc,3);  
- 
-          }  
- 
- 
- 
-          for(;;){  
- 
-                    write(listenfd,getc,3);  
- 
-          }  
- 
- 
- 
-          cout << "the end!" << endl << flush ;  
- 
- }  
客户端主要是持续的向服务器写入字节；
另外用JAVA代码进行了一个C/S程序做对比；
JAVA服务器：
- package org.corey.test;  
- 
- 
- 
- import java.io.IOException;  
- 
- import java.io.InputStream;  
- 
- import java.io.OutputStream;  
- 
- import java.net.ServerSocket;  
- 
- import java.net.Socket;  
- 
- 
- 
- public class Server {  
- 
-     public static void main(String[] args) {  
- 
-        try {  
- 
-            ServerSocket server = new ServerSocket(10000);  
- 
-            Socket requestClient = server.accept();  
- 
-            InputStream is = requestClient.getInputStream();  
- 
-            while (true) {  
- 
-               System.out.println("begin:");  
- 
-               int a=is.read();  
- 
-               if(a==-1){  
- 
-                   OutputStream os=requestClient.getOutputStream();  
- 
-                   os.write("a".getBytes());  
- 
-               }  
- 
-               System.out.println("end:"+a);  
- 
-            }  
- 
-        } catch (IOException e) {  
- 
-            e.printStackTrace();  
- 
-        }  
- 
- 
- 
-     }  
- 
- }  
JAVA客户端代码:
- package org.corey.test;  
- 
- 
- 
- import java.io.IOException;  
- 
- import java.io.OutputStream;  
- 
- import java.net.Socket;  
- 
- import java.net.UnknownHostException;  
- 
- 
- 
- public class Client {  
- 
-     public static void main(String[] args) {  
- 
-        try {  
- 
-            Socket client = new Socket("127.0.0.1", 10000);  
- 
-            OutputStream os = client.getOutputStream();  
- 
-            for (int i = 0; i < 1000; i++) {  
- 
-               os.write(i);  
- 
-            }  
- 
-            os.close();  
- 
-            client.close();  
- 
-        } catch (UnknownHostException e) {  
- 
-            e.printStackTrace();  
- 
-        } catch (IOException e) {  
- 
-            e.printStackTrace();  
- 
-        }  
- 
-     }  
- 
- }  
在LINUX系统中，每一个合理关闭(main执行结束)和意外关闭(SIGKILL)被系统中断的程序，都在结束的时候向远程服务器发送一个FIN分节。
其中四个程序分别运行在以下环境：
C++ server:Fedora
C++ client:cygwin windows
JAVA server:windows
JAVA client:windows
下面分别是几种关闭客户端进程的情况：
JAVA客户端退出
1）  非正常终止（不显示调用close）:
a)         服务器读：Connection reset异常；
b)         服务器写：socket write error；
2）  close终止：
a)         服务器读：-1，流结束符；
b)         服务器写：socket write error；
C++客户端退出
1）  非正常终止（不显示调用close）:
a)         服务器读：0，流结束符
b)         服务器写：-1 异常
2）  close终止：
a)         服务器读：0，流结束符；
b)         服务器写：-1 异常
TCP/IP的状态图如下：
![](http://hi.csdn.net/attachment/201005/29/0_12751131956OWW.gif)状态描述：
1）  服务器启动。这个时候状态为
![](http://hi.csdn.net/attachment/201005/29/0_1275113208XND0.gif)
状态为：
![](http://hi.csdn.net/attachment/201005/29/0_12751132143XLz.gif)2）  客户端连接:
![](http://hi.csdn.net/attachment/201005/29/0_1275113220zx8I.gif)
客户机状态和服务器状态：
![](http://hi.csdn.net/attachment/201005/29/0_1275113224zCjH.gif)
![](http://hi.csdn.net/attachment/201005/29/0_1275113229mFDR.gif)
这个时候，我们可以用tcpdump看到两台机器中不断的发送数据，
![](http://hi.csdn.net/attachment/201005/29/0_1275113238DFA6.gif)
3）关闭客户端
![](http://hi.csdn.net/attachment/201005/29/0_1275113242aaHg.gif)
此时服务器状态为
![](http://hi.csdn.net/attachment/201005/29/0_1275113246fDMr.gif)
这个时候，我们在tcpdump中可以见到：
![](http://hi.csdn.net/attachment/201005/29/0_12751132710jiX.gif)
在服务器ACK之前，事实上客户端经历了FIN_WAIT_1的状态，不过因为服务器很快的给予了FIN的ACK答复分节，所以这个过程转瞬即逝。
此时，客户端和服务器之间实现了所谓的半关闭。
3）  关闭服务器
客户机状态为：
![](http://hi.csdn.net/attachment/201005/29/0_1275113276Y597.gif)
我们tcpdump查看的数据为：
![](http://hi.csdn.net/attachment/201005/29/0_12751132823loM.gif)
这表示服务器那半连接也被关闭了。
我们也可以采用JAVA客户端在WINDOWS平台上连接Fedora的C++服务器，我们在tcpdump中发现如果我们不执行close操作，则不会向服务器发送FIN分节，这也是导致了为什么在讨论JAVA客户端非正常关闭的情况下，read的结果是系统异常而不是流结束符。
TIME_WAIT一般会经历两个MSL，主要是为了防止化身进程和上一个关闭的进程在网络中还没有接受到的分节产生混淆。
