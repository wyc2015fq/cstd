# PAT 1031 Hello World for U - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1031 Hello World for U （20 分)








Given any string of 

```
h  d
e  l
l  r
lowo
```

That is, the characters must be printed in the original order, starting top-down from the left vertical line with 

### Input Specification:

Each input file contains one test case. Each case contains one string with no less than 5 and no more than 80 characters in a line. The string contains no white space.

### Output Specification:

For each test case, print the input string in the shape of U as specified in the description.

### Sample Input:

```
helloworld!
```

### Sample Output:

```
h   !
e   d
l   l
lowor
```

```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define maxnum 100005

char ans[85][85];

int main(){
    memset(ans,' ', sizeof(ans));
    string s;
    cin >> s;
    int N = s.size();
    int n1,n2,n3;
    n1 = n3 = (N+2)/3;
    n2 = (N+2)%3 + n1;
    for(int i=0;i < n1;i++){
        ans[i][0] = s[i];
    }
    for(int i=0;i < n2;i++){
        ans[n1-1][i] = s[n1-1+i];
    }
    for(int i=n3-1;i >= 0;i--){
        ans[i][n2-1] = s[n1+n2-2+(n3-1-i)];
    }


    for(int i=0;i < n1;i++){
        for(int j=0;j < n2;j++){
            cout << ans[i][j];
        }
        if(i != n1)cout << endl;
    }


    return 0;
}
```

当提交出现全错的情况说明输出格式错了，比如这题，没有memset char数组是不对的，以后所有数组记得要初始化。
` `













