# Passing the Message（HDU-3410） - Alex_McAvoy的博客 - CSDN博客





2019年02月11日 20:03:03[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：59
个人分类：[HDU																[数据结构——单调栈与单调队列](https://blog.csdn.net/u011815404/article/category/8661014)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

What a sunny day! Let’s go picnic and have barbecue! Today, all kids in “Sun Flower” kindergarten are prepared to have an excursion. Before kicking off, teacher Liu tells them to stand in a row. Teacher Liu has an important message to announce, but she doesn’t want to tell them directly. She just wants the message to spread among the kids by one telling another. As you know, kids may not retell the message exactly the same as what they was told, so teacher Liu wants to see how many versions of message will come out at last. With the result, she can evaluate the communication skills of those kids. 

Because all kids have different height, Teacher Liu set some message passing rules as below: 

1.She tells the message to the tallest kid. 

2.Every kid who gets the message must retell the message to his “left messenger” and “right messenger”. 

3.A kid’s “left messenger” is the kid’s tallest “left follower”. 

4.A kid’s “left follower” is another kid who is on his left, shorter than him, and can be seen by him. Of course, a kid may have more than one “left follower”. 

5.When a kid looks left, he can only see as far as the nearest kid who is taller than him. 

The definition of “right messenger” is similar to the definition of “left messenger” except all words “left” should be replaced by words “right”. 

For example, suppose the height of all kids in the row is 4, 1, 6, 3, 5, 2 (in left to right order). In this situation , teacher Liu tells the message to the 3rd kid, then the 3rd kid passes the message to the 1st kid who is his “left messenger” and the 5th kid who is his “right messenger”, and then the 1st kid tells the 2nd kid as well as the 5th kid tells the 4th kid and the 6th kid. 

Your task is just to figure out the message passing route.

# **Input**

The first line contains an integer T indicating the number of test cases, and then T test cases follows. 

Each test case consists of two lines. The first line is an integer N (0< N <= 50000) which represents the number of kids. The second line lists the height of all kids, in left to right order. It is guaranteed that every kid’s height is unique and less than 2^31 – 1 .

# Output

For each test case, print “Case t:” at first ( t is the case No. starting from 1 ). Then print N lines. The ith line contains two integers which indicate the position of the ith (i starts form 1 ) kid’s “left messenger” and “right messenger”. If a kid has no “left messenger” or “right messenger”, print ‘0’ instead. (The position of the leftmost kid is 1, and the position of the rightmost kid is N)

# Sample Input

**255 2 4 3 152 1 4 3 5**

# Sample Output

**Case 1:0 30 02 40 50 0Case 2:0 20 01 40 03 0**


题意：t 组数据，每组给出 n 个数，对于第 i 个数，在左边求出比他小的数中最大的数的位置，在他右边求出比他小的数中最大的数的位置，若不存在就输出 0

思路：单调递减栈来维护，对于每个数左右两边各自扫一遍，一边维护一边更新

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<deque>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define INF 0x3f3f3f3f
#define N 2000001
#define LL long long
const int MOD=998244353;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
int a[N];
int resL[N],resR[N];
int main(){
    int t;
    scanf("%d",&t);
    int Case=1;
    while(t--){
        memset(resL,0,sizeof(resL));
        memset(resR,0,sizeof(resR));

        int n;
        scanf("%d",&n);
        for(int i=1;i<=n;i++)
            scanf("%d",&a[i]);

        stack<int> SL;
        for(int i=1;i<=n;i++){//从左到右
            while(!SL.empty()&&a[SL.top()]<=a[i]){
                resL[i]=SL.top();
                SL.pop();
            }
            SL.push(i);
        }

        stack<int> SR;
        for(int i=n;i>=1;i--){//从右到左
            while(!SR.empty()&&a[SR.top()]<=a[i]){
                resR[i]=SR.top();
                SR.pop();
            }
            SR.push(i);
        }

        printf ("Case %d:\n",Case++);
        for(int i=1;i<=n;i++)
            printf ("%d %d\n",resL[i],resR[i]);
    }
    return 0;
}
```






