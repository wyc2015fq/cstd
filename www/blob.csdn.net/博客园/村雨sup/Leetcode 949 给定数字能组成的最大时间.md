# Leetcode 949. 给定数字能组成的最大时间 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 949. 给定数字能组成的最大时间

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-113/problems/largest-time-for-given-digits/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-113/)







- 用户通过次数125
- 用户尝试次数213
- 通过次数127
- 提交次数774
- 题目难度Easy



给定一个由 4 位数字组成的数组，返回可以设置的符合 24 小时制的最大时间。

最小的 24 小时制时间是 00:00，而最大的是 23:59。从 00:00 （午夜）开始算起，过得越久，时间越大。

以长度为 5 的字符串返回答案。如果不能确定有效时间，则返回空字符串。



示例 1：
输入：[1,2,3,4]
输出："23:41"

示例 2：
输入：[5,5,5,5]
输出：""



提示：
- `A.length == 4`
- `0 <= A[i] <= 9`

```
class Solution {
public: 
    string ans = "";
    string largestTimeFromDigits(vector<int>& A) {
        check(A[0],A[1],A[2],A[3]);
        check(A[0],A[2],A[1],A[3]);
        check(A[0],A[3],A[1],A[2]);
        check(A[1],A[2],A[0],A[3]);
        check(A[1],A[3],A[2],A[0]);
        check(A[2],A[3],A[1],A[0]);
        return ans;
    }
    
    void check(int a,int b,int c,int d){
        string first = best(a,b,24);
        string second = best(c,d,60);
        if(first == ""||second == "")return;
        string cand = first + ":" + second;
        if(bijiao(cand,ans))ans = cand;
        return;
    }
    
    bool bijiao(string cand,string ans){
        if(ans == "")return 1;
        int a = (cand[0]-'0')*1000 + (cand[1]-'0')*100 + (cand[3]-'0')*10 + (cand[4]-'0');
        int b = (ans[0]-'0')*1000 + (ans[1]-'0')*100 + (ans[3]-'0')*10 + (ans[4]-'0');
        return a > b;
    }
    string caonima(int num){
        string res = "0";
        res += (num+'0');
        return res;
    }
    
    string best(int a,int b,int limit){
        int res = max(a*10+b<limit ? a*10+b:-1,b*10+a<limit ? b*10+a:-1);
        if(res >= 10) return to_string(res);
        else if(res>=0 && res<10)return caonima(res);
        else return "";
    }
};
```

_再也不会做这种恶心死了的题，烦死了快，各种边界条件，输出格式！！














