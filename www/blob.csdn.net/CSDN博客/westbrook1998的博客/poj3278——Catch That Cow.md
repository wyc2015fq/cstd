# poj3278——Catch That Cow - westbrook1998的博客 - CSDN博客





2018年06月11日 23:01:19[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：30








> 
Farmer John has been informed of the location of a fugitive cow and wants to catch her immediately. He starts at a point N (0 ≤ N ≤ 100,000) on a number line and the cow is at a point K (0 ≤ K ≤ 100,000) on the same number line. Farmer John has two modes of transportation: walking and teleporting. 

  * Walking: FJ can move from any point X to the points X - 1 or X + 1 in a single minute 

  * Teleporting: FJ can move from any point X to the point 2 × X in a single minute. 

  If the cow, unaware of its pursuit, does not move at all, how long does it take for Farmer John to retrieve it? 

  Input 

  Line 1: Two space-separated integers: N and K 

  Output 

  Line 1: The least amount of time, in minutes, it takes for Farmer John to catch the fugitive cow. 

  Sample Input 

  5 17 

  Sample Output 

  4 

  Hint 

  The fastest way for Farmer John to reach the fugitive cow is to move along the following path: 5-10-9-18-17, which takes 4 minutes.
看了题解才知道用bfs，刚开始用dfs一直不行… 

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <queue>
#include <cstring>
using namespace std;
const int MAXN=100005;
queue<int> q;
int step[MAXN];
bool vis[MAXN];
int head;
int next;
int n;
int k;
//队列实现广搜
int bfs(){
    //第一个节点入队
    q.push(n);
    step[n]=0;
    vis[n]=true;
    while(!q.empty()){
            //取出队首
            head=q.front();
            q.pop();
            //三种走的方式
            for(int i=0;i<3;i++){
                if(i==0){
                    next=head-1;
                }
                else if(i==1){
                    next=head+1;
                }
                else{
                    next=head*2;
                }
                if(next<0 || next>=MAXN){
                    continue;
                }
                //该点还没访问过，入队
                if(!vis[next]){
                    q.push(next);
                    //多了一步
                    step[next]=step[head]+1;
                    vis[next]=true;
                }
                //走到奶牛处
                if(next==k){
                    return step[next];
                }
            }
        }
}
int main(void){
    while(~scanf("%d%d",&n,&k)){
        memset(step,0,sizeof(step));
        memset(vis,false,sizeof(vis));
        while(!q.empty()){
            q.pop();
        }
        //只能一步一步往回走
        if(n>=k){
            printf("%d\n",n-k);
        }
        else{
            printf("%d\n",bfs());
        }
    }
    return 0;
}
```





