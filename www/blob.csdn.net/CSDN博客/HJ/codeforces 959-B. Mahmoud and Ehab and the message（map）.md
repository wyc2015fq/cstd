# codeforces 959-B. Mahmoud and Ehab and the message（map） - HJ - CSDN博客
2018年07月02日 10:55:52[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：124
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/959/B](http://codeforces.com/problemset/problem/959/B)
题目大意： 
       给出一个字典，有n个单词，另外给出每个单词与另外单词之间的值替代，求给出一个新的句子最少需要的值为多少？
解题思路： 
     map映射
```cpp
#include<iostream>
#include<algorithm>
#include<map>
#include<set>
using namespace std;
const int maxn=1e5+5;
struct node{
    long long i,val;
};
char a[maxn][25];
int b[maxn];
map<string,node> s;
set<int> p;
int main()
{
    int n,k,m;
    while(cin>>n>>k>>m)
    {
        for(int i=0;i<n;i++)
        {
            cin>>a[i];
            s[a[i]].i=i;
        }
        for(int i=0;i<n;i++)
          cin>>s[a[i]].val;
        int num;
        while(k--)
        {
            cin>>num;
            long long res=1e9+7;
            for(int j=0;j<num;j++)
            {
                cin>>b[j];
                b[j]--;
                res=min(res,s[a[b[j]]].val);
            }
            for(int j=0;j<num;j++)
              s[a[b[j]]].val=min(res,s[a[b[j]]].val);
        }
        long long ans=0;
        for(int i=0;i<m;i++)
        {
            cin>>a[i];
            ans+=s[a[i]].val;
        }
        cout<<ans<<endl;
        s.clear();
    }
    return 0;
}
```
