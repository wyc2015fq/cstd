# nyoj762——分解质因数+容斥+二分 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






## 第k个互质数

时间限制：1000 ms  |  内存限制：65535 KB

难度：4



- 描述
两个数的a，b的gcd为1，即a，b互质，现在给你一个数m，你知道与它互质的第k个数是多少吗？与m互质的数按照升序排列。

- 输入
输入m ，k （1<=m<=1000000;1<=k<=100000000）- 输出
输出第k个数。- 样例输入
10 1
10 2
10 3- 样例输出
1
3
7- 上传者
[TC_常红立](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=TC_%E5%B8%B8%E7%BA%A2%E7%AB%8B)
```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int INF = 0x3f3f3f3f;
const int maxn = 1000;
const int moder = 1000000007;
int a[20];                

void fenjie(int n)
{
    int cnt = 0;
    for(int i=2;i <= sqrt(n);i++){
        if(n%i == 0){
            a[++cnt] = i;                 //每个i便是n的质因数
            while(n%i == 0) n = n/i;
        }
    }
    if(n > 1)a[++cnt] = n;
    a[0] = cnt;
}

// 计算[1, n]内与m互质的数的个数
int que[1<<10];
int Count(int n, int m) {
    int g = 0, sum = n;
    que[++g] = 1;
    for(int i = 1; i <= a[0]; ++i){
        int t = g;
        for(int j = 1; j <= g; ++j){                //容斥。。
            que[++t] = que[j] * a[i] * -1;
            sum += n / que[t];
        }
        g = t;
    }
    return sum;
}
int Binary_search(int m, int K){
    int l = 1, r = 2000000000, mid;
    while(l <= r){
        mid = (l + r) >> 1;
        if(Count(mid, m) >= K) r = mid - 1;
        else l = mid + 1;
    }
    return l;
}
int main ()
{
    int m,k;
    while(~scanf("%d%d",&m,&k)){
        memset(a,0,sizeof(a));
        fenjie(m);
        int ans = Binary_search(m,k);
        printf("%d\n",ans);
    }
    return 0;
}
```

——这题也太变态了吧，看了题解也是半懂不懂

思路：

首先要知道，

    在[1,m]之间与m互质的数的个数=[1,m]之间的总个数-[1,m]之间与n不互质的数的个数



所以，要先对m进行质因数分解，求出m有哪些质因数，然后二分枚举答案mid，用容斥求[1,mid]内与m互质的数有多少个，让其结果与k比较



判断的时候，[1,mid]之间与m互质的数的数量 = mid - （包含一个质因子的数的个数）+（包含2个质因子的数的个数）-（包含3个质因子的数的个数）+（包含4个质因子的数的个数）.....











