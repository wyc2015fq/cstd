# codeforces 991-D. Bishwock（模拟） - HJ - CSDN博客
2018年08月09日 14:28:02[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：64
个人分类：[====ACM====																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/991/D](http://codeforces.com/problemset/problem/991/D)
解题思路：
对各种情况简单归类模拟一遍即可。
```cpp
#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
#include<set>
#include<cstdio>
#include<cstring>
#include<cmath>
using namespace std;
#define mod 1000000007
typedef long long ll;
const int maxn=1e5+10;
string a,b;
int main()
{
    while(cin>>a>>b)
    {
        int num1=0,num2=0,cnt=0;
        for(int i=0;i<a.length();i++)
        {
            if(a[i]!='X'||b[i]!='X')
            {
                if(a[i]!='X'&&b[i]!='X')   //当前有两个空 
                {
                    if(num2)    //如果前面一个位置有两个空 那么放入之后还剩一个空 
                      cnt++,num1=1,num2=0;
                    else if(num1)   //如果前面一个位置只有一个空 那么放入之后就全部抵消了 
                      cnt++,num1=0; 
                    else     //如果前面没有空 那么就把当前的两个空记录下来 
                      num2=1;
                }
                else    //当前只有一个空 
                {
                    if(num2)   //如果前面一个位置有两个空 那么就可以放入 
                      cnt++,num2=0;
                    else
                      num1=1;
                }
            }
            else
              num1=num2=0;
        }
        cout<<cnt<<endl;
    }
    return 0;
}
```
