# PAT 1005 Spell It Right - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1005 Spell It Right （20 分)








Given a non-negative integer 

### Input Specification:

Each input file contains one test case. Each case occupies one line which contains an 

### Output Specification:

For each test case, output in one line the digits of the sum in English words. There must be one space between two consecutive words, but no extra space at the end of a line.

### Sample Input:

```
12345
```

### Sample Output:
`one five`


```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

vector<string> vec={{"zero"},{"one"},{"two"},{"three"},{"four"},{"five"},{"six"},{"seven"},{"eight"},{"nine"}};

vector<int> temp;


int main(){
    string s;
    cin >> s;
    ll sum = 0;
    for(int i=0;i < s.size();i++){
        sum += (s[i]-'0');
    }
    if(sum == 0) cout << vec[0];
    while(sum){
        temp.push_back(sum%10);
        sum = sum/10;
    }
    for(int i=temp.size()-1;i>=0;i--){
        cout << vec[temp[i]];
        if(i) cout << " ";
    }


    return 0;
}
```

sum = 0的情况要注意，不会进入循环。














