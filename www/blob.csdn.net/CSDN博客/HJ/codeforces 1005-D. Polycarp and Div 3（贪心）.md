# codeforces 1005-D. Polycarp and Div 3（贪心） - HJ - CSDN博客
2018年07月10日 20:41:43[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：267
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/1005/D](http://codeforces.com/problemset/problem/1005/D)
题目大意： 
      给出一个很大很大的数，可以无限次分割，求最后所有分割而成的数中能被3整除的数量最多。
解题思路： 
     贪心做法。 
     把每个数位上的数看成一个数，对3取余，如果能整除3，则数量直接加一。接着统计余数为2的连续出现的数目，如果接下来出现的数字为1，则贪心判断连续出现2的数目是否取模3不为0，如果不为0，则可以与接下来的1组合被3整除，总数加一，依次类推，接着统计余数为1的连续出现的数目。如此循环，直到数的最后一位。
```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
using namespace std;
typedef long long ll;
string s;
int main()
{
    while(cin>>s)
    {
        for(int i=0;i<s.length();i++)
          s[i]=(s[i]-'0')%3+'0';
        int num=1,cnt=0;
        for(int i=1;i<s.length();i++)
        {
            if(s[i]!=s[i-1])    //前后字符串不相等 
            {
                if(s[i]=='0')   //后面的字符串为0 
                {
                    cnt+=num/3;
                    num=1;
                }
                else   //后面的字符串不为0 
                {
                    if(s[i-1]=='0')  //前面的字符串为0 
                    {
                        cnt+=num;
                        num=1;
                    }
                    else    //前面的字符串不为0 
                    {
                        cnt+=num/3;
                        if(num%3)   //那么就可以综合加一 
                        {
                            cnt++;
                            num=0;
                        }
                        else
                          num=1;
                    }
                }
            }
            else
              num++;
        }
        if(s[s.length()-1]=='0')
          cnt+=num;
        else
          cnt+=num/3;
        cout<<cnt<<endl;
    }
    return 0;
}
```
