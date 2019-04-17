# Ollivanders: Makers of Fine Wands since（HDU-1179） - Alex_McAvoy的博客 - CSDN博客





2019年03月12日 15:31:35[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：38








> 
# Problem Description

In Diagon Alley ,there is only one Wand-seller,peeling gold letters over the door read Ollivanders: Makers of Fine Wands since 382 BC.A single wand lay on a faded purple cushion in the dusty window. 

A tinkling bell rang somewhere in the depths of the shop as they stepped inside.It was a tiny place,empty execpt for a single spindly chair which Hagrid sat on to wait.Harry felt strangely as though he had entered a very strict library;he swallowd a log of new questions which had just occurred to him and looked instead at the thousands of narrow boxes piled neatly right up to the ceiling.For some reason,the back of his neck prickled.The very dust and silence in here seemed to tingle with some secret magic. 

'Good afternoon,'said a soft voice.Harry jumped.Hagrid must have jumped,too,because there was a loud crunching noise and he got quickly off the spindly chair. 

An old man was standing before them, his wide pale eyes shining like moons through the gloom of the shop. 

'Hello,' said Harry awkwardly. 

'Ah yes,' said the man. 'Yes,yes. I thought I'd be seeing you soon,Harry Potter.'It wasn't a question.You have your mother's eyes. It seems only yesterday she was in here herself,buying her first wand. Ten and a quarter inches long, swishy, made of willow. Nice wand for charm work.' 

Mor Ollivander moved closer to Harry.Harry wished he would blink.Those sivery eyes were a bit creepy. 

'Your father, on the other hand, favoured a mahogany wand.Eleven inches.Pliable.A little more power and excellent for transfiguration.Well ,I say your father favoured it - it's really the wand that choosed the wizard, of cource.' 

Yes, some wands fit some wizards ,as we all know.But what Harry doesn't know is Ollivander have met a big trouble.That's more than twenty years ago,When Harry's father James Potter was still a student in Hogwarts.He went Diagon Alley to buy new books,passing by Ollivander's shop.Ollivander was crazy for a problem:He was too busy to choose most suitable wand for every wizard.Even more,there are too many customer that day.Though Ollivader knew every wand's favourite,he could not choose as many wizards as possible to get the wands. So James Potter,a very clever man ,gave him a magic disk with your program ,to help him sell wands as many as possible. 

Please notice: one wand can only be sold to one wizard, and one wizard can only buy one wand,too. 

# **Input**

 There are several cases. For each case, there is two integers N and M in the first line,which mean there is N wizards and M wands(0 < N <= M <= 100). 

Then M lines contain the choices of each wand.The first integer in i+1th line is Ki,and after these there are Ki integers Bi,j which are the wizards who fit that wand. (0<=Ki<=N,1<=Bi,j<=N) 

# Output

Only one integer,shows how many wands Ollivander can sell. 

# Sample Input

**3 43 1 2 31 11 10**

# Sample Output

**2**


题意：给出 n 个巫师 m 个魔杖，并给出 m 行代表每个魔杖的选择，每一行首先给出一个 k 代表魔杖可以选择的人数，然后再给出 k 个数代表魔杖选择的对象，一个魔杖只能被卖给一个巫师，同时一个巫师只能选择一个魔杖，问最大可达成的交易数量

思路：最大匹配

由于魔杖、巫师只能是一对一的关系，而求的最大可达成的交易数量就是最大匹配数，根据巫师、魔杖的个数以及魔杖的选择来构建二分图，跑匈牙利算法即可

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
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        memset(link,-1,sizeof(link));
        for(int i=0;i<m;i++)
            G[i].clear();

        for(int x=0;x<m;x++){
            int k;
            scanf("%d",&k);
            while(k--){
                int y;
                scanf("%d",&y);
                G[x].push_back(y);
            }
        }

        printf("%d\n",hungarian());
    }
    return 0;
}
```





