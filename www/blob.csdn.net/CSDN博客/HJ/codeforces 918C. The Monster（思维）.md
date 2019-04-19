# codeforces 918C. The Monster（思维） - HJ - CSDN博客
2018年03月13日 13:27:51[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：128
个人分类：[====ACM====																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
题目链接 [http://codeforces.com/problemset/problem/918/C](http://codeforces.com/problemset/problem/918/C)
解题思路：暴力枚举区间（i,j）是否合格，对于区间内的‘？’，先假设都为‘）’，对于不合适的就改为‘（’，再判断即可。
```cpp
#include<iostream>
using namespace std;
char s[5005];
int main()
{
    while(cin>>s)
    {
        int ans=0;   //ans为括号匹配的次数  
        for(int i=0;s[i]!='\0';i++)
        {
            int l=0,sum=0;    //l为'('的个数 sum为'?'的个数 
            for(int j=i;s[j]!='\0';j++)
            {
                if(s[j]=='(')
                  l++;
                else if(s[j]==')')
                  l--;
                else       //如果碰到'?' 先看成')' 
                {
                    l--;
                    sum++;
                }
                if(l==0)  //左右括号个数匹配
                  ans++;
                else if(l<0)    //如果左括号数目比右括号数目少 调整'?'的改变 
                {
                    if(sum==0)
                      break;
                    else if(sum>0)     //并且有'?'可以改变的话 
                    {
                        l+=2;      //就把'('改成')' 
                        sum--;  
                    }
                }
            }
        }
        cout<<ans<<endl;
    }
    return 0;
}
```
