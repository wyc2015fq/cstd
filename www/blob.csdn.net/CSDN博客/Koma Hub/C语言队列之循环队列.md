# C语言队列之循环队列 - Koma Hub - CSDN博客
2017年12月05日 19:43:54[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：288
```cpp
#include<stdio.h>
#include<stdlib.h>
#define max 10
typedef struct {
    
    int data[max];
    int front;
    int rear;
    
}Queue;
int init(Queue *Q)
{
    Q->front = Q->rear = 0;
    return 1;
}
int clear(Queue *Q)
{
    Q->front = 0;
    Q->rear = 0;
    return 1;
}
int length(Queue *Q)
{
    return (Q->rear - Q->front)%max + 1;
}
int insert(Queue *Q,int x)
{
    if((Q->rear+1)%max==Q->front)
    {
        printf("Queue is Full.");
        return 0;
    }
    Q->data[Q->rear] = x;
    Q->rear = (Q->rear+1)%max;
}
int show(Queue *Q)
{
    printf("show: ");
    if(Q->front == Q->rear)
        if(Q->front == Q->rear == 0)
        {
            printf("Queue is empty.\n");
            return 0;
        }
        else 
            printf("Queue is full.\n");
    int i;
    for(i=(Q->rear)<(Q->front)?(Q->rear):(Q->front);i<(Q->rear)>(Q->front)?(Q->rear):(Q->front);i++)
        printf("%d - ",Q->data[i]);
    printf("\n");
}
int main(int argc, char*argv[])
{
    int a[10] = {1,2,3,4,5,6,7,8,9,0};
    
    Queue Q;
    
    init(&Q);
    
    insert(&Q, 1);
    insert(&Q, 2);
    clear(&Q);
    insert(&Q, 4);
    insert(&Q, 9);
    insert(&Q, 14);
    show(&Q);
    
	return 0;
}
```
