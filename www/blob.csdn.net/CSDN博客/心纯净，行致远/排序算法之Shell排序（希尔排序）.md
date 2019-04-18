# 排序算法之Shell排序（希尔排序） - 心纯净，行致远 - CSDN博客





2017年10月04日 17:41:55[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：114








Shell排序

思想：先可进行分组，再利用插入排序。分组的方法是根据步长的设置从a[0]开始，并且不大于步长，组内相邻两个元素的逻辑距离是gap。分组结束的标志是gap为0，也就是已经全部按照大小排列。



**//给定数组shell排序，从小到大排列。**



#include <stdio.h>



int main()

{

         inti,j=1,t,k,gap;

         inta[10]={1,23,41,31,45,767,48,90,6,88};



         gap=10/2;

         while(gap>0)

         {

         for(i=0;i<gap;i++)

         {

                   for(j=i+gap;j<10;)

       {

                   t=a[j];

       k=j;

       while(k>gap-1&& t< a[k-gap])   //这里的while改成if就不对了

                   {

          a[k] = a[k-gap];

          k-=gap;

       }

                   a[k]=t;

                   j+=gap;

                   }

      gap/=2;

         }

         }



         printf("\nThesorted numbers are:\n");

         for(i=0;i<10;i++)

         {        

                   printf("%5d",a[i]);

         }

         printf("\n\nend\n");

}



