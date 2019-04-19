# hdu   5935  Car（贪心） - HJ - CSDN博客
2017年03月13日 19:56:33[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：281
Problem Description 
Ruins is driving a car to participating in a programming contest. As on a very tight schedule, he will drive the car without any slow down, so the speed of the car is non-decrease real number.
Of course, his speeding caught the attention of the traffic police. Police record N positions of Ruins without time mark, the only thing they know is every position is recorded at an integer time point and Ruins started at 0.
Now they want to know the minimum time that Ruins used to pass the last position.
Input 
First line contains an integer T, which indicates the number of test cases.
Every test case begins with an integers N, which is the number of the recorded positions.
The second line contains N numbers a1, a2, ⋯, aN, indicating the recorded positions.
Limits 
1≤T≤100 
1≤N≤105 
0< ai≤109 
ai< ai+1
Output 
For every test case, you should output ‘Case #x: y’, where x indicates the case number and counts from 1 and y is the minimum time.
Sample Input
1 
3 
6 11 21
Sample Output
Case #1: 4
题目大意：
比较难懂！有一辆车现在从t=0开始走，速度不能递减（可以匀速，也可以变加速）。警察在t为整数的时候记录了N个车的位置(整数)，问到达最后一个位置时这辆车总共开了多长时间。
解题思路：
刚开始以为是递归求解，但是WA了好几发。后来转化一下思路，可以从最后一个距离往前逆序遍历，只要满足速度不能递减，求在这个情况下的最少时间就行了，那么贪心一下，还是蛮好做的。（开始傻逼了）
注意精度问题！
```cpp
#include<iostream>
#include<cstdio>
#include<algorithm>
using namespace std;
#define eps 1e-8
int s[100005];
int main()
{
    int t,n,x,ans,tt=1;
    double v;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d",&n);
        ans=1;
        for(int i=1;i<=n;i++)
          scanf("%d",&s[i]);
        v=s[n]-s[n-1];
        for(int i=n-1;i;i--)
        {
            x=s[i]-s[i-1];
            if(x<=v+eps)
            {
                ans++;
                v=x;
            }
            else
            {
                ans+=(int(double(x-eps)/v)+1);
                v=double(x)/(int(double(x-eps)/v)+1);
            }
        }
        printf("Case #%d: %d\n",tt++,ans);
    }
    return 0;
}
```
