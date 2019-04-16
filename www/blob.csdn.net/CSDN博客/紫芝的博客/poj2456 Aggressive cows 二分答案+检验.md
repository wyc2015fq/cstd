# poj2456 Aggressive cows 二分答案+检验 - 紫芝的博客 - CSDN博客





2018年07月22日 20:44:19[紫芝](https://me.csdn.net/qq_40507857)阅读数：35
个人分类：[二分的力量																[贪心](https://blog.csdn.net/qq_40507857/article/category/7568514)](https://blog.csdn.net/qq_40507857/article/category/7595714)








# Aggressive cows
|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 19723||**Accepted:** 9330|

Description

Farmer John has built a new long barn, with N (2 <= N <= 100,000) stalls. The stalls are located along a straight line at positions x1,...,xN (0 <= xi <= 1,000,000,000). 


His C (2 <= C <= N) cows don't like this barn layout and become aggressive towards each other once put into a stall. To prevent the cows from hurting each other, FJ want to assign the cows to the stalls, such that the minimum distance between any two of them is as large as possible. What is the largest minimum distance?

Input

* Line 1: Two space-separated integers: N and C 


* Lines 2..N+1: Line i+1 contains an integer stall location, xi

Output

* Line 1: One integer: the largest minimum distance

Sample Input

5 3
1
2
8
4
9
Sample Output
`3`
Hint

OUTPUT DETAILS: 


FJ can put his 3 cows in the stalls at positions 1, 4 and 8, resulting in a minimum distance of 3. 


Huge input data,scanf is recommended.

Source

[USACO 2005 February Gold](http://poj.org/searchproblem?field=source&key=USACO+2005+February+Gold)

题意：

有n个牛栏，选取m个放入牛，相当于一条线段上有n个点，选取m个点，使得相邻点之间最小距离最大

思路：

贪心+二分

二分枚举相邻两头牛之间的间距，判断大于等于此间距时，能否放下所有的牛

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;
const int N = 1e6+10;
int a[N],n,m;
bool judge(int k)//枚举间距k，看能否使任意两相邻牛
{
    int cnt = a[0], num = 1;
    //num为1表示已经第一头牛放在a[0]牛栏中
    for(int i = 1; i < n; i ++)//枚举剩下的牛栏
    {
        if(a[i] - cnt >= k)//a[i]这个牛栏和上一个牛栏间距大于等于k，表示可以再放进牛
        {
            cnt = a[i];
            num ++;//又放进了一头牛
        }
        if(num >= m) return true;//所有牛都放完了
    }
    return false;
}
void solve()
{
    int l = 1, r = a[n-1] - a[0];
    //最小距离为1，最大距离为牛栏编号最大的减去编号最小的
    while(l < r)
    {
        int mid = (l+r) >> 1;
        if(judge(mid)) l = mid + 1;
        else r = mid;
    }
    printf("%d\n",r-1);
}
int main()
{
    int i;
    while(~scanf("%d%d",&n,&m))
    {
        for(i = 0; i < n; i ++)
            scanf("%d",&a[i]);
        sort(a, a+n);//对牛栏排序
        solve();
    }
    return 0;
}
```






