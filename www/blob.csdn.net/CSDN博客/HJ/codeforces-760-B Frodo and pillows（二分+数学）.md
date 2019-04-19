# codeforces-760-B Frodo and pillows（二分+数学） - HJ - CSDN博客
2017年01月25日 17:44:33[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：573
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
n hobbits are planning to spend the night at Frodo’s house. Frodo has n beds standing in a row and m pillows (n ≤ m). Each hobbit needs a bed and at least one pillow to sleep, however, everyone wants as many pillows as possible. Of course, it’s not always possible to share pillows equally, but any hobbit gets hurt if he has at least two pillows less than some of his neighbors have.
Frodo will sleep on the k-th bed in the row. What is the maximum number of pillows he can have so that every hobbit has at least one pillow, every pillow is given to some hobbit and no one is hurt? 
Input
The only line contain three integers n, m and k (1 ≤ n ≤ m ≤ 109, 1 ≤ k ≤ n) — the number of hobbits, the number of pillows and the number of Frodo’s bed. 
Output
Print single integer — the maximum number of pillows Frodo can have so that no one is hurt. 
Examples 
Input
4 6 2
Output
2
Input
3 10 3
Output
4
Input
3 6 1
Output
3
Note
In the first example Frodo can have at most two pillows. In this case, he can give two pillows to the hobbit on the first bed, and one pillow to each of the hobbits on the third and the fourth beds.
In the second example Frodo can take at most four pillows, giving three pillows to each of the others.
In the third example Frodo can take three pillows, giving two pillows to the hobbit in the middle and one pillow to the hobbit on the third bed.
题目大意： 
   n个人（包括Frodo）在Frodo家里过夜，家里有n张床和m个枕头，每个人都至少一张床和一个枕头，但是每个人都想得到尽可能多的枕头，但是如果有任何一个人，他旁边的两个人的枕头比他少了超过一个，那么就会受伤。Frodo睡在第k个位置（k当然小于等于n）,问：在没有人受伤的情况下，Frodo最多能得到多少个枕头。
解题思路： 
    先把n个枕头每人一个分配好，剩下k=m-n个，然后进行左右两边判断， 
   若y>x-1
![这里写图片描述](http://espresso.codeforces.com/61ed39eaf46c317d69518892688ebdfccf8464b1.png)
否则如果y<=x-1
![这里写图片描述](http://espresso.codeforces.com/2f61d263277e81061ed140267d483901c8faabba.png)
然后注意一下的是需要用到二分，否则会超时。
代码如下
```cpp
#include<iostream>
using namespace std;
#define INF 1000000000
long long n,m,k;       //数据比较大，用long long 才能过
long long getsum(long long a,long long b)
{
    if(b==0)
      return 0;
    long long mind=min(a,b);
    return (2*a-mind+1)*mind/2;     //求和公式
}
int check(long long x)
{
    long long p=max((long long)0,n-k+1);    //进行右边边界判断
    long long q=max((long long)0,k-1);      //进行左边边界判断
    long long sum=getsum(x,p)+getsum(x-1,q);    //左右两边分别求和累加
    if(sum<=m)
      return 1;
    return 0;
}
int main()
{
    long long ans;
    while(cin>>n>>m>>k)
    {
        m-=n;    //首先把n个枕头平分给n个人
        long long l=0,r=INF+1,mid;
        while(l<r)            //二分查找出Frodo最大能够增加的枕头的数量
        {
            mid=(l+r)/2;
            if(check(mid))
              l=mid+1;
            else
              r=mid;
        }
        cout<<l<<endl;
    }
    return 0;
}
```
