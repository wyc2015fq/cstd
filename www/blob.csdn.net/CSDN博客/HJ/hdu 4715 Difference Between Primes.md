# hdu  4715 Difference Between Primes - HJ - CSDN博客
2016年08月19日 18:14:41[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：343
Problem Description 
All you know Goldbach conjecture.That is to say, Every even integer greater than 2 can be expressed as the sum of two primes. Today, skywind present a new conjecture: every even integer can be expressed as the difference of two primes. To validate this conjecture, you are asked to write a program.
Input 
The first line of input is a number nidentified the count of test cases(n<10^5). There is a even number xat the next nlines. The absolute value of xis not greater than 10^6.
Output 
For each number xtested, outputstwo primes aand bat one line separatedwith one space where a-b=x. If more than one group can meet it, output the minimum group. If no primes can satisfy it, output ‘FAIL’.
Sample Input
3 
6 
10 
20
Sample Output
11 5 
13 3 
23 3
解析：给出一个数n，要你找出两个素数A和B，满足A-B=n。大小为10的6次方，注意，n是绝对值小于10的6次方，要考虑负数和0的情况。基本方法是筛法素数打表，然后遍历判断与n是否仍然为素数，如果是的话开两个变量赋值直接跳出循环，否则如果到最后都没有条件存在则输出无解。
注意一个剪枝的办法：因为任何素数除了2之外都是奇数，因此任何两个都不为2的素数之差都只可能为偶数，因此当n为奇数的时候，只要判断n+2是否为素数即可判断是否有解。
另外，当n为负数的时候，把n取绝对值，实际上A-B=n可以转化为B-A=-n，实际上可以按照n大于0的方法处理，只要最后如果有解的话两个数位置对调即可。
代码如下：
```cpp
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<cstring>
#include<string>
using namespace std;
#define maxn 1000000+10
int vis[maxn+4];
int prime(int x)       //判断一个数是否素数
{
    for(int i=2;i*i<=x;i++)
        if(x%i==0)
          return 0;
    return 1;
}
void _prime()       //素数筛法打表，大小到10的六次方，复杂度为O(n)
{
    memset(vis,0,sizeof(vis));
    for(int i=2;i<=maxn;i++)
    {
        if(!vis[i])
        {
            for(int j=i+i;j<=maxn;j+=i)
                vis[j]=1;
        }
    }
    int k=0;
    for(int i=2;i<=maxn;i++)
      if(!vis[i])
        vis[k++]=i;
    vis[k]='\0';
}
int main()
{
    _prime();
    int t;
    scanf("%d",&t);
    while(t--)
    {
        int n,x,y,blag;
        scanf("%d",&n);
        if(n%2!=0&&!prime(n+2)&&n>0)      //如果n为一个大于0的奇数，并且加上一个唯一为偶数的素数2仍然还不是素数的话，即可判断无解
          printf("FAIL\n");
        else if(n==0)           //n为0的时候有唯一解
          printf("2 2\n");
        else
        {
            if(n>0)            //当n大于0的时候
            {
                for(int i=0;vis[i]!='\0';i++)
                {
                    if(prime(n+vis[i]))   //从素数表vis里面遍历寻找，逐个逐个判断素数vis[i]加上n是否还是素数，因为由A-B=n，得到A=B+n（这里A、B均为素数）
                    {
                        y=n+vis[i];
                        x=vis[i];
                        blag=1;   //判断有解
                        break;
                    }
                }
            }
            else if(n<0)    //当n小于0的时候
            {
                n=abs(n);          //把n取绝对值，跟n>0相同处理，唯一不同的就是x与y的位置互相调换
                for(int i=0;vis[i]!='\0';i++)
                {
                    if(prime(n+vis[i]))
                    {
                        x=n+vis[i];
                        y=vis[i];
                        blag=1;      //标志变量判断有解
                        break;
                    }
                }
            }
            if(blag)      //已知有解，直接输出
                printf("%d %d\n",y,x);
            else          //否则可以知道在10的6次方之内无解
                printf("FAIL\n");
        }
    }
    return 0;
}
```
