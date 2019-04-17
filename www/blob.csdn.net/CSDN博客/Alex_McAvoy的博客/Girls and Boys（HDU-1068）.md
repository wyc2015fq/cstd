# Girls and Boys（HDU-1068） - Alex_McAvoy的博客 - CSDN博客





2019年03月11日 21:34:37[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：22
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

In the second year of the university somebody started a study on the romantic relations between the students. The relation "romantically involved" is defined between one girl and one boy. For the study reasons it is necessary to find out the maximum set satisfying the condition: there are no two students in the set who have been "romantically involved". The result of the program is the number of students in such a set.

# **Input**

The input contains several data sets in text format. Each data set represents one set of subjects of the study, with the following description: 

the number of students 

the description of each student, in the following format 

student_identifier:(number_of_romantic_relations) student_identifier1 student_identifier2 student_identifier3 ... 

or 

student_identifier:(0) 

The student_identifier is an integer number between 0 and n-1 (n <=500 ), for n subjects.

# Output

For each given data set, the program should write to standard output a line containing the result.

# Sample Input

**70: (3) 4 5 61: (2) 4 62: (0)3: (0)4: (2) 0 15: (1) 06: (2) 0 130: (2) 1 21: (1) 02: (1) 0**

# Sample Output

**52**


题意：在一群男女生之间存在浪漫关系，且只存在于男女之间，现给出存在关系的人的编号，但没给具体的男女性别，现在要求一个集合，在集合中任意两人都不存浪漫关系，输出集合中元素的个数

思路：根据题意，关系彼此是对应的，但由于没有给出具体男女性别，因此可以在两边同时建图，统计最大匹配数，最后算出的最大匹配数除以 2 就是实际图的最大匹配数，再求最大独立集即可

![](https://img-blog.csdnimg.cn/20181129235906244.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

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
#define LL long long
const int MOD=1E9+7;
const int N=10000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

int n,m;//x、y中结点个数，下标从0开始
bool vis[N];//vis[i]表示是否在交替路中
int link[N];//存储连接点
vector<int> G[N];//存边
bool dfs(int x){
    for(int i=0;i<G[x].size();i++){
        int y=G[x][i];
        if(!vis[y]){//不在交替路中
            vis[y]=true;//放入交替路
            if(link[y]==-1 || dfs(link[y])){//如果是未匹配点，说明交替路是增广路
                link[y]=x;//交换路径
                return true;//返回成功
            }
        }
    }
    return false;//不存在增广路，返回失败
}
int hungarian(){
    int ans=0;//记录最大匹配数
    for(int i=0;i<n;i++){//从左侧开始每个结点找一次增广路
        memset(vis,false,sizeof(vis));
        if(dfs(i))//找到一条增广路，形成一个新匹配
            ans++;
    }
    return ans;
}
int main(){
    while(scanf("%d",&n)!=EOF&&(n)){
        memset(link,-1,sizeof(link));
        for(int i=0;i<n;i++)
            G[i].clear();

        for(int i=0;i<n;i++){
            int x,num;
            scanf("%d: (%d)",&x,&num);
            while(num--){
                int y;
                scanf("%d",&y);
                G[x].push_back(y);
            }
        }

        printf("%d\n",n-hungarian()/2);//注意除以2
    }
    return 0;
}
```




