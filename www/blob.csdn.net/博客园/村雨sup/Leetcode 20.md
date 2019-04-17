# Leetcode 20 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//嘻嘻，垃圾的我写了一个小时，各种bug，丑的要死
class Solution {
public:
    bool isValid(string s) {
        stack<char> st;
        if(s == "") return true;
        if(s[0] == ')' || s[0] == ']' || s[0] == '}') return false;
        st.push(s[0]);
        int cnt = 1;
        while(1){
            if(st.top() == '('){
                if(s[cnt] == ')'){st.pop();cnt++;}
                else if(s[cnt] == '['||s[cnt] == '{'||s[cnt] == '('){st.push(s[cnt++]);}
                else return false;
            }
            else if(st.top() == '['){
                if(s[cnt] == ']'){st.pop();cnt++;}
                else if(s[cnt] == '['||s[cnt] == '{'||s[cnt] == '('){st.push(s[cnt++]);}
                else return false;
            }
            else if(st.top() == '{'){
                if(s[cnt] == '}'){st.pop();cnt++;}
                else if(s[cnt] == '['||s[cnt] == '{'||s[cnt] == '('){st.push(s[cnt++]);}
                else return false;
            }
            else return false;

            if(cnt < s.size()&&st.empty()){st.push(s[cnt++]);}
            if(cnt >= s.size()) break;
        }
        if(st.empty()) return true;
        else return false;
    }
};
```

 别人写的：差距啊！

```
class Solution {
public:
    bool isValid(string s) {
        stack<char> parentheses;
        for (int i = 0; i < s.size(); ++i) {
            if (s[i] == '(' || s[i] == '[' || s[i] == '{') parentheses.push(s[i]);
            else {
                if (parentheses.empty()) return false;
                if (s[i] == ')' && parentheses.top() != '(') return false;
                if (s[i] == ']' && parentheses.top() != '[') return false;
                if (s[i] == '}' && parentheses.top() != '{') return false;
                parentheses.pop();
            }
        }
        return parentheses.empty();
    }
};
```













