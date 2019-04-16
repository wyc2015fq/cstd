# LightOJ 1401 No More Tic-tac-toe 博弈论SG打表 - 紫芝的博客 - CSDN博客





2018年11月13日 18:05:41[紫芝](https://me.csdn.net/qq_40507857)阅读数：503








### 题意

玩家和电脑轮流在1*N的矩形内放置棋子，每次只能放一个棋子，相同的棋子不能相邻 ，玩家先手，最后没有合法棋子可放的一方败；现在给你一个残局状态，若玩家获胜，输出yes，否则输出No；

### 分析

首先，题目给定一个残局，要判断下一步轮谁下棋；因为每次只能放一个棋子，所以棋子个数是奇数，轮到电脑下棋；

其次，要判断当前残局是否是奇异局势；无论谁遇到奇异局势，都是必败的；

SG打表

用一个三维数组表示sg函数：sg[n][i][j]

![sg](https://img-blog.csdnimg.cn/20181112214532122.png)

用 i / j =0表示表示点 "." ,i / j =1表示棋子O,i/j=2表示棋子X，sg[3][1][2]代表中间有三个空的格子，这些格子的左边是O，右边是X

对于方格个数为N的情况，可以将棋子X和棋子O放到方格1~N的任意一个格子处，每次分成左右两堆格子，由于他们是独立的左右两堆格子，因此可以异或得出分开后的sg值，最后找出没有出现过的非负整数即可

暴力打表竟然过了

```cpp
#include <cstdio>
#include <cstring>
#include <queue>
#include <algorithm>
using namespace std;
typedef long long LL;
const int maxn=109;
#define INF 0x3f3f3f3f
int sg[maxn][3][3];
bool vis[maxn];
char s[maxn];

void SG(){
    for(int i=1;i<maxn;i++){//枚举区间长度
        for(int j=0;j<3;j++){//枚举左端点
            for(int k=0;k<3;k++){//枚举右端点
                memset(vis,0,sizeof vis);//清空标记

                for(int pos=1;pos<=i;pos++){//枚举区间断点
                    if(!((pos==1&&j==1)||(pos==i&&k==1))){
                        vis[sg[pos-1][j][1]^sg[i-pos][1][k]]=1;
                    }

                    if(!((pos==1&&j==2)||(i==pos&&k==2))){
                        vis[sg[pos-1][j][2]^sg[i-pos][2][k]]=1;
                    }
                }
                //不属于这个集合的最小非负正整数
                for(int t=0;;t++)
                    if(!vis[t]){
                    sg[i][j][k]=t;
                    break;
                }

            }//k
        }//j
    }//i
}
int main(){
    int T;
    scanf("%d",&T);
    int ca=1;
    SG();
    
    while(T--){
        scanf("%s",s);
        int len=strlen(s);
       int num=0,last=0,lc=0,ans=0;
        for(int i=0;i<len;i++){
            if(s[i]!='.'){
                num++;
                int nowc=(s[i]=='O')?1:2;
                ans^=sg[i-last][lc][nowc];
                lc=nowc;last=i+1;
            }
        }
        ans^=sg[len-last][lc][0];
        printf("Case %d: ",ca++);
        if(num&1){//奇数，电脑面对的局势
            if(ans==0)  printf("Yes\n");
            else        printf("No\n");
        }else{//偶数，玩家面对的局势
            if(ans==0)  printf("No\n");
            else        printf("Yes\n");
        }
    }
}
```

打表代码

```cpp
#include <cstdio>
#include <cstring>
#include <queue>
#include <algorithm>
using namespace std;
typedef long long LL;
const int maxn=109;
#define INF 0x3f3f3f3f
int sg[maxn][3][3];
bool vis[maxn];
char s[maxn];

void SG(){
    for(int i=1;i<maxn;i++){//枚举长度
        for(int j=0;j<3;j++){
            for(int k=0;k<3;k++){
                memset(vis,0,sizeof vis);
                for(int pos=1;pos<=i;pos++){//
                    if(!((pos==1&&j==1)||(pos==i&&k==1))){
                        vis[sg[pos-1][j][1]^sg[i-pos][1][k]]=1;
                    }
                    if(!((pos==1&&j==2)||(i==pos&&k==2))){
                        vis[sg[pos-1][j][2]^sg[i-pos][2][k]]=1;
                    }
                }
                for(int t=0;;t++)
                    if(!vis[t]){
                    sg[i][j][k]=t;
                    break;
                }

            }//k
        }//j
    }//i
}
int main(){
    SG();
    printf("............\n");
    for(int i=1;i<100;i++){
        printf("%d,",sg[i][0][0]);
    }
    printf("\n");

    printf("O...........\n");
     for(int i=1;i<100;i++){
        printf("%d,",sg[i][1][0]);
    }
    printf("\n");

    printf("...........O\n");
     for(int i=1;i<100;i++){
        printf("%d,",sg[i][0][1]);
    }
    printf("\n");

    printf("...........X\n");
     for(int i=1;i<100;i++){
        printf("%d,",sg[i][0][2]);
    }
    printf("\n");

    printf("X...........\n");
     for(int i=1;i<100;i++){
        printf("%d,",sg[i][2][0]);
    }
    printf("\n");

    printf("O.........X\n");
     for(int i=1;i<50;i++){
        printf("%d,",sg[i][1][2]);
    }
    printf("\n");

    printf("X..........O\n");
     for(int i=1;i<50;i++){
        printf("%d,",sg[i][2][1]);
    }
    printf("\n");

}
```



![](https://img-blog.csdnimg.cn/20181114111626787.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)

AC代码

```cpp
#include <cstdio>
#include <cstring>
#include <queue>
#include <algorithm>
using namespace std;
typedef long long LL;
const int maxn=109;
#define INF 0x3f3f3f3f
char s[maxn];
int SG(int len,int left,int right){
    if(left==0&&right==0)
        return (len&1);

    if(left>0&&right==0||left==0&&right>0)
        return len;

    if(left>0&&right>0)
        return 0;
}
int main(){
    int T;
    scanf("%d",&T);
    int ca=1;
    while(T--){
        scanf("%s",s);
        int len=strlen(s);
        int num=0,last=0,lc=0,ans=0;
        for(int i=0;i<len;i++){
            if(s[i]!='.'){
                num++;
                int nowc=(s[i]=='O')?1:2;
                //ans^=sg[i-last][lc][nowc];
                ans^=SG(i-last,lc,nowc);
                lc=nowc;last=i+1;
            }
        }
        //ans^=sg[len-last][lc][0];
        ans^=SG(len-last,lc,0);

        printf("Case %d: ",ca++);
        if(num&1){//奇数，电脑面对的局势
            if(ans==0)  printf("Yes\n");
            else        printf("No\n");
        }else{//偶数，玩家面对的局势
            if(ans==0)  printf("No\n");
            else        printf("Yes\n");
        }
    }
    return 0;
}
```





