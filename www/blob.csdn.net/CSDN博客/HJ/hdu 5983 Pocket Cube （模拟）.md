# hdu 5983  Pocket Cube （模拟） - HJ - CSDN博客
2017年10月16日 21:32:48[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：219
[http://acm.split.hdu.edu.cn/showproblem.php?pid=5983](http://acm.split.hdu.edu.cn/showproblem.php?pid=5983)
题目大意 
给出一个2*2*2的魔方，判断是否能够两步还原。
解题思路： 
总共有前后左右上下六个方向，每次判断旋转90度或者180度，判断是否还原。另外还需要判断刚开始的时候是否已经还原。（因为还可以两步走回来）
按照题目解释的来，一步一步模拟就行了。（不要粗心，不然很难调试）
```cpp
#include<iostream>
#include<set>
#include<cstdio>
using namespace std;
int a[10][10],t;
bool check(int c[10][10])           //判断是否复原 
{
    set<int>s;
    for(int i=0;i<6;i++)
    {
        s.insert(c[i][0]);
        for(int j=1;j<4;j++)
          if(c[i][j]!=c[i][0])         //六个面每个面四个数字都要相等 
            return false;
    }
    return s.size()==6;          //六个面各个面的数字都不相同 
}
void Copy(int x[10][10])
{
    for(int i=0;i<6;i++)
      for(int j=0;j<4;j++)
        x[i][j]=a[i][j];
}
bool getchange1()
{
    int c[10][10];
    Copy(c);    
    c[0][2]=a[5][2];c[0][3]=a[5][3];
    c[4][2]=a[0][2];c[4][3]=a[0][3];
    c[2][0]=a[4][2];c[2][1]=a[4][3];
    c[5][2]=a[2][0];c[5][3]=a[2][1];
    if(check(c))
      return true;
    c[0][2]=a[4][2];c[0][3]=a[4][3];
    c[4][2]=a[2][0];c[4][3]=a[2][1];
    c[2][0]=a[5][2];c[2][1]=a[5][3];
    c[5][2]=a[0][2];c[5][3]=a[0][3];
    return check(c);
}
bool getchange2()
{
    int c[10][10];
    Copy(c);    
    c[0][0]=a[1][0];c[0][2]=a[1][2];
    c[1][0]=a[2][0];c[1][2]=a[2][2];
    c[2][0]=a[3][0];c[2][2]=a[3][2];
    c[3][0]=a[0][0];c[3][2]=a[0][2];
    if(check(c))
      return true;
    c[0][0]=a[3][0];c[0][2]=a[3][2];
    c[1][0]=a[0][0];c[1][2]=a[0][2];
    c[2][0]=a[1][0];c[2][2]=a[1][2];
    c[3][0]=a[2][0];c[3][2]=a[2][2];
    return check(c);
}
bool getchange3()
{
    int c[10][10];
    Copy(c);    
    c[1][0]=a[5][2];c[1][1]=a[5][0];
    c[4][1]=a[1][0];c[4][3]=a[1][1];
    c[3][2]=a[4][1];c[3][3]=a[4][3];
    c[5][2]=a[3][2];c[5][0]=a[3][3];
    if(check(c))
      return true;
    c[1][0]=a[4][1];c[1][1]=a[4][3];
    c[4][1]=a[3][2];c[4][3]=a[3][3];
    c[3][2]=a[5][2];c[3][3]=a[5][0];
    c[5][2]=a[1][0];c[5][0]=a[1][1];
    return check(c);
}
bool make()
{
    return getchange1()?true:(getchange2()?true:getchange3());
}
int main()
{
    cin>>t;
    while(t--)
    {
        for(int i=0;i<6;i++)
          for(int j=0;j<4;j++)
            cin>>a[i][j];
        printf("%s\n",check(a)?"YES":(make()?"YES":"NO"));
    }
    return 0;
}
```
