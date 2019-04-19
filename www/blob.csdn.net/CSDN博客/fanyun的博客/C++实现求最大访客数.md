# C++实现求最大访客数 - fanyun的博客 - CSDN博客
2019年01月31日 19:57:13[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：57
1.题目说明
      现将举行一个餐会，让访客事先填写到达时间与离开时间，为了掌握座位的数目，必须先估计不同时间的最大访客数。
2.实现思路
     这个题目看似有些复杂，其实相当简单，单就计算访客数这个目的，同时考虑同一访客的来访时间与离开时间，反而会使程式变得复杂；只要将来 访时间与离开时间分开处理就可以了，假设访客 i 的来访时间为x[i]，而离开时间为y[i]。在资料输入完毕之后，将x[i]与y[i]分别进行排序 （由小到大），道理很简单，只要先计算某时之前总共来访了多少访客，然后再减去某时之前的离开访客，就可以轻易的解出这个问题。
3.代码实现
```cpp
#include <stdio.h>
#include <stdlib.h>
#define MAX 100
#define SWAP(x, y) {int t; t = x; x = y; y = t;}
int partition(int [], int , int );
void quicksort(int [], int , int );
int maxguest(int [], int[], int, int );
int main(void)
{
    int x[MAX] = {0};
    int y[MAX] = {0};
    int time = 0;
    int count = 0;
    
    printf("\n 输入来访与离开125;时间(0~24): ");
    printf("\n范例: 10 15");
    printf("\n输入-1 -1结束\n");
    while(count < MAX)
    {
        printf("\n>>");
        scanf("%d%d", &x[count], &y[count]);
        if(x[count] < 0)
        {
            break;
        }
        count++;
    }
    
    if(count >= MAX)
    {
        printf("\n超出最大访客数(%d)", MAX);
        count--;
    }
    
    quicksort(x, 0, count);
    quicksort(y, 0, count);
    
    while(time < 25)
    {
        printf("\n %d 时的最大访客数 : %d", time, maxguest(x, y, count, time));
        time++;
    }
    printf("\n");
    return 0;
}
int maxguest(int x[], int y[], int count, int time)
{
    int i, num = 0;
    for(i = 0; i <= count; i++)
    {
        if(time > x[i])
        {
            num++;
        }
        if(time > y[i])
        {
            num--;
        }
    }
    return num;
}
int partition(int number[], int left, int right)
{
    int i, j, s;
    s = number[right];
    i = left - 1;
    for(j = left; j < right; j++)
    {
        if(number[j] <= s)
        {
            i++;
            SWAP(number[i], number[j]);
        }
    }
    SWAP(number[i+1], number[right]);
    return i+1;
}
void quicksort(int number[], int left, int right)
{
    int q;
    if(left < right)
    {
        q = partition(number, left, right);
        quicksort(number, left, q - 1);
        quicksort(number, q + 1, right);
    }
}
```
4.示例输出
![](https://img-blog.csdnimg.cn/20190131093451525.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==,size_16,color_FFFFFF,t_70)
