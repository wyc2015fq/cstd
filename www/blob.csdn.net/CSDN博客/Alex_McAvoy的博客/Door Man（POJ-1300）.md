# Door Man（POJ-1300） - Alex_McAvoy的博客 - CSDN博客





2018年11月02日 21:29:29[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：71








> 
# Problem Description

You are a butler in a large mansion. This mansion has so many rooms that they are merely referred to by number (room 0, 1, 2, 3, etc...). Your master is a particularly absent-minded lout and continually leaves doors open throughout a particular floor of the house. Over the years, you have mastered the art of traveling in a single path through the sloppy rooms and closing the doors behind you. Your biggest problem is determining whether it is possible to find a path through the sloppy rooms where you: 

Always shut open doors behind you immediately after passing through 

Never open a closed door 

End up in your chambers (room 0) with all doors closed 

In this problem, you are given a list of rooms and open doors between them (along with a starting room). It is not needed to determine a route, only if one is possible. 

# **Input**

Input to this problem will consist of a (non-empty) series of up to 100 data sets. Each data set will be formatted according to the following description, and there will be no blank lines separating data sets. 

A single data set has 3 components: 

Start line - A single line, "START M N", where M indicates the butler's starting room, and N indicates the number of rooms in the house (1 <= N <= 20). 

Room list - A series of N lines. Each line lists, for a single room, every open door that leads to a room of higher number. For example, if room 3 had open doors to rooms 1, 5, and 7, the line for room 3 would read "5 7". The first line in the list represents room 0. The second line represents room 1, and so on until the last line, which represents room (N - 1). It is possible for lines to be empty (in particular, the last line will always be empty since it is the highest numbered room). On each line, the adjacent rooms are always listed in ascending order. It is possible for rooms to be connected by multiple doors! 

End line - A single line, "END" 

Following the final data set will be a single line, "ENDOFINPUT". 

Note that there will be no more than 100 doors in any single data set.

# Output

For each data set, there will be exactly one line of output. If it is possible for the butler (by following the rules in the introduction) to walk into his chambers and close the final open door behind him, print a line "YES X", where X is the number of doors he closed. Otherwise, print "NO".

# Sample Input

**START 1 21**

**ENDSTART 0 51 2 2 3 3 4 4**


**ENDSTART 0 101 923456789**

**ENDENDOFINPUT**

# Sample Output

**YES 1NOYES 10**


题意：给出 n 个房间以及房间之间的门，再给出初始房间 m，要求从 m 开始走，能否每个房间只经过一次，而且最后到达 0 号房间，若可以则输出 YES 以及要走的房间数

思路：题实质是要求一个图中是否具有欧拉回路或欧拉通路

首先并查集判断图是否连通，再判断所有点的度数是否都为偶数，若都为偶数，则说明存在欧拉回路，如果所有点中只有两个点的度数为奇数，且一个点为 m 一个点为 0，则说明存在欧拉通路。

当存在欧拉路时，需要输出从 m 到 0 走过的房间数，实质是求走过的边数，即：边数=总度数/2

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1001
#define MOD 16007
#define E 1e-6
#define LL long long
using namespace std;
int n,m;
int degree[N];
int father[N];
int Find(int x){
    if(father[x]==-1)
        return x;
    return father[x]=Find(father[x]);
}
void Union(int x,int y){
    x=Find(x);
    y=Find(y);
    if(x!=y)
        father[x]=y;
}
int main(){
    char str[200];
    while(scanf("%s",str)==1 && str[0]=='S'){
        memset(degree,0,sizeof(degree));
        memset(father,-1,sizeof(father));

        scanf("%d%d",&m,&n);
        getchar();

        int sum=0;//总度数
        for(int i=0;i<n;i++){
            gets(str);
            int len=strlen(str);
            if(len==0)
                continue;

            int num=0;
            for(int j=0;j<=len;j++){
                if(str[j]==' ' || str[j]==0){
                    Union(i,num);

                    sum+=2;
                    degree[i]++;
                    degree[num]++;

                    num=0;
                }
                else{
                    num=num*10+str[j]-'0';
                }
            }
        }

        gets(str);
        if(Find(0)!=Find(m)){//0点与m点不通
            printf("NO\n");
            continue;
        }

        int cnt=0;//记录连通分量
        for(int i=0;i<n;i++)
            if(degree[i])
                cnt+=Find(i)==i?1:0;
        if(cnt>1){//图不连通
            printf("NO\n");
            continue;
        }

        int res=0;//记录奇数度的点个数
        for(int i=0;i<n;i++)
            res+=degree[i]%2==1?1:0;
        if(res==0 && m==0)//欧拉回路
            printf("YES %d\n",sum/2);
        else if(res==2 && m!=0 && degree[0]%2==1 && degree[m]%2==1)//欧拉通路
            printf("YES %d\n",sum/2);
        else//其他非法情况
            printf("NO\n");

    }
    return 0;
}
```





