# C语言堆栈之链栈 - Koma Hub - CSDN博客
2017年12月05日 19:39:41[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：139
在Linux下的GCC这个程序是正确的：
```cpp
#include<stdio.h>
#include<stdlib.h>
typedef struct Node{
    
    int data;
    struct Node *next;
    
}Node, *Stack;
/*  */
void show(Stack S)
{
    if(S == NULL)
    {
        printf("Stack NULL\n");
        return;
    }
    printf("show:");
    do{
        printf("%d - ",S->data);
        S = S->next;
    }while(S!=NULL);
    printf("\n");
}
void Push(Stack *S, int x)
{
    if(S == NULL)
    {
        printf("Stack NULL\n");
        return;
    }
    Node *node = (Stack)malloc(sizeof(Node));
    node->data = x;
    node->next = *S;
    *S = node;
}
int Pop(Stack *S)
{
    if(S == NULL)
    {
        printf("Stack NULL\n");
        return 0;
    }
    printf(" Pop: ");
    int a = (*S)->data;
    *S = (*S)->next;
    printf("%d",a);
    printf("\n");
    return a;
}
int main(int argc, char*argv[])
{
    int a[10] = {1,2,3,4,5,6,7,8,9,0};
    
    Stack S = (Stack)malloc(sizeof(Node));
    
    S->data = a[0];
    S->next = NULL;
    show(S);
    Push(&S, 2);
    Push(&S, 3);
    show(S);
    Pop(&S);
    show(S);
    
    
	return 0;
}
```
在win10 的CMD中这个程序是正确的：
```cpp
#include<stdio.h>
#include<stdlib.h>
typedef struct Node{
    
    int data;
    struct Node *next;
    
}Node, *Stack;
/*  */
void show(Stack S)
{
    if(S == NULL)
    {
        printf("Stack NULL\n");
        return;
    }
    printf("show:");
    do{
        printf("%d - ",S->data);
        S = S->next;
    }while(S!=NULL);
    printf("\n");
}
void Push(Stack &S, int x)
{
    if(S == NULL)
    {
        printf("Stack NULL\n");
        return;
    }
    Node *node = (Stack)malloc(sizeof(Node));
    node->data = x;
    node->next = S;
    S = node;
}
int Pop(Stack &S)
{
    if(S == NULL)
    {
        printf("Stack NULL\n");
        return 0;
    }
    printf(" Pop: ");
    int a = S->data;
    S = S->next;
    printf("%d",a);
    printf("\n");
    return a;
}
int main(int argc, char*argv[])
{
    int a[10] = {1,2,3,4,5,6,7,8,9,0};
    
    Stack S = (Stack)malloc(sizeof(Node));
    
    S->data = a[0];
    S->next = NULL;
    show(S);
    Push(S, 2);
    Push(S, 3);
    show(S);
    Pop(S);
    show(S);
    
    
	return 0;
}
```
