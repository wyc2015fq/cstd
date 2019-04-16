# LightOJ-1253 【Nim博弈】取完最后一个石子输 - 紫芝的博客 - CSDN博客





2018年11月08日 12:04:48[紫芝](https://me.csdn.net/qq_40507857)阅读数：271








Alice and Bob are playing game of **Misère Nim**. Misère Nim is a game playing on **k** piles of stones, each pile containing one or more stones. The players alternate turns and in each turn a player can select one of the piles and can remove as many stones from that pile unless the pile is empty. In each turn a player must remove at least one stone from any pile. Alice starts first. The player who removes the last stone **loses** the game.

Input

Input starts with an integer **T (****≤ 200)**, denoting the number of test cases.

Each case starts with a line containing an integer** k (1 ≤ k ≤ 100)**. The next line contains **k** space separated integers denoting the number of stones in each pile. The number of stones in a pile lies in the range **[1, 109]**.

Output

For each case, print the case number and **'Alice'** if Alice wins otherwise print **'Bob'**.

Sample Input

3

4

2 3 4 5

5

1 1 2 4 10

1

1

Sample Output

Case 1: Bob

Case 2: Alice

Case 3: Bob

题意：

有N堆石子，每次可以从一堆中取任意个石子，至少一个，拿最后一个石子的人输**（经典Nim博弈为拿走最后一颗石子胜）**

当N堆石子都为1时，奇数个石子堆，先手必败；其他情况符合Nim博弈性质。Nim博弈的必胜态在这个游戏中任然是必胜态，因为处于必胜态的人一定可以把最后一颗石子留给对方

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>
#include<iostream>
using namespace std;
typedef long long LL;
const int maxn=120;
LL a[120];
int main(){
    int T;
    int ca=1;
    scanf("%d",&T);
    while(T--){
        int n,m;
        scanf("%d",&n);
        LL ans=0;
       
        bool flag=true;
        for(int i=0;i<n;i++){
            scanf("%lld",&a[i]);
            if(a[i]!=1)  flag=false;
            ans^=a[i];
        }
        printf("Case %d: ",ca++);
        if(flag){
            if(n%2==1)
                printf("Bob\n");
            else
                printf("Alice\n");
            continue;
        }
        if(ans==0)
            printf("Bob\n");
        else
            printf("Alice\n");
    }
    return 0;
}
```





