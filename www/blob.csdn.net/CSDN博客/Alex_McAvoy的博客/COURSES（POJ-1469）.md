# COURSES（POJ-1469） - Alex_McAvoy的博客 - CSDN博客





2018年11月26日 23:26:25[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：40
个人分类：[POJ																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Consider a group of N students and P courses. Each student visits zero, one or more than one courses. Your task is to determine whether it is possible to form a committee of exactly P students that satisfies simultaneously the conditions: 

every student in the committee represents a different course (a student can represent a course if he/she visits that course) 

each course has a representative in the committee 

# **Input**

Your program should read sets of data from the std input. The first line of the input contains the number of the data sets. Each data set is presented in the following format: 

P N 

Count1 Student1 1 Student1 2 ... Student1 Count1 

Count2 Student2 1 Student2 2 ... Student2 Count2 

... 

CountP StudentP 1 StudentP 2 ... StudentP CountP 

The first line in each data set contains two positive integers separated by one blank: P (1 <= P <= 100) - the number of courses and N (1 <= N <= 300) - the number of students. The next P lines describe in sequence of the courses �from course 1 to course P, each line describing a course. The description of course i is a line that starts with an integer Count i (0 <= Count i <= N) representing the number of students visiting course i. Next, after a blank, you抣l find the Count i students, visiting the course, each two consecutive separated by one blank. Students are numbered with the positive integers from 1 to N. 

There are no blank lines between consecutive sets of data. Input data are correct. 

# Output

The result of the program is on the standard output. For each input data set the program prints on a single line "YES" if it is possible to form a committee and "NO" otherwise. There should not be any leading blanks at the start of the line.

# Sample Input

**23 33 1 2 32 1 21 13 32 1 32 1 31 1**

# Sample Output

**YESNO**


题意：给出 p 门课和 n 个学生，每门课可能有 0 或多个学生选修，现在问能否找到一种选课方案，使得 p 门课每门都正好有一个学生选修，且任意两个选了课的学生所选课都不同

思路：由于只有课与学生间存在边，因此图可以看做左边有 p 个点，右边有 n 点的二分图，由于想让课与学生连的边最多且每个学生只能连一门课，每门课也只能有一个学生，因此可以看做是一个最大匹配问题，因此可以求图的最大匹配，看最大匹配边的数目是否等于 p，若相等则输出 YES，若不等则输出 NO

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
    int t;
    scanf("%d",&t);
    while(t--){
        int p,n;
        scanf("%d%d",&p,&n);

        memset(link,-1,sizeof(link));
        for(int i=0;i<N;i++)
            G[i].clear();

        bool flag=true;
        for(int i=1;i<=p;i++){
            int num;
            scanf("%d",&num);
            if(num){
                while(num--){
                    int j;
                    scanf("%d",&j);
                    G[i].push_back(j);
                }
            }
            else
                flag=false;
        }

        if(flag){
            int res=hungarian(p);
            if(res==p)
                printf("YES\n");
            else
                printf("NO\n");
        }
        else
            printf("NO\n");
    }

    return 0;
}
```






