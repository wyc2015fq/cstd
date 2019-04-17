# 第十四届华中科技大学程序设计竞赛J——Various Tree - westbrook1998的博客 - CSDN博客





2018年04月29日 21:07:42[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：109








嗯…..基础算法不扎实，有想到并画出树的搜索的，但是……

题面：

> 
It’s universally acknowledged that there’re innumerable trees in the campus of HUST. 

  And there are many different types of trees in HUST, each of which has a number represent its type. The doctors of biology in HUST find 4 different ways to change the tree’s type x into a new type y: 

  1.    y=x+1 

  2.    y=x-1 

  3.    y=x+f(x) 

  4.    y=x-f(x) 

  The function f(x) is defined as the number of 1 in x in binary representation. For example, f(1)=1, f(2)=1, f(3)=2, f(10)=2. 

  Now the doctors are given a tree of the type A. The doctors want to change its type into B. Because each step will cost a huge amount of money, you need to help them figure out the minimum steps to change the type of the tree into B.  

  Remember the type number should always be a natural number (0 included).
题意就是说输入两个数a,b，求a到b的转换次数，有四种类型的转换 

bfs（四种类型的转换，四个树的分支）加dp（用一个一维数组记录各个数到b的转换次数，广搜过程中更新数组）

代码：

```cpp
#include <cstdio>
#include <cstring>
#include <queue>
#include <algorithm>
using namespace std;
const int maxn=1000000+500;
const int INF = 0x3f3f3f3f;
bool vis[maxn];
int ans[maxn];
queue<int> q;
int a,b;
//求二进制中1的个数
int f(int x){
    int n=0;
    while(x){
        n+=(x%2);
        x/=2;
    }
    return n;
}
//广度优先搜索
void bfs(){
    //初始化ans数组 ans[i]（i>a）表示a到i需要转换的次数
    for(int i=0;i<maxn;i++) ans[i]=INF;
    //a到a无需转换
    ans[a]=0;
    //搜索标记数组
    memset(vis,0,sizeof(vis));
    //从a开始搜索，cur表示当前访问节点
    int cur=a;
    //标记已访问
    vis[a]=1;
    //bfs
    q.push(cur);
    while(!q.empty()){
        cur=q.front();
        q.pop();
        int tmp[4]={cur+1,cur-1,cur+f(cur),cur-f(cur)};
        for(int i=0;i<4;i++){
            //转换之后更小，远离目标数字，不可取
            if(tmp[i]<0) continue;
            //tmp[i]为转换后的数字（暂时），如果未标记，则搜索
            if(!vis[tmp[i]]){
                vis[tmp[i]]=1;
                q.push(tmp[i]);
                //到tmp[i]的转换次数比到cur的转换次数多1,（从cur转换到tmp）
                ans[tmp[i]]=ans[cur]+1;
            }
            //如果已经转换到b，那么结束搜索
            if(tmp[i]==b) return;
        }
    }
    return;
}
int main(){
    scanf("%d%d",&a,&b);
    bfs();
    //输出ans[b]代表a到b需要转换的次数
    printf("%d\n",ans[b]);
}
```





