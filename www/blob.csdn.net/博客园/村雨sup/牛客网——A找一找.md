# 牛客网——A找一找 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





链接：[https://www.nowcoder.net/acm/contest/71/A](https://www.nowcoder.net/acm/contest/71/A)
来源：牛客网



## 题目描述


给定n个正整数，请找出其中有多少个数x满足：在这n个数中存在数y=kx，其中k为大于1的整数

## 输入描述:

第一行输入一个n

接下来一行输入n个正整数ai

## 输出描述:
输出符合条件个数

示例1



## 输入

5
1 2 3 4 5



## 输出

2



## 说明

5个数中1和2符合条件，1是后面每个数的因子，2是4的因子




## 备注:
1≤n,ai≤1000000
```
#include<bits/stdc++.h>
using namespace std;
typedef long long  ll;

const int inf = 0x3f3f3f3f;
const int maxn = 40000 + 20;
const int moder = 1e9 + 7;
const int K = 256;
const int MAXN=1000005;
int cnt[MAXN];

int main()
{
    int n;
    cin >> n;
    for(int i=0;i < n;i++)
    {
        int a;
        cin >> a;
        cnt[a]++;
    }

    int num = 0;
    for(int i=1;i < MAXN;i++)
    {
        if(cnt[i] > 0)
        {
            for(int j=2*i;j < MAXN;j=j+i)
            {
                if(cnt[j] > 0)
                {
                    num += cnt[i];
                    break;
                }

            }
        }
    }
    cout << num << endl;
    return 0;
}
```

需要注意的是输入的i可能会重复因此是用 cnt [i] ++;












