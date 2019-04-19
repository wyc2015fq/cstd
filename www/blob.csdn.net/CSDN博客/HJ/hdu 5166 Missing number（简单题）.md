# hdu  5166 Missing number（简单题） - HJ - CSDN博客
2017年08月04日 21:43:20[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：201
个人分类：[====ACM====																[---------OJ---------](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
There is a permutation without two numbers in it, and now you know what numbers the permutation has. Please find the two numbers it lose.
Input 
There is a number T shows there are T test cases below. (T≤10) 
For each test case , the first line contains a integers n , which means the number of numbers the permutation has. In following a line , there are n distinct postive integers.(1≤n≤1,000)
Output 
For each case output two numbers , small number first.
Sample Input
2 
3 
3 4 5 
1 
1
Sample Output 
1 2 
2 3
用vis[]数组记录去重判断即可。
```cpp
#include<iostream>
#include<cstring>
using namespace std;
int vis[1005]; 
int main()
{
    int t,n;
    cin>>t;
    while(t--)
    {
        cin>>n;
        memset(vis,0,sizeof(vis));
        int cnt=0,ans=0,x,a[3];
        for(int i=0;i<n;i++)
        {
            cin>>x;
            ans=max(ans,x);
            vis[x]=1;
        }
        for(int i=1;i<ans;i++)
          if(vis[i]==0)
            a[cnt++]=i;
        if(cnt==0)
          cout<<ans+1<<" "<<ans+2;
        else if(cnt==1)
          cout<<a[0]<<" "<<ans+1;
        else
          cout<<a[0]<<" "<<a[1];
        cout<<endl;
    } 
    return 0;
}
```
