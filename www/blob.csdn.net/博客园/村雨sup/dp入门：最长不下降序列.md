# dp入门：最长不下降序列 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
#include "bits/stdc++.h"
using namespace std;
int a[5000],dp[5000];
int main()
{
    int n;
    cin >> n;
    int a[n];
    int dp[n];
    for(int i = 0;i < n;i++){
        cin >> a[i];
        dp[i] = 1;
    }

    int maxm = 0;
    for(int i = 1;i < n;i++){
        for(int j = 0;j < i;j++){
            if(a[j] < a[i]){
                dp[i] = max(dp[i],dp[j]+1);
            }
        }
        if(maxm < dp[i]){
            maxm = dp[i];
        }
    }
    cout << maxm;
    return 0;
}
```

一开始把题意理解错了，以为是连续不下降的，wa了好几次，最后看了题解搞了好久才明白。











