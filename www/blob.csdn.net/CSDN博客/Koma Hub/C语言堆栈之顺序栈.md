# C语言堆栈之顺序栈 - Koma Hub - CSDN博客
2017年12月05日 19:30:19[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：111
在linux下GCC这个程序是正确的：
```cpp
#include<stdio.h>
#include<stdlib.h>
#define len 10;
typedef struct{
    
    int elem[10];
    int top;
    
}SqStack;
void show(SqStack S)
{
    printf("show:");
    int i;
    for(i=0;i<S.top;i++)
    {
        printf("%d - ",S.elem[i]);
    }
    printf("\n");
}
SqStack InitStack()
{
    SqStack S;
    S.top = 0;
    return S;
}
void Push(SqStack *S,int x)
{
    S->elem[S->top++] = x;
}
int Pop(SqStack *S)
{
    return S->elem[S->top--];
}
int main(int argc, char*argv[])
{
    int a[10] = {1,2,3,4,5,6,7,8,9,0};
    
    SqStack S = InitStack();
    
    Push(&S, 1);
    Push(&S, 2);
    
    show(S);
    
    Pop(&S);
    
    show(S);
    
	return 0;
}
```
在win10 CMD下的GCC这个程序是正确的：
```cpp
#include<stdio.h>
#include<stdlib.h>
const int len = 10;
typedef struct{
    
    int elem[len];
    int top;
    
}SqStack;
void show(SqStack S)
{
    printf("show:");
    int i;
    for(i=0;i<S.top;i++)
    {
        printf("%d - ",S.elem[i]);
    }
    printf("\n");
}
SqStack InitStack()
{
    SqStack S;
    S.top = 0;
    return S;
}
void Push(SqStack &S,int x)
{
    S.elem[S.top++] = x;
}
int Pop(SqStack &S)
{
    return S.elem[S.top--];
}
int main(int argc, char*argv[])
{
    int a[10] = {1,2,3,4,5,6,7,8,9,0};
    
    SqStack S = InitStack();
    
    Push(S, 1);
    Push(S, 2);
    
    show(S);
    
    Pop(S);
    
    show(S);
    
	return 0;
}
```
