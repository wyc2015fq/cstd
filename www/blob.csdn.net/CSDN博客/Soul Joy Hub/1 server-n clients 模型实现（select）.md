# 1 server - n clients 模型实现（select） - Soul Joy Hub - CSDN博客

2016年06月13日 10:00:11[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：328标签：[服务器																[客户端																[多进程																[select																[多线程](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=select&t=blog)](https://so.csdn.net/so/search/s.do?q=多进程&t=blog)](https://so.csdn.net/so/search/s.do?q=客户端&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)
个人分类：[并发/网络编程](https://blog.csdn.net/u011239443/article/category/6268713)


http://blog.csdn.net/u011239443/article/details/51655333

拓扑结构：

![](http://images.cnitblog.com/blog/709693/201502/171842176273760.png)

各个客户端创建读写管道，通过“上下线信息管道”向服务器发送上下线信息和读写管道名称。服务器接受信息，修改链表（存储客户端信息）。客户端、服务器打开读写管道，服务器通过“W”管道接收从客户端发来的信息，在根据链表同个其他各个“R”管道向其他客户端发送信息。

# 具体流程：

## 1、建立上下线信息管道

服务器：

```
1 mkfifo(path_name, 0666);// 创建管道 —— 专用于接收客户端上下线信息
 2 
 3     printf("mkfifo over!\n");
 4 
 5     fd_listen = open(path_name, O_RDONLY);
 6     if(fd_listen == -1)
 7     {
 8         printf("open server_fifo fail!\n");
 9         exit(1);
10     }
```

客户端：

```
1 //打开上下线管道
 2     int fd_server ;
 3     char path_name[128]="";
 4     char fifo_name[128] ;
 5     char msg[1024] ="" ;
 6     char fifo1[128], fifo2[128] ;
 7     int fd_recv, fd_send ;
 8     sprintf(path_name, "%s/%s", PIPE_PATH, PIPE_NAME);
 9 
10     fd_server = open(path_name, O_WRONLY);
11     if(fd_server == -1)
12     {
13         printf("open fail!\n");
14         exit(1) ;
15     }
```

## 2、客户端建立读写管道

```
// 建造读写管道  pid_r.fifo pid_w.fifo
    //
    memset(fifo_name, 0, 128);
    sprintf(fifo_name, "%u_r.fifo", getpid());
    memset(path_name, 0, sizeof(path_name));
    sprintf(path_name, "%s/%s", PIPE_PATH, fifo_name);

    strcpy(fifo1, path_name);
    if(-1 == mkfifo(path_name, 0666) )
    {
        printf("mkfif fail: %s\n", path_name);
        exit(1) ;
    }

    printf("%s open\n", path_name);

    memset(fifo_name, 0, 128);
    sprintf(fifo_name, "%u_w.fifo", getpid());
    memset(path_name, 0, sizeof(path_name));
    sprintf(path_name, "%s/%s", PIPE_PATH, fifo_name);

    strcpy(fifo2, path_name);
    if(mkfifo(path_name, 0666) == -1 )
    {
        
        printf("mkfif fail: %s\n", path_name);
        exit(1) ;
    }
    printf("%s open\n", path_name);

    printf("mkfifo over!\n");
```

## 3、上线处理

客户端发送上线信息：

```
1 //发送上线信息
2     sprintf(msg, "%u on\n", getpid());
3     printf("msg: %s\n", msg);
4     write(fd_server, msg, strlen(msg));
```

服务器监听到后处理：

```
1 // 读写是针对客户端而言的： pid_r.fifo(c_r - s_w)   pid_w.fifo(c_w - s_r)
 2                 printf("client: %d on\n", client_pid);
 3                 //pid_r.fifo  s_w
 4                 //构建管道名字符串
 5                 memset(fifo_name, 0, 128) ;
 6                 sprintf(fifo_name, "%d_r.fifo", client_pid);
 7                 memset(path_name, 0, 128) ;
 8                 sprintf(path_name, "%s/%s", PIPE_PATH, fifo_name);
 9             
10                 //新增链表节点
11                 pnew = (pCLIENT)calloc(1, sizeof(CLIENT));
12                 pnew ->m_id = client_pid ;
13                 printf("pid_r.fifo: %s\n", path_name);
14                 pnew ->m_send = open(path_name, O_WRONLY);
15                 printf("send_fd: %d\n", pnew ->m_send);
16 
17                 //打开“W”管道 pid_w.fifo  s_r
18                 memset(fifo_name, 0, 128) ;
19                 sprintf(fifo_name, "%d_w.fifo", client_pid);
20                 memset(path_name, 0, 128) ;
21                 sprintf(path_name, "%s/%s", PIPE_PATH, fifo_name);
22 
23 
24                 printf("pid_w.fifo: %s\n", path_name);
25                 pnew ->m_recv = open(path_name, O_RDONLY);
26                 printf("recv_fd: %d\n", pnew ->m_recv);
27                 printf("open client fifo: %d, %d\n", pnew ->m_send, pnew ->m_recv);
28 
29 
30                 FD_SET(pnew ->m_recv, &rd_sets);//把pid_w.fifo 句柄 加到读集合中去 主要：更新的是读集合，而不是副本集合
31 
32                 pnew ->m_next = plist ;  //插入链表
33                 plist = pnew ;
```

客户端也打开管道：

```
1     memset(fifo_name, 0,  128);
 2     sprintf(fifo_name, "%u_r.fifo", getpid());
 3     memset(path_name, 0, sizeof(path_name));
 4     sprintf(path_name, "%s/%s", PIPE_PATH, fifo_name);
 5 
 6     fd_recv = open(path_name, O_RDONLY);
 7 
 8     memset(fifo_name, 0,  128);
 9     sprintf(fifo_name, "%u_w.fifo", getpid());
10     memset(path_name, 0, sizeof(path_name));
11     sprintf(path_name, "%s/%s", PIPE_PATH, fifo_name);
12 
13     fd_send = open(path_name, O_WRONLY);
14 
15     printf("fifo open %d %d\n", fd_send, fd_recv);
```

# 4、客户端、服务器对通信信息处理

客户端监听到键盘的输入信息，则转发给服务器：

```
1 if(FD_ISSET(0, &rd_sets))
2             {
3                 memset(msg, 0, sizeof(msg)) ;
4                 sprintf(msg, "from %u: ", getpid());
5                 write(fd_send, msg, strlen(msg));
6 
7             }
```

客户端监听服务器发来的信息，并打印：

```
1      if(FD_ISSET(fd_recv, &rd_sets))
2              {
3                  memset(msg, 0, sizeof(msg)) ;
4                  read(fd_recv, msg, 1024);
5                  write(1, msg, strlen(msg));    
6            }
```

服务器监听到客户端发来的信息，根据链表内客户端的信息，进行转发：

```
1     //2、遍历链表，监听其他的管道文件（用于服务器和客户端通信）句柄
 2         pcur = plist ;
 3         while(pcur)
 4         {
 5             if(FD_ISSET(pcur ->m_recv, &bak_sets))// translate
 6             {
 7                 memset(msg, 0, 1024);
 8                 read(pcur -> m_recv, msg, 1024);
 9 
10                 dispatch_msg(plist,pcur, msg);
11             }
12             pcur = pcur ->m_next ;
13         }
```

```
1 void dispatch_msg(pCLIENT phead,pCLIENT pcur, char* msg)
2 {
3     while(phead)
4     {
5         if(phead!=pcur)
6             write(phead ->m_send, msg, strlen(msg));
7         phead = phead ->m_next ;
8     }
9 }
```

# 完整代码

server：

```
1 #include<stdio.h>
  2 #include<stdlib.h>
  3 #include<string.h>
  4 #include<unistd.h>
  5 #include<sys/stat.h>
  6 #include<sys/types.h>
  7 #include<fcntl.h>
  8 #include<sys/select.h>
  9 #include<sys/time.h>
 10 #define PIPE_PATH "/home/soso/Desktop/2-17/FIFO"
 11 #define PIPE_NAME "server.fifo"
 12 typedef struct tag
 13 {
 14     int m_id ;
 15     int m_send;
 16     int m_recv;
 17     struct tag* m_next ;
 18 }CLIENT, *pCLIENT;
 19 
 20 
 21 void dispatch_msg(pCLIENT phead,pCLIENT pcur, char* msg)
 22 {
 23     while(phead)
 24     {
 25         if(phead!=pcur)
 26             write(phead ->m_send, msg, strlen(msg));
 27         phead = phead ->m_next ;
 28     }
 29 }
 30 
 31 int main(int argc, char* argv[])
 32 {
 33     int fd_listen ;  //文件句柄
 34     char path_name[128] = "" ;
 35     char fifo_name[128] ;
 36     char msg[1024];
 37 
 38     char client_stat[5]  = "";//客户端状态
 39     int client_pid ;  // 客户端进程ID
 40     sprintf(path_name, "%s/%s", PIPE_PATH, PIPE_NAME);//路径名 = 路径/管道名
 41 
 42     mkfifo(path_name, 0666);// 创建管道 —— 专用于接收客户端上下线信息
 43 
 44     printf("mkfifo over!\n");
 45 
 46     fd_listen = open(path_name, O_RDONLY);
 47     if(fd_listen == -1)
 48     {
 49         printf("open server_fifo fail!\n");
 50         exit(1);
 51     }
 52 
 53 
 54     pCLIENT plist = NULL, pcur, pnew, ppre ;
 55 
 56     fd_set rd_sets, bak_sets; //读集合 和 备份读集合
 57     FD_ZERO(&rd_sets);//初始化清空
 58     FD_ZERO(&bak_sets);
 59     FD_SET(fd_listen, &rd_sets); //将fd_listen句柄 加入到 读集合
 60     while(1)
 61     {
 62         bak_sets = rd_sets ;//每次循环更新副本集合
 63         printf("selecting...\n");
 64         select(1024, &bak_sets, NULL, NULL, NULL);//监听集合
 65 
 66         //1、监听fd_listen 管道文件句柄（专用于服务器接收客户端上下线信息）
 67         if(FD_ISSET(fd_listen, &bak_sets))  //若监听到 fd_listen
 68         {
 69             memset(msg,0, 1024);
 70             if( read(fd_listen, msg, 1024) == 0 ) //读取管道信息；但没有客户端 write的时候，read的返回值是0
 71             {
 72                 printf("no clients!\n");
 73                 continue ;
 74             }
 75 
 76             memset(client_stat, 0, sizeof(client_stat));
 77             sscanf(msg, "%d%s", &client_pid, client_stat);//信息格式：“client_pid client_stat\n”
 78             if(strncmp("on", client_stat, 2) == 0)//client on"pid on\n"
 79             {// 读写是针对客户端而言的： pid_r.fifo(c_r - s_w)   pid_w.fifo(c_w - s_r)
 80                 printf("client: %d on\n", client_pid);
 81                 //pid_r.fifo  s_w
 82                 //构建管道名字符串
 83                 memset(fifo_name, 0, 128) ;
 84                 sprintf(fifo_name, "%d_r.fifo", client_pid);
 85                 memset(path_name, 0, 128) ;
 86                 sprintf(path_name, "%s/%s", PIPE_PATH, fifo_name);
 87             
 88                 //新增链表节点
 89                 pnew = (pCLIENT)calloc(1, sizeof(CLIENT));
 90                 pnew ->m_id = client_pid ;
 91                 printf("pid_r.fifo: %s\n", path_name);
 92                 pnew ->m_send = open(path_name, O_WRONLY);
 93                 printf("send_fd: %d\n", pnew ->m_send);
 94 
 95                 //pid_w.fifo  s_r
 96                 memset(fifo_name, 0, 128) ;
 97                 sprintf(fifo_name, "%d_w.fifo", client_pid);
 98                 memset(path_name, 0, 128) ;
 99                 sprintf(path_name, "%s/%s", PIPE_PATH, fifo_name);
100 
101 
102                 printf("pid_w.fifo: %s\n", path_name);
103                 pnew ->m_recv = open(path_name, O_RDONLY);
104                 printf("recv_fd: %d\n", pnew ->m_recv);
105                 printf("open client fifo: %d, %d\n", pnew ->m_send, pnew ->m_recv);
106 
107 
108                 FD_SET(pnew ->m_recv, &rd_sets);//把pid_w.fifo 句柄 加到读集合中去 主要：更新的是读集合，而不是副本集合
109 
110                 pnew ->m_next = plist ;  //插入链表
111                 plist = pnew ;
112 
113             }else//client off "pid off\n"
114             {
115                 printf("client: %d off\n", client_pid);
116                 ppre = NULL ;//前驱指针
117                 pcur = plist ;
118                 while(pcur && pcur ->m_id != client_pid) //遍历到该客户端PID
119                 {
120                     ppre = pcur ;
121                     pcur = pcur ->m_next ;
122                 }
123 
124 
125                 if(pcur == NULL)
126                 {
127                     printf("not exist!\n");
128                     continue ;
129                 }else
130                 {
131                     //删除节点
132                     if(ppre == NULL) 
133                     {
134                         plist = pcur ->m_next ;
135                     }else
136                     {
137                         ppre ->m_next = pcur ->m_next ;
138                     }
139 
140                     //关闭文件
141                     close(pcur ->m_send) ;
142                     close(pcur ->m_recv) ;
143 
144                     //把 pcur ->m_recv 从读集合中删除
145                     FD_CLR(pcur ->m_recv, &rd_sets);  
146 
147                     free(pcur);     //释放内存
148                     printf("clear ok !\n");
149 
150                 }
151             }
152         }
153 
154         //2、遍历链表，监听其他的管道文件（用于服务器和客户端通信）句柄
155         pcur = plist ;
156         while(pcur)
157         {
158             if(FD_ISSET(pcur ->m_recv, &bak_sets))// translate
159             {
160                 memset(msg, 0, 1024);
161                 read(pcur -> m_recv, msg, 1024);
162 
163                 dispatch_msg(plist,pcur, msg);
164             }
165             pcur = pcur ->m_next ;
166         }
167     }
168     return 0 ;
169 }
```

client：

```
1 #include<stdio.h>
  2 #include<stdlib.h>
  3 #include<string.h>
  4 #include<unistd.h>
  5 #include<sys/stat.h>
  6 #include<sys/types.h>
  7 #include<fcntl.h>
  8 #include<sys/select.h>
  9 #include<sys/time.h>
 10 #define PIPE_PATH "/home/soso/Desktop/2-17/FIFO"
 11 #define PIPE_NAME "server.fifo"
 12 int main(int argc, char* argv[])
 13 {
 14     //1、向服务器通知上线下线信息
 15 
 16     //打开上下线管道
 17     int fd_server ;
 18     char path_name[128]="";
 19     char fifo_name[128] ;
 20     char msg[1024] ="" ;
 21     char fifo1[128], fifo2[128] ;
 22     int fd_recv, fd_send ;
 23     sprintf(path_name, "%s/%s", PIPE_PATH, PIPE_NAME);
 24 
 25     fd_server = open(path_name, O_WRONLY);
 26     if(fd_server == -1)
 27     {
 28         printf("open fail!\n");
 29         exit(1) ;
 30     }
 31     
 32     
 33 
 34     // 建造读写管道  pid_r.fifo pid_w.fifo
 35     //
 36     memset(fifo_name, 0, 128);
 37     sprintf(fifo_name, "%u_r.fifo", getpid());
 38     memset(path_name, 0, sizeof(path_name));
 39     sprintf(path_name, "%s/%s", PIPE_PATH, fifo_name);
 40 
 41     strcpy(fifo1, path_name);
 42     if(-1 == mkfifo(path_name, 0666) )
 43     {
 44         printf("mkfif fail: %s\n", path_name);
 45         exit(1) ;
 46     }
 47 
 48     printf("%s open\n", path_name);
 49 
 50     memset(fifo_name, 0, 128);
 51     sprintf(fifo_name, "%u_w.fifo", getpid());
 52     memset(path_name, 0, sizeof(path_name));
 53     sprintf(path_name, "%s/%s", PIPE_PATH, fifo_name);
 54 
 55     strcpy(fifo2, path_name);
 56     if(mkfifo(path_name, 0666) == -1 )
 57     {
 58         
 59         printf("mkfif fail: %s\n", path_name);
 60         exit(1) ;
 61     }
 62     printf("%s open\n", path_name);
 63 
 64     printf("mkfifo over!\n");
 65 
 66 
 67     //发送上线信息
 68     sprintf(msg, "%u on\n", getpid());
 69     printf("msg: %s\n", msg);
 70 
 71     write(fd_server, msg, strlen(msg));
 72 
 73     //
 74     memset(fifo_name, 0,  128);
 75     sprintf(fifo_name, "%u_r.fifo", getpid());
 76     memset(path_name, 0, sizeof(path_name));
 77     sprintf(path_name, "%s/%s", PIPE_PATH, fifo_name);
 78 
 79     fd_recv = open(path_name, O_RDONLY);
 80 
 81     memset(fifo_name, 0,  128);
 82     sprintf(fifo_name, "%u_w.fifo", getpid());
 83     memset(path_name, 0, sizeof(path_name));
 84     sprintf(path_name, "%s/%s", PIPE_PATH, fifo_name);
 85 
 86     fd_send = open(path_name, O_WRONLY);
 87 
 88     printf("fifo open %d %d\n", fd_send, fd_recv);
 89 
 90     fd_set rd_sets ;
 91     FD_ZERO(&rd_sets);
 92     while(1)
 93     {
 94         FD_SET(0, &rd_sets);
 95         FD_SET(fd_recv, &rd_sets);
 96 
 97         select(1024, &rd_sets, NULL, NULL, NULL);
 98 
 99             if(FD_ISSET(0, &rd_sets))
100             {
101                 memset(msg, 0, sizeof(msg)) ;
102                 sprintf(msg, "from %u: ", getpid());
103                 if(read(0, msg + strlen(msg),  1024 - strlen(msg) )  == 0)
104                 {
105                     printf("off!\n");
106                     memset(msg, 0, sizeof(msg));
107                     sprintf(msg, "%d off\n", getpid());
108                     write(fd_server, msg, strlen(msg));
109 
110                     close(fd_send);
111                     close(fd_recv);
112 
113                     unlink(fifo1);
114                     unlink(fifo2);
115                     break ;
116                 }
117                 write(fd_send, msg, strlen(msg));
118 
119             }
120             if(FD_ISSET(fd_recv, &rd_sets))
121             {
122                 memset(msg, 0, sizeof(msg)) ;
123                 read(fd_recv, msg, 1024);
124                 write(1, msg, strlen(msg));    
125             }
126     }
127 }
```

![](https://img-blog.csdn.net/20170505090633904?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


