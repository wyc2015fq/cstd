# hdu  5023 A Corrupt Mayor's Performance Art（线段树+set） - HJ - CSDN博客
2017年01月26日 22:51:18[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：260
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------数据结构--------																[线段树](https://blog.csdn.net/feizaoSYUACM/article/category/6308065)](https://blog.csdn.net/feizaoSYUACM/article/category/6130307)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
Corrupt governors always find ways to get dirty money. Paint something, then sell the worthless painting at a high price to someone who wants to bribe him/her on an auction, this seemed a safe way for mayor X to make money.
Because a lot of people praised mayor X’s painting(of course, X was a mayor), mayor X believed more and more that he was a very talented painter. Soon mayor X was not satisfied with only making money. He wanted to be a famous painter. So he joined the local painting associates. Other painters had to elect him as the chairman of the associates. Then his painting sold at better price.
The local middle school from which mayor X graduated, wanted to beat mayor X’s horse fart(In Chinese English, beating one’s horse fart means flattering one hard). They built a wall, and invited mayor X to paint on it. Mayor X was very happy. But he really had no idea about what to paint because he could only paint very abstract paintings which nobody really understand. Mayor X’s secretary suggested that he could make this thing not only a painting, but also a performance art work.
This was the secretary’s idea:
The wall was divided into N segments and the width of each segment was one cun(cun is a Chinese length unit). All segments were numbered from 1 to N, from left to right. There were 30 kinds of colors mayor X could use to paint the wall. They named those colors as color 1, color 2 …. color 30. The wall’s original color was color 2. Every time mayor X would paint some consecutive segments with a certain kind of color, and he did this for many times. Trying to make his performance art fancy, mayor X declared that at any moment, if someone asked how many kind of colors were there on any consecutive segments, he could give the number immediately without counting.
But mayor X didn’t know how to give the right answer. Your friend, Mr. W was an secret officer of anti-corruption bureau, he helped mayor X on this problem and gained his trust. Do you know how Mr. Q did this?
Input 
There are several test cases.
For each test case:
The first line contains two integers, N and M ,meaning that the wall is divided into N segments and there are M operations(0  <  N <= 1,000,000; 0< M<=100,000)
Then M lines follow, each representing an operation. There are two kinds of operations, as described below:
1) P a b c 
a, b and c are integers. This operation means that mayor X painted all segments from segment a to segment b with color c ( 0 < a<=b <= N, 0 < c <= 30).
2) Q a b 
a and b are integers. This is a query operation. It means that someone asked that how many kinds of colors were there from segment a to segment b ( 0 < a<=b <= N).
Please note that the operations are given in time sequence.
The input ends with M = 0 and N = 0.
Output 
For each query operation, print all kinds of color on the queried segments. For color 1, print 1, for color 2, print 2 … etc. And this color sequence must be in ascending order.
Sample Input
5 10 
P 1 2 3 
P 2 3 4 
Q 2 3 
Q 1 3 
P 3 5 4 
P 1 2 7 
Q 1 3 
Q 3 4 
P 5 5 8 
Q 1 5 
0 0
Sample Output
4 
3 4 
4 7 
4 
4 7 8
Source 
2014 ACM/ICPC Asia Regional Guangzhou Online
题目大意： 
处理一段数据，可以区间更新，可以区间查找，每一次查找，都是查找给定区间内不相同的数值，按从小到大的顺序输出。
解题思路： 
开一个线段树，lazy处理一下，如果发现新的数据，压入集合set里，最后输出就好了。
注意：代码中有一些需要注意的点，推荐仔细理解。
```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
#include<set>
using namespace std;
#define imax 1000005
struct node{
    int l,r;
    int val;
}f[imax*3];
int l,r;
set<int>v;
void build(int t,int l,int r)
{
    f[t].l=l;
    f[t].r=r;
    f[t].val=0;  
    if(l==r)
      return ;
    int mid=(l+r)/2;
    build(t*2,l,mid);
    build(t*2+1,mid+1,r);
}
void insert(int t,int l,int r,int c)
{
    if(f[t].l==l&&f[t].r==r)
    {
        f[t].val=c;
        return ;
    }
//  if(f[t].val==c)  //如果需要更新的数据跟当前节点原来存在的数据相同，则不必更新（这一行可写可不写，但是可以减少一点O（n)） 
//    return ;
    if(f[t].val)
    {
        f[t*2].val=f[t*2+1].val=f[t].val;     //lazy操作  表示在区间内数值都相同（都为val） 
        f[t].val=0;                          //标记消除 
    }
    int mid=(f[t].l+f[t].r)/2;
    if(r<=mid)
      insert(t*2,l,r,c);
    else
    {
        if(l>mid)
          insert(t*2+1,l,r,c);
        else
        {
            insert(t*2,l,mid,c);
            insert(t*2+1,mid+1,r,c);
        }
    } 
}
void query(int t,int l,int r)
{
    if(f[t].val)          //如果当前节点左右数值区间内数值都相同 
    {
        v.insert(f[t].val);       //压入集合防止出现重复 
        return ;
    }
    int mid=(f[t].l+f[t].r)/2;
    if(r<=mid)
      query(t*2,l,r);
    else
    {
        if(l>mid)
          query(t*2+1,l,r);
        else
        {
            query(t*2,l,mid);
            query(t*2+1,mid+1,r);
        }
    }
}
int main()
{
    int n,m,a,b,c;
    while(cin>>n>>m)
    {
        if(n==0&&m==0)
          break;
        build(1,1,n);   
        f[1].val=2;         //注意这一项，f[1]的值应该要为2         
        char s[10];
        for(int i=0;i<m;i++)
        {
            cin>>s>>a>>b;
            if(s[0]=='P')
            {
                cin>>c;
                insert(1,a,b,c);
            } 
            else
            {
                v.clear();
                query(1,a,b);
                int ss=v.size();
                set<int>::iterator t;
                for(t=v.begin();t!=v.end();t++)
                {
                    cout<<*t;
                    if(ss>1)
                      cout<<" ";
                    ss--;
                }
                cout<<endl;  
            }
        }
    }
    return 0;
}
```
