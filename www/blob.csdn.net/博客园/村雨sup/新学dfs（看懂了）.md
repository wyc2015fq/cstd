# 新学dfs（看懂了） - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







在N*N的迷宫内，“#”为墙，“.”为路，“s”为起点，“e”为终点，一共4个方向可以走。从左上角（(0,0)“s”）位置处走到右下角（(n-1,n-1)“e”）位置处，可以走通则输出YES，不可以走则输出NO。




输入描述 Input Description


输入的第一行为一个整数m，表示迷宫的数量。 
其后每个迷宫数据的第一行为一个整数n（n≤16），表示迷宫的边长，接下来的n行每行n个字符，字符之间没有空格分隔。




输出描述 Output Description


输出有m行，每行对应的迷宫能走，则输出YES，否则输出NO。




样例输入 Sample Input

1 
7 
s...##. 
.#..... 
....... 
..#.... 
..#...# 
###...# 
......e


```
#include "bits/stdc++.h"
using namespace std;
int n;
char a[20][20];
int mark[20][20];

void dfs(int x,int y)
{
    if(x==0||y==0||x>n||y>n)
        return;
    if(mark[x][y] == 0)
    {
        mark[x][y] = 1;
        if(a[x+1][y] == '.'||a[x+1][y] == 'e')
            dfs(x+1,y);
        if(a[x-1][y] == '.'||a[x-1][y] == 'e')
            dfs(x-1,y);
        if(a[x][y+1] == '.'||a[x][y+1] == 'e')
            dfs(x,y+1);
        if(a[x][y-1] == '.'||a[x][y-1] == 'e')
            dfs(x,y-1);
    }
}

int main()
{
    int m;
    cin >> m;
    while(m--)
    {

        cin >> n;
        for(int i=0;i < 20;i++)   //mark == 0
            for(int j=0;j < 20;j++)
                mark[i][j] = 0;

        for(int i=1;i <= n;i++)
            for(int j=1;j <= n;j++)
                cin >> a[i][j];

        dfs(1,1);

        if(mark[n][n] == 1)
            cout << "YES" <<endl;
        else
            cout << "NO" <<endl;



    }
    return 0;
}
```

总的来说就是递归，比较好理解。













