# codeforces 962-B. Students in Railway Carriage（暴力+贪心） - HJ - CSDN博客
2018年07月02日 11:07:14[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：66
个人分类：[====ACM====																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/962/B](http://codeforces.com/problemset/problem/962/B)
题目大意： 
      一辆列车上有n个座位，但是有些座位放了东西不能够坐。现在有A类和B类数人，给他们安排座位，并且A类的人不能喝B类的人直接相邻坐，计算最多能够坐多少人。
解题思路： 
     枚举每个放了东西不能做的点，将两个放了东西不能坐的点之间区间值保存到vec数组中，然后贪心安排A和B类人即可；
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
