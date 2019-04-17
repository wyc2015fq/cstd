# hdu1415——Jugs - westbrook1998的博客 - CSDN博客





2018年07月06日 10:25:42[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：83标签：[bfs																[倒水问题](https://so.csdn.net/so/search/s.do?q=倒水问题&t=blog)](https://so.csdn.net/so/search/s.do?q=bfs&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
In the movie “Die Hard 3”, Bruce Willis and Samuel L. Jackson were confronted with the following puzzle. They were given a 3-gallon jug and a 5-gallon jug and were asked to fill the 5-gallon jug with exactly 4 gallons. This problem generalizes that puzzle.  

  You have two jugs, A and B, and an infinite supply of water. There are three types of actions that you can use: (1) you can fill a jug, (2) you can empty a jug, and (3) you can pour from one jug to the other. Pouring from one jug to the other stops when the first jug is empty or the second jug is full, whichever comes first. For example, if A has 5 gallons and B has 6 gallons and a capacity of 8, then pouring from A to B leaves B full and 3 gallons in A.  

  A problem is given by a triple (Ca,Cb,N), where Ca and Cb are the capacities of the jugs A and B, respectively, and N is the goal. A solution is a sequence of steps that leaves exactly N gallons in jug B. The possible steps are  

  fill A  

  fill B  

  empty A  

  empty B  

  pour A B  

  pour B A  

  success  

  where “pour A B” means “pour the contents of jug A into jug B”, and “success” means that the goal has been accomplished.  

  You may assume that the input you are given does have a solution.  

  Input 

  Input to your program consists of a series of input lines each defining one puzzle. Input for each puzzle is a single line of three positive integers: Ca, Cb, and N. Ca and Cb are the capacities of jugs A and B, and N is the goal. You can assume 0 < Ca <= Cb and N <= Cb <=1000 and that A and B are relatively prime to one another.  

  Output 

  Output from your program will consist of a series of instructions from the list of the potential output lines which will result in either of the jugs containing exactly N gallons of water. The last line of output for each puzzle should be the line “success”. Output lines start in column 1 and there should be no empty lines nor any trailing spaces.  

  Sample Input 

  3 5 4  

  5 7 3  

  Sample Output 

  fill B  

  pour B A  

  empty A  

  pour B A  

  fill B  

  pour B A  

  success  

  fill A  

  pour A B  

  fill A  

  pour A B  

  empty B  

  pour A B  

  success 
又是一道倒水问题 bfs加保存输出路径  这次终于会做了 也对bfs和保存路径有了更加深刻的理解

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <queue>
#include <vector>
using namespace std;
const int MAXN=1050;
struct node{
    int a,b;
    node(int _a,int _b):a(_a),b(_b){}
};
int A,B,N;
int vis[MAXN][MAXN];
vector<int> path[MAXN][MAXN];
void op(int &a,int &b,int i){
    switch(i){
        case 1:{
            a=A;
            break;
        }
        case 2:{
            b=B;
            break;
        }
        case 3:{
            a=0;
            break;
        }
        case 4:{
            b=0;
            break;
        }
        case 5:{
            if(a+b>B){
                a=(a+b)-B;
                b=B;
            }
            else{
                b+=a;
                a=0;
            }
            break;
        }
        case 6:{
            if(b+a>A){
                b=(b+a)-A;
                a=A;
            }
            else{
                a+=b;
                b=0;
            }
            break;
        }
    }
}
void op_print(int i){
    switch(i){
        case 1:{
            printf("fill A\n");
            break;
        }
        case 2:{
            printf("fill B\n");
            break;
        }
        case 3:{
            printf("empty A\n");
            break;
        }
        case 4:{
            printf("empty B\n");
            break;
        }
        case 5:{
            printf("pour A B\n");
            break;
        }
        case 6:{
            printf("pour B A\n");
            break;
        }
    }
}
void bfs(){
    memset(vis,-1,sizeof(vis));
    for(int i=0;i<A;i++){
        for(int j=0;j<B;j++){
            path[i][j].clear();
        }
    }
    queue<node> que;
    que.push(node(0,0));
    vis[0][0]=0;
    while(!que.empty()){
        node tmp=que.front();
        que.pop();
        int ta=tmp.a;
        int tb=tmp.b;
        if(tb==N){
            for(int i=0;i<path[ta][tb].size();i++){
                op_print(path[ta][tb][i]);
            }
            printf("success\n");
            return;
        }
        for(int i=1;i<=6;i++){
            int ta=tmp.a;
            int tb=tmp.b;
            op(ta,tb,i);
            if(vis[ta][tb]==-1){
                vis[ta][tb]=vis[tmp.a][tmp.b]+1;
                for(int j=0;j<vis[tmp.a][tmp.b];j++){
                    path[ta][tb].push_back(path[tmp.a][tmp.b][j]);
                }
                path[ta][tb].push_back(i);
                que.push(node(ta,tb));
            }
        }
    }
}
int main(void){
    while(~scanf("%d%d%d",&A,&B,&N)){
        bfs();
    }
    return 0;
}
```






