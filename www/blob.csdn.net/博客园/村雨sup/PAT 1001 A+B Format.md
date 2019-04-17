# PAT 1001 A+B Format - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1001 A+B Format （20 分)








Calculate 

### Input Specification:

Each input file contains one test case. Each case contains a pair of integers 

### Output Specification:

For each test case, you should output the sum of 

### Sample Input:

```
-1000000 9
```

### Sample Output:

```
-999,991
```

```
#include<iostream>
#include<cstring>
#include<string>
#include<vector>
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

int main(){
    ll a,b;
    cin >> a >> b;
    ll c = a+b;
    if(c == 0){cout << 0;return 0;}
    if(c<0){
        cout << "-";
        c = -c;
    }

    vector<int> vec;

    int cnt = 0;
    while(c){
        if(cnt%3 == 0)vec.push_back(-1);
        vec.push_back(c%10);
        c = c/10;
        cnt++;
    }


    for(int i=vec.size()-1;i>0;i--){
        if(vec[i]==-1)cout << ",";
        else cout << vec[i];
    }

//    for(auto num:vec) cout << num << " ";



    return 0;
}
```

都用leetcode风格写，nice！
` `













