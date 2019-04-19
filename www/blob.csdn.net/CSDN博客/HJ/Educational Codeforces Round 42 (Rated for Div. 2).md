# Educational Codeforces Round 42 (Rated for Div. 2) - HJ - CSDN博客
2018年04月18日 15:50:49[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：73
个人分类：[====ACM====																[---------总结---------																[团队/个人套题训练题解](https://blog.csdn.net/feizaoSYUACM/article/category/7516342)](https://blog.csdn.net/feizaoSYUACM/article/category/6625811)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
A题   Equator 
水题，求前缀和大于等于总和的最小下标是多少？
```cpp
#include<iostream>
using namespace std;
int s[200005];
int main()
{
    int n;
    while(cin>>n)
    {
        int x;
        long long res=0,ans=0;
        for(int i=1;i<=n;i++)
        {
            cin>>s[i];
            ans+=s[i];
        }
        for(int i=1;;i++)
        {
            res+=s[i];
            if(res*2>=ans)
            {
                cout<<i<<endl;
                break;
            }
        }
    }
    return 0;
}
```
B题  Students in Railway Carriage 
题目大意：有a、b两种学生，但这个两种学生不能坐在相邻的位置，问最多能坐几个学生下去
解题思路：对于每一段长度为k的空位，一种学生能有(k+1)/2个人坐上去，另一种学生能有k/2个人坐上去，每次对于a、b两种学生进行贪心就行了。
```cpp
#include<iostream>
#include<vector> 
using namespace std;
vector<int> p;
int main()
{
    int n,a,b;
    while(cin>>n>>a>>b)
    {
        char c;
        int res=0,num1=a,num2=b;
        for(int i=0;i<n;i++)
        {
            cin>>c;
            if(c=='.')
              res++;
            else
            {
                if(res)
                  p.push_back(res);
                res=0;  
            }
        }
        if(res)
          p.push_back(res);
        for(int i=0;i<p.size();i++)
        {
            int s1=(p[i]+1)/2,s2=p[i]/2;
            if(num1<num2)
              swap(s1,s2);
            num1-=min(num1,s1);
            num2-=min(num2,s2);
        }
        p.clear();
        cout<<a+b-num1-num2<<endl;
    }
    return 0;
}
```
C题 Make a Square 
题目大意：给出一个数，取其中的若干位构成一个新的数（保证没有前导0），问最少需要减去多少个数位，保证这个构成的新的数是一个平方数。
解题思路：假设这个数长度为len，直接根据 1~2^(len) 暴力枚举，判断即可。
```cpp
#include<iostream>
#include<cmath>
#include<cstdio> 
#include<cstring>
using namespace std;
#define INF 0x3f3f3f3f
int main()
{
    char a[20];
    while(cin>>a))
    {
        int len=pow(2,strlen(a)),ans=INF;
        for(int i=1;i<len;i++)
        {
            int res=i,cnt=0,t=0;
            char s[20];
            for(int j=0;j<strlen(a);j++,res=res>>1)
            {
                if(res&1&&(cnt==0?(a[j]!='0'):true))
                  s[cnt++]=a[j];
                else
                  t++;
            }
            s[cnt++]=0;
            sscanf(s,"%d",&res);
            if((int)sqrt(res)*(int)sqrt(res)==res&&res)
              ans=min(t,ans);
        }
        if(ans==INF)
          cout<<-1<<endl; 
        else
          cout<<ans<<endl;
    }
    return 0;
}
```
D题 Merge Equals 
题目大意：在一个数组中，若出现两个相同的数，则将前一个数删去，将后一个数*2倍，问直到最后所有的数都不相同的时候这个新的数组是什么？
待补….
E题  Byteland, Berland and Disputed Cities 
待补…
F题 Simple Cycles Edges 
题目大意：求没有被两个及以上的简单环包含的边的条数
待补…
