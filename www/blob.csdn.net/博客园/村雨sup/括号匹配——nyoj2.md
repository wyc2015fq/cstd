# 括号匹配——nyoj2 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





感觉自己的逻辑就像屎一样，这么简单的题目写了2个小时，以后写题还是要在纸上先列好提纲，不然如果你直接上机，遇到n多个bug的时候，容易迷失自我，去拆东补西的修bug而忽视了整片代码的逻辑的正确性。

在写这题的时候，我在while中定义了while，简直智障，要想到while本身就是一个循环，放着不用充当if判断语句真是搞笑。

```
#include <bits/stdc++.h>
using namespace std;

int candis(char x,char y) //能相消
{
    if(x == '('){
        if(y == ')') return 1;
        else return 0;
    }
    if(x == '['){
        if(y == ']') return 1;
        else return 0;
    }
    return 0;
}

int canadd(char x,char y) //能相加
{
    if(x == '['){
        if(y == '('||y == '[') return 1;
        else return 0;
    }
    if(x == '('){
        if(y == '['||y == '(') return 1;
        else return 0;
    }
    return 0;
}

int main()
{
    int t;
    cin >> t;
    while(t--){
        string s1;
        int flag = 1;
        int i=0;
        cin >> s1;
        stack<char>s;
        while(i < s1.length()){
            if(s.empty()&&(s1[i] == ')'||s1[i] == ']')){ //情况1
                flag = 0;
                break;
            }
            if(s.empty()){
                s.push(s1[i]);
            }
            else{
                if(candis(s.top(),s1[i])) { //如果能相消，则栈弹出一个
                    s.pop();
                }
                else if(canadd(s.top(),s1[i])){ //如果能相加则入栈一个
                    s.push(s1[i]);
                }
                else{                          //如果是非法遇到，则直接break
                    flag = 0;
                    break;
                }
            }
            i++;
        }
        if(!s.empty()) flag = 0;     //如果结束后栈中还有剩余，则no

        if(flag == 0) cout << "No" << endl;
        else cout << "Yes" << endl;
    }
    return 0;
}
```

——











