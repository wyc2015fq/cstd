# 超级赛亚ACMer（HDU-5246） - Alex_McAvoy的博客 - CSDN博客





2019年01月24日 11:06:50[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：83
个人分类：[HDU																[基础算法——贪心](https://blog.csdn.net/u011815404/article/category/7628972)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

百小度是一个ACMer，也是一个超级赛亚人，每个ACMer都有一个战斗力，包括百小度。 所谓超级赛亚人的定义，是说如果在对抗中刚好接近极限状态，那就会激发斗志，实力提升.

具体来说，就是百小度现在要接受一些ACMer的挑战了，这些ACMer有n个人，第i个人的战斗力是a[i]。

 百小度接下来可以自主安排与这n个ACMer的PK顺序，他要想在PK赛中赢过另外一个ACMer，就必须使得自己的战斗力不小于对方（平局情况他会按照百小度字典上的规则把自己排在第一).

如果百小度的战斗力大于对方，那么百小度就会轻易获胜，得不到锻炼并且骄傲起来，他以后的战斗力将保持在这个值，再也不会发生改变。如果百小度的战斗力等于对方，那么百小度在获胜的同时也会感到很吃力，但是这会激发百小度的斗志，使得他刻苦刷题，在下场PK赛之前，战斗力最多提升k点（即可以提升0~k点任意值）.

k是百小度的潜力提升上限，会被给定一个初始值，这个潜力提升上限k在后面的比赛中会下降.

每战胜一个ACMer，这个潜力上限k将减少1（因为超级赛亚人百小度也会感到累），但k最低只会减少到0，即不会出现战斗力下降的情况。也就是第一次比赛如果激发了百小度的斗志，他能把战斗力提升0~k的任一值，如果第二次比赛继续被激发斗志，他能在第一次提升后的基础上，把战斗力再提升0 ~ max(0,k−1),依次类推…

m是百小度的初始战斗力上限，也就是百小度第一次进行PK赛的时候，可以选择0~m的任意一个值作为他的战斗力.

现在希望你编写程序，判断一下百小度是否战胜所有的ACMer.

# Input

输入包含多组数据（数据不超过500组)

第一行一个整数T，表示T组数据

对于每组数据，第一行包括三个整数n,m,k(1≤n≤104,1≤m,k≤108)

第二行包括n个正整数，表示彪形大汉的战斗力（战斗力为不超过1012的正整数）

# Output

对于每组数据，先输出一行Case #i: (1≤i≤T)

如果百小度能打败所有的ACMer，再输出"why am I so diao?"

否则再输出"madan!"

# Sample Input

**25 11 315 13 10 9 85 11 38 9 10 13 16**

# Sample Output

**Case #1:why am I so diao?Case #2:madan!**


————————————————————————————————————————————

思路：贪心题

将 n 个人按照战斗力从小到大排好序后，首先判断第一个人的战斗力是否大于 m，若大于则说明无法打败，若小于，就找战斗力大于 m 的第一个人的位置，记为 pos

由于起始战斗力可在 0~m 中任选，可以更新 m 的值，让他等于战斗力大于 m 的第一个人的前面的那个人，即令 m=a[pos-1]

然后判断当前的 m 加上 k 后是否大于第 pos 个人的战斗力，若可以，则更新 m，让 m 等于 pos 的人的战斗力，即 m=a[pos]，然后 k--，不断更新 m、k 的值，直到有一个人打不过退出或能打败所有人

注意使用 long long

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
#define PI acos(-1.0)
#define E 1e-6
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 10001
#define LL long long
using namespace std;
LL a[N];
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        LL n,m,k;
        scanf("%lld%lld%lld",&n,&m,&k);
        for(LL i=1;i<=n;i++)
            scanf("%lld",&a[i]);

        sort(a+1,a+1+n);

        printf("Case #%d:\n",Case++);
        if(m<a[1]){//给定的m小于最小值
            printf("madan!\n");
        }
        else{
            LL pos;
            for(LL i=1;i<=n;i++){
                if(a[i]>m){//记录第一个大于m的值的位置
                    pos=i;
                    break;
                }
            }

            m=a[pos-1];//更新初始m值
            bool flag=true;
            for(;pos<=n;pos++){
                if(a[pos]-k>m){//第一个大于m的值减去k的最大值仍大于m的值
                    printf("madan!\n");
                    flag=false;
                    break;
                }
                else{
                    m=a[pos];//更新m
                    k--;//更新k
                }
            }
            if(flag)
                printf("why am I so diao?\n");
        }
    }
	return 0;
}
```






