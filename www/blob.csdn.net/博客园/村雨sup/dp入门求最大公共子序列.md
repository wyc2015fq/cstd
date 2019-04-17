# dp入门求最大公共子序列 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
#include "bits/stdc++.h"
using namespace std;
char a[5000],b[5000];
int num[5000][5000];
int main()
{
    cin >> a >> b;
    int lena = strlen(a);
    int lenb = strlen(b);

    memset(num,0, sizeof(num));

    for(int i=1;i <= lena;i++){
        for(int j=1;j <= lenb;j++){
            if(a[i-1] == b[j-1])
                num[i][j] = num[i-1][j-1]+1;
            else if(num[i][j-1] > num[i-1][j])
                num[i][j] = num[i][j-1];
            else
                num[i][j] = num[i-1][j];
        }
    }
    cout << num[lena][lenb];
}
```

第一次没有完全理解到底为什么这样做，很无奈，看了许多博文也还是不懂，勉强靠记忆，自己重写了一遍











