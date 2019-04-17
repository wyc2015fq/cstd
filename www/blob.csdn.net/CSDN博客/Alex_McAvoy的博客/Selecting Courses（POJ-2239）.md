# Selecting Courses（POJ-2239） - Alex_McAvoy的博客 - CSDN博客





2018年11月26日 23:49:00[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：28
个人分类：[POJ																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

It is well known that it is not easy to select courses in the college, for there is usually conflict among the time of the courses. Li Ming is a student who loves study every much, and at the beginning of each term, he always wants to select courses as more as possible. Of course there should be no conflict among the courses he selects. 

There are 12 classes every day, and 7 days every week. There are hundreds of courses in the college, and teaching a course needs one class each week. To give students more convenience, though teaching a course needs only one class, a course will be taught several times in a week. For example, a course may be taught both at the 7-th class on Tuesday and 12-th class on Wednesday, you should assume that there is no difference between the two classes, and that students can select any class to go. At the different weeks, a student can even go to different class as his wish. Because there are so many courses in the college, selecting courses is not an easy job for Li Ming. As his good friends, can you help him?

# **Input**

The input contains several cases. For each case, the first line contains an integer n (1 <= n <= 300), the number of courses in Li Ming's college. The following n lines represent n different courses. In each line, the first number is an integer t (1 <= t <= 7*12), the different time when students can go to study the course. Then come t pairs of integers p (1 <= p <= 7) and q (1 <= q <= 12), which mean that the course will be taught at the q-th class on the p-th day of a week.

# Output

For each test case, output one integer, which is the maximum number of courses Li Ming can select.

# Sample Input

**51 1 12 1 1 2 21 2 22 3 2 3 31 3 3**

# Sample Output

**4**


题意：规定每周每天有 12 节课，先给出 n 个课程，以及这 n 个课程在第 p 天的第 q 节上，如果几个课都在同一天的同一节上，那么需要选择其中一个，不能选择多个课程，求一周最多能上几节课

思路：将 n 节课看成左边的点集，将每周 7*12=84 节课看成右边的点集，即为一个二分图，根据题意，现在要选出最多的点对，使得左边的点尽可能的与右边匹配，即为二分图的最大匹配问题

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
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 10001
#define LL long long
using namespace std;
bool vis[N];
int link[N];
vector<int> G[N];
bool dfs(int x){
    for(int i=0;i<G[x].size();i++){
        int y=G[x][i];
        if(!vis[y]){
            vis[y]=true;
            if(link[y]==-1 || dfs(link[y])){
                link[y]=x;
                return true;
            }
        }
    }
    return false;
}
int hungarian(int n)
{
    int ans=0;
    for(int i=1;i<=n;i++){
        memset(vis,false,sizeof(vis));
        if(dfs(i))
            ans++;
    }
    return ans;
}
int main(){
    int n;
    while(scanf("%d",&n)!=EOF&&n){
        memset(link,-1,sizeof(link));
        for(int i=0;i<N;i++)
            G[i].clear();

        for(int i=1;i<=n;i++){
            int num;
            scanf("%d",&num);
            while(num--){
                int p,q;
                scanf("%d%d",&p,&q);
                G[i].push_back( (p-1)*12+q );
            }
        }
        printf("%d\n",hungarian(n));

    }
    return 0;
}
```






