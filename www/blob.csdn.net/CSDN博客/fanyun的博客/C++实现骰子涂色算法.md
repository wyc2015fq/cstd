# C++实现骰子涂色算法 - fanyun的博客 - CSDN博客
2018年09月28日 20:24:15[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：107
1、骰子涂色算法要求
     输入两个骰子，判断两个骰子是否等价，每个骰子用6个字母表示，例如 123456， 顺序为上前左右后下。
2、思路
    任取一个面让其位于正前( 总共6个面)，然后转动上下左右四个面(转4次)，这样一个骰子的所有可能表示形式就全遍历到了。 
举个例子，骰子123456上下方向转动时，改变的是上前下后即1265四个面，可转动至2651，6512，5126总计四种。
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
int fa[6][6] = {
    1,2,3,4,5,6,
5,1,3,4,6,2,
    2,6,3,4,1,5,
    1,4,2,5,3,6,
    1,5,4,3,2,6,
    1,3,5,2,4,6
};
int fb[4][6] = {
    1,2,3,4,5,6,
    3,2,6,1,5,4,
    6,2,4,3,5,1,
    4,2,1,6,5,3
};
bool is(char s[], char b[])
{
    for(int i=0;i<4;i++)
    {
        char t[7];
        for(int j=0;j<6;j++)
            t[j] = s[fb[i][j]-1];
        t[6] = '\0';
        if(strcmp(t, b) == 0)
            return true;
    }
    return false;
}
int main()
{
    char a[7], b[7], s[15];
    while(cin>>s)
    {
        for(int i=0;i<6;i++)
            a[i] = s[i];
        for(int i=0;i<6;i++)
            b[i] = s[i+6];
        int i;
        b[6] = a[6] = '\0';
        for(i=0;i<6;i++)
        {
            char t[7];
            for(int j=0;j<6;j++)
                t[j] = a[fa[i][j]-1];
            t[6] = '\0';
            if(is(t, b))
                break;
        }
        if(i == 6)
            cout<<"FALSE"<<endl;
        else
            cout<<"TRUE"<<endl;
    }
    return 0;
}
```
