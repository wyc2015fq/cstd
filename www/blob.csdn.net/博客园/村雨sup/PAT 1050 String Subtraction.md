# PAT 1050 String Subtraction - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1050 String Subtraction （20 分)








Given two strings 

### Input Specification:

Each input file contains one test case. Each case consists of two lines which gives 

### Output Specification:

For each test case, print 

### Sample Input:

```
They are students.
aeiou
```

### Sample Output:

```
Thy r stdnts.
```

```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define MAXN 10005

map<char,int> mp;
vector<char> vec;


int main(){
    string s1,s2;
    getline(cin,s1);
    getline(cin,s2);

    for(int i=0;i < s2.size();i++){
        mp[s2[i]]++;
    }


    for(int i=0;i < s1.size();i++){
        if(!mp[s1[i]]) cout << s1[i];
    }
    return 0;
}
```

——
` `













