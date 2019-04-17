# Purification（CF-330C） - Alex_McAvoy的博客 - CSDN博客





2019年02月11日 13:48:21[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：69
个人分类：[CodeForces																[搜索——暴力搜索及打表](https://blog.csdn.net/u011815404/article/category/7925875)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

You are an adventurer currently journeying inside an evil temple. After defeating a couple of weak zombies, you arrived at a square room consisting of tiles forming an n × n grid. The rows are numbered 1 through n from top to bottom, and the columns are numbered 1 through n from left to right. At the far side of the room lies a door locked with evil magical forces. The following inscriptions are written on the door:

The cleaning of all evil will awaken the door!

Being a very senior adventurer, you immediately realize what this means. You notice that every single cell in the grid are initially evil. You should purify all of these cells.

The only method of tile purification known to you is by casting the "Purification" spell. You cast this spell on a single tile — then, all cells that are located in the same row and all cells that are located in the same column as the selected tile become purified (including the selected tile)! It is allowed to purify a cell more than once.

You would like to purify all n × n cells while minimizing the number of times you cast the "Purification" spell. This sounds very easy, but you just noticed that some tiles are particularly more evil than the other tiles. You cannot cast the "Purification" spell on those particularly more evil tiles, not even after they have been purified. They can still be purified if a cell sharing the same row or the same column gets selected by the "Purification" spell.

Please find some way to purify all the cells with the minimum number of spells cast. Print -1 if there is no such way.

# Input

The first line will contain a single integer n (1 ≤ n ≤ 100). Then, n lines follows, each contains n characters. The j-th character in the i-th row represents the cell located at row i and column j. It will be the character 'E' if it is a particularly more evil cell, and '.' otherwise.

# Output

If there exists no way to purify all the cells, output -1. Otherwise, if your solution casts x "Purification" spells (where x is the minimum possible number of spells), output x lines. Each line should consist of two integers denoting the row and column numbers of the cell on which you should cast the "Purification" spell.

# Examples

**Input**

3

.E.

E.E

.E.

**Output**

1 1

2 2

3 3

**Input**

3

EEE

E..

E.E

**Output**

-1

**Input**

5

EE.EE

E.EE.

E...E

.EE.E

EE.EE

**Output**

3 3

1 3

2 2

4 4

5 3

# Note

The first example is illustrated as follows. Purple tiles are evil tiles that have not yet been purified. Red tile is the tile on which "Purification" is cast. Yellow tiles are the tiles being purified as a result of the current "Purification" spell. Green tiles are tiles that have been purified previously.

![](https://img-blog.csdnimg.cn/20190211130953993.jpg)

In the second example, it is impossible to purify the cell located at row 1 and column 1.

For the third example:

![](https://img-blog.csdnimg.cn/20190211131005643.jpg)


题意：给出一张 n*n 的图，整张图需要净化，在图中 " . "代表可以净化的点，在其上可以净化这个点所在的行、列，" E " 代表无法去净化的点，即无法到达，只能通过 " . " 的净化来净化，如果能净化整张图，就输出每个格子的位置，否则输出 -1 

思路：

看题意比较难，但观察下面的 Note 仔细想一想并没有那么难

首先，如同样例 2，对于某一行列均是 E 的情况，那么注定无解，因此这种情况直接输出 -1 即可

![](https://img-blog.csdnimg.cn/20190211132631628.png)

其次，若是某几行全是 E，那么为不与第一种情况冲突，可以保证剩下的行有足够的 " . " 使得可覆盖全图，对于这种情况，进行构造即可，每一列都输出第一个 " . " 的位置，可保证一定能覆盖全图

![](https://img-blog.csdnimg.cn/20190211133310428.png)

然后，若是某几列全是 E，那么为不与第一种情况冲突，可以保证剩下的列有足够的 " . " 使得可覆盖全图，对于这种情况，进行构造即可，每一行都输出第一个 " . " 的位置，可保证一定能覆盖全图

![](https://img-blog.csdnimg.cn/20190211133336599.png)

对于其他的情况，一定是行、列没有全是 E 的情况，那么同样可以找每一行每一列第一个 " . " 的位置即可

![](https://img-blog.csdnimg.cn/20190211133633630.png)

最后，根据以上 4 种情况，进行暴力搜索即可

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
#include<deque>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define INF 0x3f3f3f3f
#define N 10001
#define LL long long
const int MOD=998244353;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
char G[N][N];
int row[N],col[N];//记录每一行每一列的E的个数
int main() {
    int n;
    cin>>n;
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            cin>>G[i][j];

    bool flagRow=false;
    bool flagCol=false;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            if(G[i][j]=='E'){
                row[i]++;//第i行E的个数+1
                col[j]++;//第j列E的个数+1
                if(row[i]==n)
                    flagRow=true;
                if(col[j]==n)
                    flagCol=true;
                if(flagRow&&flagCol){//某行某列E的个数全是E的情况
                    cout<<-1<<endl;
                    return 0;
                }
            }
        }
    }

    if(flagRow){//行全是E的情况
        for(int j=1;j<=n;j++){
            for(int i=1;i<=n;i++){
                if(G[i][j]=='.'){
                    cout<<i<<" "<<j<<endl;
                    break;
                }
            }
        }
    }
    else if(flagCol){//列全是E的情况
        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                if(G[i][j]=='.'){
                    cout<<i<<" "<<j<<endl;
                    break;
                }
            }
        }
    }
    else{//行或列不存在全是E的情况
        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                if(G[i][j]=='.'){
                    cout<<i<<" "<<j<<endl;
                    break;
                }
            }
        }
    }

    return 0;
}
```






