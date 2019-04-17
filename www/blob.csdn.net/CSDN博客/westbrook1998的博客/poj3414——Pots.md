# poj3414——Pots - westbrook1998的博客 - CSDN博客





2018年07月04日 21:26:09[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：32标签：[BFS																[搜索																[路径输出](https://so.csdn.net/so/search/s.do?q=路径输出&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)





> 
You are given two pots, having the volume of A and B liters respectively. The following operations can be performed: 

  FILL(i)        fill the pot i (1 ≤ i ≤ 2) from the tap; 

  DROP(i)      empty the pot i to the drain; 

  POUR(i,j)    pour from pot i to pot j; after this operation either the pot j is full (and there may be some water left in the pot i), or the pot i is empty (and all its contents have been moved to the pot j). 

  Write a program to find the shortest possible sequence of these operations that will yield exactly C liters of water in one of the pots. 

  Input 

  On the first and only line are the numbers A, B, and C. These are all integers in the range from 1 to 100 and C≤max(A,B). 

  Output 

  The first line of the output must contain the length of the sequence of operations K. The following K lines must each describe one operation. If there are several sequences of minimal length, output any one of them. If the desired result can’t be achieved, the first and only line of the file must contain the word ‘impossible’. 

  Sample Input 

  3 5 4 

  Sample Output 

  6 

  FILL(2) 

  POUR(2,1) 

  DROP(1) 

  POUR(2,1) 

  FILL(2) 

  POUR(2,1)
也不知道什么时候才能自己独立做出一道题来…

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <queue>
#include <cstring>
using namespace std;
const int MAXN=150;
int A,B,C;
//vis[i][j]保存初始状态到状态(i,j)所需要的步数
int vis[MAXN][MAXN];
//ans[i][j][k]保存初始状态到状态(i,j)的第k步的操作(0-6)
int ans[MAXN][MAXN][MAXN];
struct node{
    int a,b;
    node(int _a,int _b):a(_a),b(_b){}
};
//六种操作
//打印路径
void printop(int op){
    switch(op){
        case 0:{
            puts("FILL(1)");
            break;
        }
        case 1:{
            puts("FILL(2)");
            break;
        }
        case 2:{
            puts("POUR(1,2)");
            break;
        }
        case 3:{
            puts("POUR(2,1)");
            break;
        }
        case 4:{
            puts("DROP(1)");
            break;
        }
        case 5:{
            puts("DROP(2)");
            break;
        }
    }
}
//a瓶b瓶现在的水容量 操作
void op(int &a,int &b,int op){
    //实际操作
    switch(op){
        //将a装满为A
        case 0:{
            a=A;
            break;
        }
        case 1:{
            b=B;
            break;
        }
        case 2:{
            if(b+a<=B){
                b+=a;
                a=0;
            }
            else{
                a-=(B-b);
                b=B;
            }
            break;
        }
        case 3:{
            if(b+a<=A){
                a+=b;
                b=0;
            }
            else{
                b-=(A-a);
                a=A;
            }
            break;
        }
        case 4:{
            a=0;
            break;
        }
        case 5:{
            b=0;
            break;
        }
    }
}
void bfs(){
    queue<node> que;
    que.push(node(0,0));
    vis[0][0]=0;
    while(!que.empty()){
        node tmp=que.front();
        que.pop();
        int ta=tmp.a;
        int tb=tmp.b;
        //只要有一个瓶子里面的水达到C
        if(tmp.a==C || tmp.b==C){
            //该状态可以满足C 所以输出vis[该状态]
            printf("%d\n",vis[tmp.a][tmp.b]);
            //循环输出路径
            for(int i=0;i<vis[ta][tb];i++){
                int op=ans[ta][tb][i];
                printop(op);
            }
            //输出后记得返回
            return;
        }
        //对当前节点循环尝试六种操作
        for(int i=0;i<6;i++){
            int ta=tmp.a;
            int tb=tmp.b;
            op(ta,tb,i);
            if(vis[ta][tb]==-1){
                vis[ta][tb]=vis[tmp.a][tmp.b]+1;
                //将vis[tmp.a][tmp.b]的路径中相同的部分复制到vis[ta][tb]
                for(int j=0;j<vis[tmp.a][tmp.b];j++){
                    ans[ta][tb][j]=ans[tmp.a][tmp.b][j];
                }
                //再加上新的路径
                ans[ta][tb][vis[tmp.a][tmp.b]]=i;
                //BFS
                que.push(node(ta,tb));
            }
        }
    }
    //如果前面没返回 说明impossible
    puts("impossible");
}
int main(void){
    scanf("%d%d%d",&A,&B,&C);
    //初始化为-1
    memset(vis,-1,sizeof(vis));
    bfs();
    return 0;
}
```](https://so.csdn.net/so/search/s.do?q=搜索&t=blog)](https://so.csdn.net/so/search/s.do?q=BFS&t=blog)




