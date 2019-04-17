# hdu2033——人见人爱A+B - westbrook1998的博客 - CSDN博客





2018年02月19日 19:16:53[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：94








## 题目：

> 
Problem Description 

  HDOJ上面已经有10来道A+B的题目了，相信这些题目曾经是大家的最爱，希望今天的这个A+B能给大家带来好运，也希望这个题目能唤起大家对ACM曾经的热爱。 

  这个题目的A和B不是简单的整数，而是两个时间，A和B 都是由3个整数组成，分别表示时分秒，比如，假设A为34 45 56，就表示A所表示的时间是34小时 45分钟 56秒。  
Input 

  输入数据有多行组成，首先是一个整数N，表示测试实例的个数，然后是N行数据，每行有6个整数AH,AM,AS,BH,BM,BS，分别表示时间A和B所对应的时分秒。题目保证所有的数据合法。  

Output 

  对于每个测试实例，输出A+B，每个输出结果也是由时分秒3部分组成，同时也要满足时间的规则（即：分和秒的取值范围在0~59），每个输出占一行，并且所有的部分都可以用32位整数表示。  

Sample Input 

  2 

  1 2 3 4 5 6 

  34 45 56 12 23 34  
Sample Output 

  5 7 9 

  47 9 30  
Author 

  lcy

## 题解：

```
#include <stdio.h>
    struct time
    {
        int h;
        int m;
        int s;
    }t1,t2,t3;
    void fun(void)
    {
        t3.s=0;
        t3.m=0;
        t3.h=0;
        if(t1.s+t2.s>=60)
        {
            t3.m+=(t1.s+t2.s+t3.s)/60;
            t3.s=(t1.s+t2.s+t3.s)%60;
        }
        else
        {
            t3.s+=(t1.s+t2.s);
        }
        if(t1.m+t2.m+t3.m>=60)
        {
            t3.h+=(t1.m+t2.m+t3.m)/60;
            t3.m=(t1.m+t2.m+t3.m)%60;
        }
        else{
            t3.m+=(t2.m+t1.m);
        }
        t3.h+=(t2.h+t1.h);
    }
    int main(void)
    {
        int num;
        scanf("%d",&num);
        while(num--)
        {
            scanf("%d %d %d %d %d %d",&t1.h,&t1.m,&t1.s,&t2.h,&t2.m,&t2.s);
            fun();
            printf("%d %d %d\n",t3.h,t3.m,t3.s);
        }
        return 0;
    }
```







