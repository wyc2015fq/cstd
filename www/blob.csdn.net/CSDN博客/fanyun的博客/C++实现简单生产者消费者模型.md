# C++实现简单生产者消费者模型 - fanyun的博客 - CSDN博客
2018年02月25日 22:13:21[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：437
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
      消费者/生产者模型是多线程编程开发的常用模型，该模型通过平衡生产者线程和消费者线程的工作能力来提高程序整体的数据处理能力。
       一般来说，我们的资源池是有限的，访问资源需要加锁，访问完以后需要解锁，生产者需要在资源池未满的情况下才能生产产品，消费者需要在资源池不空的情况下才能消费产品。且在设计时需要注意死锁问题。
       下面采用semaphore,资源池为环形缓冲区，来实现生产者消费者模型：    
```cpp
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERROR(func, no) { \
        fprintf(stderr, "%s: %s\n", func, strerror(no)); \
        exit(EXIT_FAILURE); \
    }
#define DEFAULT_CONSUMER_CNT    1
#define DEFAULT_PRODUCER_CNT    3
#define DEFAULT_BUFFER_SIZE 10
static size_t in; //  producer's current pos
static size_t out; // consumer's current pos
static size_t consumer_id; // current product id for consumer 
static size_t producer_id; // current product id for producer
static size_t consumer_cnt; // count of consumers
static size_t producer_cnt; // count of producers
static size_t buff_size; // resource buffer size
static int* g_buffer = NULL; // pointer to resource buffer
static pthread_t* g_thread = NULL; // pointer to thread IDs
static pthread_mutex_t g_mutex; //mutex
static sem_t* g_sem_empty_ptr = NULL; // semaphore for consumer
static sem_t* g_sem_full_ptr = NULL; // semaphore for producer
void* consume(void* arg) {
    int id = *(int*)arg;
    free(arg);
    while(1) {
        printf("Consumer[%d] waits buffer not empty\n", id);
        sem_wait(g_sem_empty_ptr);
        pthread_mutex_lock(&g_mutex);
        int i, flag;
        for(i=0; i<buff_size; ++i) {
            printf("%2d:", i);
            if(0 == g_buffer[i]) { 
                printf(" nil");
                if(out == i) {
                    out = (out+1)%buff_size;
                    printf("\n");
                    flag = 1;
                    continue;
                }
            } else {
                printf(" %d", g_buffer[i]);
            }
            if(out == i) {  
                printf("\t<-- consume[%d]", id);
                flag = 0;
            }
            printf("\n");
        }
        if(0 == flag) {
            consumer_id = g_buffer[out];
            printf("Consumer[%d] begins consuming product %lu\n", id, consumer_id);
            g_buffer[out] = 0;
            out = (out+1)%buff_size;
            printf("Consumer[%d] ends consuming product %lu\n", id, consumer_id);
            pthread_mutex_unlock(&g_mutex);
            sem_post(g_sem_full_ptr);
        } else {
            printf("*************No product to consume\n"); 
            pthread_mutex_unlock(&g_mutex);
        }
        sleep(1);
    }
    return NULL;
}
void* produce(void* arg) {
    int id = *(int*)arg;
    free(arg);
    while(1) {
        printf("Producer[%d] waits buffer not full\n", id);
        sem_wait(g_sem_full_ptr);
        pthread_mutex_lock(&g_mutex);
        int i, flag;
        for(i=0; i<buff_size; ++i) {
            printf("%2d:", i);
            if(0 == g_buffer[i]) {
                printf(" nil");
            } else {
                printf(" %d", g_buffer[i]);
                if(in == i) {
                    in = (in+1)%buff_size;
                    printf("\n");
                    flag = 1;
                    continue;
                }
            }
            if(in == i) {
                printf("\t<-- produce[%d]", id);
                flag = 0;
            }
            printf("\n");
        }
        if(0 == flag) {
            g_buffer[in] = ++producer_id;
            printf("Producer[%d] begins to produce product %lu\n", id, producer_id);
            in = (in+1)%buff_size;
            printf("Producer[%d] ends to produce product %lu\n", id, producer_id);
            pthread_mutex_unlock(&g_mutex);
            sem_post(g_sem_empty_ptr);
        } else {
            printf("*************No buffer to product\n");
            pthread_mutex_unlock(&g_mutex);
        }
        sleep(1);
    }
    return NULL;
}
int main(int argc, char** argv)
{
        consumer_cnt = DEFAULT_CONSUMER_CNT;
        producer_cnt = DEFAULT_PRODUCER_CNT;
    buff_size = DEFAULT_BUFFER_SIZE;
        char* prog = argv[0];
        int ch; 
        while ((ch = getopt(argc, argv, "b:c:p:")) != -1) {
             switch (ch) {
             case 'b':
                     buff_size = atoi(optarg);
                     //printf("-b option: %s\n", optarg);
             case 'c':
                     consumer_cnt = atoi(optarg);
                     //printf("-c option: %s\n", optarg);
                     break;
             case 'p':
                     producer_cnt = atoi(optarg);
                     //printf("-p option: %s\n", optarg);
                     break;
             case '?':
             default:
                     printf("Usage: %s [-b buffersize] [-p producer_cnt] [-c consumer_cnt]\n"
                                "\tdefault buffersize=10, producer_cnt=3, consumer_cnt=1\n", prog);
                     exit(EXIT_FAILURE);
             }   
        }
    g_buffer = (int*)malloc(buff_size*sizeof(int));
    g_thread = (pthread_t*)malloc((consumer_cnt+producer_cnt)*sizeof(pthread_t));
    memset(g_buffer, 0, buff_size*sizeof(int));
    memset(g_thread, 0, (consumer_cnt+producer_cnt)*sizeof(pthread_t));
    g_sem_full_ptr = (sem_t*)malloc(sizeof(sem_t));
    g_sem_empty_ptr = (sem_t*)malloc(sizeof(sem_t));
    g_sem_full_ptr = sem_open("sem_producer", O_CREAT, 0, buff_size); //set semaphore full initially to the buffer length 
    g_sem_empty_ptr = sem_open("sem_consumer", O_CREAT, 0, 0); //set semaphore full initially to 0
    pthread_mutex_init(&g_mutex, NULL);
    int i, ret;
    for(i=0; i<consumer_cnt; ++i) {
        void *arg = malloc(sizeof(int));
        memcpy(arg, &i, sizeof(int));
        ret = pthread_create(&g_thread[i], NULL, consume, arg);
        if(ret) {
            ERROR("pthread_create", ret);
        }
    }
    for(i=0; i<producer_cnt; ++i) {
        void *arg = malloc(sizeof(int));
        memcpy(arg, &i, sizeof(int));
        ret = pthread_create(&g_thread[i+consumer_cnt], NULL, produce, arg);
        if(ret) {
            ERROR("pthread_create", ret);
        }
    }
    for(i=0; i<consumer_cnt+producer_cnt; ++i) {
        ret = pthread_join(g_thread[i], NULL);
        if(ret) {
            ERROR("pthread_join", ret);
        }
    }
    sem_close(g_sem_full_ptr);
    sem_close(g_sem_empty_ptr);
    sem_unlink("sem_consumer");
    sem_unlink("sem_producer");
    pthread_mutex_destroy(&g_mutex);
    free(g_buffer);
    free(g_thread);
    free(g_sem_full_ptr);
    free(g_sem_empty_ptr);
    exit(EXIT_SUCCESS);
```
