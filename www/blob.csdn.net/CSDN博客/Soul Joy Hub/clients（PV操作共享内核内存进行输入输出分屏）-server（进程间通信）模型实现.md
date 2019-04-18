# clients（PV操作共享内核内存进行输入输出分屏） - server（进程间通信）模型实现 - Soul Joy Hub - CSDN博客

2016年06月13日 10:00:20[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：437标签：[PV																[进程间通信																[内存																[server																[多进程](https://so.csdn.net/so/search/s.do?q=多进程&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=内存&t=blog)](https://so.csdn.net/so/search/s.do?q=进程间通信&t=blog)](https://so.csdn.net/so/search/s.do?q=PV&t=blog)
个人分类：[并发/网络编程](https://blog.csdn.net/u011239443/article/category/6268713)


http://blog.csdn.net/u011239443/article/details/51655335

1、拓扑结构

![](http://images.cnitblog.com/blog/709693/201502/231025171115720.png)

# 2、PV操作共享内核内存进行输入输出分屏

(1)

```
1 int semop（int semid，struct sembuf *sops，size_t nsops）；
```

### 功能描述

操作一个或一组信号。


semid：

信号集的识别码，可通过semget获取。

sops：

指向存储信号操作结构的数组指针，信号操作结构的原型如下

```
1 struct sembuf
2 {
3 unsigned short sem_num; /* semaphore number */
4 short sem_op; /* semaphore operation */
5 short sem_flg; /* operation flags */
6 };
```

这三个字段的意义分别为：

sem_num：

操作信号在信号集中的编号，第一个信号的编号是0。

sem_op：

如果其值为正数，该值会加到现有的信号内含值中。通常用于释放所控资源的使用权；如果sem_op的值为负数，而其绝对值又大于信号的现值，操作将会阻塞，直到信号值大于或等于sem_op的绝对值。通常用于获取资源的使用权；如果sem_op的值为0，则操作将暂时阻塞，直到信号的值变为0。

sem_flg：

信号操作标志，可能的选择有两种

IPC_NOWAIT //对信号的操作不能满足时，semop()不会阻塞，并立即返回，同时设定错误提示。

SEM_UNDO //程序结束时(不论正常或不正常)，保证信号值会被重设为semop()调用前的值。这样做的目的在于避免程序在异常情况下结束时未将锁定的资源解锁，造成该资源永远锁定。

nsops：信号操作结构的数量，恒大于或等于1。

```
1 void P(int semid) 
 2 {
 3     struct sembuf my_buf ;
 4     memset(&my_buf, 0, sizeof(my_buf) );
 5     my_buf.sem_num = 0 ;
 6     my_buf.sem_op = -1 ;
 7     my_buf.sem_flg = SEM_UNDO ;
 8     semop(semid, &my_buf, 1);
 9 }
10 void V(int semid)
11 {
12     struct sembuf my_buf ;
13     memset(&my_buf, 0, sizeof(my_buf) );
14     my_buf.sem_num = 0 ;
15     my_buf.sem_op = 1 ;
16     my_buf.sem_flg = SEM_UNDO ;
17     semop(semid, &my_buf, 1);
18     
19 }
```

 (2)

出错：-1，错误原因存于error中

(3)

int semget(key_t key, int nsems, int semflg);

key：

所创建或打开[信号量集](http://baike.baidu.com/view/3839686.htm)的键值。

nsems：

创建的[信号量集](http://baike.baidu.com/view/3839686.htm)中的信号量的个数，该参数只在创建信号量集时有效。

semflg：

调用函数的操作类型，也可用于设置[信号量集](http://baike.baidu.com/view/3839686.htm)的访问权限，两者通过or表示

```
1     shm_key = (key_t)atoi(argv[1]);
2     sem_key = (key_t)atoi(argv[2]);
3 
4     my_shm = shmget(shm_key, sizeof(MBUF), 0666|IPC_CREAT);
5 
6     my_sem = semget(sem_key, 1, 0666 | IPC_CREAT);
```

(4)

int semctl(int semid,int semnum,int cmd, /*union semun arg*/);

SETVAL设置信号量集中的一个单独的信号量的值。

```
1 semctl(my_sem, 0, SETVAL, 1);
```


(5)

出错：-1，错误原因存于error中

```
1 p = (pMBUF)shmat(my_shm, NULL, 0);
```


(6)

client_in PV操作：

```
1 while( P(my_sem), p -> m_flag == 1)
2         {
3             V(my_sem);
4             sleep(1);
5         }
6         strcpy(p ->m_buf, line);
7         p ->m_flag = 1 ;
8         V(my_sem);
```

 (7)

出错：-1，错误原因存于error中

```
1 shmdt(p);
```

(8)

删除内存 删除 信号量

出错：-1，错误原因存于error中


```
1 shmctl(my_shm, IPC_RMID, NULL) ;
2 semctl(my_sem, 0, IPC_RMID);//0为信号量编号
```


# 3、clients - server 通信

## （1）上下线信息管道

server：

```
1 char file_name[128]  = "";
 2     char client_r[128], client_w[128];
 3     char line[32] ;
 4     int fd_read ;
 5     int client_id ;
 6     int fd_cr , fd_cw ;
 7     FILE* fp ;
 8     sprintf(file_name, "%s/%s", PATH, NAME);
 9     mkfifo(file_name, 0666) ;
10     fd_read = open(file_name, O_RDONLY);
11     open(file_name, O_WRONLY);
12     
13     fp = fdopen(fd_read, "r");
```

client_in:

```
1 char server_name[128]= "" ;
2     char read_file[128], write_file[128] ;
3     char msg[32] ="" ;
4     int fd_r, fd_w ;
5     sprintf(server_name, "%s/%s", PATH, SERVER);
6     int fd_server = open(server_name, O_WRONLY);
7     sprintf(msg, "%d\n", getpid());
8     write(fd_server, msg, strlen(msg));
```

## (2) 读写管道

client_in：

```
1 memset(read_file, 0, 128);
 2     memset(write_file, 0, 128);
 3 
 4     sprintf(read_file, "%s/%d_r.fifo", PATH, getpid());
 5     sprintf(write_file, "%s/%d_w.fifo", PATH, getpid());
 6 
 7     mkfifo(read_file, 0666);
 8     mkfifo(write_file, 0666);
 9 
10     fd_r = open(read_file, O_RDONLY);
11     fd_w = open(write_file, O_WRONLY);
```

server:

```
while(memset(line, 0 , 32), fgets(line, 32, fp) != NULL)// "pid\n"
    {// cr  cw pid_r.fifo  pid_w.fifo
        sscanf(line, "%d", &client_id);
        printf("client: %d request !\n", client_id) ;
        memset(client_r, 0, 128);
        memset(client_w, 0, 128);

        sprintf(client_r, "%s/%d_r.fifo", PATH, client_id);

        sprintf(client_w, "%s/%d_w.fifo", PATH, client_id);

        fd_cw = open(client_r, O_WRONLY);
        fd_cr = open(client_w, O_RDONLY);
        /*
          ****************
                */
        close(fd_cr);
        close(fd_cw);
    }
```

## (3)信息传递

client_in:

```
1 while(memset(line, 0, 1024), fgets(line, 1024, stdin) != NULL)
 2 {
 3 write(fd_w, line, strlen(line));
 4 
 5 memset(line, 0, 1024);
 6 
 7 read(fd_r, line, 1024);
 8 /**
 9 ************
10 */
11 
12 }
```

server:

```
1 write(fd_wr, buf, strlen(buf));
```

# 4、server fork 子进程对客户端传来的信息进行处理

## （1）fork

```
1 if(fork() == 0)
2         {
3             child_main(fd_cr, fd_cw);
4             close(fd_cr);
5             close(fd_cw);
6             exit(1);
7         }
```

##  （2）reserve 函数

```
1 void reverse(char* str)
 2 {
 3     int bg, end ;
 4     char tmp ;
 5     bg = 0 ;
 6     end = strlen(str) - 1 ;
 7     while(bg < end)
 8     {
 9         tmp = str[bg] ;
10         str[bg] = str[end] ;
11         str[end] = tmp ;
12         bg ++ ;
13         end -- ;
14     }
15 
16 }
```

##  （3） child_main  read and write

```
1 void child_main(int fd_rd, int fd_wr)
 2 {
 3     char buf[1024] ;
 4     while(memset(buf, 0, 1024), read(fd_rd, buf, 1024) != 0)
 5     {
 6             reverse(buf);
 7             write(fd_wr, buf, strlen(buf));
 8     }
 9 
10 }
```

##  (4)防止僵尸进程

　　如果子进程先退出，系统不会自动清理掉子进程的环境，而必须由父进程调用wait或waitpid函数来完成清理工作，如果父进程不做清理工作，则已经退出的子进程将成为僵尸进程(defunct)。


```
1 signal(17, child_handle);
```

下次收到17号信号（子进程退出信号） 就 调用 child_handle 函数

```
1 void child_handle(int sig_num)
2 {
3     printf("a child exit!\n");
4     wait(NULL);
5 }
```

# 5、详细代码：

server：

```
1 #include<stdio.h>
 2 #include<stdlib.h>
 3 #include<string.h>
 4 #include<unistd.h>
 5 #include<sys/stat.h>
 6 #include<sys/types.h>
 7 #include<fcntl.h>
 8 #include<signal.h>
 9 #define PATH "/home/comst/pipe"
10 #define NAME "server.fifo"
11 
12 void child_handle(int sig_num)
13 {
14     printf("a child exit!\n");
15     wait(NULL);
16 }
17 void reverse(char* str)
18 {
19     int bg, end ;
20     char tmp ;
21     bg = 0 ;
22     end = strlen(str) - 1 ;
23     while(bg < end)
24     {
25         tmp = str[bg] ;
26         str[bg] = str[end] ;
27         str[end] = tmp ;
28         bg ++ ;
29         end -- ;
30     }
31 
32 }
33 void child_main(int fd_rd, int fd_wr)
34 {
35     char buf[1024] ;
36     while(memset(buf, 0, 1024), read(fd_rd, buf, 1024) != 0)
37     {
38             reverse(buf);
39             write(fd_wr, buf, strlen(buf));
40     }
41 
42 }
43 int main(int argc, char* argv[])
44 {
45     signal(17, child_handle);
46     char file_name[128]  = "";
47     char client_r[128], client_w[128];
48     char line[32] ;
49     int fd_read ;
50     int client_id ;
51     int fd_cr , fd_cw ;
52     FILE* fp ;
53     sprintf(file_name, "%s/%s", PATH, NAME);
54     mkfifo(file_name, 0666) ;
55     fd_read = open(file_name, O_RDONLY);
56     open(file_name, O_WRONLY);
57     
58     fp = fdopen(fd_read, "r");
59 
60     while(memset(line, 0 , 32), fgets(line, 32, fp) != NULL)// "pid\n"
61     {// cr  cw pid_r.fifo  pid_w.fifo
62         sscanf(line, "%d", &client_id);
63         printf("client: %d request !\n", client_id) ;
64         memset(client_r, 0, 128);
65         memset(client_w, 0, 128);
66 
67         sprintf(client_r, "%s/%d_r.fifo", PATH, client_id);
68 
69         sprintf(client_w, "%s/%d_w.fifo", PATH, client_id);
70 
71         fd_cw = open(client_r, O_WRONLY);
72         fd_cr = open(client_w, O_RDONLY);
73         if(fork() == 0)
74         {
75             child_main(fd_cr, fd_cw);
76             close(fd_cr);
77             close(fd_cw);
78             exit(1);
79         }
80         close(fd_cr);
81         close(fd_cw);
82     }
83 
84 
85 
86     
87     memset(file_name, 0, 128);
88     sprintf(file_name, "%s/%s", PATH, NAME);
89     unlink(file_name);
90     return 0 ;
91 }
```



client.h:

```
1 #ifndef __CLINET_H__
 2 #define __CLINET_H__
 3 
 4 #include<stdio.h>
 5 #include<stdlib.h>
 6 #include<string.h>
 7 #include<unistd.h>
 8 #include<sys/types.h>
 9 #include<sys/ipc.h>
10 #include<sys/shm.h>
11 #include<sys/sem.h>
12 typedef struct tag
13 {
14     int m_flag ;
15     char m_buf[1024] ;
16 }MBUF, *pMBUF;
17 void P(int semid) ;
18 void V(int semid);
19 #endif
```





client_in:

```
1 #include "client.h"
 2 #include<sys/stat.h>
 3 #include<sys/types.h>
 4 #include<sys/fcntl.h>
 5 #define PATH "/home/comst/pipe"
 6 #define SERVER "server.fifo"
 7 int main(int argc, char* argv[])//shm_key sem_key
 8 {
 9 
10     char server_name[128]= "" ;
11     char read_file[128], write_file[128] ;
12     char msg[32] ="" ;
13     int fd_r, fd_w ;
14     sprintf(server_name, "%s/%s", PATH, SERVER);
15     int fd_server = open(server_name, O_WRONLY);
16     sprintf(msg, "%d\n", getpid());
17     write(fd_server, msg, strlen(msg));
18 
19     memset(read_file, 0, 128);
20     memset(write_file, 0, 128);
21 
22     sprintf(read_file, "%s/%d_r.fifo", PATH, getpid());
23     sprintf(write_file, "%s/%d_w.fifo", PATH, getpid());
24 
25     mkfifo(read_file, 0666);
26     mkfifo(write_file, 0666);
27 
28     
29 
30 
31     fd_r = open(read_file, O_RDONLY);
32     fd_w = open(write_file, O_WRONLY);
33 
34 
35     key_t shm_key, sem_key ;
36     int my_shm, my_sem ;
37 
38     char line[1024] ;
39 
40     pMBUF p ;
41     shm_key = (key_t)atoi(argv[1]);
42     sem_key = (key_t)atoi(argv[2]);
43 
44     my_shm = shmget(shm_key, sizeof(MBUF), 0666|IPC_CREAT);
45 
46     my_sem = semget(sem_key, 1, 0666 | IPC_CREAT);
47     semctl(my_sem, 0, SETVAL, 1);
48 
49 
50     p = (pMBUF)shmat(my_shm, NULL, 0);
51     memset(p, 0, sizeof(MBUF));
52 
53     while(memset(line, 0, 1024), fgets(line, 1024, stdin) != NULL)
54     {
55         write(fd_w, line, strlen(line));
56 
57         memset(line, 0, 1024);
58 
59         read(fd_r, line, 1024);
60         while( P(my_sem), p -> m_flag == 1)
61         {
62             V(my_sem);
63             sleep(1);
64         }
65         strcpy(p ->m_buf, line);
66         p ->m_flag = 1 ;
67         V(my_sem);
68 
69     }
70 
71     while( P(my_sem), p -> m_flag == 1)
72     {
73         V(my_sem);
74         sleep(1);
75     }
76     strcpy(p ->m_buf, "over");
77     p ->m_flag = 1 ;
78     V(my_sem);
79 
80     sleep(3);
81 
82 
83 
84 
85     shmdt(p);
86     shmctl(my_shm, IPC_RMID, NULL) ;
87 
88     semctl(my_sem, 0, IPC_RMID);
89 
90 
91 
92 }
```



client_out:

```
1 #include "client.h"
 2 int main(int argc, char* argv[])//shm_key sem_key
 3 {
 4     key_t shm_key, sem_key ;
 5     int my_shm, my_sem ;
 6 
 7     char line[1024] ;
 8 
 9     pMBUF p ;
10     shm_key = atoi(argv[1]);
11     sem_key = atoi(argv[2]);
12 
13     my_shm = shmget(shm_key, sizeof(MBUF), 0666);
14 
15     my_sem = semget(sem_key, 1, 0666 );
16     semctl(my_sem, 0, SETVAL, 1);
17 
18 
19     p = (pMBUF)shmat(my_shm, NULL, 0);
20     memset(p, 0, sizeof(MBUF));
21 
22     while(1)
23     {
24         while(P(my_sem), p -> m_flag == 0)
25         {
26             V(my_sem);
27             sleep(1);
28         }
29         printf("%d :  %s\n", getpid(), p -> m_buf);
30         if(strcmp(p ->m_buf, "over") == 0)
31         {
32 
33             V(my_sem);
34             break ;
35         }
36         p -> m_flag = 0 ;
37         V(my_sem);
38 
39     }
40 
41 
42 
43 
44 
45     shmdt(p);
46 
47 
48 
49 
50 }
```




P_V func

```
1 #include "client.h"
 2 void P(int semid) 
 3 {
 4     struct sembuf my_buf ;
 5     memset(&my_buf, 0, sizeof(my_buf) );
 6     my_buf.sem_num = 0 ;
 7     my_buf.sem_op = -1 ;
 8     my_buf.sem_flg = SEM_UNDO ;
 9     semop(semid, &my_buf, 1);
10 }
11 void V(int semid)
12 {
13     struct sembuf my_buf ;
14     memset(&my_buf, 0, sizeof(my_buf) );
15     my_buf.sem_num = 0 ;
16     my_buf.sem_op = 1 ;
17     my_buf.sem_flg = SEM_UNDO ;
18     semop(semid, &my_buf, 1);
19     
20 }
```

![](https://img-blog.csdn.net/20170507094908078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


