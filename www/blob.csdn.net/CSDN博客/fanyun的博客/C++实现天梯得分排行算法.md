# C++实现天梯得分排行算法 - fanyun的博客 - CSDN博客
2019年01月31日 19:57:03[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：137
1.使用C++实现天梯积分排行简单算法
     给出一组用户和用户积分，给出排行榜。
2.实现原理
     简单排序的思想，就不多说了。
3.代码实现
```cpp
#include<stdio.h>
#include<stdlib.h>
#define MAX 100
#define MIN 0 
int main(void)
{
    int score[MAX + 1] = {0};
    int juni[MAX + 2] = {0};
    int count = 0, i;
    
    printf("输入分数， -1 结束：");
    do
    {
        scanf("%d", &score[count++]);
    }while(score[count - 1] != -1);
    count--;
    
    for(i =  0; i < count; i++)
    {
        juni[score[i]]++;
    }
    juni[MAX + 1] = 1;
    
    for(i = MAX; i >= MIN; i--)
    {
        juni[i] = juni[i] + juni[i + 1];
    }
    printf("得分\t排行\n");
    
    for(i = 0; i < count; i++)
    {
        printf("%d\t%d\n", score[i], juni[score[i] + 1]);
    }
    return 0;
}
```
4.代码输出
![](https://img-blog.csdnimg.cn/20190131095221879.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==,size_16,color_FFFFFF,t_70)
