# PAT 1008 Elevator - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1008 Elevator （20 分)








The highest building in our city has only one elevator. A request list is made up with 

For a given request list, you are to compute the total time spent to fulfill the requests on the list. The elevator is on the 0th floor at the beginning and does not have to return to the ground floor when the requests are fulfilled.

### Input Specification:

Each input file contains one test case. Each case contains a positive integer 

### Output Specification:

For each test case, print the total time on a single line.

### Sample Input:

```
3 2 3 1
```

### Sample Output:

```
41
```

```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;



int main(){
    int n;
    cin >> n;
    int sum = n*5;
    int cur = 0;

    for(int i=0;i < n;i++){
        int x; cin >> x;
        int t = x-cur;
        sum += (t>0)?t*6:(-t)*4;
        cur = x;
    }
    cout << sum;


    return 0;
}
```

？这题也太水了吧
` `













