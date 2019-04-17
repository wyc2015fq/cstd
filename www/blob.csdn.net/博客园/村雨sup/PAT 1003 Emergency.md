# PAT 1003 Emergency - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1003 Emergency （25 分)








As an emergency rescue team leader of a city, you are given a special map of your country. The map shows several scattered cities connected by some roads. Amount of rescue teams in each city and the length of each road between any pair of cities are marked on the map. When there is an emergency call to you from some other city, your job is to lead your men to the place as quickly as possible, and at the mean time, call up as many hands on the way as possible.

### Input Specification:

Each input file contains one test case. For each test case, the first line contains 4 positive integers: 

### Output Specification:

For each test case, print in one line two numbers: the number of different shortest paths between 

### Sample Input:

```
5 6 0 2
1 2 1 5 3
0 1 1
0 2 2
0 3 1
1 2 1
2 4 1
3 4 1
```

### Sample Output:
`2 4`


```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
int mp[505][505] = {0};
int vis[505] = {0};
int chenshi,daolu,start,over;
int val[505] = {0};
int cnt=0;
int minval = INT_MAX;
int maxnum = INT_MIN;

void dfs(int x,int nowval,int nownum){
    if(x == over){
        if(nowval < minval){minval = nowval;cnt=1;maxnum = nownum;}
        else if(nowval == minval){cnt++;maxnum = max(maxnum,nownum);}
        return;
    }

    for(int i=0;i < chenshi;i++){
        if(!vis[i]&&mp[x][i]){
            vis[i] = 1;
            dfs(i,nowval+mp[x][i],nownum+val[i]);
            vis[i] = 0;
        }
    }

    return;
}



int main(){

    cin >> chenshi >> daolu >> start >> over;
    vis[start] = 1;

    for(int i=0;i < chenshi;i++) cin >> val[i];

    while(daolu--){
        int i,j,n;
        cin >> i >> j;cin >> n;
        mp[i][j] = mp[j][i] = n;
    }

    dfs(start,0,val[start]);
    cout << cnt << " " << maxnum;

    return 0;
}
```

一发入魂，我太牛逼了。

复习了图的dfs，一开始漏掉了是无向图的条件。














