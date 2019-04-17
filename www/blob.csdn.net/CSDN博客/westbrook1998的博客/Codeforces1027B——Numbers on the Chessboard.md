# Codeforces1027B——Numbers on the Chessboard - westbrook1998的博客 - CSDN博客





2018年08月28日 13:02:41[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：39标签：[思维](https://so.csdn.net/so/search/s.do?q=思维&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
You are given a chessboard of size n×n. It is filled with numbers from 1 to n2 in the following way: the first ⌈n22⌉ numbers from 1 to ⌈n22⌉ are written in the cells with even sum of coordinates from left to right from top to bottom. The rest n2−⌈n22⌉ numbers from ⌈n22⌉+1 to n2 are written in the cells with odd sum of coordinates from left to right from top to bottom. The operation ⌈xy⌉ means division x by y rounded up. 

  For example, the left board on the following picture is the chessboard which is given for n=4 and the right board is the chessboard which is given for n=5. 
![这里写图片描述](https://odzkskevi.qnssl.com/4a6f86be6e4fe210f1c5239eba5fac0c?v=1535243707)

  You are given q queries. The i-th query is described as a pair xi,yi. The answer to the i-th query is the number written in the cell xi,yi (xi is the row, yi is the column). Rows and columns are numbered from 1 to n. 

  Input 

  The first line contains two integers n and q (1≤n≤109, 1≤q≤105) — the size of the board and the number of queries. 

  The next q lines contain two integers each. The i-th line contains two integers xi,yi (1≤xi,yi≤n) — description of the i-th query. 

  Output 

  For each query from 1 to q print the answer to this query. The answer to the i-th query is the number written in the cell xi,yi (xi is the row, yi is the column). Rows and columns are numbered from 1 to n. Queries are numbered from 1 to q in order of the input. 

  Examples 

  Input 

  4 5 

  1 1 

  4 4 

  4 3 

  3 2 

  2 4 

  Output 

  1 

  8 

  16 

  13 

  4 

  Input 

  5 4 

  2 1 

  4 2 

  3 3 

  3 4 

  Output 

  16 

  9 

  7 

  20 

  Note 

  Answers to the queries from examples are on the board in the picture from the problem statement.
给一个n*n的数字矩阵 然后1到n^2/2的就从左到右 从上到下依次放在行列加起来是偶数的格子里 然后n^2/2+1的就放在奇数格子里 

然后询问某个格子的数是多少 

其实就是根据n是奇数还是偶数 还有询问的位置是奇数还是偶数然后推理一下 找一下规律即可 

最后答案要开long long  而n也要开long long 不然中间结果会溢出
代码：

```
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;
long long n,q;
long long x,y;
int main(void){
    //freopen("data.txt","r",stdin);
    scanf("%lld%lld",&n,&q);
    while(q--){
        scanf("%lld%lld",&x,&y);
        long long ans=0;
        if((x+y)%2){
            if(n%2){
                ans=(n*n/2)+1+n*((x-1)/2);
                //printf("%lld\n",ans);
                if(x%2==0){
                    ans+=(n/2);
                    //printf("%lld\n",ans);
                }
                if(y%2){
                    ans+=(y+1)/2;
                    //printf("%lld\n",ans);
                }
                else{
                    ans+=(y/2);
                }

            }
            else{
                //n/2
                ans=n*n/2+(n/2)*(x-1);
                if(y%2){
                    ans+=(y+1)/2;
                }
                else{
                    ans+=(y/2);
                }
            }
        }
        else{
            if(n%2){
                ans=n*((x-1)/2);
                if(x%2==0){
                    ans+=(n/2)+1;
                }
                if(y%2){
                    ans+=(y+1)/2;
                }
                else{
                    ans+=(y/2);
                }

            }
            else{
                //n/2
                ans=(n/2)*(x-1);
                if(y%2){
                    ans+=(y+1)/2;
                }
                else{
                    ans+=(y/2);
                }
            }
        }
        printf("%lld\n",ans);
    }
    return 0;
}
```






