# hdu6438——Buy and Resell - westbrook1998的博客 - CSDN博客





2018年09月29日 22:36:40[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：22标签：[贪心](https://so.csdn.net/so/search/s.do?q=贪心&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
The Power Cube is used as a stash of Exotic Power. There are n cities numbered 1,2,…,n where allowed to trade it. The trading price of the Power Cube in the i-th city is ai dollars per cube. Noswal is a foxy businessman and wants to quietly make a fortune by buying and reselling Power Cubes. To avoid being discovered by the police, Noswal will go to the i-th city and choose exactly one of the following three options on the i-th day:
- spend ai dollars to buy a Power Cube
- resell a Power Cube and get ai dollars if he has at least one Power Cube
- do nothing

Obviously, Noswal can own more than one Power Cubes at the same time. After going to the n cities, he will go back home and stay away from the cops. He wants to know the maximum profit he can earn. In the meanwhile, to lower the risks, he wants to minimize the times of trading (include buy and sell) to get the maximum profit. Noswal is a foxy and successful businessman so you can assume that he has infinity money at the beginning.

Input

There are multiple test cases. The first line of input contains a positive integer T (T≤250), indicating the number of test cases. For each test case:

The first line has an integer n. (1≤n≤105)

The second line has n integers a1,a2,…,an where ai means the trading price (buy or sell) of the Power Cube in the i-th city. (1≤ai≤109)

It is guaranteed that the sum of all n is no more than 5×105.

Output

For each case, print one line with two integers —— the maximum profit and the minimum times of trading to get the maximum profit.

Sample Input

3

4

1 2 10 9

5

9 5 9 10 5

2

2 1

Sample Output

16 4

5 2

0 0

Hint

In the first case, he will buy in 1, 2 and resell in 3, 4.

profit = - 1 - 2 + 10 + 9 = 16

In the second case, he will buy in 2 and resell in 4.

profit = - 5 + 10 = 5

In the third case, he will do nothing and earn nothing.

profit = 0
给一列数，遇到小的就买，遇到大的就卖，但必须手头现在买有东西，求最小的买卖次数和最大的利润

贪心的思想，用一个从小到大的优先队列来存，每遇到一个数都加入优先队列，然后判断是否大于队列的最小值（贪心思想，要利润最大），如果不是就这样过，如果是，操作次数cnt加1，然后总利润就加上这个差值，然后还有一个vis数组用来记录这个卖出价格是否出现过，如果是比如1买4卖 4买8买就可以省去一个操作，所以这样处理完就把这个队列最小值去掉，再加上新的值

比如样例2

9 5 9 10 5模拟一下，到第三个数的时候队列中会有3个9，其中两个是作为买入价格的9，另一个是卖出的价格作为中间价格
代码：

```
#include <cstdio>
#include <algorithm>
#include <queue>
#include <cstring>
#include <map>
using namespace std;
typedef long long ll;
const int N=1e9+50;
int t,n;
int main(void){
    scanf("%d",&t);
    while(t--){
        scanf("%d",&n);
        priority_queue<int,vector<int>,greater<int>> q;
        map<int,int> vis;
        ll ans=0;
        ll cnt=0;
        int x;
        for(int i=0;i<n;i++){
            scanf("%d",&x);
            q.push(x);
            if(q.top()<x){
                cnt++;
                ans+=x-q.top();
                if(vis[q.top()]){
                    vis[q.top()]--;
                    cnt--;
                }
                q.pop();
                q.push(x);
                vis[x]++;
            }
        }
        printf("%lld %lld\n",ans,cnt*2);
    }
    return 0;
}
```






