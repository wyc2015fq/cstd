# The Suspects（POJ-1611） - Alex_McAvoy的博客 - CSDN博客





2019年02月20日 17:53:58[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：35








> 
# Problem Description

Severe acute respiratory syndrome (SARS), an atypical pneumonia of unknown aetiology, was recognized as a global threat in mid-March 2003. To minimize transmission to others, the best strategy is to separate the suspects from others. 

In the Not-Spreading-Your-Sickness University (NSYSU), there are many student groups. Students in the same group intercommunicate with each other frequently, and a student may join several groups. To prevent the possible transmissions of SARS, the NSYSU collects the member lists of all student groups, and makes the following rule in their standard operation procedure (SOP). 

Once a member in a group is a suspect, all members in the group are suspects. 

However, they find that it is not easy to identify all the suspects when a student is recognized as a suspect. Your job is to write a program which finds all the suspects.

# Input

The input file contains several cases. Each test case begins with two integers n and m in a line, where n is the number of students, and m is the number of groups. You may assume that 0 < n <= 30000 and 0 <= m <= 500. Every student is numbered by a unique integer between 0 and n−1, and initially student 0 is recognized as a suspect in all the cases. This line is followed by m member lists of the groups, one line per group. Each line begins with an integer k by itself representing the number of members in the group. Following the number of members, there are k integers representing the students in this group. All the integers in a line are separated by at least one space. 

A case with n = 0 and m = 0 indicates the end of the input, and need not be processed.

# Output

For each case, output the number of suspects in one line.

# **Sample Input**

**100 42 1 25 10 13 11 12 142 0 12 99 2200 21 55 1 2 3 4 51 00 0**

# Sample Output

**411**


题意：有 n 个学生，m 个小组数，学生编号从 0 到 n-1，如果一组中的 0 号学生患病，那么整个组的学生都需要隔离，问需要隔离的人数

思路：并查集水题

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
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define N 50001
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
int father[N];
int a[N],num[N];
int Find(int x){
    if(x==father[x])
        return x;
    return father[x]=Find(father[x]);
}
void Union(int x,int y){
    x=Find(x);
    y=Find(y);
    if(x!=y){
        father[x]=y;
        num[y]+=num[x];
    }
}
int main(){
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        for(int i=0;i<n;i++){
            father[i]=i;
            num[i]=1;
        }

        while(m--){
            int t;
            scanf("%d",&t);
            for(int i=0;i<t;i++)
                scanf("%d",&a[i]);
            for(int i=0;i<t-1;i++)
                Union(a[i],a[i+1]);
        }
        int res=Find(0);
        printf("%d\n",num[res]);
    }
    return 0;
}
```





