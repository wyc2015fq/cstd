# hdu 1041 Computer Transformation（规律+大数） - HJ - CSDN博客
2017年09月08日 17:56:23[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：252
[http://acm.hdu.edu.cn/showproblem.php?pid=1041](http://acm.hdu.edu.cn/showproblem.php?pid=1041)
递推公式：
```
d[i]=2*d[i-1]+d[i-2];
```
因为数据比较大，因此需要用到大数相加。
```cpp
#include<iostream>
#include<cstring> 
#include<string>
using namespace std;
string d[1005];
string add(string s1,string s2)
{
    if(s1.length()<s2.length())
      swap(s1,s2);
    for(int i=s1.length()-1,j=s2.length()-1;i>=0;i--,j--)
    {
        s1[i]=char(s1[i]+(j>=0?s2[j]-'0':0));
        if(s1[i]>'9')
        {
            s1[i]=char((s1[i]-'0')%10+'0');
            if(i)
              s1[i-1]++;
            else
              s1="1"+s1;
        }
    }
    return s1;
}
int main()
{
    d[1]="0",d[2]="1";
    for(int i=3;i<=1000;i++)
      d[i]=add(d[i-1],add(d[i-2],d[i-2]));
    int n;
    while(cin>>n)
      cout<<d[n]<<endl;
    return 0;
}
```
