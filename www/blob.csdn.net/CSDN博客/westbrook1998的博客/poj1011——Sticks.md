# poj1011——Sticks - westbrook1998的博客 - CSDN博客





2018年07月05日 09:20:08[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：40








> 
George took sticks of the same length and cut them randomly until all parts became at most 50 units long. Now he wants to return sticks to the original state, but he forgot how many sticks he had originally and how long they were originally. Please help him and design a program which computes the smallest possible original length of those sticks. All lengths expressed in units are integers greater than zero. 

  Input 

  The input contains blocks of 2 lines. The first line contains the number of sticks parts after cutting, there are at most 64 sticks. The second line contains the lengths of those parts separated by the space. The last line of the file contains zero. 

  Output 

  The output should contains the smallest possible length of original sticks, one per line. 

  Sample Input 

  9 

  5 2 1 5 2 1 5 2 1 

  4 

  1 2 3 4 

  0 

  Sample Output 

  6 

  5
经典的dfs+剪枝问题  

做我是不会做的…就只能看看题解 抄抄答案这样子

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=100;
int n;
bool flag;
int stick[MAXN];
bool vis[MAXN];
bool cmp(int a,int b){
    return a>b;
}
//想要拼的长度 当前已用过的stick数 当前所拼的长度 当前检查的小棒
void dfs(int len,int dep,int now_len,int u){
    if(flag){
        return;
    }
    //当前长度为0 开始重新搜索 即找到下一个未访问的最长stick
    if(now_len==0){
        int k=0;
        while(vis[k]){
            k++;
        }
        vis[k]=true;
        dfs(len,dep+1,stick[k],k+1);
        //注意要重新置为false
        vis[k]=false;
        return;
    }
    //已拼完一根
    if(now_len==len){
        //已经用完stick
        if(dep==n){
            flag=true;
        }
        else{
            //还有stick 重新从头开始dfs
            dfs(len,dep,0,0);
            return;
        }
    }
    //不满足前面的情况 即当前的len即不为0也不为len
    //剪枝 从当前检查的stick开始 因为stick已经从大到小排序了
    for(int i=u;i<n;i++){
        //如果已拼完的长度加上下一根stick就已经超过 就不要
        if(!vis[i] && now_len+stick[i]<=len){
            //不重复搜索
            if(!vis[i-1] && stick[i]==stick[i-1]){
                continue;
            }
            //如果都能通过前面两个条件 就选择这第i条stick递归dfs
            vis[i]=true;
            dfs(len,dep+1,now_len+stick[i],i+1);
            vis[i]=false;
        }
    }
}
int main(void){
    while(~scanf("%d",&n) && n){
        int sum=0;
        flag=false;
        for(int i=0;i<n;i++){
            scanf("%d",&stick[i]);
            sum+=stick[i];
        }
        sort(stick,stick+n,cmp);
        //枚举能被sum整除的长度
        int i;
        for(i=stick[0];i<sum;i++){
            if(sum%i==0){
                memset(vis,0,sizeof(vis));
                dfs(i,0,0,0);
                if(flag){
                    break;
                }
            }
        }
        printf("%d\n",i);
    }
    return 0;
}
```





