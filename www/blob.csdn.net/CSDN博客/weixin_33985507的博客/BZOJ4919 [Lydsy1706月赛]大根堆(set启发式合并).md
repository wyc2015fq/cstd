# BZOJ4919: [Lydsy1706月赛]大根堆(set启发式合并) - weixin_33985507的博客 - CSDN博客
2018年09月21日 21:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
## 题意
[题目链接](https://www.lydsy.com/JudgeOnline/problem.php?id=4919)
## Sol
如果给出的树是链的话显然就是LIS
不是链的时候直接当链做，每个节点维护一个multiset表示计算LIS过程中的单调栈
启发式合并即可
时间复杂度：$O(nlog^2n)$
```
#include<bits/stdc++.h>
#define sit multiset<int>::iterator 
using namespace std;
const int MAXN = 2e5 + 10;
inline int read() {
    char c = getchar(); int x = 0, f = 1;
    while(c < '0' || c > '9') {if(c == '-') f = -1; c = getchar();}
    while(c >= '0' && c <= '9') x = x * 10 + c - '0', c = getchar();
    return x * f;
}
vector<int> v[MAXN];
multiset<int> s[MAXN];
int N, val[MAXN];
void dfs(int x, int fa) {
    for(int i = 0; i < v[x].size(); i++) {
        int to = v[x][i];
        if(to == fa) continue;
        dfs(to, x);
        if(s[to].size() > s[x].size()) swap(s[to], s[x]);
        for(sit it = s[to].begin(); it != s[to].end(); it++)
            s[x].insert(*it);
        s[to].clear();
    }
    sit it = s[x].lower_bound(val[x]);
    if(it != s[x].end()) s[x].erase(it);
    s[x].insert(val[x]);
}
main() {
    N = read();
    for(int i = 1; i <= N; i++) {
        val[i] = read();
        int x = read();
        v[i].push_back(x); v[x].push_back(i);
    }
    dfs(1, 0);
    printf("%d", s[1].size());
    return 0;
}
```
