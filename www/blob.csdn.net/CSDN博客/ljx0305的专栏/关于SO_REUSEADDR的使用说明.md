# 关于SO_REUSEADDR的使用说明 - ljx0305的专栏 - CSDN博客
2008年08月04日 08:43:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：803标签：[socket																[测试																[stream																[unix																[服务器																[网络](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=unix&t=blog)](https://so.csdn.net/so/search/s.do?q=stream&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[Linux程序及使用																[网络编程](https://blog.csdn.net/ljx0305/article/category/403915)](https://blog.csdn.net/ljx0305/article/category/394700)
                考WINDOWS   网络编程技术   
  1.   可以对一个端口进行多次绑定，一般这个是不支持使用的；   
  2.   对于监听套接字，比较特殊。如果你定义了SO_REUSEADDR，并且让两个套接字在同一个端口上进行接听，那么对于由谁来ACCEPT，就会出现歧义。如果你定义个SO_REUSEADDR，只定义一个套接字在一个端口上进行监听，如果服务器出现意外而导致没有将这个端口释放，那么服务器重新启动后，你还可以用这个端口，因为你已经规定可以重用了，如果你没定义的话，你就会得到提示，ADDR已在使用中。 
  我用在多播的时候，也经常使用SO_REUSEADDR，也是为了防止机器出现意外，导致端口没有释放，而使重启后的绑定失败～
 我的总结：防止服务器在发生意外时，端口未被释放～可以重新使用～
 一个兄弟提出的问题：
 SOCKET   BindSocket1,BindSocket2.;   
              struct   sockaddr_in     MyAddress;   
              MyAddress.sin_family   =AF_INET;   
              MyAddress.sin_addr.sin_addr=strMyAddress;   
              MyAddress.sin_port   =htonl(MyPort);   
              setsockopt(....,SO_REUSEADDR,.....)
              Bind(BindSocket1,(strunct   socketaddr*)MyAddress,sizeof(MyAddress))   
              Bind(BindSocket2,(strunct   socketaddr*)MyAddress,sizeof(MyAddress))   
              Listen(BindSocket1,5);   
              Listen(BindSocket2,5);   
              ....   
              这样可以么,有问题或是要注意的地方?     
             问绑定第二次是否可以成功？
            答复：
            网友vmstat多次提出了这个问题：SO_REUSEADDR有什么用处和怎么使用。而
且很多网友在编写网络程序时也会遇到这个问题。所以特意写了这么一篇文章，
希望能够解答一些人的疑难。 
    其实这个问题在Richard Stevens的《Unix网络编程指南》卷一里有很详细的
解答(中文版P166-168页)。这里我只是写几个基本的例子来验证这个问题。 
    首先声明一个问题：当两个socket的address和port相冲突，而你又想重用地
址和端口，则旧的socket和新的socket都要已经被设置了SO_REUSEADDR特性，只
有两者之一有这个特性还是有问题的。 
    SO_REUSEADDR可以用在以下四种情况下。 
    (摘自《Unix网络编程》卷一，即UNPv1) 
    1、当有一个有相同本地地址和端口的socket1处于TIME_WAIT状态时，而你启
动的程序的socket2要占用该地址和端口，你的程序就要用到该选项。 
    2、SO_REUSEADDR允许同一port上启动同一服务器的多个实例(多个进程)。但
每个实例绑定的IP地址是不能相同的。在有多块网卡或用IP Alias技术的机器可
以测试这种情况。 
    3、SO_REUSEADDR允许单个进程绑定相同的端口到多个socket上，但每个soc
ket绑定的ip地址不同。这和2很相似，区别请看UNPv1。 
    4、SO_REUSEADDR允许完全相同的地址和端口的重复绑定。但这只用于UDP的
多播，不用于TCP。
例子1:测试上面第一种情况。
#include <netinet/in.h>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <sys/socket.h>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <time.h>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <stdio.h>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <string.h>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#define MAXLINE 100 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)int main(int argc, char** argv) 
![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)int listenfd,connfd; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)struct sockaddr_in servaddr; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)char buff[MAXLINE+1]; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)   time_t ticks; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)   unsigned short port; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)int flag=1,len=sizeof(int); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)   port=10013; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if( (listenfd=socket(AF_INET,SOCK_STREAM,0)) ==-1) 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)     perror("socket"); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)     exit(1); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)   }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)   bzero(&servaddr,sizeof(servaddr)); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)   servaddr.sin_family=AF_INET; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)   servaddr.sin_addr.s_addr=htonl(INADDR_ANY); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)   servaddr.sin_port=htons(port); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if( setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, len) ==-
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)1) 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)      perror("setsockopt"); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)      exit(1); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)   }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if( bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) ==
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)-1) 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)      perror("bind"); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)      exit(1); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)   }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)else
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)      printf("bind call OK!/n"); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if( listen(listenfd,5) ==-1) 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)      perror("listen"); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)      exit(1); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)   }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)for(;;) 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if( (connfd=accept(listenfd,(struct sockaddr*)NULL,NULL)) ==-1)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)          perror("accept"); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)          exit(1); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)      }
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)if( fork() ==0)/**//*child process*/
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)        close(listenfd);/**//*这句不能少，原因请大家想想就知道了。*/
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        ticks=time(NULL); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        snprintf(buff,100,"%.24s/r/n",ctime(&ticks)); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        write(connfd,buff,strlen(buff)); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        close(connfd); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        sleep(1); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        execlp("f1-9d",NULL); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        perror("execlp"); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        exit(1); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)     }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)     close(connfd); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)     exit(0);/**//* end parent*/
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)  }
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
测试：编译为f1-9d程序，放到一个自己PATH环境变量里的某个路径里，例如$HO
ME/bin，运行f1-9d，然后telnet localhost 10013看结果。 
2、第二种情况我没有环境测，所以就不给测试程序了，大家有条件的可以自己写
一个来测试一下。 
3、测试第三种情况的程序 
#include <netinet/in.h>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <sys/socket.h>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <time.h>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <stdio.h>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <string.h>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#define MAXLINE 100 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)int main(int argc, char** argv) 
![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)int fd1,fd2; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)struct sockaddr_in servaddr1,servaddr2; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)char buff[MAXLINE+1]; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)   time_t ticks; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)   unsigned short port; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)int flag=1,len=sizeof(int); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)   port=10013; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if( (fd1=socket(AF_INET,SOCK_STREAM,0)) ==-1) 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)       perror("socket"); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)       exit(1); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)   }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if( (fd2=socket(AF_INET,SOCK_STREAM,0)) ==-1) 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)       perror("socket"); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)       exit(1); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)   }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)   bzero(&servaddr1,sizeof(servaddr1)); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)   bzero(&servaddr2,sizeof(servaddr2)); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)   servaddr1.sin_family=AF_INET; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)   servaddr2.sin_family=AF_INET; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if( inet_pton(AF_INET, "127.0.0.1", &servaddr1.sin_addr) <=0) 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)printf("inet_pton() call error:127.0.0.1/n"); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)exit(1); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if( inet_pton(AF_INET, "128.160.1.230", &servaddr2.sin_addr) <=0) 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)printf("inet_pton() call error:128.160.1.230/n"); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)exit(1); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)servaddr1.sin_port=htons(port); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)servaddr2.sin_port=htons(port); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if( setsockopt(fd1, SOL_SOCKET, SO_REUSEADDR, &flag, len) ==-1) 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)perror("setsockopt"); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)exit(1); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if( setsockopt(fd2, SOL_SOCKET, SO_REUSEADDR, &flag, len) ==-1) 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)perror("setsockopt"); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)exit(1); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if( bind(fd1,(struct sockaddr*)&servaddr1,sizeof(servaddr1)) ==-1)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)perror("bind fd1"); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)exit(1); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if( bind(fd2,(struct sockaddr*)&servaddr2,sizeof(servaddr2)) ==-1)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)perror("bind fd2"); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)exit(1); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)printf("bind fd1 and fd2 OK!/n"); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)/**//*put other process here*/
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)getchar(); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)exit(0);/**//* end */
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
4、由于第四种情况只用于UDP的多播，和TCP的使用没多大关系，所以就不写测试
例子了。自己有兴趣的可以写。 
以上的程序都是在Linux下编译通过的。也可以在其他unix平台运行。 
参考资料： 
1、《Unix网络编程》卷一 R. Stevens 
2、《Socket programming FAQ》 [http://www.ibrado.com/sock-faq/](http://www.ibrado.com/sock-faq/)
引用:http://blog.csdn.net/liusujian02/archive/2007/12/18/1944520.aspx
