# C语言线性表之顺序表 - Koma Hub - CSDN博客
2017年12月04日 19:32:23[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：195
个人分类：[C/C++																[Algorithm](https://blog.csdn.net/Rong_Toa/article/category/7221428)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
```cpp
#include<stdio.h>
#include<stdlib.h>
typedef int bool;
#ifndef false
#define false 0
#endif
#ifndef true 
#define true 1
#endif
#define MaxSize 100
typedef struct{
    int elem[MaxSize];
    int length;
} List;
/* 基本算法——遍历 */
void show(List L)
{
    int i;
    for(i=0;i<L.length;i++)
    {
        printf("%d - ",L.elem[i]);
    }
    printf("\n");
}
/* 基本算法——set */
void set(int *a, List *L)
{
    int i;
    for(i=0;i<L->length;i++)
    {
        L->elem[i] = a[i];
    }
}
/* 插入算法 */
void insert(List *L, int index, int val)
{
    if(index<0||index>L->length)
    {
        printf("Out of the List boundary\n");
        return;
    }
    L->length += 1;
    int i;
    for(i=L->length-1;i>index;i--)
    {
        L->elem[i] = L->elem[i-1];
    }
    L->elem[index] = val;
}
/* 删除算法 */
void delete(List *L, int index)
{
    if(index<0||index>=L->length)
    {
        printf("Out of the List boundary\n");
        return;
    }
    int i;
    for(i =index; i<L->length-1;i++)
    {
        L->elem[i] = L->elem[i+1];
    }
    L->length -= 1;
}
void main(int argc, char*argv[])
{
    List L;
    int a[10] = {0,1,2,3,4,5,6,7,8,9};
    L.length = 10;
    set(a, &L);
    show(L);
    insert(&L, 3, 12);
    show(L);
    delete(&L, 6);
    show(L);
}
```
结果：
```
-bash-4.1$ gcc -o a Demo.c
-bash-4.1$ ./a
0 - 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 
0 - 1 - 2 - 12 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 
0 - 1 - 2 - 12 - 3 - 4 - 6 - 7 - 8 - 9 -
```
