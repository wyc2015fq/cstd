# Discovering Gold（LightOJ-1030） - Alex_McAvoy的博客 - CSDN博客





2019年04月02日 18:50:51[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：16
个人分类：[LightOJ																[动态规划——概率 DP 与期望 DP](https://blog.csdn.net/u011815404/article/category/8820787)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

You are in a cave, a long cave! The cave can be represented by a 1 x N grid. Each cell of the cave can contain any amount of gold.

Initially you are in position 1. Now each turn you throw a perfect 6 sided dice. If you get X in the dice after throwing, you add X to your position and collect all the gold from the new position. If your new position is outside the cave, then you keep throwing again until you get a suitable result. When you reach the Nth position you stop your journey. Now you are given the information about the cave, you have to find out the expected number of gold you can collect using the given procedure.

# **Input**

Input starts with an integer T (≤ 100), denoting the number of test cases.

Each case contains a blank line and an integer N (1 ≤ N ≤ 100) denoting the dimension of the cave. The next line contains N space separated integers. The ith integer of this line denotes the amount of gold you will get if you come to the ith cell. You may safely assume that all the given integers will be non-negative and no integer will be greater than 1000.

# Output

For each case, print the case number and the expected number of gold you will collect. Errors less than 10-6 will be ignored.

# Sample Input

**3**

**1101**

**210 3**

**33 6 9**

# Sample Output

**Case 1:  101.0000000000Case 2:  13.000Case 3:  15**


题意：t 组数据，每组给出 n 个数，代表 n 个格子的值，现在要从第一个格子出发前往第 n 个格子，每到达一个格子都能扔一次 1~6 的骰子，决定下一步走到哪个位置，若当前位置+骰子掷出的值>n 则重新掷骰子，直到到达第 n 个格子结束，问从最终获得格子上的值的期望值

思路：

最后求的是期望值，从前向后无法直接推出，使用期望 DP 从后向前推，那么当前的状态是由所有后继状态的期望乘以其到达它的概率之和得到的

因此从后向前推时，走过的点是一定能拿到的，因此令每个的点的初始期望值等于每个点的初始值，即：f[i]=num[i]

则：
- 在第 n 个地方，获得的数目为 f[n]
- 在第 n-1 个地方，掷骰子有一种情况，获得的数目为：f[n-1]+f[n]
- 在第 n-2 个地方，掷骰子有两种情况，获得的数目为：f[n-2]+f[(n-2)+1]/2+f[(n-2)+2]/2
- 在第 n-3 个地方，掷骰子有三种情况，获得的数目为：f[n-3]+f[(n-3)+1]/3+f[(n-3)+2]/3+f[(n-3)+3]/3
- ....
- 在第 i 个地方，掷骰子有六种情况，获得的数目为：f[i]=f[i]+f[i+1]/6+f[i+2]/6+f[i+3]/6+f[i+1]/6+f[i+5]/6+f[i+6]/6

如上进行逆推，最后得到的 f[0] 就是最终的期望值

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
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define E 1e-9
#define PI acos(-1.0)
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=7;
const int N=100000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
double num[N];//每个格子的值
double f[N];//第i个格子的期望值
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        int n;
        scanf("%d",&n);

        memset(f,0,sizeof(f));
        for(int i=1;i<=n;i++){
            scanf("%lf",&num[i]);
            f[i]=num[i];//每个格子的初始期望值都是每个格子的值
        }

        for(int i=n-1;i>=1;i--){//从后向前逆推
            for(int j=1;j<=6;j++){//最多6种情况
                int k=min(6,n-i);//第i个点掷骰子的点数
                if(i+j<=n)
                    f[i]=f[i]+f[i+j]/k;//加上k种可能的情况
            }
        }

        printf("Case %d: %.6lf\n",Case++,f[1]);
    }

    return 0;
}
```






