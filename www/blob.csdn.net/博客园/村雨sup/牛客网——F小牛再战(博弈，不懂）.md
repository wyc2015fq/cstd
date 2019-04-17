# 牛客网——F小牛再战(博弈，不懂） - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





链接：[https://www.nowcoder.net/acm/contest/75/F](https://www.nowcoder.net/acm/contest/75/F)
来源：牛客网



## 题目描述



共有N堆石子，已知每堆中石子的数量，两个人轮流取石子，每次只能选择N堆石子中的一堆取一定数量的石子（最少取一个），取过子之后，还可以将该堆石子中剩余的石子随意选取几个放到其它的任意一堆或几堆上。等哪个人无法取子时就表示此人输掉了游戏。注意：一堆石子没有子之后，就不能再往此处放石子了。

假设每次都是小牛先取石子，并且游戏双方都绝对聪明，现在给你石子的堆数、每堆石子的数量，请判断出小牛能否获胜。


## 输入描述:
可能有多组测试数据(测试数据组数不超过1000)
每组测试数据的第一行是一个整数，表示N(1<=N<=10)
第二行是N个整数分别表示该堆石子中石子的数量。（每堆石子数目不超过100）
当输入的N为0时，表示输入结束
## 输出描述:
对于每组测试数据，输出Win表示小牛可以获胜，输出Lose表示小牛必然会败。

示例1



## 输入

3
2 1 3
2
1 1
0



## 输出

Win
Lose




## 备注:
提示：
例如：如果最开始有4堆石子，石子个数分别为3 1 4 2，而小牛想决定要先拿走第三堆石子中的两个石子（石子堆状态变为3 1 2 2），然后他可以使石子堆达到的状态有以下几种：
3 1 2 2（不再移动石子）
4 1 1 2（移动到第一堆一个）
3 2 1 2（移动到第二堆一个）
3 1 1 3（移动到第四堆一个）
5 1 0 2（全部移动到第一堆）
3 3 0 2（全部移动到第二堆）
3 1 0 4（全部移动到最后）
```
#include<bits/stdc++.h>
using namespace std;
#define ull unsigned long long
typedef long long  ll;
const int inf = 0x3f3f3f3f;
const int moder = 1e9 + 7;
const int MAXN=1000000;


int main()
{
    int n;
    while(cin >> n)
    {
        int a[101];
        memset(a,0,sizeof(a));
        if(n == 0) break;
        int t;
        for(int i=0;i < n;i++)
        {
            cin >> t;
            a[t]++;
        }
        int flag = 0;

        for(int i=0;i < 101;i++)
        {
            if(a[i]%2 == 1)
            {
                flag = 1;
                break;
            }
        }
        if(flag == 1) cout << "Win" << endl;
        else cout << "Lose" << endl;

    }


    return 0;
}
```

代码意思是找出堆中相同的个数为奇数的，就赢了。












