# Leetcode 17 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//狂练回溯，巧用备胎
class Solution {
public:
    vector<string> letterCombinations(string digits) {        
        vector<string> res;
        if(digits == "") return res;
        string add;
        DFS(res,add,digits,0);
        return res;
    }

    void DFS(vector<string>& res,string add,string digits,int pos){
        if(pos == digits.size()){
            res.push_back(add);
            return;
        }
        else{
            string temp;
            int t = digits[pos]-'0';
            if(t == 2) temp = "abc";
            else if(t == 3)temp = "def";
            else if(t == 4)temp = "ghi";
            else if(t == 5)temp = "jkl";
            else if(t == 6)temp = "mno";
            else if(t == 7)temp = "pqrs";
            else if(t == 8)temp = "tuv";
            else if(t == 9)temp = "wxyz";
            for(int i=0;i < temp.size();i++){
                string beitai= add+temp[i]; //巧用备胎！！
                DFS(res,beitai,digits,pos+1);
            }
        }
    }
};
```













