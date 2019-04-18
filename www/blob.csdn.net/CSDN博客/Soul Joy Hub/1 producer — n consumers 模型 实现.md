# 1 producer — n consumers 模型 实现 - Soul Joy Hub - CSDN博客

2016年06月13日 10:05:48[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：407标签：[多线程](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)
个人分类：[并发/网络编程](https://blog.csdn.net/u011239443/article/category/6268713)



http://blog.csdn.net/u011239443/article/details/51655475

![](http://images.cnitblog.com/blog/709693/201502/071441001721894.png)

```
1 #include<stdio.h>
  2 #include<string.h>
  3 #include<pthread.h>
  4 #include<stdlib.h>
  5 #include<unistd.h>
  6 #include<queue>
  7 using namespace std;
  8 #define LEN 128
  9 typedef struct task_que
 10 {
 11     queue<char*> string_queue;
 12     int m_flag ;        //结束标志
 13     int m_capacity;
 14     pthread_mutex_t m_lock;
 15     pthread_cond_t m_pro , m_con;
 16 }QUE , *pQUE;
 17 void* thd_func(void* arg);
 18 void put(pQUE pq, char* src);
 19 void get(pQUE pq, char* dest);
 20 int main(int argc, char *argv[])
 21 {
 22     QUE aque;
 23     aque.m_flag = 0;
 24     int nthds = atoi( argv[1] );   //线程的个数
 25     aque.m_capacity = atoi( argv[2] );//字符串队列的大小
 26     pthread_mutex_init( &aque.m_lock , NULL ); //锁
 27     pthread_cond_init( &aque.m_pro ,NULL );   //生产者条件变量
 28     pthread_cond_init( &aque.m_con , NULL );  //消费者条件变量
 29     //开辟线程空间
 30     pthread_t *thd_arr = ( pthread_t*)calloc(nthds, sizeof(pthread_t));
 31     int* ntasks = (int*)calloc(nthds, sizeof(int));//用来记录 线程工作了几次
 32     //创建线程
 33     int index;
 34     for(index = 0; index < nthds; index ++)
 35     {    
 36         pthread_create( thd_arr + index, NULL, thd_func,(void*)&aque );
 37     }
 38 
 39 
 40     //输入字符串到队列中
 41     char buf[LEN] ;
 42     while( memset(buf, 0, LEN), fgets(buf, LEN, stdin) != NULL)
 43     {
 44         put(&aque, buf);
 45     }
 46 
 47 
 48 
 49     //发出结束字符串
 50     strcpy(buf, "over");
 51     put(&aque, buf);
 52 
 53     for(index = 0; index < nthds; index ++)
 54     {
 55         pthread_join(thd_arr[index], (void**)(ntasks + index ));
 56     }
 57     for(index = 0; index < nthds; index ++)
 58     {
 59         printf("%d ", ntasks[index]);
 60     }
 61     printf("\n");
 62 
 63     pthread_mutex_destroy(&aque.m_lock);
 64     pthread_cond_destroy(&aque.m_pro);
 65     pthread_cond_destroy(&aque.m_con);
 66     return 0 ;
 67 }
 68 void put(pQUE pq,  char* src)      //把字符串写到队列中
 69 {
 70     pthread_mutex_lock(&pq ->m_lock);   //加锁
 71     while(pq ->string_queue.size() == pq ->m_capacity)  //队列满则阻塞
 72     {
 73         pthread_cond_wait(&pq -> m_pro, &pq ->m_lock);
 74 
 75     }
 76     //插入队列
 77     char* tem =  ( char*)calloc( LEN , sizeof( char ));
 78     strcpy(tem,src);
 79     pq->string_queue.push(tem);
 80     pthread_mutex_unlock(&pq -> m_lock);  //解锁
 81     pthread_cond_broadcast(&pq ->m_con);  //唤醒所有消费者线程
 82 
 83 
 84 }
 85 
 86 void get(pQUE pq, char* dest)
 87 {
 88     pthread_mutex_lock(&pq ->m_lock); //加锁
 89     while(pq ->m_flag == 0 &&  pq ->string_queue.empty()  ) //队列空 并且未结束 则阻塞
 90     {
 91         pthread_cond_wait(&pq ->m_con, &pq ->m_lock);
 92     }
 93     if(pq ->m_flag == 1)                 //判断结束标志
 94     {
 95         pthread_mutex_unlock(&pq ->m_lock); //解锁
 96         return ;                         
 97     }
 98     //出队
 99     strcpy(dest, pq ->string_queue.front());
100     pq->string_queue.pop();
101 
102     pthread_mutex_unlock(&pq ->m_lock);
103     pthread_cond_signal(&pq ->m_pro);
104 
105 }
106 
107 void* thd_func(void* arg)
108 {
109     pQUE pq = (pQUE)arg ;
110     char buf[LEN] ;
111     int ncnt = 0 ;
112     while(1)
113     {
114         memset(buf, 0, LEN) ;
115         get(pq, buf);
116         if(pq ->m_flag == 1)            //判断结束标志
117         {
118             printf("%u exit!\n", pthread_self());
119             pthread_exit((void*)ncnt);             //退出
120         }
121         ncnt ++ ;
122         printf("%u: %s\n", pthread_self(), buf);  //打印字符串
123         if(strcmp("over", buf) == 0)              //判断结束字符串        
124         {
125             pq ->m_flag = 1 ;                    //把结束符号置为 1
126             pthread_cond_broadcast(&pq ->m_con); //唤醒所有线程
127             pthread_exit((void*)ncnt);           //退出
128         }
129         if(ncnt & 1 == 1) sleep(1);     //简单的负载平衡
130     }
131 }
```

![](https://img-blog.csdn.net/20170510080041672?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

