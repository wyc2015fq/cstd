# linux socket 传输大文件解决方案

置顶 2015年06月14日 11:08:46 [海南之恋](https://me.csdn.net/zhqianpeng) 阅读数：1671



思路： 1  服务器端 把大文件分包 每一个包大小建立一个socket（对应一个线程）进行传输

​           2 客户端 对每一个包对应一个线程（同时对应一个socket）进行接收

​      3 在发送每一个包时 包前要发送这个包的大小 和对应的偏移量

​      4  在32位系统中要开启大文件的宏开关

​      5 利用pread pwrite 可以保证原子操作

废话不多说 直接附上源码 测试结果 在局域网中 能达到10Mb/s 若有问题 欢迎直接交流 qq:294178101



```cpp
/*



*******************
server.cpp 源代码 by 海南之恋



说明： 在32系统中运行  



*******************



 



*/



//#define _XOPEN_SOURCE 500  



#define _FILE_OFFSET_BITS 64  //开启大文件支持宏开关



//#define _LARGEFILE64_SOURCE



#include <stdio.h>



#include <stdlib.h>



#include <string.h>



#include <unistd.h>



#include <sys/types.h>



#include <sys/socket.h>



#include <sys/time.h>



#include <netinet/in.h>



#include <arpa/inet.h>



#include <pthread.h>



#include <fcntl.h>



#include <errno.h>



#include <sys/stat.h>



 



 



 



#define BUFFER_SIZE 4096



#define MAX_LISTEN 2048



 



typedef struct



{



	long long cur;



	int size;



	int sockfd;



}thread_data;



int filefd;



int file_block_size = 0;



 



void* sender(void* s)



{



    thread_data tdata = *((thread_data*)s);



    long long cur = tdata.cur;



    int size = tdata.size;



    int connfd = tdata.sockfd;



    char buf[BUFFER_SIZE] = {0};



    int read_count;



    char head_buf[29] = {0};



	snprintf(head_buf,29,"%016lld:%011d",cur,size);



    send(connfd, &head_buf, strlen(head_buf), 0);



	long long  read_size = 0;



    while (size)



    {    // (sizeof(buf) < size)?sizeof(buf):size 最后一次读取的时候 千万不要多读哦



        read_count = pread(filefd, buf,(sizeof(buf) < size)?sizeof(buf):size, cur + read_size);



        if (read_count < 0 && errno == EINTR)



        {                           //pread 是原子操作 这样的话可以多个线程同时读写一个文件



            puts("break by signal");



			continue;



        }



        else if (read_count == 0)



        {



            break;



        }



		else if(read_count < 0)



		{



			perror("pread"); exit(1);



		}



        send(connfd, buf, read_count, 0);



        size -= read_count;



        read_size += read_count;



    }



    close(connfd);



	printf("cur = %lld, end\n",tdata.cur);



    free(s); //要释放掉哦



	



    pthread_exit((void*)connfd);



}



 



int main(int argc, char** argv)



{



    if (argc != 3)



    {



        fprintf(stderr, "please input: %s  port filename\n", argv[0]);



        exit(1);



    }



    printf("sizeof(off_t) = %d\n",sizeof(off_t)); //开启宏开关后 32系统该值为8字节



    const int port = atoi(argv[1]);



    const char* filename = argv[2];



 



    struct stat statbuf;



    if (lstat(filename, &statbuf) < 0)//超时2G的一定要开启大文件 宏开关



    {



        perror("lstat error");



        exit(EXIT_FAILURE);



    }



 



    long long file_len = (long long)statbuf.st_size;



    printf("file len: %lld\n",file_len);



	if(file_len > (long long)1024*1024*1024*2)



	{



		file_block_size = 1024*1024*128;



	}



	else



	{



		file_block_size = 1024*1024*64;



	}



	printf("file_block_size = %d\n", file_block_size);



    int packcount = file_len / file_block_size;



    int lastpacksize = int(file_len - packcount * file_block_size);



    if (lastpacksize){packcount++;}



        



	printf("packcount = %d\n",packcount);



    struct sockaddr_in server_address;



    memset(&server_address, 0, sizeof(server_address));



    server_address.sin_family = AF_INET;



    server_address.sin_port = htons(port);



	server_address.sin_addr.s_addr = INADDR_ANY;



    int listenfd = socket(AF_INET, SOCK_STREAM, 0);



    if (listenfd < 0)



    {



        perror("socket error");



        exit(EXIT_FAILURE);



    }



 



    int reuse = 1; //端口复用



    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)



    {



        perror("setsockopt SO_REUSEADDR error");



        exit(EXIT_FAILURE);



    }



 



    if (bind(listenfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)



    {



        perror("bind error");



        exit(EXIT_FAILURE);



    }



 



    if (listen(listenfd, MAX_LISTEN) < 0) //注意监听数 一定要大于开启的线程数哦



    {



        perror("listen error");



        exit(EXIT_FAILURE);



    }



	else



	{



		printf("listen success\n");



	}



    struct timeval start, end;



    gettimeofday(&start, NULL);



    int connfd = accept(listenfd, NULL, NULL);



    if (connfd < 0)



    {



        perror("accept error");



        exit(EXIT_FAILURE);



    }



 



    int thread_number = htonl(packcount); //尽量用htonl 尽量不要用htons 本机字节序转换成网络字节序



									//一般系统本机都是使用小段 网络字节序是大端							



    int ret = send(connfd, &thread_number, sizeof(thread_number), 0);



    if (ret < 0)



    {



        perror("send error");



        exit(EXIT_FAILURE);



    }



    close(connfd);



 



    pthread_t* tid = (pthread_t*)malloc(packcount * sizeof(pthread_t)); //要么全局变量 要么自己申请



                                                        //自己申请 记得要最后释放掉



    if ( (filefd = open(filename, O_RDONLY)) < 0)



    {



        perror("open error");



        exit(EXIT_FAILURE);



    }



 



    for (long long i = 0; i < packcount; ++i)



    {



        connfd = accept(listenfd, NULL, NULL);



        if (connfd < 0)



        {



            perror("accept error");



            exit(EXIT_FAILURE);



        }



        thread_data* data = (thread_data*)malloc(sizeof(thread_data)); //给线程传参 线程执行完后在线程中释放掉



        if (i == packcount - 1 && lastpacksize)



        {



            data->cur = (long long)i * file_block_size; //注意偏移量一定是long long 型的



			printf("data->cur = %lld\n",data->cur);



            data->size = lastpacksize;



        }



        else



        {



            data->cur = i * file_block_size;



            data->size = file_block_size;



        }



        data->sockfd = connfd;



        pthread_create(&tid[i], NULL, sender, (void*)data);



    }



 



    for (int i = 0; i < packcount; ++i)



    {



        void* ret;



        pthread_join(tid[i], &ret);



        printf("the thread which handling %d connecttion socket finished sending\n", (int)ret); //注意是void* 与int 之间的转换



    }



 



    close(listenfd);



    close(filefd);



    free(tid);



 



    gettimeofday(&end, NULL);



    double timeuse = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;



    timeuse /= 1000000;



    printf("run time = %f\n", timeuse);



 



    exit(EXIT_SUCCESS);



}
/*



*******************
client.cpp 源代码 by 海南之恋



说明： 在32系统中运行 



*******************



 



*/



//#define _XOPEN_SOURCE 500  



#define _FILE_OFFSET_BITS 64  //开启大文件宏开关



//#define _LARGEFILE64_SOURCE



#include <stdio.h>



#include <stdlib.h>



#include <string.h>



#include <unistd.h>



#include <fcntl.h>



#include <sys/types.h>



#include <sys/socket.h>



#include <netinet/in.h>



#include <arpa/inet.h>



#include <errno.h>



#include <sys/time.h>



#include <pthread.h>



 



#define BUFFER_SIZE 4096



 



//32系统 可能会截断 一定要自己写 或则atoll有的系统不支持 故



long long ato_ll(const char* p_str) //string --> long long 



{



  



  long long result = 0;



  long long mult = 1;



  unsigned int len = strlen(p_str); // strlen(p_str) unsigned int



  unsigned int i;



 



  for (i=0; i<len; ++i)



  {



    char the_char = p_str[len-(i+1)];



    long long val;



    if (the_char < '0' || the_char > '9')



    {



      return 0;



    }



    val = the_char - '0';



    val *= mult;



    result += val;



    mult *= 10;



  }



  return result;



}



 



 



 



 



 



 



struct sockaddr_in server_address;



int filefd;



 



 



 



void* receive(void* s)



{



    int thread_order = *(int*)s;



   



    printf("thread_order = %d\n",thread_order);



    char buf[BUFFER_SIZE];



    int sockfd = socket(AF_INET, SOCK_STREAM, 0);



    if (sockfd < 0)



    {



        perror("socket error");



        exit(EXIT_SUCCESS);



    }



 



    if (connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)



    {



        fprintf(stderr, "thread %d connect error number %d: %s\n",thread_order, errno, strerror(errno));



        exit(EXIT_FAILURE);



    }



	printf("conncet success,thread id = %d\n",thread_order);



	char head_buf[29] = {0};



    int ret = recv(sockfd, head_buf, sizeof(head_buf) - 1, MSG_WAITALL); //接受每个包的头部



    if (ret < 0)



    {



        fprintf(stderr, "thread %d recv error number %d: %s\n",



                thread_order, errno, strerror(errno));



        exit(EXIT_FAILURE);



    }



	char* cur_ptr = head_buf;



	char* bk = strchr(head_buf,':');



	if(bk!=NULL)



	{



		*bk = '\0';



	}



	char* size_ptr = bk + 1;



	



    long long cur = ato_ll(cur_ptr);



    int size = atoi(size_ptr);



	printf("thread %d cur = %lld size = %d\n",thread_order,cur,size);



    while (size)



    {



        ret = read(sockfd, buf, BUFFER_SIZE);



        if (ret < 0 && errno ==EINTR)



        {



            puts("break by signal");



			continue;



        }



        else if (ret == 0)



        {



            break;



        }



		else if(ret < 0)



		{



			perror("read");



			exit(1);



		}



        if(pwrite(filefd, buf, ret, cur) < 0)



		{



			perror("pwrite");



			exit(1);



		}



        cur += ret;



        size -= ret;



    }



 



    close(sockfd);



    fprintf(stderr, "thread %d finished receiving\n", thread_order);



	free(s);



    pthread_exit((void*)thread_order);



}



 



int main(int argc, char** argv)



{



    if (argc != 3)



    {



        fprintf(stderr, "usage: %s server_ip port\n", argv[0]);



        exit(EXIT_FAILURE);



    }



    const char* ip = argv[1];



    const int port = atoi(argv[2]);



	printf("sizeof(off_t) = %d\n",sizeof(off_t));



    memset(&server_address, 0, sizeof(server_address));



    server_address.sin_family = AF_INET;



    server_address.sin_port = htons(port);



    inet_pton(AF_INET, ip, &server_address.sin_addr);



 



    int sockfd = socket(AF_INET, SOCK_STREAM, 0);



    if (sockfd < 0)



    {



        perror("socket error");



        exit(EXIT_FAILURE);



    }



 



    struct timeval start, end;



    gettimeofday(&start, NULL);



    if (connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)



    {



        perror("connect error");



        exit(EXIT_FAILURE);



    }



 



    int thread_number = 0;



    int ret = recv(sockfd, &thread_number, sizeof(thread_number), MSG_WAITALL);



    if (ret < 0)



    {



        perror("recv MSG_WAITALL error");



        exit(EXIT_FAILURE);



    }



	



    thread_number = ntohl(thread_number); //网络字节序转换成本机字节序



	printf("thread_number = %d\n",thread_number);



	if(thread_number >500){puts(">>>>500");exit(1);} //开的线程太多了



	//O_TRUNC 若文件存在 则把文件截断为零



    if ( (filefd = open("receive_file.rmvb", O_WRONLY | O_CREAT |O_TRUNC, 0777)) < 0)



    {



        perror("open error");



        exit(EXIT_FAILURE);



    }



	else



	{



		printf("open success\n");



	}



    pthread_t* tid = (pthread_t*)malloc(thread_number * sizeof(pthread_t));



	if(tid==NULL)



	{



		perror("malloc::");



		exit(1);



	}



	printf("thread_number = %d\n",thread_number);



	



    for (int i = 0; i < thread_number; ++i)



    {



	



		int* thread_id = (int*) malloc(sizeof(int)); //记得在线程中释放



		*thread_id = i;



        pthread_create(&tid[i], NULL, receive, (void*)thread_id);



 



    }



 



    for (int i = 0; i < thread_number; ++i)



    {



        char *ret;



        pthread_join(tid[i], (void**)&ret);



        printf("thread %d finished receiving\n", i);



    }



 



    close(sockfd);



    close(filefd);



    free(tid);



 



    gettimeofday(&end, NULL);



    double timeuse = 1000000*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;



    timeuse /= 1000000;



    printf("run time = %f\n", timeuse);



 



    exit(EXIT_SUCCESS);



}
```