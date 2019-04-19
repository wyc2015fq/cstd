# hdu 3183 A Magic Lamp（贪心+RMQ） - HJ - CSDN博客
2018年04月20日 18:56:00[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：85
个人分类：[====ACM====																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=3183](http://acm.hdu.edu.cn/showproblem.php?pid=3183)
题目大意：略
解题思路：要证明一个定理，只要出现a[i]>a[j] ，(i=j-1)，那么就一定需要删除a[i]，用a[j]来顶替a[i]的位置。
因为需要删除m个数字，那么每次只要在长度为n-m的区间里面找出一个最小的数，下一次查找从这个下标之后，如此反复，循环m次即可。
```cpp
#include<iostream>
#include<string>
using namespace std;
string s;
int main()
{
    int m;
    while(cin>>s>>m)
    {
        int p=0,pos=m,blag=0;
        for(int i=0;i<s.length()-m;i++)
        {
            char c=s[p];
            for(int j=p+1;j<=pos;j++)
              if(s[j]<c)
                c=s[p=j];
            pos++;
            p++;
            if(!blag)
            {
                if(c!='0')
                {
                    cout<<c;
                    blag=1;
                }
            }
            else
              cout<<c;
        }
        if(!blag)
          cout<<"0";
        cout<<endl;
    }
    return 0;
}
```
