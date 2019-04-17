# Applese 的减肥计划（2019牛客寒假算法基础集训营 Day4-J） - Alex_McAvoy的博客 - CSDN博客





2019年02月03日 12:11:01[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：93
个人分类：[牛客																[数学——其他](https://blog.csdn.net/u011815404/article/category/8279050)](https://blog.csdn.net/u011815404/article/category/8639591)








> 
# 【题目描述】

Applese 最近又长胖了，于是它打算减肥——练习举重。

他在举重的时候用两只手往不同方向用力，从而把杠铃举起来。

已知 Applese 两只手分别产生的力的大小，以及它们之间的夹角，试求两力合力的大小。

# 【输入描述】

仅一行三个整数 f1,f2,a，分别表示两只手产生的力的大小以及它们之间的夹角

1≤f1,f2≤100

0≤a≤180

# 【输出描述】

输出一个实数表示两力合力的大小，要求相对误差或绝对误差不超过 10−6。

严格来讲，如果你的答案是 a，而标准答案是 b，那么当 |a−b|/max{1,|b|}≤10−6 时，你的答案会被认为是正确的。

# 【样例】

示例1

输入

6 8 90

输出

10.0000000000

示例2

输入

10 10 60

输出

17.3205080757


思路：

根据余弦定理，计算两个力的合力即可，注意 cos 函数中的参数为弧度

![](https://img-blog.csdnimg.cn/20190203121023198.jpg)

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
double calculate(double f1,double f2,double a){
    return sqrt ( f1*f1+f2*f2+2*f1*f2*cos((a/180)*PI) );
}
int main(){
    double f1,f2,a;
    cin>>f1>>f2>>a;

    double f=calculate(f1,f2,a);
    printf("%.10lf\n",f);

    return 0;

}
```






