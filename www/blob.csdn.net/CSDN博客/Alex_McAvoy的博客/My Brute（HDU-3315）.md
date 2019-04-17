# My Brute（HDU-3315） - Alex_McAvoy的博客 - CSDN博客





2018年12月10日 13:17:50[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：46
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Seaco is a beautiful girl and likes play a game called “My Brute”. Before Valentine’s Day, starvae and xingxing ask seaco if she wants to spend the Valentine’s Day with them, but seaco only can spend it with one of them. It’s hard to choose from the two excellent boys. So there will be a competition between starvae and xingxing. The competition is like the game “My Brute”.

Now starvae have n brutes named from S1 to Sn and xingxing’s brutes are named from X1 to Xn. A competition consists of n games. At the beginning, starvae's brute Si must versus xingxing’s brute Xi. But it’s hard for starvae to win the competition, so starvae can change his brutes’ order to win more games. For the starvae’s brute Si, if it wins the game, starvae can get Vi scores, but if it loses the game, starvae will lose Vi scores. Before the competition, starvae’s score is 0. Each brute can only play one game. After n games, if starvae’s score is larger than 0, we say starvae win the competition, otherwise starvae lose it. 

It’s your time to help starvae change the brutes’ order to make starvae’s final score be the largest. If there are multiple orders, you should choose the one whose order changes the least from the original one. The original order is S1, S2, S3 … Sn-1, Sn, while the final order is up to you.

For starvae’s brute Si (maybe this brute is not the original brute Si, it is the ith brute after you ordered them) and xingxing’s brute Xi, at first Si has Hi HP and Xi has Pi HP, Si’s damage is Ai and Xi’s is Bi, in other words, if Si attacks, Xi will lose Ai HP and if Xi attacks, Si will lose Bi HP, Si attacks first, then it’s Xi’s turn, then Si… until one of them’s HP is less than 0 or equal to 0, that, it lose the game, and the other win the game.

Come on, starvae’s happiness is in your hand!

# **Input**

First line is a number n. (1<=n<=90) Then follows a line with n numbers mean V1 to Vn. (0<Vi<1000) Then follows a line with n numbers mean H1 to Hn. (1<=Hi<=100)Then follows a line with n numbers mean P1 to Pn. (1<=Pi<=100) Then follows a line with n numbers mean A1 to An.(1<=Ai<=50) Then follows a line with n numbers mean B1 to Bn. (1<=Bi<=50) A zero signals the end of input and this test case is not to be processed.

# Output

For each test case, if starvae can win the competition, print the largest score starvae can get, and then follow a real percentage means the similarity between the original order and the final order you had changed, round it to three digits after the decimal point. If starvae can’t win the competition after changing the order, please just print “Oh, I lose my dear seaco!” Maybe the sample can help you get it.

# Sample Input

**34 5 66 8 1012 14 167 7 67 3 534 5 66 8 1012 14 165 5 55 5 50**

# Sample Output

**7 33.333%Oh, I lose my dear seaco!**


题意：有 S1-Sn 的 n 个勇士要与 X1-Xn 的 n 个勇士比赛，开始时，Si 的比赛对象是 Xi，若 Si 赢了 Xi，将获得 Vi 分，否则获得 -Vi 分，开始对决时，Si 有初始生命 Hi，初始攻击 Ai，Xi 有初始生命 Pi，初始攻击 Bi，Si 先出手，然后 Xi 失去 Ai 生命，若 Xi 未死，则 Xi 出手，Si 失去 Bi 生命，重复过程，直到有一方生命值小于等于 0 时，决斗结束

现在要重新安排 S、X 双方的决斗顺序，使可获取的分最多，输出可获得的最大值与原匹配与新匹配之间的相似性

思路：题目一开始给出的对决方式实质是一个匹配，现在要构造一个最优匹配，且要优先使用原边，套用模版进行计算即可，思路与[Assignment（HDU-2853）](https://blog.csdn.net/u011815404/article/details/84938003)很像，不同的是，边权值需要自己进行计算，对于左 i 到右 j 的权值，要用一个循环来判断哪边的生命值先为 0

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
int n;
int G[N][N];
int Lx[N],Ly[N];
bool visX[N],visY[N];
int linkX[N],linkY[N];
bool dfs(int x){
    visX[x]=true;
    for(int y=1;y<=n;y++){
        if(!visY[y]){
            int temp=Lx[x]+Ly[y]-G[x][y];
            if(temp==0){
                visY[y]=true;
                if(linkY[y]==-1 || dfs(linkY[y])){
                    linkX[x]=y;
                    linkY[y]=x;
                    return true;
                }
            }
        }
    }
    return false;
}
void update(){
    int minn=INF;
    for(int i=1;i<=n;i++)
        if(visX[i])
            for(int j=1;j<=n;j++)
                if(!visY[j])
                    minn=min(minn,Lx[i]+Ly[j]-G[i][j]);

    for(int i=1;i<=n;i++)
        if(visX[i])
            Lx[i]-=minn;

    for(int i=1;i<=n;i++)
        if(visY[i])
            Ly[i]+=minn;
}
int KM(){
    memset(linkX,-1,sizeof(linkX));
    memset(linkY,-1,sizeof(linkY));

    for(int i=1;i<=n;i++){
        Lx[i]=Ly[i]=0;
        for(int j=1;j<=n;j++)
            Lx[i]=max(Lx[i],G[i][j]);
    }

    for(int i=1;i<=n;i++){
        while(true){
            memset(visX,false,sizeof(visX));
            memset(visY,false,sizeof(visY));

            if(dfs(i))
                break;
            else
                update();
        }
    }

    int ans=0;
    for(int i=1;i<=n;i++)
        if(linkY[i]!=-1)
            ans+=G[linkY[i]][i];

    return ans;
}
int v[N],h[N],p[N],a[N],b[N];
int attack(int x,int y){
    int hpX=h[x],hpY=p[y];
    int vX=a[x],vY=b[y];
    while(hpX&&hpY){
        hpY-=vX;

        if(hpY<=0)
            return v[x];

        hpX-=vY;
        if(hpX<=0)
            return -v[x];
    }
}
int main(){
    while(scanf("%d",&n)!=EOF&&(n)){

        for(int i=1;i<=n;i++)
            scanf("%d",&v[i]);
        for(int i=1;i<=n;i++)
            scanf("%d",&h[i]);
        for(int i=1;i<=n;i++)
            scanf("%d",&p[i]);
        for(int i=1;i<=n;i++)
            scanf("%d",&a[i]);
        for(int i=1;i<=n;i++)
            scanf("%d",&b[i]);

        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                G[i][j]=attack(i,j);

        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                G[i][j]=G[i][j]*(n+1);
                if(i==j)
                    G[i][j]++;
            }
        }

        int ans=KM();
        int temp1=ans/(n+1);
        int temp2=ans%(n+1);
        if(temp1<=0)
            printf("Oh, I lose my dear seaco!\n");
        else
            printf("%d %.3lf%%\n",temp1,100*temp2*1.0/n);


    }
    return 0;
}
```






