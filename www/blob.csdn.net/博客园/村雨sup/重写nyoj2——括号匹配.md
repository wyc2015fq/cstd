# 重写nyoj2——括号匹配 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
#include "bits/stdc++.h"
using namespace std;

int comp(char s1,char s2){
    if((s1 == '(' && s2 == ')') || (s1 == '['&&s2 == ']')) return 1;
    if((s1 == '(' && s2 == '(')||(s1 == '(' && s2 == '[')||(s1 == '[' && s2 == '(')||(s1 == '[' && s2 == '[')) return 2;
}



int main()
{
    int t;
    cin >> t;
    while(t--){
        string s;
        cin >> s;
        stack<char> st;
        int flag = 0;
        for(int i=0;i < s.size();i++){
            if(st.empty()){
                if(s[i] == '('||s[i] == '[') st.push(s[i]);
                else{
                    flag = 1;break;
                }
            }
            else{
                if(comp(st.top(),s[i]) == 1){
                    st.pop();
                }
                else{
                    st.push(s[i]);
                }
            }
        }
        if(!st.empty()||flag == 1) cout << "No" << endl;
        else cout << "Yes" << endl;
    }
    return 0;
}
```

今天复习了数据结构，突然想重写这个问题，只写了10分钟吧，就AC了，记得第一次写的时候写了两个小时，代码又臭又长，有时候代码能力就是这么不知不觉的增长。。。

分两种情况：1.栈空 2 栈非空

最后判断是否break出来的或者栈中有残余











