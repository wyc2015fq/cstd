# Leetcode 73 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        vector<vector<int>> store;
        int m = matrix.size();
        int n = matrix[0].size();
        for(int i=0;i < m;i++){
            for(int j=0;j < n;j++){
                if(matrix[i][j] == 0){
                    vector<int> temp;
                    temp.push_back(i);
                    temp.push_back(j);
                    store.push_back(temp);
                    temp.clear();
                }
            }
        }
        for(int i=0;i < store.size();i++){
            int a = store[i][0];
            int b = store[i][1];
            for(int j=0;j < m;j++) matrix[j][b] = 0;
            for(int j=0;j < n;j++) matrix[a][j] = 0;
        }
    }
};
```













