# CSU  1596: Dick.Z 的炉石赛（模拟） - HJ - CSDN博客
2017年01月31日 03:24:01[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：244
个人分类：[====ACM====																[---------OJ---------](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description
Dick.Z 的炉石赛 
集训队即将举办一场炉石传说的积分赛，在某场比赛之前，选手必须事先决定自己三场比赛要使用的职业以及他们的出场顺序，一旦确定，出场顺序就不可以改变。获胜的场次将会记入总积分，所以即使0：2也坚持将第三场打完。而且不论前一场输赢，下一场里双方都必须更换事先决定的职业继续比赛。在这个游戏里有9种职业，9种职业互相克制。现在dick政将提供你职业之间比赛的平均胜率。请你根据两位选手预先提交的出场职业顺序计算第一位选手的期望胜场。
各职业的对战平均胜率： 
我方\对方 Druid Hunter Mage Shaman Paladin Warlock Warrior Rogue Priest 
Druid    0.50   0.58  0.59  0.45   0.56    0.58    0.57   0.39   0.38 
Hunter   0.42   0.50  0.65  0.75   0.68    0.52    0.40   0.55   0.60 
Mage     0.41   0.35  0.50  0.46   0.48    0.47    0.44   0.57   0.54 
Shaman   0.55   0.25  0.54  0.50   0.53    0.51    0.63   0.52   0.42 
Paladin  0.44   0.32  0.52  0.47   0.50    0.47    0.46   0.55   0.46 
Warlock  0.42   0.48  0.53  0.49   0.53    0.50    0.44   0.40   0.59 
Warrior  0.43   0.60  0.56  0.37   0.54    0.56    0.50   0.68   0.39 
Rogue    0.61   0.45  0.43  0.48   0.45    0.60    0.32   0.50   0.48 
Priest   0.62   0.40  0.46  0.58   0.54    0.41    0.61   0.52   0.50
Input
第一行包含一个整数T(1<=T<=100) 
以下有T组数据， 
每组包含2行，每行有三个字符串，中间用空格隔开，第一行表示第一个选手的出场顺序，第二行表示第二个选手的出场顺序。保证字符串合法
Output
对于每组数据输出一个数，表示第一位选手的期望胜场，保留2位小数。
Sample Input
2 
Warrior Rogue Priest 
Shaman Mage Paladin 
Hunter Hunter Hunter 
Shaman Shaman Shaman
Sample Output
1.34 
2.25
暴力枚举！
```
#include<iostream>
#include<cstdio>
#include<cstring> 
using namespace std;
int main()
{
    double a[10][10]={{0.50,0.58,0.59,0.45,0.56,0.58,0.57,0.39,0.38}, 
                      {0.42,0.50,0.65,0.75,0.68,0.52,0.40,0.55,0.60}, 
                      {0.41,0.35,0.50,0.46,0.48,0.47,0.44,0.57,0.54},
                      {0.55,0.25,0.54,0.50,0.53,0.51,0.63,0.52,0.42},
                      {0.44,0.32,0.52,0.47,0.50,0.47,0.46,0.55,0.46},
                      {0.42,0.48,0.53,0.49,0.53,0.50,0.44,0.40,0.59},
                      {0.43,0.60,0.56,0.37,0.54,0.56,0.50,0.68,0.39},
                      {0.61,0.45,0.43,0.48,0.45,0.60,0.32,0.50,0.48},
                      {0.62,0.40,0.46,0.58,0.54,0.41,0.61,0.52,0.50}};
    int t;
    char b[10][20]={"Druid","Hunter","Mage","Shaman","Paladin","Warlock","Warrior","Rogue","Priest"};
    cin>>t;
    while(t--)
    {
        char s1[5][15],s2[5][15];
        int p[5],q[5];
        double ans=0;
        for(int i=0;i<3;i++)
        {
            cin>>s1[i];
            for(int j=0;j<9;j++)
              if(strcmp(s1[i],b[j])==0)
              {
                  p[i]=j;
                  break;
              }
        }
        for(int i=0;i<3;i++)
        {
            cin>>s2[i];
            for(int j=0;j<9;j++)
              if(strcmp(s2[i],b[j])==0)
              {
                  q[i]=j;
                  break;
              }
        }
        for(int i=0;i<3;i++)
          ans+=a[p[i]][q[i]];
        printf("%.2lf\n",ans);
    }       
    return 0;
}
```
