# C语言队列之链队列 - Koma Hub - CSDN博客
2017年12月05日 20:41:58[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：155
个人分类：[C/C++																[Algorithm](https://blog.csdn.net/Rong_Toa/article/category/7221428)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
```cpp
#include<stdio.h>
#include<stdlib.h>
typedef struct Node{
    int data;
    struct Node *next;
}Node , *LinkList;
typedef struct {
    LinkList front, rear;
   
}LinkQueue;
LinkQueue initQueue()
{
    LinkQueue Q;  
    Q.front = Q.rear = (LinkList)malloc(sizeof(Node));
    Q.front->next = NULL;
    return Q;
}
void insert(LinkQueue *Q, int x)
{
    LinkList L = (LinkList)malloc(sizeof(Node));
    
    L->data = x;
    L->next = NULL;
    Q->rear->next = L;
    Q->rear = L;
}
/* 从队头到队尾依次对队列Q中每个元素输出 */
void show(LinkQueue Q)
{
    if(Q.front == Q.rear){
        printf("show: LinkQueue is empty.\n");
    }else{
        Node *p = Q.front->next;
        printf("show: ");
        do{
            printf("%d - ",p->data);
            p = p->next;
        }while(p != NULL);
        printf("\n");
    }
}
void setQueueWithArray(LinkQueue *Q, int *a)
{
    int i=0;
    do{
        insert(Q, a[i]);
    
        i++;
    }while(*(a+i) != 0 && (a+i) != NULL);
}
/* 清空队列 */
void clearQueue(LinkQueue *Q)
{
    Node *p = Q->front;
    Q->front = Q->rear;
    Node *q;
    while(p->next != NULL){
        q = p;
        p = p->next;
        free(q);
    }
}
/* 销毁队列 */
void deleteQueue(LinkQueue *Q)
{
    clearQueue(Q);
    free(Q);
}
int main(int argc, char*argv[])
{
    int a[10] = {1,2,3,4,0,6,7,8,9,0};
    
    LinkQueue Q = initQueue();
    
    show(Q);
    insert(&Q, 11);
    show(Q);
    insert(&Q, 12);
    show(Q);
    clearQueue(&Q);
    show(Q);
    clearQueue(&Q);
    setQueueWithArray(&Q,a);
    setQueueWithArray(&Q,a);
    show(Q);
    //deleteQueue(&Q);
    clearQueue(&Q);
    show(Q);
	return 0;
}
```
结果为：
```
-bash-4.1$ gcc -o a Demo.c
-bash-4.1$ ./a
show: LinkQueue is empty.
show: 11 - 
show: 11 - 12 - 
show: LinkQueue is empty.
show: 1 - 2 - 3 - 4 - 1 - 2 - 3 - 4 - 
show: LinkQueue is empty.
-bash-4.1$
```
如果删除掉了链队列，再输出的话，就会产生如下错误：
```cpp
deleteQueue(&Q);
show(Q);
```
错误如下：
```
-bash-4.1$ gcc -o a Demo.c
-bash-4.1$ ./a
show: LinkQueue is empty.
show: 11 - 
show: 11 - 12 - 
show: LinkQueue is empty.
show: 1 - 2 - 3 - 4 - 1 - 2 - 3 - 4 - 
*** glibc detected *** ./a: munmap_chunk(): invalid pointer: 0x00007ffd8cd04770 ***
======= Backtrace: =========
/lib64/libc.so.6[0x3eb0e75f4e]
./a[0x4007d8]
./a[0x400922]
/lib64/libc.so.6(__libc_start_main+0xfd)[0x3eb0e1ed5d]
./a[0x400529]
======= Memory map: ========
00400000-00401000 r-xp 00000000 08:11 1085796266                         /home/Toa/algorithmsDataStructures/study1/a
00600000-00601000 rw-p 00000000 08:11 1085796266                         /home/Toa/algorithmsDataStructures/study1/a
00b6e000-00b8f000 rw-p 00000000 00:00 0                                  [heap]
3eb0a00000-3eb0a20000 r-xp 00000000 08:03 31457308                       /lib64/ld-2.12.so
3eb0c1f000-3eb0c20000 r--p 0001f000 08:03 31457308                       /lib64/ld-2.12.so
3eb0c20000-3eb0c21000 rw-p 00020000 08:03 31457308                       /lib64/ld-2.12.so
3eb0c21000-3eb0c22000 rw-p 00000000 00:00 0 
3eb0e00000-3eb0f8a000 r-xp 00000000 08:03 31457310                       /lib64/libc-2.12.so
3eb0f8a000-3eb118a000 ---p 0018a000 08:03 31457310                       /lib64/libc-2.12.so
3eb118a000-3eb118e000 r--p 0018a000 08:03 31457310                       /lib64/libc-2.12.so
3eb118e000-3eb118f000 rw-p 0018e000 08:03 31457310                       /lib64/libc-2.12.so
3eb118f000-3eb1194000 rw-p 00000000 00:00 0 
3ebb200000-3ebb216000 r-xp 00000000 08:03 31457727                       /lib64/libgcc_s-4.4.7-20120601.so.1
3ebb216000-3ebb415000 ---p 00016000 08:03 31457727                       /lib64/libgcc_s-4.4.7-20120601.so.1
3ebb415000-3ebb416000 rw-p 00015000 08:03 31457727                       /lib64/libgcc_s-4.4.7-20120601.so.1
7f381fa16000-7f381fa19000 rw-p 00000000 00:00 0 
7f381fa32000-7f381fa35000 rw-p 00000000 00:00 0 
7ffd8ccf0000-7ffd8cd07000 rw-p 00000000 00:00 0                          [stack]
7ffd8cda9000-7ffd8cdaa000 r-xp 00000000 00:00 0                          [vdso]
ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]
已放弃 (core dumped)
```
