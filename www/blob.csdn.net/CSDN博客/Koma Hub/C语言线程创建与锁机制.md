# C语言线程创建与锁机制 - Koma Hub - CSDN博客
2018年06月07日 14:02:09[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：633
```cpp
#include<stdio.h>
#include<pthread.h>   //线程
#include<semaphore.h> //锁
#define N 10
void *thread(void *vargp);
char **ptr;
static int count = 0;
int main(int argc, char **argv)
{
    int i;
    pthread_t tid;
    char *msgs[N] = { "0","1","2","3","4",
                      "5","6","7","8","9"};
    
    ptr = msgs;
    
    for(i=0;i<N;i++)
    {
        pthread_create(&tid, NULL, thread, (void*)i); 
    }
    
    pthread_exit(NULL);
    
    return 0;
}
struct __sem_t *muteX;//锁
void *thread(void *vargp)
{
    int myid = (int)vargp;
    static int cnt = 0;
    count ++;
    sem_init(&muteX, 0, 1);
    sem_wait(&muteX);
    printf("[%d]: %s (cnt = %d), %d\n", 
            myid, ptr[myid], ++cnt, count);
    sem_post(&muteX);
    
    return NULL;
}
```
编译与运行：
```cpp
$ gcc pthread.c
$ ./a.exe
[0]: 0 (cnt = 1), 1
[1]: 1 (cnt = 2), 2
[2]: 2 (cnt = 3), 3
[3]: 3 (cnt = 4), 4
[4]: 4 (cnt = 5), 5
[5]: 5 (cnt = 6), 6
[6]: 6 (cnt = 7), 7
[7]: 7 (cnt = 8), 8
[8]: 8 (cnt = 9), 9
[9]: 9 (cnt = 10), 10
```
此为笔记。            
