# hdu  4710  Balls Rearrangement（规律） - HJ - CSDN博客
2016年08月19日 18:40:08[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：324
个人分类：[----------数学----------																[递推公式&结论题																[====ACM====																[HDU																[---------OJ---------](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)](https://blog.csdn.net/feizaoSYUACM/article/category/6308108)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)
Problem Description 
Bob has N balls and A boxes. He numbers the balls from 0 to N-1, and numbers the boxes from 0 to A-1. To find the balls easily, he puts the ball numbered x into the box numbered a if x = a mod A. 
Some day Bob buys B new boxes, and he wants to rearrange the balls from the old boxes to the new boxes. The new boxes are numbered from 0 to B-1. After the rearrangement, the ball numbered x should be in the box number b if x = b mod B. 
This work may be very boring, so he wants to know the cost before the rearrangement. If he moves a ball from the old box numbered a to the new box numbered b, the cost he considered would be |a-b|. The total cost is the sum of the cost to move every ball, and it is what Bob is interested in now.
Input 
The first line of the input is an integer T, the number of test cases.(0大于T小于等于50) 
Then T test case followed. The only line of each test case are three integers N, A and B.(1<=N<=1000000000, 1<=A,B<=100000).
Output 
For each test case, output the total cost.
Sample Input
3 
1000000000 1 1 
8 2 4 
11 5 3
Sample Output
0 
8 
16
题目意思：现在有很多球，球按照0~N-1编号，并且还有两堆装球的盒子，一堆新盒子按照0~A-1编号，一堆老盒子按照0~B-1编号，现在按照取模的方式先把球放到新盒子里面去，比如说如果有8个球，2个新盒子，则第一个盒子放编号为0、2、4、6的球，第二个新盒子放编号为1、3、5、7的球。然后又要把球从新盒子放到老盒子里面去，同样按照取模的方式，比如现在有4个老盒子，则情况如下：
```
（盒子的编号，下同）：（球的编号）
                              0：0、4
                              1：1、5
                              2：2、6
                              3：3、7
```
然后求出0~N-1的这个N个球在新老盒子者编号的差值的绝对值之和
解析：题目意思理解了，可是数据太大（10的9次方），根本不能按照正常思路来做（否则超时）。题目要求的是编号X的球在两种盒子编号差值的绝对值之和，求一个球的差值的绝对值的时候，公式如下：
```
abs(n%a-n%b)
```
根据同余，可以很快想到一个以最小公倍数为一次循环的循环节 
但是如果a和b的最小公倍数很大很大呢？怎么办？
我们需要继续从里面找规律，下面给出一组数，大家观察：
```
5: 0 1 2 3 4 0 1 2 3 4 0 1 2 3 4 0  
3：0 1 2 0 1 2 0 1 2 0 1 2 0 1 2 0 
|a-b|：0 0 0 3 3 2 1 1 1 4 1 1 2 2 2 0
```
观察可知，当n%a或者n%b为0，到n%a或者n%b为0，这段中，|a-b|是相等的
这样有用吗？当然有用，每次记录下这个相等的差值，再求出两个点之间的数据个数，用差值乘以数据个数，就可以计算出这段数据的差值绝对值的和而不需要一个数据一个数据去跑了，会大大减少时间的复杂度。
请大家注意理解，代码如下：
```cpp
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<cstring>
#include<string>
using namespace std;
int gcd(int a,int b)    //求最大公约数
{
    return b==0?a:gcd(b,a%b);
}
__int64 find(__int64 a,__int64 b,__int64 c)
{
    __int64 x=0,y=0,cnt,i=0,rr,ans=0;
    for(;i<c;i+=cnt)    //根据规律，在前一个k%a=0或者k%b=0和后一个k%a=0或者k%b=0之间，差值是相等的
    {
        cnt=min(a-x,b-y);   
        if(cnt+i>c)
          cnt=c-i;    //计算这两个之间的长度（或者说个数）
        rr=abs(int(y-x));   //计算这个相等的差值
        ans+=cnt*rr;   //相乘，达到减少循环的目的
        x=(x+cnt)%a;
        y=(y+cnt)%b;
    }
    return ans;
}
int main()
{
    __int64 t,n,a,b,k,sum;
    cin>>t;
    while(t--)
    {
        cin>>n>>a>>b;
        k=a*b/gcd(a,b);   //求最小公倍数  可以发现a跟b的最小公倍数为一个循环节
        sum=0;
        if(a!=b)
          sum=(n/k)*find(a,b,k)+find(a,b,n%k);   //求出循环节的总差值以及取模的差值和，用循环节的总差值乘以循环次数再加上取模的差值和得到的就是结果了
        cout<<sum<<endl;
    }
    return 0;
}
```
