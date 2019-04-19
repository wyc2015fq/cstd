# hdu  2510 符号三角形（dfs+打表） - HJ - CSDN博客
2017年07月01日 15:54:27[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：364
Problem Description 
符号三角形的 第1行有n个由“+”和”-“组成的符号 ，以后每行符号比上行少1个，2个同号下面是”+“，2个异 号下面是”-“ 。计算有多少个不同的符号三角形，使其所含”+“ 和”-“ 的个数相同 。 n=7时的1个符号三角形如下: 
+ + - + - + + 
+ - - - - + 
- + + + - 
- + + - 
- + - 
- - 
+
Input 
每行1个正整数n <=24,n=0退出.
Output 
n和符号三角形的个数.
Sample Input
15 
16 
19 
20 
0
Sample Output
15 1896 
16 5160 
19 32757 
20 59984
先用dfs搜一遍，然后把结果记录下来，直接打表。
```
/* 
//dfS搜一遍
#include<iostream>
using namespace std;
int count,s[105];
int a[25][12500];
void dfs(int k)
{
    if(k>24)
      return ;
    for(int i=0;i<=1;i++)
    {
        a[1][k]=i;
        count+=i;
        for(int j=2;j<=k;j++)
        {
            a[j][k-j+1]=a[j-1][k-j+1]^a[j-1][k-j+2];           //异或运算 把'-'看成1,'+'看成0 即1^1=0 1^0=1 0^1=1 0^0=0 
            count+=a[j][k-j+1];
        }
        if(count*2==k*(k+1)/2)
          s[k]++;
        dfs(k+1);
        count-=i;
        for(int j=2;j<=k;j++)
        {
            a[j][k-j+1]=a[j-1][k-j+1]^a[j-1][k-j+2];
            count-=a[j][k-j+1];
        }
    }
}
int main()
{
    memset(s,0,sizeof(s));
    count=0;
    dfs(1);
    int n;
    while(cin>>n&&n)
      cout<<n<<" "<<s[n]<<endl;
    return 0;
}
*/
#include<iostream>   
using namespace std;     
int result[24]={0,0,4,6,0,0,12,40,0,0,171,410,0,0,1896,5160,0,0,32757,59984,0,0,431095,822229};   
int main()   
{   
    int n;   
    cin>>n;  
    while(n!=0)  
    {  
        cout<<n<<" "<<result[n-1]<<endl;  
        cin>>n;  
    }  
    return 0;  
}
```
