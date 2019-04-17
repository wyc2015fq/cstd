# Card Game Cheater（HDU-1528） - Alex_McAvoy的博客 - CSDN博客





2019年03月13日 08:59:27[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：60
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Adam and Eve play a card game using a regular deck of 52 cards. The rules are simple. The players sit on opposite sides of a table, facing each other. Each player gets k cards from the deck and, after looking at them, places the cards face down in a row on the table. Adam’s cards are numbered from 1 to k from his left, and Eve’s cards are numbered 1 to k from her right (so Eve’s i:th card is opposite Adam’s i:th card). The cards are turned face up, and points are awarded as follows (for each i ∈ {1, . . . , k}):

If Adam’s i:th card beats Eve’s i:th card, then Adam gets one point.

If Eve’s i:th card beats Adam’s i:th card, then Eve gets one point.

A card with higher value always beats a card with a lower value: a three beats a two, a four beats a three and a two, etc. An ace beats every card except (possibly) another ace.

If the two i:th cards have the same value, then the suit determines who wins: hearts beats all other suits, spades beats all suits except hearts, diamond beats only clubs, and clubs does not beat any suit. 

For example, the ten of spades beats the ten of diamonds but not the Jack of clubs. This ought to be a game of chance, but lately Eve is winning most of the time, and the reason is that she has started to use marked cards. In other words, she knows which cards Adam has on the table before he turns them face up. Using this information she orders her own cards so that she gets as many points as possible.

Your task is to, given Adam’s and Eve’s cards, determine how many points Eve will get if she plays optimally. 

# **Input**

There will be several test cases. The first line of input will contain a single positive integer N giving the number of test cases. After that line follow the test cases.

Each test case starts with a line with a single positive integer k <= 26 which is the number of cards each player gets. The next line describes the k cards Adam has placed on the table, left to right. The next line describes the k cards Eve has (but she has not yet placed them on the table). A card is described by two characters, the first one being its value (2, 3, 4, 5, 6, 7, 8 ,9, T, J, Q, K, or A), and the second one being its suit (C, D, S, or H). Cards are separated by white spaces. So if Adam’s cards are the ten of clubs, the two of hearts, and the Jack of diamonds, that could be described by the line

TC 2H JD

# Output

For each test case output a single line with the number of points Eve gets if she picks the optimal way to arrange her cards on the table.

# Sample Input

**31JDJH25D TC4C 5H32H 3H 4H2D 3D 4D**

# Sample Output

**112**


题意：两个人打牌，Adam 已经将牌按顺序放好，Eve 可以看到 Adam 的牌，所以 Eve 可以优化自己的牌的顺序，使得自己每个位置的牌赢 Adam 位置上的牌的个数最多，每张牌由 值+类型 构成，值按照 2, 3, 4, 5, 6, 7, 8 ,9, T, J, Q, K, A 的顺序依次增大，类型按照  C, D, S, H 的顺序依次增大，一张牌赢拿一分，平局或输掉不拿分，求最多能拿几分

思路：

可将两个人的牌构建一个二分图，根据**贪心**的思想，如果左边 i 牌 > 右边 j 牌，则连一条 <i,j> 边，由于要获得最大得分，每条匹配边就是一分，因此本题实质上就是找**图的最大匹配**

由于牌是数字+字母混合组成，因此两张牌的比较十分麻烦，可以考虑在读取数据的时候可以将每张牌转换成不同的大小，这样每张牌就有一个独特的分数，比较两张牌的大小就转换为比较两张牌的分数

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
const int N=1000+5;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
int n;
bool vis[N];//vis[i]表示是否在交替路中
int link[N];//存储连接点
vector<int> G[N];//存边
bool dfs(int x){
    for(int i=0;i<G[x].size();i++){//对x的每个邻接点
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
    memset(link,-1,sizeof(link));
    for(int i=1;i<=n;i++){//从左侧开始每个结点找一次增广路
        memset(vis,false,sizeof(vis));
        if(dfs(i))//找到一条增广路，形成一个新匹配
            ans++;
    }
    return ans;
}
int getScore(string str){//将牌转换为分数
    int ans=0;

    if(str[0]>='2'&&str[0]<='9')
        ans+=(str[0]-'0')*10;
    else if(str[0]=='T')
        ans+=100;
    else if(str[0]=='J')
        ans+=110;
    else if(str[0]=='Q')
        ans+=120;
    else if(str[0]=='K')
        ans+=130;
    else if(str[0]=='A')
        ans+=140;

    if(str[1]=='C')
        ans+=1;
    else if(str[1]=='D')
        ans+=2;
    else if(str[1]=='S')
        ans+=3;
    else if(str[1]=='H')
        ans+=4;

    return ans;
}
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        for(int i=0;i<N;i++)
            G[i].clear();

        scanf("%d",&n);
        string str;
        int scoreAdam[2000],scoreEve[2000];
        for(int i=1;i<=n;i++){//eve
            cin>>str;
            scoreAdam[i]=getScore(str);
        }
        for(int i=1;i<=n;i++){//adam
            cin>>str;
            scoreEve[i]=getScore(str);
        }

        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                if(scoreEve[i]>scoreAdam[j])//eve的牌大于adam
                    G[i].push_back(j);//建边

        printf("%d\n",hungarian());
    }
    return 0;
}
```






