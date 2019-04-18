# HDU - 5410 CRB and His Birthday - weixin_33985507的博客 - CSDN博客
2017年06月21日 13:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
题目大意：CRB生日。妈妈要给他买礼物，妈妈有M元钱。这家店有N种礼物，由于店长和妈妈是熟人，所以若第i种礼物买x件的话。店长会给妈妈Ai*x+Bi颗糖果。现给出每种礼物的单位价格、Ai值与Bi值，问妈妈最多能拿到多少颗糖果。
解题思路：01背包 + 全然背包
```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        int N, M, DP[2010] = {0};
        scanf("%d%d", &M, &N);
        for (int r = 1; r <= N; r++) {
            int W, A, B;
            scanf("%d%d%d", &W, &A, &B);
            for (int i = M; i >= W; i--)
                DP[i] = max(DP[i], DP[i-W] + A + B);
            for (int i = W; i <= M; i++)
                DP[i] = max(DP[i], DP[i-W] + A);
        }
        printf("%d\n", DP[M]);
    }
    return 0;
}
```
