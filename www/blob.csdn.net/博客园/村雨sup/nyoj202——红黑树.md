# nyoj202——红黑树 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





为了看懂这条题我还专门去看了看红黑树，结果大佬告诉我：左旋右旋不会影响中序遍历......

然后就写了个简单的中序遍历......

```
#include <bits/stdc++.h>
using namespace std;
const int maxn = 20;

struct node
{
    int data;
    int lchild,rchild;
}nd[maxn];

void mid_search(int k)
{
    if(k != -1){
        mid_search(nd[k].lchild);
        printf("%d\n",nd[k].data);
        mid_search(nd[k].rchild);
    }
}




int main()
{
    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;
        memset(nd,0, sizeof(nd));
        for(int i=0;i < n;i++){
            int a,b,c;
            cin >> a >> b >> c;
            nd[a].data = a;
            nd[a].lchild = b;
            nd[a].rchild = c;
        }
        int m;
        cin >> m;
        for(int i=0;i < m;i++){
            int x,y;
            cin >> x >> y;
        }
        mid_search(0);
    }
    return 0;
}
```

——改日学一下红黑树的写法（FLAG）











