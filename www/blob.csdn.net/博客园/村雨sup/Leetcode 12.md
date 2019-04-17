# Leetcode 12 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//日积月累，水滴石穿
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) return "";   //必须要加空的特判
        string res = "";
        for(int i=0;i < strs[0].size();i++){
            for(int j=0;j < strs.size();j++){
                if(i >= strs[j].size() || strs[j][i] != strs[0][i])
                    return res;
            }
            res.push_back(strs[0][i]);
        }
        return res;
    }
};
int main()
{
    string s[3] = {"abca","abc"};  //内容为string的vector
    vector<string> n;
    n.insert(n.begin(),s,s+2);

    Solution a;
    cout << a.longestCommonPrefix(n) << endl;

    return 0;
}
```

_











