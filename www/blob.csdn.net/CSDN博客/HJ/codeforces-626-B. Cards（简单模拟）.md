# codeforces-626-B. Cards（简单模拟） - HJ - CSDN博客
2017年02月02日 02:18:43[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：184
个人分类：[====ACM====																[codeforce																[---------OJ---------](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Catherine has a deck of n cards, each of which is either red, green, or blue. As long as there are at least two cards left, she can do one of two actions:
```
take any two (not necessarily adjacent) cards with different colors and exchange them for a new card of the third color;
take any two (not necessarily adjacent) cards with the same color and exchange them for a new card with that color.
```
She repeats this process until there is only one card left. What are the possible colors for the final card? 
Input
The first line of the input contains a single integer n (1 ≤ n ≤ 200) — the total number of cards.
The next line contains a string s of length n — the colors of the cards. s contains only the characters ‘B’, ‘G’, and ‘R’, representing blue, green, and red, respectively. 
Output
Print a single string of up to three characters — the possible colors of the final card (using the same symbols as the input) in alphabetical order. 
Examples 
Input
2 
RB
Output
G
Input
3 
GRG
Output
BR
Input
5 
BBBBB
Output
B
Note
In the first sample, Catherine has one red card and one blue card, which she must exchange for a green card.
In the second sample, Catherine has two green cards and one red card. She has two options: she can exchange the two green cards for a green card, then exchange the new green card and the red card for a blue card. Alternatively, she can exchange a green and a red card for a blue card, then exchange the blue card and remaining green card for a red card.
In the third sample, Catherine only has blue cards, so she can only exchange them for more blue cards.
题目大意:
有red、green，blue三种颜色，作为卡片出现在一个串中，现在有两个规则：
（1）用任意两张相同颜色的卡片可以换回一张该颜色的卡片
（2）用任意两张不同颜色的卡片可以换回一张另外第三种颜色的卡片
求最后在只剩一张卡片的时候有多少种情况？输出不超过三个字符，如果字符超过一个，以字典序从小到大输出
解题思路：
分情况讨论： 
（1）如果只有一张卡片，很好办，直接输出就ok了
（2）如果有两张，很简单，两种情况（一种两张相同直接输出其中一个，另外一种是两张不同输出第三个就好了额）
（3）麻烦的是三张及三张以上如何处理？
仔细来看我们不难知道，如果刚好是三张：
①三种都不同②三种都相同③两种相同一种不同
如果超过三张：
①三种颜色都出现过②所有卡片颜色相同③只出现过两种颜色的卡片（一对多则输出两种，二对多则输出三种）
详细处理请看代码（本题很能考验一个人逻辑思维能力和代码实现水平）
```cpp
#include<bits/stdc++.h>
using namespace std;
char c[3]={'B','G','R'};
int d[5];
int change(char a,char b)     //两种颜色变第三种颜色 
{
    if(a==b)
      return a;
    int vis[3]={0};
    for(int i=0;i<3;i++)
    {
        if(a==c[i])
          vis[i]++;
        if(b==c[i])
          vis[i]++;
    }
    for(int i=0;i<3;i++)
      if(!vis[i])
        return i;
}
void G()                  //统计颜色出现的种数，以字符形式输出 
{
    char ss[10];
    int k=0;
    for(int i=0;i<3;i++)
      if(d[i])
        ss[k++]=c[i];
    ss[k]='\0';
    cout<<ss<<endl;
}
int main()
{
    int n;
    char s[100005];
    while(cin>>n)
    {
        cin>>s;
        if(n==1)              //一张卡片的情况 
          cout<<s[0]<<endl;
        else if(n==2)         //两张卡片的情况 
        {
            if(s[0]==s[1])
              cout<<s[0]<<endl;
            else                 //如果两张卡片不同，则需要改变一下 
            {
                memset(d,0,sizeof(d));
                d[change(s[0],s[1])]=1;
                G();
            }
        }
        else               //三张及三张卡片以上的情况 
        {
            int cnt=0;
            memset(d,0,sizeof(d));
            for(int i=0;i<n;i++)
              for(int j=0;j<3;j++)
                if(s[i]==c[j])
                  d[j]++;
            for(int i=0;i<3;i++)
              if(d[i])
                cnt++;
            if(cnt==3||cnt==1)   //如果是三种颜色都有或者只出现一种颜色的卡片则可以直接统计输出 
              G();
            else               //否则分一对多和二对多的情况讨论 
            {
                cnt=0;
                for(int i=0;i<3;i++)
                  if(d[i]>1)
                    cnt++;
                if(cnt==2)         //如果是二对多，则到最后可能三种颜色的卡片都会出现 
                  cout<<"BGR"<<endl;
                else             //如果是一对多，那个多的颜色卡片最后一定不会出现，大家可以拿些数据模拟一下 
                {
                    int k;
                    for(int i=0;i<3;i++)
                      if(d[i]>1)
                        k=i;          //记录下这个多的颜色的卡片是哪一种 
                    for(int i=0;i<3;i++)
                    {
                        if(i!=k)           //这个时候这个多的颜色的卡片就要假设不存在了 
                          d[i]=1;
                        else            //并且假设其他两种颜色的卡片都存在 
                          d[i]=0;
                    }
                    G();
                }
            } 
        }
    }
    return 0; 
}
```
