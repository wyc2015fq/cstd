# Guardian of Decency（POJ-2771） - Alex_McAvoy的博客 - CSDN博客





2018年11月29日 22:42:49[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：98








> 
# Problem Description

Frank N. Stein is a very conservative high-school teacher. He wants to take some of his students on an excursion, but he is afraid that some of them might become couples. While you can never exclude this possibility, he has made some rules that he thinks indicates a low probability two persons will become a couple: 

Their height differs by more than 40 cm. 

They are of the same sex. 

Their preferred music style is different. 

Their favourite sport is the same (they are likely to be fans of different teams and that would result in fighting).

So, for any two persons that he brings on the excursion, they must satisfy at least one of the requirements above. Help him find the maximum number of persons he can take, given their vital information. 

# **Input**

The first line of the input consists of an integer T ≤ 100 giving the number of test cases. The first line of each test case consists of an integer N ≤ 500 giving the number of pupils. Next there will be one line for each pupil consisting of four space-separated data items: 

an integer h giving the height in cm; 

a character 'F' for female or 'M' for male; 

a string describing the preferred music style; 

a string with the name of the favourite sport.

No string in the input will contain more than 100 characters, nor will any string contain any whitespace. 

# Output

For each test case in the input there should be one line with an integer giving the maximum number of eligible pupils.

# Sample Input

**2435 M classicism programming0 M baroque skiing43 M baroque chess30 F baroque soccer827 M romance programming194 F baroque programming67 M baroque ping-pong51 M classicism programming80 M classicism Paintball35 M baroque ping-pong39 F romance ping-pong110 M romance Paintball**

# Sample Output

**37**


题意：老师带学生去旅游，但是担心学生会发生恋爱关系，因此带出去的学生彼此间要满足以下条件中的一个：身高相差 40cm 以上、同性、喜欢音乐风格不同、喜欢运动相同，求最多能带多少学生出去

思路：左点集表示男同学，右点集表示女同学，对于任意两个学生，如果他们不满足上述条件中的任一个，就在他们间连一条边，从而构建一个二分图，求出该图中的最大独立集即可

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
#define N 1001
#define LL long long
using namespace std;
int n,m;
bool vis[N];
int link[N];
bool G[N][N];
bool dfs(int x){
    for(int y=1;y<=m;y++){
        if(G[x][y]&&!vis[y]){
            vis[y]=true;
            if(link[y]==-1 || dfs(link[y]))	{
                link[y]=x;
                return true;
            }
        }
    }
    return false;
}
int hungarian(){
    int ans=0;
    for(int i=1;i<=n;i++){
        memset(vis,false,sizeof(vis));
        if(dfs(i))
            ans++;
    }
    return ans;
}
struct Node{
    int height;
    string music;
    string sport;
    Node(){}
    Node(int height,string& s1,string& s2):height(height),music(s1),sport(s2){}
    bool buildEdge(Node &rhs){
        return (abs(height-rhs.height)<=40 && music==rhs.music && sport!=rhs.sport);
    }
}boy[N],girl[N];
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        memset(link,-1,sizeof(link));
        memset(G,false,sizeof(G));
        n=m=0;

        int num;
        scanf("%d",&num);
        for(int i=0;i<num;i++){
            int height;
            string sex;
            string music;
            string sport;
            cin>>height>>sex>>music>>sport;
            if(sex[0]=='M')
                boy[++n]=Node(height,music,sport);
            else
                girl[++m]=Node(height,music,sport);
        }

        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)
                if(boy[i].buildEdge(girl[j]))
                    G[i][j]=true;

        printf("%d\n",num-hungarian());
    }
    return 0;
}
```





