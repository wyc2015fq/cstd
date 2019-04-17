# Rochambeau（POJ-2912） - Alex_McAvoy的博客 - CSDN博客





2018年11月17日 16:31:22[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：90








> 
# Problem Description

N children are playing Rochambeau (scissors-rock-cloth) game with you. One of them is the judge. The rest children are divided into three groups (it is possible that some group is empty). You don’t know who is the judge, or how the children are grouped. Then the children start playing Rochambeau game for M rounds. Each round two children are arbitrarily selected to play Rochambeau for one once, and you will be told the outcome while not knowing which gesture the children presented. It is known that the children in the same group would present the same gesture (hence, two children in the same group always get draw when playing) and different groups for different gestures. The judge would present gesture randomly each time, hence no one knows what gesture the judge would present. Can you guess who is the judge after after the game ends? If you can, after how many rounds can you find out the judge at the earliest?

# **Input**

Input contains multiple test cases. Each test case starts with two integers N and M (1 ≤ N ≤ 500, 0 ≤ M ≤ 2,000) in one line, which are the number of children and the number of rounds. Following are M lines, each line contains two integers in [0, N) separated by one symbol. The two integers are the IDs of the two children selected to play Rochambeau for this round. The symbol may be “=”, “>” or “<”, referring to a draw, that first child wins and that second child wins respectively.

# Output

There is only one line for each test case. If the judge can be found, print the ID of the judge, and the least number of rounds after which the judge can be uniquely determined. If the judge can not be found, or the outcomes of the M rounds of game are inconsistent, print the corresponding message.

# Sample Input

**3 30<11<22<03 50<10>11<21>20<24 40<10>12<32>31 0**

# Sample Output

**Can not determinePlayer 1 can be determined to be the judge after 4 linesImpossiblePlayer 0 can be determined to be the judge after 0 lines**


———————————————————————————————————————————————————————

思路：带权并查集的种类问题。

基础操作与食物链非常相似，但是有可能出现一个或多个异常。在以下方法中，我们枚举每个人是异常的情况，观察有多少个人可能是裁判。如果有多个人成为裁判使体系融洽，那么不能确定；如果没有一个人成为裁判后体系融洽，那么就没有答案；如果只有一个人可能成为裁判，那么我们得出判断的轮数，就是其他人成为裁判时，使体系不融洽的最大轮数。

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
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 10001
#define LL long long
using namespace std;
struct Res{
    int u, v;
    int r;
}r[N];

int f[N], num[N], err[N];
int n, m;

int find(int x){
    if(x != f[x]){
        int fa = f[x];
        f[x] = find(f[x]);
        num[x] = (num[x] + num[fa]) % 3;
    }
    return f[x];
}
void init(int n){
    for(int i = 0; i < n; i++) {
        f[i] = i;
        num[i] = 0;
    }
}

int main(){
    while(scanf("%d%d", &n, &m) != EOF){
        for(int i = 0; i < m; i++){
            char ch;
            scanf("%d%c%d", &r[i].u, &ch, &r[i].v);
            if(ch == '=')   r[i].r = 0;
            else if(ch == '<') r[i].r = 1;
            else if(ch == '>') r[i].r = 2;
        }
        memset(err, -1, sizeof(err));
        for(int i = 0; i < n; i++){
            init(n);
            for(int j = 0; j < m; j++){
                if(i == r[j].u || i == r[j].v) continue;
                int fa = find(r[j].u), fb = find(r[j].v);
                if(fa == fb){
                    if(num[r[j].v] != (num[r[j].u] + r[j].r) % 3){
                        err[i] = j + 1;
                        break;
                    }
                }
                else{
                    f[fb] = fa;
                    num[fb] = (num[r[j].u] - num[r[j].v] + 3 + r[j].r) % 3;
                }
            }
        }
        int cnt = 0, ans1 = 0, ans2 = 0;
        for(int i = 0; i < n; i++){
            if(err[i] == -1){
                cnt ++;
                ans1 = i;
            }
            ans2 = max(ans2, err[i]);
        }
        if(cnt == 0)    printf("Impossible\n");
        else if(cnt > 1)    printf("Can not determine\n");
        else printf("Player %d can be determined to be the judge after %d lines\n", ans1, ans2);
    }
    return 0;
}
```



