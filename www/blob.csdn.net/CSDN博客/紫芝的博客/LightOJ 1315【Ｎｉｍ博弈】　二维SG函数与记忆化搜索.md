# LightOJ 1315【Ｎｉｍ博弈】　二维SG函数与记忆化搜索 - 紫芝的博客 - CSDN博客





2018年11月08日 17:55:45[紫芝](https://me.csdn.net/qq_40507857)阅读数：171
个人分类：[博弈论](https://blog.csdn.net/qq_40507857/article/category/7632032)









A Hyper Knight is like a chess knight except it has some special moves that a regular knight cannot do. Alice and Bob are playing this game (you may wonder why they always play these games!). As always, they both alternate turns, play optimally and Alice starts first. For this game, there are 6 valid moves for a hyper knight, and they are shown in the following figure (circle shows the knight).

![](https://vj.e949.cn/a6bf4819cd0bd7cf98097fa9b327f34f?v=1541345707)

They are playing the game in an infinite chessboard where the upper left cell is (0, 0), the cell right to (0, 0) is (0, 1). There are some hyper knights in the board initially and in each turn a player selects a knight and gives a valid knight move as given. And the player who cannot make a valid move loses. Multiple knights can go to the same cell, but exactly one knight should be moved in each turn.

Now you are given the initial knight positions in the board, you have to find the winner of the game.

Input

Input starts with an integer **T (****≤ 200)**, denoting the number of test cases.

Each case starts with a line containing an integer **n (1 ≤ n ≤ 1000)** where **n** denotes the number of hyper knights. Each of the next **n** lines contains two integers **x y****(0 ≤ x, y < 500)** denoting the position of a knight.

Output

For each case, print the case number and the name of the winning player.

Sample Input

2

1

1 0

2

2 5

3 5

Sample Output

Case 1: Bob

Case 2: Alice

### 题意：

**给定一个棋盘，左上角为（0,0），棋盘中有多个骑士，每一个骑士只能按照图中的6种方式移动，两个人轮流移动棋盘中任意一个骑士，当轮到某一个人移动骑士时，棋盘中的骑士都已经不能移动了则判定为输，Alice先移动棋盘中的骑士，最后输出Alice和Bob谁赢谁输。**

### 题解：

**典型的博弈SG函数，对每一个骑士的起始位置求SG值，然后将所有的SG值进行异或，如果其值为0，则先手必败，即Bob 获胜，否则先手必胜，Alice获胜。又由于这道题是二维的，因此每一个位置都是由x和y两个值来决定的，因此这道题无法使用打表的方式进行求SG值，需要时dfs的方式，SG初始化为-1即可。**

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn = 1100;
int sg[maxn][maxn];
int t, n;
int s[6][2] = {{1, -2}, {-1, -3}, {-1, -2}, {-2, -1}, {-3, -1}, {-2, 1}};
struct node {
	int x, y;
}p[maxn];
 
int SG_dfs(int x, int y)
{
    int i;
    if(sg[x][y]!=-1)
        return sg[x][y];
    bool vis[maxn];
    memset(vis,0,sizeof(vis));
    int cnt = 0;
    for(i=0;i<6;i++)
    {
    	int tx = x+s[i][0];
    	int ty = y+s[i][1];
        if(tx>=0 && ty>=0)
        {
            vis[SG_dfs(tx, ty)]=1;
        }
    }
    int e;
    for(i=0;;i++)
        if(!vis[i])
        {
            e=i;
            break;
        }
    return sg[x][y]=e;
} 
 
 
int main(){
	memset(sg, -1, sizeof(sg));
	int Case = 1;
	scanf("%d", &t);
	while(t--){
		scanf("%d", &n);
		for(int i=0; i<n; i++){
			scanf("%d%d", &p[i].x, &p[i].y);
		}
		for(int i=0; i<=n; i++){
			SG_dfs(p[i].x, p[i].y);
		} 
		int ans = 0;
		for(int i=0; i<n; i++){
			ans ^= sg[p[i].x][p[i].y];
		}
		printf("Case %d: ", Case++);
		if(ans == 0) cout<<"Bob"<<endl;
		else cout<<"Alice"<<endl;
	}	
}
```





