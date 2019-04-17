# Leetcode 131 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    vector<vector<string>> partition(string s) {
        vector<string> add;
        vector<vector<string>> res;
        dfs(res,add,s,0);
        return res;
    }
    void dfs(vector<vector<string>>& res,vector<string> add,string s,int start){
        if(start == s.size()){
            res.push_back(add);
        }
        else{
            for(int i=start;i < s.size();i++){
                if(ishui(s,start,i)){
                    add.push_back(s.substr(start,i-start+1)); //子串的写法
                    dfs(res,add,s,i+1);
                    add.pop_back();
                }
            }
        }
    }
    
    bool ishui(string s,int start,int end){
        while(start <= end){
            if(s[start] != s[end]) return false;
            start++;end--;
        }
        return true;
    }
};
```

_











