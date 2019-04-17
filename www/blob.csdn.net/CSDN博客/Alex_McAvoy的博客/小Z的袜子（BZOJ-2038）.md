# 小Z的袜子（BZOJ-2038） - Alex_McAvoy的博客 - CSDN博客





2019年03月05日 16:55:35[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：35








> 
# Problem Description

作为一个生活散漫的人，小Z每天早上都要耗费很久从一堆五颜六色的袜子中找出一双来穿。终于有一天，小Z再也无法忍受这恼人的找袜子过程，于是他决定听天由命……

具体来说，小Z把这N只袜子从1到N编号，然后从编号L到R(L 尽管小Z并不在意两只袜子是不是完整的一双，甚至不在意两只袜子是否一左一右，他却很在意袜子的颜色，毕竟穿两只不同色的袜子会很尴尬。

你的任务便是告诉小Z，他有多大的概率抽到两只颜色相同的袜子。当然，小Z希望这个概率尽量高，所以他可能会询问多个(L,R)以方便自己选择。

# **Input**

输入文件第一行包含两个正整数N和M。N为袜子的数量，M为小Z所提的询问的数量。接下来一行包含N个正整数Ci，其中Ci表示第i只袜子的颜色，相同的颜色用相同的数字表示。再接下来M行，每行两个正整数L，R表示一个询问。

# Output

包含M行，对于每个询问在一行中输出分数A/B表示从该询问的区间[L,R]中随机抽出两只袜子颜色相同的概率。若该概率为0则输出0/1，否则输出的A/B必须为最简分数。（详见样例）

# Sample Input

**6 41 2 3 3 3 22 61 33 51 6**

# Sample Output

**2/50/11/14/15**

# Hint

【样例解释】

询问1：共C(5,2)=10种可能，其中抽出两个2有1种可能，抽出两个3有3种可能，概率为(1+3)/10=4/10=2/5。

询问2：共C(3,2)=3种可能，无法抽到颜色相同的袜子，概率为0/3=0/1。

询问3：共C(3,2)=3种可能，均为抽出两个3，概率为3/3=1/1。

注：上述C(a, b)表示组合数，组合数C(a, b)等价于在a个不同的物品中选取b个的选取方案数。

【数据规模和约定】

30%的数据中 N,M ≤ 5000；

60%的数据中 N,M ≤ 25000；

100%的数据中 N,M ≤ 50000，1 ≤ L < R ≤ N，Ci ≤ N。


思路：

对于每一个区间询问 [l,r]，要求的 ![ans=\frac{\sum^{i=n}_{i=1}(cnt[color[i]]-1)*\frac{cnt[color[i]]}{2}}{(r-l+1)*(r-l)}](https://private.codecogs.com/gif.latex?ans%3D%5Cfrac%7B%5Csum%5E%7Bi%3Dn%7D_%7Bi%3D1%7D%28cnt%5Bcolor%5Bi%5D%5D-1%29*%5Cfrac%7Bcnt%5Bcolor%5Bi%5D%5D%7D%7B2%7D%7D%7B%28r-l&plus;1%29*%28r-l%29%7D)

即：![ans=\frac{\sum^{i=n}_{i=1}(cnt[color[i]]^2)-(r-l-+1)}{(r-l+1)*(r-l)}](https://private.codecogs.com/gif.latex?ans%3D%5Cfrac%7B%5Csum%5E%7Bi%3Dn%7D_%7Bi%3D1%7D%28cnt%5Bcolor%5Bi%5D%5D%5E2%29-%28r-l-&plus;1%29%7D%7B%28r-l&plus;1%29*%28r-l%29%7D)

其中，![cnt[color[i]]](https://private.codecogs.com/gif.latex?cnt%5Bcolor%5Bi%5D%5D) 是第 i 种颜色在 [l,r] 中出现的次数

因此，只要求出各询问区间中 ![cnt[color[i]]^2](https://private.codecogs.com/gif.latex?cnt%5Bcolor%5Bi%5D%5D%5E2) 的值，即可得出答案

此外，n*m 最大为 50000*50000，注意使用 long long

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
#define E 1e-9
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=10007;
const int N=1000000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

struct Node{
    int l,r;//询问的左右端点
    int id;//询的编号
}q[N];
int n,m;
LL color[N];
int block;//分块
LL ans,cnt[N*2];
LL nume[N],deno[N];//分子、分母

bool cmp(Node a,Node b){//奇偶性排序
    return (a.l/block)^(b.l/block)?a.l<b.l:(((a.l/block)&1)?a.r<b.r:a.r>b.r);
}
LL GCD(LL x,LL y){
    return y==0?x:GCD(y,x%y);
}
void change(int pos,int num){
	ans+=2*cnt[color[pos]]*num+1;
	cnt[color[pos]]+=num;
}
int main(){
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;++i)
        scanf("%lld",&color[i]);
    for(int i=1;i<=m;i++){
        scanf("%d%d",&q[i].l,&q[i].r);
        q[i].id=i;
    }

    block=n/sqrt(m*2/3);//分块
    sort(q+1,q+m+1,cmp);//对询问进行排序

    LL l=1,r=0;
    ans=0;
    for(int i=1;i<=m;i++){
        int ql=q[i].l,qr=q[i].r;
        while(l<ql) change(l++,-1);
        while(r>qr) change(r--,-1);
        while(l>ql) change(--l,1);
        while(r<qr) change(++r,1);

        LL a=ans-(qr-ql+1);
        LL b=(LL)(qr-ql+1)*(qr-ql);

        LL gcd=GCD(a,b);
        nume[q[i].id]=a/gcd;
        deno[q[i].id]=b/gcd;
    }

    for(int i=1;i<=m;i++)
        printf("%lld/%lld\n",nume[i],deno[i]);

    return 0;
}
```





