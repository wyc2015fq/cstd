# hdu  2206  IP的计算（模拟） - HJ - CSDN博客
2017年03月31日 21:57:18[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：232
个人分类：[====ACM====																[---------OJ---------](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
在网络课程上，我学到了很多有关IP的知识。IP全称叫网际协议，有时我们又用IP来指代我们的IP网络地址，现在IPV4下用一个32位无符号整数来表示，一般用点分方式来显示，点将IP地址分成4个部分，每个部分为8位，表示成一个无符号整数（因此不需要用正号出现），如192.168.100.16，是我们非常熟悉的IP地址，一个IP地址串中没有空格出现（因为要表示成一个32数字）。 
但是粗心的我，常常将IP地址写错，现在需要你用程序来判断。
Input 
输入有多个case，每个case有一行，不超过100个字符。
Output 
对于每个case，判断输入的IP是否正确，如果正确输入YES，否则NO。
Sample Input
192.168.100.16
Sample Output
YES
解题思路：
注意考虑一下，正确IP地址有如下要求：
1）四组数据，三个实点，并且实点必须在数据与数据之间； 
2）数据没有前导0，数据必须大于等于0并且小于256； 
3）没有其他非法字符；
```cpp
#include<iostream>
#include<cstring>
#include<cstdio> 
using namespace std;
char s[1005];
int main()
{
    while(gets(s))    //注意这个串中可能会出现空格 
    {
        int a[10],ans[10],blag=0,k=0,p=0,ss=strlen(s);
        memset(a,0,sizeof(a));
        memset(ans,0,sizeof(ans));        //每次的case都要重新初始化 
        if(ss>15||s[0]=='.'||s[ss-1]=='.')   //长度不能超过15，并且最前面与最后面都不能为'.' 
          blag=1;
        else            
        {
            for(int i=0;i<ss;i++)
            { 
                if(s[i]=='.'&&s[i+1]=='.')    //并且保证'.'之间一定有数 不能直接相连 
                  blag=1;   
                if(s[i]=='.')      //记录下'.'的个数 
                  k++;
                else if(s[i]<'0'||s[i]>'9')    //检验是否出现非法字符 
                  blag=1;
            }
            if(k!=3)       //如果'.'的个数不是3个，则可以直接判断非法 
              blag=1;
            if(!blag)       //如果合法，则继续往下判断 
            {
                for(int i=0;i<ss;i++)
                {
                    if(s[i]=='.')                       
                      p++;
                    else
                    {
                        a[p]=a[p]*10+s[i]-'0';    
                        ans[p]++;
                    }
                }           
                for(int i=0;i<=p;i++)
                  if(a[p]>255||ans[p]>3)    //a[]记录下每一段的数值，ans记录下每一段的长度 
                    blag=1;
            } 
        }
        if(!blag)        //如果刚好三个'.' 四个数 并且整个过程都合法 则判断正确 
          cout<<"YES"<<endl;
        else
          cout<<"NO"<<endl;
    }
    return 0;
}
```
