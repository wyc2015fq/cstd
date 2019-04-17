# nyoj117——树状数组升级版（树状数组+离散化） - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






## 求逆序数

时间限制：2000 ms  |  内存限制：65535 KB

难度：5



- 描述

在一个排列中，如果一对数的前后位置与大小顺序相反，即前面的数大于后面的数，那么它们就称为一个逆序。一个排列中逆序的总数就称为这个排列的逆序数。

现在，给你一个N个元素的序列，请你判断出它的逆序数是多少。

比如 1 3 2 的逆序数就是1。


- 输入
第一行输入一个整数T表示测试数据的组数（1<=T<=5)
每组测试数据的每一行是一个整数N表示数列中共有N个元素（2〈=N〈=1000000）
随后的一行共有N个整数Ai(0<=Ai<1000000000)，表示数列中的所有元素。

数据保证在多组测试数据中，多于10万个数的测试数据最多只有一组。- 输出
输出该数列的逆序数- 样例输入
2
2
1 1
3
1 3 2- 样例输出
0
1- 来源
[[张云聪]原创](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=%5B%E5%BC%A0%E4%BA%91%E8%81%AA%5D%E5%8E%9F%E5%88%9B)- 上传者
[张云聪](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=%E5%BC%A0%E4%BA%91%E8%81%AA)


```
#include "bits/stdc++.h"
using namespace std;
typedef long long ll;
const int INF = 0x3f3f3f3f;
const int maxn = 1000010;

int c[maxn];
int d[maxn];
int n;

struct node
{
    ll val;
    int pos;
}nd[maxn];

bool cmpp(const node &a, const node &b)
{
    if(a.val == b.val)return a.pos < b.pos;
    return a.val < b.val;
}

int lowbit(ll x)
{
    return x&(-x);
}

int query(ll x)
{
    int res = 0;
    while(x){
        res += d[x];
        x -= lowbit(x);
    }
    return res;
}

void add(int x,int v)
{
    while(x <= n){
        d[x] += v;
        x += lowbit(x);
    }
}


int main()
{
    int t;
    scanf("%d",&t);
    while(t--) {
        memset(nd,0,sizeof(nd));
        memset(d,0,sizeof(d));
        memset(c,0,sizeof(c));
        scanf("%d",&n);
        for(int i=1;i <= n;i++){
            int x;
            scanf("%d",&x);
            nd[i].val = x;
            nd[i].pos = i;
        }
        sort(nd+1,nd+n+1,cmpp);//数组离散化↓↓
        for(int i=1;i <= n;i++){
            c[nd[i].pos] = i;
        }                     //         ↑↑
        ll res = 0;
        for(int i=1;i <= n;i++){
            add(c[i],1);
            res += i - query(c[i]);
        }
        printf("%lld\n",res);
    }
    return 0;
}
```

---数组肯定是不能开到1000000000的，这时候我们就需要离散化处理。

其实离散化很简单，我们使用离散化的时候，元素之间只有大小关系，比如逆序对，因此我们可以将数组疯狂压缩，输入多少个元素，其实就只需要开多大的数组。











