# yoyo思维题(困难)  组合数学 - 紫芝的博客 - CSDN博客





2018年07月21日 11:50:54[紫芝](https://me.csdn.net/qq_40507857)阅读数：34
个人分类：[数论](https://blog.csdn.net/qq_40507857/article/category/7647322)









## 问题 B: yoyo思维题(困难)

时间限制: 1 Sec  内存限制: 256 MB

提交: 11  解决: 3

[[提交](http://101.200.61.174/JudgeOnline/submitpage.php?cid=1003&pid=1&langmask=0)][[状态](http://101.200.61.174/JudgeOnline/problemstatus.php?id=1731)][[讨论版](http://101.200.61.174/JudgeOnline/bbs.php?pid=1731&cid=1003)][命题人:[qianyouyou](http://101.200.61.174/JudgeOnline/userinfo.php?user=qianyouyou)][[Edit](http://101.200.61.174/JudgeOnline/admin/problem_edit.php?id=1731&getkey=1EA9CD4E34)] [TestData]

## 题目描述

小琳，小花，小薇，yoyo，他们每个人手上有一堆牌，牌的张数分别为x1，x2，x3，x4，每张牌都不一样。现有n名同学，n=x1+x2+x3+x4。每名同学均需要一张牌，于是他们按顺序每人随机到四个人那里拿取牌顶的一张牌，最后一个人刚好拿到剩下的最后一张牌。排队拿牌的同学的顺序是固定的，选择拿谁的牌是不确定的。假如发牌的人手上的牌发完了，则要拿牌的同学会选择其他发牌的人。请问有多少种取法取走所有的牌。

## 输入

首行输入t，代表t组测试样例

对于每一行，输入四个整数a，b，c，d，输入为均不超过500的正整数

## 输出

对于每组样例，输出一个整数表示答案，答案对10^9+7取模

## 样例输入

1
5 4 2 3
## 样例输出

2522520
## 提示

本题作为思维题，并未用到stl，仅锻炼一下大家解决问题的能力。用到的数学知识相对多一点。

### 题解

题目大致可以理解为4堆牌a,b,c,d，每次从一堆牌里拿出牌顶的一张牌，问共有多少种拿法。

其实我们可以一堆一堆的分析，假设只有一堆a时，只有1种拿法，

那两堆a,b时我们可以认为是从a个牌中插入b张牌，用数学表达式就是C(b,a+b);

那么三堆的话我们可以把前两堆看成一堆，那么表达式就是C(c,a+b+c)，

这是我们需要与前两堆的组成方法相乘，就是C(b,a+b)C(c,a+b+c)。

4堆的话就是C(b,a+b)C(c,a+b+c)C(d,a+b+c+d)。所以答案就是C(a,a)C(b,a+b)C(c,a+b+c)C(d,a+b+c+d)。

此外，有一公式C(a,b) = C(a,b-1)+C(a-1,b-1)，所以我们用数组来代替C(m,n)操作

参考代码：

```cpp
#include<iostream>
#include<cstring>
#define ll long long
using namespace std;
const int maxn = 501;
const ll mod = 1000000007;
ll a[4], sum[4] = { 0 };
ll dp[maxn * 4][maxn * 4];
//打表，递推公式C(a,b) = C(a,b-1)+C(a-1,b-1)
void init() {
	dp[0][0] = 0;
	for (int i = 1; i < 4 * maxn; i++) {
		dp[i][0] = 1;
		for (int j = 1; j < i; j++) {
			dp[i][j] = dp[i - 1][j] + dp[i - 1][j - 1];
			dp[i][j] %= mod;
		}
		dp[i][i] = 1;
	}
}
int main() {
    int t;
    cin>>t;
    while(t--){
	init();//打表
	ll ans = 1;
	//这一步可要可不要，其实就是将a,a+b,a+b+c,a+b+c+d存进sum里
	for (int i = 0; i < 4; i++) {
		!i ? sum[i] = 0 : sum[i] = sum[i - 1];
		cin >> a[i];
		sum[i] += a[i];
		if (a[i] > sum[i] - a[i]) a[i] = sum[i] - a[i];
	}
	//将对应3组排列组合相乘，及C(b,a+b)C(c,a+b+c)C(d,a+b+c+d)
	for (int i = 1; i < 4; i++) {
		ans *= dp[sum[i]][a[i]];
		ans %= mod;
	}
	cout << ans << endl;}
	return 0;
}
```

自己写的代码：

```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxn=510,mod=1e9+7;
ll mp[maxn*4][maxn*4];

int main()
{
    mp[0][0]=0;
    for(int i=1;i<=2000;i++)
        mp[i][1]=i;
    for(int i=1;i<=2000;i++)
        for(int j=2;j<=2000;j++)
    mp[i][j]=((mp[i-1][j])%mod+(mp[i-1][j-1])%mod)%mod;
/*
    for(int i=1;i<=50;i++){
        for(int j=1;j<=50;j++){
            printf("%d ",mp[i][j]);
        }
    printf("\n");
    }
*/
    int T;
    cin>>T;
    while(T--){
        int a,b,c,d;
        scanf("%d%d%d%d",&a,&b,&c,&d);
        printf("%d\n",(((mp[a+b][b]%mod)*(mp[a+b+c][c]%mod))%mod*(mp[a+b+c+d][d]%mod))%mod);
    }
    return 0;
}
```





