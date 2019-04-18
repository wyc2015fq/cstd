# 1135Missile - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 16:40:16[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：451
个人分类：[ACM](https://blog.csdn.net/puqutogether/article/category/2598999)









## Missile

时间限制(普通/Java):1000MS/3000MS          运行内存限制:65536KByte
总提交:123            测试通过:37


描述



Long , long ago ,country A invented a missile system to destroy the missiles from their enemy . That system can launch only one missile to destroy multiple missiles if the heights of all the missiles
 form a non-decrease sequence .

But recently , the scientists found that the system is not strong enough . So they invent another missile system . The new system can launch one single missile to destroy many more enemy missiles . Basically
 , the system can destroy the missile from near to far . When the system is begun , it chooses one enemy missile to destroy , and then destroys a missile whose height is lower and farther than the first missile . The third missile to destroy is higher and farther
 than the second missile … the odd missile to destroy is higher and farther than the previous one , and the even missile to destroy is lower and farther than the previous one .

Now , given you a list of the height of missiles from near to far , please find the most missiles that can be destroyed by one missile launched by the new system .



输入



The input contains multiple test cases .

In each test case , first line is an integer n ( 0<n<=1000 ) , which is the number of missiles to destroy . Then follows one line which contains n integers ( <=109 )
 , the height of the missiles followed by distance .

The input is terminated by n = 0 .



输出

For each case , print the most missiles that can be destroyed in one line .

样例输入

4

5 3 2 4

3

1 1 1

0


样例输出

3

1


题目来源

NUPT ACM










这道题目一定要理解题目的意思，需要我们求解的是先找出一个较高的，然后如果存在不符合条件的导弹，跳过去，继续判断后面的导弹是否满足条件。




代码如下：


```cpp
#include<iostream>
using namespace std;

int Missile(int p[], int start, int n)
{
     int num=1;
     int i, flag=1;
     for(i=start; i<n-1; i++)
     {
          if(flag==1)
          {
               if(p[i]>p[i+1])
               {
                    num+=1;
                    flag=2;
               }
          }

          if(flag==2)
          {
               if(p[i]<p[i+1])
               {
                    num+=1;
                    flag=1;
               }
          }
     }

     return num;
}

int main()
{
     int n;
     cin>>n;
     int *p;
     while(n!=0)
     {
          p=new int[n];

          int i;
          for(i=0; i<n; i++)  //input the array
               cin>>p[i];

          //search for the optimal one
          int optimalNum=0, numTemp;
          for(i=0; i<n; i++)
          {
               numTemp=Missile(p, i, n);
//               cout<<numTemp<<endl;
               if(numTemp>optimalNum)
                    optimalNum=numTemp;
          }

          cout<<optimalNum<<endl;

          delete []p;
          cin>>n;
     }
     return 0;
}
```









