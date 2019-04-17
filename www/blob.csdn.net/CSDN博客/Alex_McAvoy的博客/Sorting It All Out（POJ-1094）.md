# Sorting It All Out（POJ-1094） - Alex_McAvoy的博客 - CSDN博客





2019年02月23日 19:16:04[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：48
个人分类：[POJ																[图论——AOV网与拓扑排序](https://blog.csdn.net/u011815404/article/category/7813285)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

An ascending sorted sequence of distinct values is one in which some form of a less-than operator is used to order the elements from smallest to largest. For example, the sorted sequence A, B, C, D implies that A < B, B < C and C < D. in this problem, we will give you a set of relations of the form A < B and ask you to determine whether a sorted order has been specified or not.

# **Input**

Input consists of multiple problem instances. Each instance starts with a line containing two positive integers n and m. the first value indicated the number of objects to sort, where 2 <= n <= 26. The objects to be sorted will be the first n characters of the uppercase alphabet. The second value m indicates the number of relations of the form A < B which will be given in this problem instance. Next will be m lines, each containing one such relation consisting of three characters: an uppercase letter, the character "<" and a second uppercase letter. No letter will be outside the range of the first n letters of the alphabet. Values of n = m = 0 indicate end of input.

# Output

For each problem instance, output consists of one line. This line should be one of the following three: 

Sorted sequence determined after xxx relations: yyy...y. 

Sorted sequence cannot be determined. 

Inconsistency found after xxx relations. 

where xxx is the number of relations processed at the time either a sorted sequence is determined or an inconsistency is found, whichever comes first, and yyy...y is the sorted, ascending sequence. 

# Sample Input

**4 6A<BA<CB<CC<DB<DA<B3 2A<BB<A26 1A<Z0 0**

# Sample Output

**Sorted sequence determined after 4 relations: ABCD.Inconsistency found after 2 relations.Sorted sequence cannot be determined.**


题意：每组给定 n 个点，从 A 到 Z，再给定 m 组大小关系，判断这些字母能否组成唯一的拓扑序列

思路：拓扑排序

根据题意，首先判断是否有环，若有环，则一定不成立，然后再判断是否有序即可

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
#define N 30
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

int n,m;
bool G[N][N];
int in[N],temp[N];
int res[N];
int topSort(){
    int pos;
    int ans=1;//初始为1，默认有序，为-1时不能确定，为0时存在环

    for(int i=1;i<=n;i++)//备份
        temp[i]=in[i];

    int cnt=0;
    for(int i=1;i<=n;i++){
        int num=0;//零入度顶点的个数
        for(int j=1;j<=n;j++){//查找零入度点个数
            if(temp[j]==0){
                num++;
                pos=j;//记录一个零入度顶点的位置
            }
        }

        if(num==0){//零入度的点个数为0，有环
            ans=0;
            return ans;
        }

        if(num>1)//零入度的个数>1,说明其他点间未确定关系，此时仍为无序，需要继续遍历
            ans=-1;

        res[cnt++]=pos;//零入度点按顺序存入结果
        temp[pos]=-1;

        for (int j=1;j<=n;j++)//删除以pos为起点的边
            if(G[pos][j])
                temp[j]--;
    }
    return ans;
}

int main()
{
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        memset(G,false,sizeof(G));
        memset(in,0,sizeof(in));

        bool flag=false;
        for(int i=1;i<=m;i++){
            string str;
            cin>>str;
            if(flag)
                continue;

            int x=str[0]-'A'+1;
            int y=str[2]-'A'+1;
            G[x][y]=true;
            in[y]++;

            int num=topSort();
            //cout<<num<<endl;
            if(num==0){//存在环
                printf("Inconsistency found after %d relations.\n",i);
                flag=true;
            }
            else if(num==1){
                printf("Sorted sequence determined after %d relations: ",i);
                for(int j=0; j<n; j++) //按顺序输出字符
                    printf("%c",res[j]+'A'-1);
                printf(".\n");
                flag=true;
            }
        }
        if(!flag)//无结果
            printf("Sorted sequence cannot be determined.\n");

    }
    return 0;
}
```






