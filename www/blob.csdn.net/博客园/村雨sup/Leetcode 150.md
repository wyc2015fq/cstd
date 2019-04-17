# Leetcode 150 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        stack<int> st;
        for(int i=0;i < tokens.size();i++){
            char temp = tokens[i][tokens[i].size()-1];
            if(temp >= '0' && temp <= '9'){
                st.push(trams(tokens[i]));
            }
            else{
                int a = st.top();
                st.pop();
                int b = st.top();
                st.pop();
                int res = 0;
                if(tokens[i] == "+") res = a+b;
                if(tokens[i] == "-") res = b-a;
                if(tokens[i] == "*") res = a*b;
                if(tokens[i] == "/") res = b/a;
                st.push(res);
            }
        }
        return st.top();
    }
    int trams(string s){
        int res = 0;
        int flag = 0;
        int j = 0;
        if(s[0] == '-'){
            flag = 1;
            j = 1;
        }
        for(int i=j;i < s.size();i++){
            char temp = s[i];
            res = res*10 + (temp-'0');
        }
        if(flag == 1) res = 0-res;
        return res;
    }
};
```

字符串string几大坑人之处：1. 必须要用char = s[0]才能进行比较和变数字（char - '0'）

　　　　　　　　　　　　  2.s == " " (双冒号） s[0] == ' ' (单冒号）











