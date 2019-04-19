# codeforces 478C. Table Decorations（规律） - HJ - CSDN博客
2017年07月12日 10:04:09[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：202
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
You have r red, g green and b blue balloons. To decorate a single table for the banquet you need exactly three balloons. Three balloons attached to some table shouldn’t have the same color. What maximum number t of tables can be decorated if we know number of balloons of each color?
Your task is to write a program that for given values r, g and b will find the maximum number t of tables, that can be decorated in the required manner. 
Input
The single line contains three integers r, g and b (0 ≤ r, g, b ≤ 2·109) — the number of red, green and blue baloons respectively. The numbers are separated by exactly one space. 
Output
Print a single integer t — the maximum number of tables that can be decorated in the required manner. 
Examples 
Input
5 4 3
Output
4
Input
1 1 1
Output
1
Input
2 3 3
Output
2
Note
In the first sample you can decorate the tables with the following balloon sets: “rgg”, “gbb”, “brr”, “rrg”, where “r”, “g” and “b” represent the red, green and blue balls, respectively.
题目大意： 
  已知红色、绿色、蓝色的气球的数目，装扮一个桌子要三个气球，每个桌子的气球颜色数必须大于等于2。求最多能装扮多少个桌子？
解题思路： 
先用s[]存好，从小到大排个序。先考虑最小的s[0]：
因为s[0]<=s[1]并且s[0]<=s[2]  因此有 s[0]*2<=s[1]+s[2]
处理完s[0]之后就剩下s[1]和s[2]了，此时只要取整3即可。
但是需要考虑一种特殊情况（即一个数特别大，那么只能按照其他两个小的数来取了）
```cpp
#include<iostream>
#include<algorithm>
using namespace std;
long long s[5];
int main()
{
    while(cin>>s[0]>>s[1]>>s[2])
    {
        sort(s,s+3);    
        long long ans=(s[1]+s[2]-s[0]*2)/3+s[0]; 
        if(s[2]>(s[0]+s[1])*2)
          ans=s[0]+s[1];
        cout<<ans<<endl;
    }
    return 0;
}
```
