# 最大公约数，最小公倍数(C语言实现)  - ljx0305的专栏 - CSDN博客
2008年07月29日 23:25:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：2085标签：[语言																[c																[system](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)
个人分类：[算法](https://blog.csdn.net/ljx0305/article/category/437635)
/*
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  程序：求最大公约数，和最小公倍数 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  时间：2007年10月8日
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  名称：maxandmin.c
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  方法：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  1.先求出最大公约数，用辗转相除法
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  2.再通过最大公约数来求最小公倍数
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  3.min = (m/max)*(n/max)*max=(m*n)/max (说明：max为所求得的最大公约数，min为所求得的最小公倍数
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)*/
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <stdio.h>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int main(void)
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)int m,n; /**//*m,n为输入数据，目的是求解m和n的最大公约数以及最小公倍数*/
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int i,t,min=0,max=0;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    printf("Please input the num(m and n): ");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    scanf("%d %d",&m,&n);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    t = m*n ;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if (n==0|| m ==0 )
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            printf("Your data is wrong!!! ");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            exit(1);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if ( m % n ==0  )
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         max = n ;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         min = m ;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)goto end ;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)elseif ( n % m ==0)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         max = m ;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         min = n ;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)goto end ;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)while ( n !=0 ) /**//*辗转相除法:除数作为被除数，余数作为除数，当除数为：0时，被除数就是最大公约数*/
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      i = m % n ;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      m = n ;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      n = i ;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    max = m ;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    min = t / max ;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    end:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        printf("max= %d , min = %d  ",max,min);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    system("pause");
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return0 ;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
