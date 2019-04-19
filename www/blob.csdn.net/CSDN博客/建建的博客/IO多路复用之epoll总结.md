# IO多路复用之epoll总结 - 建建的博客 - CSDN博客
2017年03月25日 17:20:36[纪建](https://me.csdn.net/u013898698)阅读数：114
个人分类：[Linux](https://blog.csdn.net/u013898698/article/category/6754383)
1、基本知识
　　epoll是在2.6内核中提出的，是之前的select和poll的增强版本。相对于select和poll来说，epoll更加灵活，没有描述符限制。epoll使用一个文件描述符管理多个描述符，将用户关系的文件描述符的事件存放到内核的一个事件表中，这样在用户空间和内核空间的copy只需一次。
2、epoll接口
　　epoll操作过程需要三个接口，分别如下：
```
#include <sys/epoll.h>
int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
```
（1） int epoll_create(int size);
　　创建一个epoll的句柄，size用来告诉内核这个监听的数目一共有多大。这个参数不同于select()中的第一个参数，给出最大监听的fd+1的值。需要注意的是，当创建好epoll句柄后，它就是会占用一个fd值，在linux下如果查看/proc/进程id/fd/，是能够看到这个fd的，所以在使用完epoll后，必须调用close()关闭，否则可能导致fd被耗尽。
（2）int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
　　epoll的事件注册函数，它不同与select()是在监听事件时告诉内核要监听什么类型的事件epoll的事件注册函数，它不同与select()是在监听事件时告诉内核要监听什么类型的事件，而是在这里先注册要监听的事件类型。第一个参数是epoll_create()的返回值，第二个参数表示动作，用三个宏来表示：
EPOLL_CTL_ADD：注册新的fd到epfd中；
EPOLL_CTL_MOD：修改已经注册的fd的监听事件；
EPOLL_CTL_DEL：从epfd中删除一个fd；
第三个参数是需要监听的fd，第四个参数是告诉内核需要监听什么事，struct epoll_event结构如下：
```
struct epoll_event {
  __uint32_t events;  /* Epoll events */
  epoll_data_t data;  /* User data variable */
};
```
events可以是以下几个宏的集合：
EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
EPOLLOUT：表示对应的文件描述符可以写；
EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
EPOLLERR：表示对应的文件描述符发生错误；
EPOLLHUP：表示对应的文件描述符被挂断；
EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
（3） int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
　　等待事件的产生，类似于select()调用。参数events用来从内核得到事件的集合，maxevents告之内核这个events有多大，这个maxevents的值不能大于创建epoll_create()时的size，参数timeout是超时时间（毫秒，0会立即返回，-1将不确定，也有说法说是永久阻塞）。该函数返回需要处理的事件数目，如返回0表示已超时。
3、工作模式
　　epoll对文件描述符的操作有两种模式：LT（level trigger）和ET（edge trigger）。LT模式是默认模式，LT模式与ET模式的区别如下：
　　LT模式：当epoll_wait检测到描述符事件发生并将此事件通知应用程序，应用程序可以不立即处理该事件。下次调用epoll_wait时，会再次响应应用程序并通知此事件。
　　ET模式：当epoll_wait检测到描述符事件发生并将此事件通知应用程序，应用程序必须立即处理该事件。如果不处理，下次调用epoll_wait时，不会再次响应应用程序并通知此事件。
ET模式在很大程度上减少了epoll事件被重复触发的次数，因此效率要比LT模式高。epoll工作在ET模式的时候，必须使用非阻塞套接口，以避免由于一个文件句柄的阻塞读/阻塞写操作把处理多个文件描述符的任务饿死。
4、测试程序
　　编写一个服务器回射程序echo，练习epoll过程。
服务器代码如下所示：
```
1 #include <stdio.h>
  2 #include <stdlib.h>
  3 #include <string.h>
  4 #include <errno.h>
  5 
  6 #include <netinet/in.h>
  7 #include <sys/socket.h>
  8 #include <arpa/inet.h>
  9 #include <sys/epoll.h>
 10 #include <unistd.h>
 11 #include <sys/types.h>
 12 
 13 #define IPADDRESS   "127.0.0.1"
 14 #define PORT        8787
 15 #define MAXSIZE     1024
 16 #define LISTENQ     5
 17 #define FDSIZE      1000
 18 #define EPOLLEVENTS 100
 19 
 20 //函数声明
 21 //创建套接字并进行绑定
 22 static int socket_bind(const char* ip,int port);
 23 //IO多路复用epoll
 24 static void do_epoll(int listenfd);
 25 //事件处理函数
 26 static void
 27 handle_events(int epollfd,struct epoll_event *events,int num,int listenfd,char *buf);
 28 //处理接收到的连接
 29 static void handle_accpet(int epollfd,int listenfd);
 30 //读处理
 31 static void do_read(int epollfd,int fd,char *buf);
 32 //写处理
 33 static void do_write(int epollfd,int fd,char *buf);
 34 //添加事件
 35 static void add_event(int epollfd,int fd,int state);
 36 //修改事件
 37 static void modify_event(int epollfd,int fd,int state);
 38 //删除事件
 39 static void delete_event(int epollfd,int fd,int state);
 40 
 41 int main(int argc,char *argv[])
 42 {
 43     int  listenfd;
 44     listenfd = socket_bind(IPADDRESS,PORT);
 45     listen(listenfd,LISTENQ);
 46     do_epoll(listenfd);
 47     return 0;
 48 }
 49 
 50 static int socket_bind(const char* ip,int port)
 51 {
 52     int  listenfd;
 53     struct sockaddr_in servaddr;
 54     listenfd = socket(AF_INET,SOCK_STREAM,0);
 55     if (listenfd == -1)
 56     {
 57         perror("socket error:");
 58         exit(1);
 59     }
 60     bzero(&servaddr,sizeof(servaddr));
 61     servaddr.sin_family = AF_INET;
 62     inet_pton(AF_INET,ip,&servaddr.sin_addr);
 63     servaddr.sin_port = htons(port);
 64     if (bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
 65     {
 66         perror("bind error: ");
 67         exit(1);
 68     }
 69     return listenfd;
 70 }
 71 
 72 static void do_epoll(int listenfd)
 73 {
 74     int epollfd;
 75     struct epoll_event events[EPOLLEVENTS];
 76     int ret;
 77     char buf[MAXSIZE];
 78     memset(buf,0,MAXSIZE);
 79     //创建一个描述符
 80     epollfd = epoll_create(FDSIZE);
 81     //添加监听描述符事件
 82     add_event(epollfd,listenfd,EPOLLIN);
 83     for ( ; ; )
 84     {
 85         //获取已经准备好的描述符事件
 86         ret = epoll_wait(epollfd,events,EPOLLEVENTS,-1);
 87         handle_events(epollfd,events,ret,listenfd,buf);
 88     }
 89     close(epollfd);
 90 }
 91 
 92 static void
 93 handle_events(int epollfd,struct epoll_event *events,int num,int listenfd,char *buf)
 94 {
 95     int i;
 96     int fd;
 97     //进行选好遍历
 98     for (i = 0;i < num;i++)
 99     {
100         fd = events[i].data.fd;
101         //根据描述符的类型和事件类型进行处理
102         if ((fd == listenfd) &&(events[i].events & EPOLLIN))
103             handle_accpet(epollfd,listenfd);
104         else if (events[i].events & EPOLLIN)
105             do_read(epollfd,fd,buf);
106         else if (events[i].events & EPOLLOUT)
107             do_write(epollfd,fd,buf);
108     }
109 }
110 static void handle_accpet(int epollfd,int listenfd)
111 {
112     int clifd;
113     struct sockaddr_in cliaddr;
114     socklen_t  cliaddrlen;
115     clifd = accept(listenfd,(struct sockaddr*)&cliaddr,&cliaddrlen);
116     if (clifd == -1)
117         perror("accpet error:");
118     else
119     {
120         printf("accept a new client: %s:%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);
121         //添加一个客户描述符和事件
122         add_event(epollfd,clifd,EPOLLIN);
123     }
124 }
125 
126 static void do_read(int epollfd,int fd,char *buf)
127 {
128     int nread;
129     nread = read(fd,buf,MAXSIZE);
130     if (nread == -1)
131     {
132         perror("read error:");
133         close(fd);
134         delete_event(epollfd,fd,EPOLLIN);
135     }
136     else if (nread == 0)
137     {
138         fprintf(stderr,"client close.\n");
139         close(fd);
140         delete_event(epollfd,fd,EPOLLIN);
141     }
142     else
143     {
144         printf("read message is : %s",buf);
145         //修改描述符对应的事件，由读改为写
146         modify_event(epollfd,fd,EPOLLOUT);
147     }
148 }
149 
150 static void do_write(int epollfd,int fd,char *buf)
151 {
152     int nwrite;
153     nwrite = write(fd,buf,strlen(buf));
154     if (nwrite == -1)
155     {
156         perror("write error:");
157         close(fd);
158         delete_event(epollfd,fd,EPOLLOUT);
159     }
160     else
161         modify_event(epollfd,fd,EPOLLIN);
162     memset(buf,0,MAXSIZE);
163 }
164 
165 static void add_event(int epollfd,int fd,int state)
166 {
167     struct epoll_event ev;
168     ev.events = state;
169     ev.data.fd = fd;
170     epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
171 }
172 
173 static void delete_event(int epollfd,int fd,int state)
174 {
175     struct epoll_event ev;
176     ev.events = state;
177     ev.data.fd = fd;
178     epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
179 }
180 
181 static void modify_event(int epollfd,int fd,int state)
182 {
183     struct epoll_event ev;
184     ev.events = state;
185     ev.data.fd = fd;
186     epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
187 }
```
客户端也用epoll实现，控制STDIN_FILENO、STDOUT_FILENO、和sockfd三个描述符，程序如下所示：
```
1 #include <netinet/in.h>
  2 #include <sys/socket.h>
  3 #include <stdio.h>
  4 #include <string.h>
  5 #include <stdlib.h>
  6 #include <sys/epoll.h>
  7 #include <time.h>
  8 #include <unistd.h>
  9 #include <sys/types.h>
 10 #include <arpa/inet.h>
 11 
 12 #define MAXSIZE     1024
 13 #define IPADDRESS   "127.0.0.1"
 14 #define SERV_PORT   8787
 15 #define FDSIZE        1024
 16 #define EPOLLEVENTS 20
 17 
 18 static void handle_connection(int sockfd);
 19 static void
 20 handle_events(int epollfd,struct epoll_event *events,int num,int sockfd,char *buf);
 21 static void do_read(int epollfd,int fd,int sockfd,char *buf);
 22 static void do_read(int epollfd,int fd,int sockfd,char *buf);
 23 static void do_write(int epollfd,int fd,int sockfd,char *buf);
 24 static void add_event(int epollfd,int fd,int state);
 25 static void delete_event(int epollfd,int fd,int state);
 26 static void modify_event(int epollfd,int fd,int state);
 27 
 28 int main(int argc,char *argv[])
 29 {
 30     int                 sockfd;
 31     struct sockaddr_in  servaddr;
 32     sockfd = socket(AF_INET,SOCK_STREAM,0);
 33     bzero(&servaddr,sizeof(servaddr));
 34     servaddr.sin_family = AF_INET;
 35     servaddr.sin_port = htons(SERV_PORT);
 36     inet_pton(AF_INET,IPADDRESS,&servaddr.sin_addr);
 37     connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
 38     //处理连接
 39     handle_connection(sockfd);
 40     close(sockfd);
 41     return 0;
 42 }
 43 
 44 
 45 static void handle_connection(int sockfd)
 46 {
 47     int epollfd;
 48     struct epoll_event events[EPOLLEVENTS];
 49     char buf[MAXSIZE];
 50     int ret;
 51     epollfd = epoll_create(FDSIZE);
 52     add_event(epollfd,STDIN_FILENO,EPOLLIN);
 53     for ( ; ; )
 54     {
 55         ret = epoll_wait(epollfd,events,EPOLLEVENTS,-1);
 56         handle_events(epollfd,events,ret,sockfd,buf);
 57     }
 58     close(epollfd);
 59 }
 60 
 61 static void
 62 handle_events(int epollfd,struct epoll_event *events,int num,int sockfd,char *buf)
 63 {
 64     int fd;
 65     int i;
 66     for (i = 0;i < num;i++)
 67     {
 68         fd = events[i].data.fd;
 69         if (events[i].events & EPOLLIN)
 70             do_read(epollfd,fd,sockfd,buf);
 71         else if (events[i].events & EPOLLOUT)
 72             do_write(epollfd,fd,sockfd,buf);
 73     }
 74 }
 75 
 76 static void do_read(int epollfd,int fd,int sockfd,char *buf)
 77 {
 78     int nread;
 79     nread = read(fd,buf,MAXSIZE);
 80         if (nread == -1)
 81     {
 82         perror("read error:");
 83         close(fd);
 84     }
 85     else if (nread == 0)
 86     {
 87         fprintf(stderr,"server close.\n");
 88         close(fd);
 89     }
 90     else
 91     {
 92         if (fd == STDIN_FILENO)
 93             add_event(epollfd,sockfd,EPOLLOUT);
 94         else
 95         {
 96             delete_event(epollfd,sockfd,EPOLLIN);
 97             add_event(epollfd,STDOUT_FILENO,EPOLLOUT);
 98         }
 99     }
100 }
101 
102 static void do_write(int epollfd,int fd,int sockfd,char *buf)
103 {
104     int nwrite;
105     nwrite = write(fd,buf,strlen(buf));
106     if (nwrite == -1)
107     {
108         perror("write error:");
109         close(fd);
110     }
111     else
112     {
113         if (fd == STDOUT_FILENO)
114             delete_event(epollfd,fd,EPOLLOUT);
115         else
116             modify_event(epollfd,fd,EPOLLIN);
117     }
118     memset(buf,0,MAXSIZE);
119 }
120 
121 static void add_event(int epollfd,int fd,int state)
122 {
123     struct epoll_event ev;
124     ev.events = state;
125     ev.data.fd = fd;
126     epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
127 }
128 
129 static void delete_event(int epollfd,int fd,int state)
130 {
131     struct epoll_event ev;
132     ev.events = state;
133     ev.data.fd = fd;
134     epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
135 }
136 
137 static void modify_event(int epollfd,int fd,int state)
138 {
139     struct epoll_event ev;
140     ev.events = state;
141     ev.data.fd = fd;
142     epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
143 }
```
5、测试结果
![](http://images.cnitblog.com/blog/305504/201308/17013658-d6aa0df188b24c6da6759c4c611570b3.png)
![](http://images.cnitblog.com/blog/305504/201308/17013713-0bd444572e064c8e80d58d6221277eb4.png)
![](http://images.cnitblog.com/blog/305504/201308/17013723-571312abdc2945a68016a080690fc3fb.png)
