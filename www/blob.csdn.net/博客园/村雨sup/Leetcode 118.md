# Leetcode 118 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    vector<vector<int>> generate(int numRows) {
        vector<vector<int>> res;
        if(numRows == 0) return res;
        vector<int> add;
        add.push_back(1);
        res.push_back(add);
        if(numRows == 1) return res;
        
        for(int i=1;i < numRows;i++){
            add.clear();
            add.push_back(1);
            for(int j=1;j < i;j++){
                add.push_back(res[i-1][j-1]+res[i-1][j]);
            }
            add.push_back(1);
            res.push_back(add);
        }
        return res;
    }
};
```

_











