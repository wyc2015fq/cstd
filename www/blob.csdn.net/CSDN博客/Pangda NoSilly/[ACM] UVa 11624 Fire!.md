# [ACM] UVa 11624 Fire! - Pangda NoSilly - CSDN博客

2017年05月25日 18:59:36[hepangda](https://me.csdn.net/hepangda)阅读数：269


```cpp
#include<iostream>
#include<queue>
#include<cstring>

using namespace std;
const int MAXDATA = 1005;
const int NOTV = -1;
const int OPR[4][2] = { 0, 1, 0, -1, 1, 0, -1, 0};

struct d2Pos {
    int x, y, d;
};

int X__, Y__, R__;
char g[MAXDATA][MAXDATA];
int fire[MAXDATA][MAXDATA],
    joev[MAXDATA][MAXDATA];

queue<d2Pos> fireQ;
d2Pos joe;

void readg() {
    R__ = 0;
    while (!fireQ.empty())
        fireQ.pop();

    cin >> X__ >> Y__;
    for (int i = 0; i < X__; i++) {
        for (int j = 0; j < Y__; j++) {
            char t;
            cin >> t;
            g[i][j] = t;
            joev[i][j] = NOTV;
            switch (t) {
            case 'J':
                joev[i][j] = 0;
                fire[i][j] = NOTV;
                joe.x = i; joe.y = j; joe.d = 0;
            case '.':
                fire[i][j] = NOTV;
                break;
            case 'F':
                fire[i][j] = 0;
                d2Pos t1;
                t1.x = i; t1.y = j; t1.d = 0;
                fireQ.push(t1);
            }
        }
    }
}

bool inmaze(d2Pos t) {
    return (t.x >= 0 && t.y >= 0 && t.x < X__ && t.y < Y__);
}

void Search_Fire() {
    d2Pos t, v;

    while (!fireQ.empty()) {
        t = fireQ.front();
        fireQ.pop();

        for (int i = 0; i < 4; i++) {
            v.d = t.d + 1;
            v.x = t.x + OPR[i][0];
            v.y = t.y + OPR[i][1];

            if (!inmaze(v))
                continue;
            if (g[v.x][v.y] == '#')
                continue;
            if (fire[v.x][v.y] != NOTV)
                continue;

            fireQ.push(v);
            fire[v.x][v.y] = v.d;

        }
    }
}

void Search_Joe() {
    queue<d2Pos> Q;
    Q.push(joe);

    d2Pos t, v;

    while (!Q.empty()) {
        t = Q.front();
        Q.pop();

        if (t.x == 0 || t.y == 0 || t.x == X__ - 1 || t.y == Y__ - 1) {
            R__ = t.d + 1;
            return;
        }

        for (int i = 0; i < 4; i++) {
            v.d = t.d + 1;
            v.x = t.x + OPR[i][0];
            v.y = t.y + OPR[i][1];

            if (!inmaze(v))
                continue;
            if (g[v.x][v.y] == '#')
                continue;
            if (fire[v.x][v.y] != NOTV && v.d >= fire[v.x][v.y])
                continue;
            if (joev[v.x][v.y] != -1)
                continue;

            Q.push(v);
            joev[v.x][v.y] = v.d;

        }
    }
}

int main() {
    int T;
    cin >> T;
    while (T--) {
        readg();
        Search_Fire();
        Search_Joe();
        if (R__ == 0)
            cout << "IMPOSSIBLE" << endl;
        else
            cout << R__ << endl;
    }
    return 0;
}
```

