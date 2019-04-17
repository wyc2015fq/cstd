# Leetcode 784 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





//这代码可真丑陋，但我学到了两点1：char和string可以无缝互相转换2：char可以直接加减数字进行转换string不行

```
class Solution {
public:
    vector<string> letterCasePermutation(string S) {
        vector<string> res;
        string add;
        DFS(res,S,0,add);
        return res;
    }
    
    char func(char temp){
        if(temp >= 97&&temp <= 122){
            temp -= 32;
        }
        else if(temp <= 90&&temp >= 65){
            temp += 32;
        }
        return temp;
    }
    
    void DFS(vector<string>& res,string s,int pos,string add){
        if(add.size() == s.size()){
            res.push_back(add);
        }
        else{
            for(int i=pos;i < s.size();i++){
                char t = s[i];   
                if(t >= 48 && t <= 57){
                    add += s[i];
                }
                else{
                    string temp = add;
                    temp += func(t);
                    DFS(res,s,i+1,temp);
                    add += s[i];
                }
            }
        if(add.size() == s.size()){ // 这里又加了个是因为害怕最后一个是数字
            res.push_back(add);
        }
            
        }
    }
};
```

 好方法：很简洁

```
class Solution {
public:
    vector<string> letterCasePermutation(string S) {
        vector<string> res;
        helper(S, 0, res);
        return res;
    }
    void helper(string& s, int pos, vector<string>& res) {
        if (pos == s.size()) {
            res.push_back(s);
            return;
        }
        helper(s, pos + 1, res);
        if (s[pos] > '9') {
            s[pos] ^= 32;
            helper(s, pos + 1, res);
        }
    }
};
```













