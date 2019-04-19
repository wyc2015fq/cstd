# hdu 4801 Pocket Cube（dfs+模拟） - HJ - CSDN博客
2017年10月30日 21:13:11[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：209
[http://acm.hdu.edu.cn/showproblem.php?pid=4801](http://acm.hdu.edu.cn/showproblem.php?pid=4801)
题目大意： 
现在有一个2*2*2的魔方，给出指定步数，问你在指定步数之内最多能还原几个面？
解题思路： 
dfs搜6次，每次模拟7次，分别是前，左，上（各顺时针逆时针90度）
```
#include<iostream> 
using namespace std;
struct node{
    int s[25];
};
int n,ans,turn[6][24] = {
    {0,1,8,14,4,3,7,13,17,9,10,2,6,12,16,15,5,11,18,19,20,21,22,23},
    {0,1,11,5,4,16,12,6,2,9,10,17,13,7,3,15,14,8,18,19,20,21,22,23},
    {1,3,0,2,23,22,4,5,6,7,10,11,12,13,14,15,16,17,18,19,20,21,9,8},
    {2,0,3,1,6,7,8,9,23,22,10,11,12,13,14,15,16,17,18,19,20,21,5,4},
    {6,1,12,3,5,11,16,7,8,9,4,10,18,13,14,15,20,17,22,19,0,21,2,23},
    {20,1,22,3,10,4,0,7,8,9,11,5,2,13,14,15,6,17,12,19,16,21,18,23},
};
int Judge(node p)
{
    return (p.s[0]==p.s[1]&&p.s[0]==p.s[2]&&p.s[0]==p.s[3])+
           (p.s[4]==p.s[5]&&p.s[4]==p.s[10]&&p.s[4]==p.s[11])+
           (p.s[6]==p.s[7]&&p.s[6]==p.s[12]&&p.s[6]==p.s[13])+
           (p.s[8]==p.s[9]&&p.s[8]==p.s[14]&&p.s[8]==p.s[15])+
           (p.s[16]==p.s[17]&&p.s[16]==p.s[18]&&p.s[16]==p.s[19])+
           (p.s[20]==p.s[21]&&p.s[20]==p.s[22]&&p.s[20]==p.s[23]);
}
node change(node p,int i)
{
    node q;
    for(int j=0;j<24;j++)
      q.s[j]=p.s[turn[i][j]];
    return q;
}
void dfs(node p,int cur,int i)
{   
    if(cur>n||ans==6)
      return ;   
    node q=change(p,i);
    ans=max(ans,Judge(q));
    for(int i=0;i<6;i++)
      dfs(q,cur+1,i);
}
int main()
{
    while(cin>>n)
    {
        node p;
        for(int i=0;i<24;i++)
          cin>>p.s[i];
        ans=Judge(p);
        for(int i=0;i<6;i++)
          dfs(p,1,i);
        cout<<ans<<endl;
    }
    return 0;
}
```
