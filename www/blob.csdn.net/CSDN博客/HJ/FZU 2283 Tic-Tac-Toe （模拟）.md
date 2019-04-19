# FZU 2283 Tic-Tac-Toe （模拟） - HJ - CSDN博客
2017年08月23日 16:24:31[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：385
[http://acm.fzu.edu.cn/problem.php?pid=2283](http://acm.fzu.edu.cn/problem.php?pid=2283)
解题思路： 
因为需要判断后两步是否能获胜，考虑到对方会走一步（堵住一个方向），那么现在存在的局面双方各执有的棋子数就一定大于等于两颗的。接着你需要两个步获胜，正中间的位置就不能让对方霸占。
```cpp
#include<iostream>
using namespace std;
char s[4][4];
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        char c;
        int cnt=0,ans;
        for(int i=0;i<3;i++)
          for(int j=0;j<3;j++)
          {
              cin>>s[i][j];
              if(s[i][j]=='.')
                cnt++;
          }
        cin>>c;
        if(cnt<=5)
        {
            if(s[1][1]==c||s[1][1]=='.')
              ans=1;
            else
              ans=0;
        }
        else
          ans=0;
        if(ans)
          cout<<"Kim win!"<<endl;
        else
          cout<<"Cannot win!"<<endl;
    }
    return 0;
}
```
