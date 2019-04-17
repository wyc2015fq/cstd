# Board - westbrook1998的博客 - CSDN博客





2018年10月06日 19:25:08[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：51标签：[矩阵																[思维](https://so.csdn.net/so/search/s.do?q=思维&t=blog)](https://so.csdn.net/so/search/s.do?q=矩阵&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
链接：[https://www.nowcoder.com/acm/contest/206/B](https://www.nowcoder.com/acm/contest/206/B)

来源：牛客网

题目描述

恬恬有一个nx n的数组。她在用这个数组玩游戏：

开始时，数组中每一个元素都是0。

恬恬会做某些操作。在一次操作中，她可以将某一行的所有元素同时加上一个值，也可以将某一列的所有元素同时加上一个值。

在几次操作后，一个元素被隐藏了。你能帮助她回忆隐藏的数是几吗？

输入描述:

第一行一个整数n（1≤ n≤ 1000）。

接下来n行每行n个整数表示数组a。

第(i+1)行的第j个元素表示aij（aij=-1或0≤ aij ≤ 10000）。-1表示隐藏的元素。

输出描述:

仅一个整数表示答案。

示例1

输入

复制

3

1 2 1

0 -1 0

0 1 0

输出

复制

1
有点坑这道题，一开始想了两个思路都判了好多秒才错，感觉差点什么特例没判，最后队友终于找出，差点自闭

思路就是每一行每一列依次删除每一行/列的最小值，隐藏数的那一行和列就不删，删到最后剩下就是隐藏数和所在的那一行和列，把行和列的数加起来就是隐藏数，然后坑点就是一开始直接加g[x][0] g[0][y] 没有考虑到如果隐藏数就是在0这个位置呢，就错了

代码：

```
#include <cstdio>
#include <algorithm>
using namespace std;
int n;
const int N=1005;
int g[N][N];
int hang[N];
int lie[N];
int main(void){
    scanf("%d",&n);
    int x,y;
    for(int i=0;i<n;i++){
        int Min=0x3f3f3f3f;
        for(int j=0;j<n;j++){
            scanf("%d",&g[i][j]);
            if(g[i][j]==-1){
                x=i;
                y=j;
                continue;
            }
            Min=min(Min,g[i][j]);
        }
        hang[i]=Min;
    }
    for(int i=0;i<n;i++){
        bool flag=true;
        for(int j=0;j<n;j++){
            if(g[i][j]==-1){
                flag=false;
                break;
            }
        }
        if(flag){
            for(int j=0;j<n;j++){
                g[i][j]-=hang[i];
            }
        }
    }
    for(int j=0;j<n;j++){
        int Min=0x3f3f3f3f;
        for(int i=0;i<n;i++){
            Min=min(Min,g[i][j]);
        }
        bool flag=true;
        for(int i=0;i<n;i++){
            if(g[i][j]==-1){
                flag=false;
                break;
            }
        }
        if(flag){
            for(int i=0;i<n;i++){
                g[i][j]-=Min;
            }
        }
    }
    // for(int i=0;i<n;i++){
    //     for(int j=0;j<n;j++){
    //         printf("%d ",g[i][j]);
    //     }
    //     printf("\n");
    // }
    int fx=0;
    for(int j=0;j<n;j++){
        if(g[x][j]!=-1){
            fx=g[x][j];
            break;
        }
    }
    int fy=0;
    for(int i=0;i<n;i++){
        if(g[i][y]!=-1){
            fy=g[i][y];
            break;
        }
    }
    //printf("%d\n",g[x][0]+g[0][y]);
    printf("%d\n",fx+fy);
    return 0;
}
```






