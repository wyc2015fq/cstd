# codeforces 962-C. Make a Square（暴力） - HJ - CSDN博客
2018年07月02日 12:12:33[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：97
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/962/C](http://codeforces.com/problemset/problem/962/C)
题目大意： 
      给出一个个数，要求可以删除任意个任意数位上的数，是否存在一个最少的删除，使得操作之后的数为一个平方数。要求操作后的数不能有前导零。输出最少删除的个数。
解题思路： 
     因为数值只有9位，因此可以暴力枚举2^len，其中len为数的长度。考虑前导零的特殊情况即可；
```cpp
#include<iostream>
#include<cstring>
#include<cmath>
using namespace std;
int vis[10];
string s;
int main()
{
    while(cin>>s)
    {
        memset(vis,0,sizeof(vis));
        int len=s.length(),num=len;
        for(int i=1;i<(1<<len);i++)
        {
            int j=len-1,ans=0,cnt=0;
            for(int res=i;res;j--)
              res/=2;
            if(s[j+1]=='0')    //判断是否有前导零 
              continue; 
            for(int j=len-1,res=i,p=1;j>=0;j--,res/=2)
            {
                if(res%2==0)
                {
                    cnt++;
                    continue;
                }
                ans=ans+(s[j]-'0')*p;
                p*=10;
            }
            int p=sqrt(ans);
            if(p*p==ans&&ans!=0)
              num=min(num,cnt);
        }
        if(num==len)
          cout<<-1<<endl;
        else
          cout<<num<<endl;
    }
    return 0;
}
```
