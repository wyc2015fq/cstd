# Largest Rectangle in a Histogram（HDU-1506） - Alex_McAvoy的博客 - CSDN博客





2019年02月10日 18:23:22[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：63
个人分类：[HDU																[数据结构——单调栈与单调队列](https://blog.csdn.net/u011815404/article/category/8661014)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

A histogram is a polygon composed of a sequence of rectangles aligned at a common base line. The rectangles have equal widths but may have different heights. For example, the figure on the left shows the histogram that consists of rectangles with the heights 2, 1, 4, 5, 1, 3, 3, measured in units where 1 is the width of the rectangles: 

![](https://img-blog.csdnimg.cn/20190210172922705.jpg)

Usually, histograms are used to represent discrete distributions, e.g., the frequencies of characters in texts. Note that the order of the rectangles, i.e., their heights, is important. Calculate the area of the largest rectangle in a histogram that is aligned at the common base line, too. The figure on the right shows the largest aligned rectangle for the depicted histogram.

# **Input**

The input contains several test cases. Each test case describes a histogram and starts with an integer n, denoting the number of rectangles it is composed of. You may assume that 1 <= n <= 100000. Then follow n integers h1, ..., hn, where 0 <= hi <= 1000000000. These numbers denote the heights of the rectangles of the histogram in left-to-right order. The width of each rectangle is 1. A zero follows the input for the last test case.

# Output

For each test case output on a single line the area of the largest rectangle in the specified histogram. Remember that this rectangle must be aligned at the common base line.

# Sample Input

**7 2 1 4 5 1 3 34 1000 1000 1000 10000**

# Sample Output

**84000**


题意：每组给出 n 个数，代表矩形的高，每个矩形的宽固定都是 1，求矩形的最大面积

思路：单调栈

由于宽是固定的，因此最大矩形面积一定是一串连续的矩形中最小的矩形的高，因此可以枚举每一个矩形的高，将他当作最矮的矩形，那么只需要知道宽即可，由于将每个矩形当作最矮的矩形，因此他左右两边矩形的高度应该大于他，依次类推，使用单调栈来维护，即可得出矩形的宽，最后求出面积，再从所有的面积中求出最大的即可

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
#define N 100001
#define LL long long
const int MOD=998244353;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
LL a[N];
LL resL[N],resR[N];
int main(){
    int n;
    while(scanf("%d",&n)!=EOF&&(n)){
        memset(resL,0,sizeof(resL));
        memset(resR,0,sizeof(resR));

        for(int i=1;i<=n;i++)
            scanf("%lld",&a[i]);

        stack<LL> SL;
        for(int i=1;i<=n;i++){//当前矩形左边能延展到的矩形的下标
            while(!SL.empty()&&a[SL.top()]>=a[i])
                SL.pop();
            if(!SL.empty())
                resL[i]=SL.top();
            else
                resL[i]=0;

            SL.push(i);
        }

        stack<LL> SR;
        for(int i=n;i>=1;i--){//当前矩形右边能延展到的矩形的下标
            while(!SR.empty()&&a[SR.top()]>=a[i])
                SR.pop();
            if(!SR.empty())
                resR[i]=SR.top();
            else
                resR[i]=n+1;

            SR.push(i);
        }

        LL maxx=0;
        for(int i=1;i<=n;i++)
            maxx=max(maxx,a[i]*(resR[i]-resL[i]-1));

        printf("%lld\n",maxx);
    }
    return 0;
}
```






