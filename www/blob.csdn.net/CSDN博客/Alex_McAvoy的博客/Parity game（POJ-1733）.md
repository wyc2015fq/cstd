# Parity game（POJ-1733） - Alex_McAvoy的博客 - CSDN博客





2018年11月17日 16:43:42[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：91








> 
# Problem Description

Now and then you play the following game with your friend. Your friend writes down a sequence consisting of zeroes and ones. You choose a continuous subsequence (for example the subsequence from the third to the fifth digit inclusively) and ask him, whether this subsequence contains even or odd number of ones. Your friend answers your question and you can ask him about another subsequence and so on. Your task is to guess the entire sequence of numbers. 

You suspect some of your friend's answers may not be correct and you want to convict him of falsehood. Thus you have decided to write a program to help you in this matter. The program will receive a series of your questions together with the answers you have received from your friend. The aim of this program is to find the first answer which is provably wrong, i.e. that there exists a sequence satisfying answers to all the previous questions, but no such sequence satisfies this answer.

# **Input**

The first line of input contains one number, which is the length of the sequence of zeroes and ones. This length is less or equal to 1000000000. In the second line, there is one positive integer which is the number of questions asked and answers to them. The number of questions and answers is less or equal to 5000. The remaining lines specify questions and answers. Each line contains one question and the answer to this question: two integers (the position of the first and last digit in the chosen subsequence) and one word which is either `even' or `odd' (the answer, i.e. the parity of the number of ones in the chosen subsequence, where `even' means an even number of ones and `odd' means an odd number).

# Output

There is only one line in output containing one integer X. Number X says that there exists a sequence of zeroes and ones satisfying first X parity conditions, but there exists none satisfying X+1 conditions. If there exists a sequence of zeroes and ones satisfying all the given conditions, then number X should be the number of all the questions asked.

# Sample Input

**1051 2 even3 4 odd5 6 even1 6 even7 10 odd**

# Sample Output

**3**


————————————————————————————————————————————————————

思路：与 [How Many Answers Are Wrong（HDU-3038）](https://blog.csdn.net/u011815404/article/details/80424400)类似，由于数据范围较大，需要首先进行离散化。由于只有奇偶两种状态，所以只需要用**0**和**1**表示状态即可。在路径压缩时**num[x] = num[x] ^ num[fa]**

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
const int LEN = 7;
int f[N], num[N], des[2 * N];

struct Query{
    int u, v, d;
}q[N];

int find(int x)
{
    if(x != f[x]){
        int fa = f[x];
        f[x] = find(f[x]);
        num[x] = num[x] ^ num[fa];
    }
    return f[x];
}

void init(int n)
{
    for(int i = 0; i <= n; i++){
        f[i] = i;
        num[i] = 0;
    }
}


int main()
{
    int n, m;
    while(scanf("%d%d", &n, &m) != EOF){
        bool conf = false;
        int ans = 0, cnt = 0;
        int u, v, fa, fb;
        char s[LEN];
        for(int i = 0; i < m; i++){
            scanf("%d%d%s", &q[i].u, &q[i].v, s);
            if(s[0] == 'o') q[i].d = 1;
            else q[i].d = 0;
            des[cnt++] = q[i].u - 1;
            des[cnt++] = q[i].v;
        }
        sort(des, des + cnt);
        cnt = unique(des, des + cnt) - des;
        init(cnt);
        for(int i = 0; i < m; i++){
            if(!conf){
                u = lower_bound(des, des + cnt, q[i].u - 1) - des;
                v = lower_bound(des, des + cnt, q[i].v) - des;
                fa = find(u), fb = find(v);
                if(fa == fb){
                    if((num[u] + q[i].d) % 2 != num[v]) conf = true;
                }
                else{
                    f[fb] = fa;
                    num[fb] = (2 + num[u] - num[v] + q[i].d) % 2;
                }
                if(!conf) ans++;
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
```





