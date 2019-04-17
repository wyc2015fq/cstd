# PAT 1041 Be Unique - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1041 Be Unique （20 分)








Being unique is so important to people on Mars that even their lottery is designed in a unique way. The rule of winning is simple: one bets on a number chosen from [1]. The first one who bets on a unique number wins. For example, if there are 7 people betting on { 5 31 5 88 67 88 17 }, then the second one who bets on 31 wins.

### Input Specification:

Each input file contains one test case. Each case contains a line which begins with a positive integer 

### Output Specification:

For each test case, print the winning number in a line. If there is no winner, print `None` instead.

### Sample Input 1:

```
7 5 31 5 88 67 88 17
```

### Sample Output 1:

```
31
```

### Sample Input 2:

```
5 888 666 666 888 888
```

### Sample Output 2:

```
None
```

```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define MAXN 100005

int a[MAXN];



int main(){
    int n;cin >> n;
    map<int,int>mp;
    for(int i=0;i < n;i++){cin >> a[i];mp[a[i]]++;}
    int flag = 1;
    for(int i=0;i < n;i++){
        if(mp[a[i]] == 1){cout << a[i];flag=0;break;}
    }
    if(flag) cout << "None";
    return 0;
}
```

我，水题之王
` `













