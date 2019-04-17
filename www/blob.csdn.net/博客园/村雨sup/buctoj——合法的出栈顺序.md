# buctoj——合法的出栈顺序 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





## 题目描述

我们知道，一个入栈序列是{1,2,3}的合法出栈序列有{3,2,1}，{1,2,3}等，而{3,1,2}是不合法的.现在冰语有一个长度为n的序列A(保证序列内数字唯一,且1<=A[i]<=n)。他想知道这个序列是不是入栈顺序{1,2,3,...n}的合法出栈序列，你能告诉他么?

## 输入

第一行为T，表示样例个数
每个样例第一行为n，第二行为n个数
1<=T<=1e4,3<=n<=100

## 输出

对于每个样例输出一行,合法输出"YES"，反之输出"NO",

## 样例输入
1
3
3 1 2

```
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1000;

int a[maxn];

int main()
{
    int t;
    scanf("%d",&t);
    while(t--){
        int n;
        scanf("%d",&n);
        memset(a,0,sizeof(a));
        for(int i=1;i <= n;i++){
            scanf("%d",&a[i]);
        }
        stack<int>st;
        stack<int>res;
        for(int i=n;i >= 1;i--){
            st.push(a[i]);
        }
        for(int i=1;i <= n;i++){
            res.push(i);
            while(st.size()&&res.size()&&res.top()==st.top()){
                st.pop();
                res.pop();
            }
        }
        if(!st.size()){
            printf("YES\n");
        }
        else
            printf("NO\n");
    }
}
```

——











