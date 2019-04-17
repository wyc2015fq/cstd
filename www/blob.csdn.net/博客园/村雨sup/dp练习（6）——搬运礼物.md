# dp练习（6）——搬运礼物 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







### **3409 搬运礼物**

CodeVS原创





 时间限制: 1 s



 空间限制: 64000 KB



 题目等级 : 青铜 Bronze




[题解](http://codevs.cn/wiki/solution/?problem_id=3409)












题目描述 Description


小浣熊松松特别喜欢交朋友，今年松松生日，就有N个朋友给他送礼物。可是要把这些礼物搬回家是一件很困难的事，具体来说，如果松松一次搬运x件礼物，就要花费w[x]的体力（显而易见，有w[x]<=w[x+1]，搬得越多耗费体力越多）。松松并不在意他会搬多少次，但是他想知道，自己最少花费多少体力，就可以把礼物全部搬回家。




输入描述 Input Description


第一行包含一个整数N，代表有N个朋友给松松送礼物。

接下来N行，第i行表示一次搬运i件礼物花费的体力w[i]。




输出描述 Output Description


输出仅一行，为松松搬运这N件礼物最少耗费的体力。




样例输入 Sample Input


3

1

4

7




样例输出 Sample Output


3




数据范围及提示 Data Size & Hint


对于100%的数据，0<=N<=5000，0<=w[x]<=50000。



```
#include<bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin >> n;
    int a[n];
    for(int i=1;i <= n;i++)
        cin >> a[i];


    
    for(int i=2;i <= n;i++)
    {
        for(int j=1;j <= i-1;j++)
        {
            a[i] = min(a[i],a[j] + a[i-j]);
        }
    }
    cout << a[n] <<endl;
    return 0;

}
```

这里直接用数组a，因为基础情况就是所给的值。

DP：首先一定要相信，你所运算的那一个格前面的所有的格都是已经取到了最优解，然后结合暴力的思想。最后复杂度为O(n^2).
















