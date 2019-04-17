# Applese 的取石子游戏（2019牛客寒假算法基础集训营 Day4-A） - Alex_McAvoy的博客 - CSDN博客





2019年02月02日 14:31:04[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：134








> 
# 【题目描述】

Applese 和 Bpplese 在玩取石子游戏，规则如下：

一共有偶数堆石子排成一排，每堆石子的个数为 ai。两个人轮流取石子，Applese先手。每次取石子只能取最左一堆或最右一堆，且必须取完。最后取得的石子多者获胜。假设双方都足够聪明，最后谁能够获胜呢？

# 【输入描述】

第一行是一个正偶数 n，表示石子的堆数。

第二行是 n 个正整数 a1,a2,…,an，表示每堆石子的个数。

2≤n≤105

1≤ai≤105

∑ai 为奇数

# 【输出描述】

输出一个字符串“Applese”或“Bpplese”，表示胜者的名字。

# 【样例】

示例1

输入

4

2 3 3 3

输出

Applese


思路：博弈

由于堆数 n 是偶数，那么先手方有必胜策略：选择所有的奇数项或者偶数项

先手方首先比较奇数项之和和偶数项之和，由于石子总数是奇数，那么一定不相等，因此有：

  1）若奇数项之和比较大，那么先手就取第一项，剩下的序列首尾都是偶数项，后手就只能取偶数项，然后又剥离出一个奇数项，接着被先手取走，剩下的首尾又是偶数项，以此类推，先手就能取到所有的奇数项，后手只能取到所有的偶数项，那么先手获胜

2）若偶数项之和比较大，那么先手就取最后一项，剩下的序列首尾都是奇数项，后手就只能取奇数项，然后又剥离出一个偶数项，接着被先手取走，剩下的首尾又是奇数项，以此类推，先手就能取到所有的偶数项，后手只能取到所有的奇数项，那么先手获胜

故而直接输出 Applese 即可

# 【源代码】

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
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
using namespace std;
int a[N];
int main(){
    int n;
    cin>>n;
    for(int i=1;i<=n;i++)
        cin>>a[i];
    cout<<"Applese"<<endl;
    return 0;

}
```



