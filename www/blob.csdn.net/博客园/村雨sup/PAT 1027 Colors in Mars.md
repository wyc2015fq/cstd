# PAT 1027 Colors in Mars - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1027 Colors in Mars （20 分)








People in Mars represent the colors in their computers in a similar way as the Earth people. That is, a color is represented by a 6-digit number, where the first 2 digits are for `Red`, the middle 2 digits for `Green`, and the last 2 digits for `Blue`. The only difference is that they use radix 13 (0-9 and A-C) instead of 16. Now given a color in three decimal numbers (each between 0 and 168), you are supposed to output their Mars RGB values.

### Input Specification:

Each input file contains one test case which occupies a line containing the three decimal color values.

### Output Specification:

For each test case you should output the Mars RGB value in the following format: first output `#`, then followed by a 6-digit number where all the English characters must be upper-cased. If a single color is only 1-digit long, you must print a `0` to its left.

### Sample Input:

```
15 43 71
```

### Sample Output:

```
#123456
```

```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define maxnum 100005

char s[3] = {'A','B','C'};


int main(){


    int a[3];
    for(int i=0;i < 3;i++) cin >> a[i];
    cout << "#";
    for(int i=0;i < 3;i++){
        vector<char> vec;
        if(a[i] == 0){cout << "00";continue;}
        while(a[i]){
            int num = a[i]%13;
            if(num < 10){vec.push_back(num+'0');}
            else vec.push_back(s[num%10]);
            a[i] = a[i]/13;
        }
        if(vec.size() == 1) cout << 0;
        for(int i=vec.size()-1;i >= 0;i--){
            cout << vec[i];
        }
    }
    return 0;
}
```

——再次注意0这个数据，在进制转换中一直会出。
` `













