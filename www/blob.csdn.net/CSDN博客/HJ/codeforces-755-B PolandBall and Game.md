# codeforces-755-B  PolandBall and Game - HJ - CSDN博客
2017年01月17日 03:00:17[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：640
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
PolandBall is playing a game with EnemyBall. The rules are simple. Players have to say words in turns. You cannot say a word which was already said. PolandBall starts. The Ball which can’t say a new word loses.
You’re given two lists of words familiar to PolandBall and EnemyBall. Can you determine who wins the game, if both play optimally? 
Input
The first input line contains two integers n and m (1 ≤ n, m ≤ 103) — number of words PolandBall and EnemyBall know, respectively.
Then n strings follow, one per line — words familiar to PolandBall.
Then m strings follow, one per line — words familiar to EnemyBall.
Note that one Ball cannot know a word more than once (strings are unique), but some words can be known by both players.
Each word is non-empty and consists of no more than 500 lowercase English alphabet letters. 
Output
In a single line of print the answer — “YES” if PolandBall wins and “NO” otherwise. Both Balls play optimally.
Examples
Input 
5 1 
polandball 
is 
a 
cool 
character 
nope
Output 
YES
Input 
2 2 
kremowka 
wadowicka 
kremowka 
wiedenska
Output 
YES
Input 
1 2 
a 
a 
b
Output 
NO
Note 
In the first example PolandBall knows much more words and wins effortlessly. 
In the second example if PolandBall says kremowka first, then EnemyBall cannot use that word anymore. EnemyBall can only say wiedenska. PolandBall says wadowicka and wins.
题目大意：A、B两个人手里都有一些字符串卡片，现在两个人轮流把自己的卡片摆出来，要求摆出来的字符串不能存在任何两份相同的（也就是说，如果A拿出了卡片“abab”,就算B手里面有同样的卡片，A先拿出来了，B就不能再拿这张了），A先拿，如果到最后B剩下的卡片比A少就输出“YES”，否则输出”NO”。
解法： 
    先去重，然后再判断A和B拥有不同卡片数目，如果出现两者数目相等情况，就进一步统计A和B同时都有的卡片的数目，判断奇偶性。
代码如下：
```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
using namespace std;
int main()
{
    int n,m;
    while(cin>>n>>m)
    {
        char a[1005][505],b[1005][505];
        for(int i=0;i<n;i++)
          cin>>a[i];  
        for(int i=0;i<m;i++)
          cin>>b[i];
        for(int i=0;i<n;i++)
          for(int j=i+1;j<n;j++)
            if(strcmp(a[i],a[j])==0)
              n--;
        for(int i=0;i<m;i++)
          for(int j=i+1;j<m;j++)
            if(strcmp(b[i],b[j])==0)
              m--;
        if(n>m)
          cout<<"YES"<<endl;
        else if(n==m)
        {
            int blag=0,cnt=0;
            for(int i=0;i<n;i++)
            {
                blag=0;
                for(int j=0;j<m;j++)
                  if(strcmp(a[i],b[j])==0)
                  {
                      blag=1;
                      break;
                  }
                if(blag)     //统计A和B拥有相同卡片的数目
                  cnt++;
          }
          if(cnt%2)         //因为是A先出，如果相同卡片数目为偶数，那么到最后肯定是B有一张卡片出不出，A就会获胜
            cout<<"YES"<<endl;
          else
            cout<<"NO"<<endl;
        }
        else
          cout<<"NO"<<endl;
    }
    return 0;
}
```
