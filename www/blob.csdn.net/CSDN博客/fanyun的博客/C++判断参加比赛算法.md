# C++判断参加比赛算法 - fanyun的博客 - CSDN博客
2018年11月28日 21:27:41[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：118
1.Ａ、Ｂ、Ｃ、Ｄ、Ｅ五名学生有可能参加计算机竞赛，根据下列条件判断哪些人参加了竞赛
(1).Ａ参加时，Ｂ也参加；
(2).Ｂ和Ｃ只有一个人参加；
(3).Ｃ和Ｄ或者都参加，或者都不参加；
(4).Ｄ和Ｅ中至少有一个人参加；
(5).如果Ｅ参加，那么Ａ和Ｄ也都参加。
2.代码实现
```cpp
#include<stdio.h>
int main()
{
    char name[]={'A','B','C','D','E'};
    int i,value[5];
    for(value[0]=0;value[0]<2;value[0]++)
        for(value[1]=0;value[1]<2;value[1]++)
            for(value[2]=0;value[2]<2;value[2]++)
                for(value[3]=0;value[3]<2;value[3]++)
                    for(value[4]=0;value[4]<2;value[4]++)
                    {
                        if((value[1]>=value[0])
                           &&(value[1]+value[2]==1)
                           &&value[2]==value[3]
                           &&(value[3]+value[4])
                           &&(!value[4]||(value[4]&&value[0]&&value[3])))
                            for(i=0;i<5;i++)
                                if(value)
                                    printf("%c参加\t",name);
                                else
                                    printf("%c不参加\t",name);
                    }
    return 0;    
}
```
3.代码输出
A不参加  B不参加   C参加   D参加   E不参加  
