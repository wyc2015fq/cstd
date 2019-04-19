# C++实现黑白棋算法 - fanyun的博客 - CSDN博客
2018年09月28日 20:23:51[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：689
1、黑白棋算法要求
      实现黑白棋，判断是否可以落子，思路上难度不大，细节问题较多，细节很重要，代码实现可以有很多种，第一次实现难免有冗余的代码，冗余部分可以使用函数实现，尽量保证代码的可复用性。另外，输出黑白棋子数量的时候，数字要格式化输出占两位(%2d)。
2、思路
     见代码。
3、代码实现
```cpp
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <queue>
#include <string.h>
#include <set>
#include <stack>
#include <stdlib.h>
#include <time.h>
using namespace std;
char mp[10][10];//储存棋局
int f[2][10][10];//标记黑白子可执行合法操作的点
int dx[] = {-1,-1,-1, 1,1,1,0, 0};//八个方向
int dy[] = {-1, 0, 1,-1,0,1,1,-1};
void is(int a, char b, int x, int y)//判断是否可以落子
{
    for(int k=0;k<8;k++)
    {
        int tx = x+dx[k];
        int ty = y+dy[k];
        int num = 0;
        while(tx>=0 && ty>=0
              && tx<8 && ty<8
              && mp[tx][ty] == b)
        {
            tx += dx[k];
            ty += dy[k];
            ++num;
        }
        if(num>=1 && mp[tx][ty] == '-')
            f[a][tx][ty] |= 1<<k;
            //利用位运算同时保存k即方向，便于后面落子时直接定向翻转。
    }
}
void init()//获得黑白子可执行操作的点
{
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            if(mp[i][j] == 'W')
                is(0, 'B', i, j);
            else if(mp[i][j] == 'B')
                is(1, 'W', i, j);
}
bool print(bool isprint, int p)
//p为当前操作者，isprint表示是否输出，便于将此函数输出查询两用
{
    bool flag = false;
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            if(f[p][i][j])
            {
                if(flag && isprint)
                    cout<<" ";
                flag = true;
                if(isprint)
                    cout<<'('<<i+1<<','<<j+1<<")";
                else
                    return flag;
            }
    if(isprint)
    {
        if(flag)
            cout<<endl;
        else
            cout<<"No legal move."<<endl;
    }
    return flag;
}
void move(int x, int y, int p)
{
    char ch;
    if(p == 0)
        ch = 'W';
    else
        ch = 'B';
    mp[x][y] = ch;
    for(int i=0;i<8;i++)
    {
        if(f[p][x][y] & 1<<i)
        {
            int tx = x-dx[i];//为什么是减不是加呢？嘿嘿
            int ty = y-dy[i];
            while(tx>=0 && ty>=0
                  && tx<8 && ty<8
                  && mp[tx][ty] != ch)
            {
                mp[tx][ty] = ch;
                tx -= dx[i];
                ty -= dy[i];
            }
        }
    }
}
int main()
{
    int n;
    cin>>n;
    while(n--)
    {
        memset(mp, 0, sizeof(mp));
        for(int i=0;i<8;i++)
            cin>>mp[i];
        char ch[5];int p;
        cin>>ch;
        if(ch[0] == 'W')
            p = 0;
        else
            p = 1;
        while(ch[0] != 'Q')
        {
            memset(f, 0, sizeof(f));
            init();
            cin>>ch;
            if(ch[0] == 'L')
                print(true, p);
            else if(ch[0] == 'M')
            {
                if(!print(false, p))
                    p ^= 1;//更换玩家
                move(ch[1]-'0'-1, ch[2]-'0'-1, p);
                p ^= 1;
                int wnum = 0;
                int bnum = 0;
                for(int i=0;i<8;i++)
                    for(int j=0;j<8;j++)
                    {
                        if(mp[i][j] == 'W')
                            ++wnum;
                        if(mp[i][j] == 'B')
                            ++bnum;
                    }
                printf("Black - %2d White - %2d\n", bnum, wnum);
            }
        }
        for(int i=0;i<8;i++)
            cout<<mp[i]<<endl;
        if(n != 0)
            cout<<endl;
    }
    return 0;
}
```
