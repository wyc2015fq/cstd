# PAT 1046 Shortest Distance - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1046 Shortest Distance （20 分)








The task is really simple: given 

### Input Specification:

Each input file contains one test case. For each case, the first line contains an integer 

### Output Specification:

For each test case, print your results in 

### Sample Input:

```
5 1 2 4 14 9
3
1 3
2 5
4 1
```

### Sample Output:

```
3
10
7
```

```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define MAXN 100005

int a[MAXN];
ll dis[MAXN] = {0};



int main(){
    int n;cin >> n;
    ll sum = 0;
    for(int i=0;i < n;i++){
        cin >> a[i];sum+=a[i];
        dis[i+1] = sum;
    }
    int t;cin >> t;
    int cnt = 0;
    while(t--){

        int x,y;cin >> x >> y;
        if(x == y){cout << 0 << endl;continue;}
        if(x > y)swap(x,y);
        ll sum1 = dis[y-1] - dis[x-1];

        ll sum2 = dis[x-1] - dis[0] + dis[n] - dis[y-1];

        cout << min(sum1,sum2) << endl;

    }
    return 0;
}
```

两点之间距离是距0的距离之差，线段树和树状数组差不多就这个性质吧。
` `













