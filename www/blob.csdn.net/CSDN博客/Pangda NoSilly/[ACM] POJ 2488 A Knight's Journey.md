# [ACM] POJ 2488 A Knight's Journey - Pangda NoSilly - CSDN博客

2017年08月02日 17:06:18[hepangda](https://me.csdn.net/hepangda)阅读数：222
个人分类：[ACM																[算法](https://blog.csdn.net/hepangda/article/category/6896906)](https://blog.csdn.net/hepangda/article/category/6685366)


纪念一下自己寒假被支配了半个月的恐惧。

## Description

整天待在方块里的骑士感到特别的无聊，于是他决定来一场说走就走的旅行。 

然而他只能走日字，如右图所示，如果骑士当前在棋盘的正中央，他可以走标记有白点的八个区域。 

骑士知道世界是一个列数和行数均不超过8（即8×8）的棋盘。 

并且骑士有一点强迫症，如果用A-Z来表示列，1-99来表示横行，他只愿意走字典序最小的一条道路。 

你能帮助勇敢的骑士制定一个适合他的旅行计划，使得他可以走遍整个棋盘吗？骑士可以在任一方块出发或者结束。

## Input

第一行中有一个正整数n，代表数据有n组。 

对于每组数据，都含有两个正整数p和q(1 <= p * q <= 26)，代表棋盘有p行q列。

## Output

每组数据首先应当输出”Scenario #i:”，i代表输出的是第i组数据的结果。 

然后在一行之内输出一条可以走遍棋盘的路径，如果有多条路径可以走遍棋盘，那么输出按字典序排序第一的路径。最后，留一个空行。若现在是最后一条数据，则不留空行。

在输出路径时，用A代表第一列，B代表第二列..以此类推。而使用1代表第一行，2代表第二行。 

例如，若要表示从第一行第一列到第二行第三列，可以用字符串：A1C3来表示。

## Sample Input

3 

1 1 

2 3 

4 3

### Sample Output

Scenario #1: 

A1

Scenario #2: 

impossible

Scenario #3: 

A1B3C1A2B4C2A3B1C3A4B2C4

### Code

```cpp
#include<iostream>
#include<cstring>
using namespace std;

struct d2Pos {
    int x, y;
};
d2Pos result[1000];
int p__, q__;
bool vis[30][30];
bool found;

bool check(int x, int y) {
    if (x <= p__ && x > 0 && y <= q__ && y > 0 && !vis[x][y] && !found) {
        return true;
    }
    return false;
}

void dfs(d2Pos tp, int depth) { 
    result[depth] = tp;
    if (depth == p__ * q__ ) {  
        found = true;
        return;
    }

    static const int OPR[8][2] = { -1, -2,  1, -2,
                                   -2, -1,  2, -1,
                                   -2,  1,  2,  1,
                                   -1,  2,  1,  2 };

    for (int i = 0; i < 8; i++) {
        d2Pos np = { tp.x + OPR[i][0] , tp.y + OPR[i][1] };
        if (check(np.x, np.y)) {
            vis[np.x][np.y] = true;
            dfs(np, depth + 1);
            vis[np.x][np.y] = false;
        }
    }
}

int main() {
    int cas;
    cin >> cas;
    for (int casno = 0; casno < cas; casno++) {
        found = false;
        memset(vis, 0, sizeof(vis));
        cin >> p__ >> q__;
        d2Pos temp = { 1, 1 };
        vis[1][1] = true;
        dfs(temp, 1);
        cout << "Scenario #" << casno + 1 << ":" << endl;

        if (!found) {
            cout << "impossible" << endl;
        } else {
            for (int i = 1; i <= p__ * q__; i++) {
                cout << char(result[i].y + 'A' - 1) << result[i].x;
            }
            cout << endl;
        }
        if (casno != cas - 1)
            cout << endl;
    }
    return 0;
}
```

