# POJ 2993 Emag eht htiw Em Pleh（模拟） - HJ - CSDN博客
2017年08月19日 18:33:52[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：200
[http://poj.org/problem?id=2993](http://poj.org/problem?id=2993)
跟2996有点像，只不过思路却还是要想一想，做了一个小时才弄出来。
```cpp
#include<iostream>
#include<vector>
#include<cstdio> 
using namespace std;
char c='P',s[2][105],r[]={'.',':'},line[]={"+---+---+---+---+---+---+---+---+"};
struct node{
    int x,y;
}k;
vector<node>p[2][30];
void getpair()
{
    for(int x=0;x<2;x++)
    {
        for(int i=8;s[x][i]!='\0';i++)
        {
            if(s[x][i]>='a'&&s[x][i]<='z')
            {
                if(s[x][i-1]==',')
                {
                    k.y=s[x][i]-'a';
                    k.x=8-(s[x][++i]-'0');
                    p[x][c-'A'].push_back(k);
                }
                else
                {
                    k.y=s[x][i]-'a';
                    k.x=8-(s[x][++i]-'0'); 
                    p[x][s[x][i-2]-'A'].push_back(k);
                }
            }
        }
    }
}
int find(int i,int x,int y)
{
    for(int j=0;j<26;j++)
     for(int q=0;q<p[i][j].size();q++)
       if(p[i][j][q].x==x&&p[i][j][q].y==y)
         return j;
    return -1;
}
int main()
{
    gets(s[0]);
    gets(s[1]);
    getpair();
    cout<<line<<endl;
    int blag=1;
    for(int i=0;i<8;i++)
    {
        printf("|");
        blag=1-blag;
        for(int j=0;j<8;j++)
        {
            int h1=find(0,i,j),h2=find(1,i,j);          //h1表示查找到白棋  h2表示查找到黑棋 
            printf("%c%c%c|",r[blag],h1==-1?((h2==-1)?r[blag]:(h2+'a')):(h1+'A'),r[blag]);
            blag=1-blag;
        }
        printf("\n%s\n",line);
    }
    return 0;
}
```
