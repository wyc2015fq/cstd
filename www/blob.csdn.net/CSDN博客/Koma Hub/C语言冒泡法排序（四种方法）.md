# C语言冒泡法排序（四种方法） - Koma Hub - CSDN博客
2017年12月04日 19:00:08[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：1003
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
void exchange(int *a, int *b)
{
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
}
void BubbleSort1(int a[], int n)
{
    int i,j;
    for(i=0;i<n-1;i++)
    {
        for(j=0;j<n-i-1;j++)
        {
            if(a[j]>a[j+1]) //从小到大排列
            {
                exchange(&a[j],&a[j+1]);
            }
        }
    }
}
void BubbleSort2(int a[], int n)
{
    int i,j;
    for(i=1;i<n;i++)
    {
        for(j=0;j<n-i;j++)
        {
            if(a[j]>a[j+1]) //从小到大排列
            {
                exchange(&a[j],&a[j+1]);
            }
        }
    }
}
void BubbleSort3(int a[], int n)
{
    int i,j;
    for(i=0;i<n-1;i++)
    {
        for(j=i;j<n;j++)
        {
            if(a[i]>a[j]) //从小到大排列
            {
                exchange(&a[i],&a[j]);
            }
        }
    }
}
void BubbleSort4(int a[], int n)
{
    int i,j;
    bool change;
    for ( i=0; i<n-1; i++ ) 
    {
        change = false;
        for ( j=0; j<n-i-1; j++ )
            if ( a[j]>a[j+1] ) //从小到大排列
            {
                exchange(&a[j],&a[j+1]);
                change = true;
            }
        if ( !change ) break;
    }
}
void show(int *a, int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        printf("%d - ",a[i]);
    }
    printf("\n");
}
void main(int argc, char*argv[])
{
    int a[10] = {2,1,4,3,8,7,6,9,5,0};
    bool b;
    show(a,10);
    BubbleSort4(a,10);
    show(a,10);
}
```
