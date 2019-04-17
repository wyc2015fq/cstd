# Leetcode 120 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    int minimumTotal(vector<vector<int>>& triangle) {
        if(triangle.size() == 1) return triangle[0][0];
        triangle[1][0] += triangle[0][0];triangle[1][1] += triangle[0][0];
        if(triangle.size() == 2) return min(triangle[1][0],triangle[1][1]);
        int a = min(triangle[1][0],triangle[1][1]);
        for(int i=0;i < 2;i++){
            triangle[2][i] += a;
        }
        triangle[2][2] += triangle[1][1];
        for(int i=3;i < triangle.size();i++){
            triangle[i][0] += min(triangle[i-1][0],triangle[i-1][1]);
            for(int j=1;j < triangle[i].size()-2;j++){
                int x = min(triangle[i-1][j-1],triangle[i-1][j]);
                int y = min(x,triangle[i-1][j+1]);
                triangle[i][j] += y;
            }
            triangle[i][triangle[i].size()-2] += min(triangle[i-1][triangle[i].size()-2],triangle[i-1][triangle[i].size()-3]);
            triangle[i][triangle[i].size()-1] += triangle[i-1][triangle[i].size()-2];
        }
        return findmin(triangle[triangle.size()-1]);
    }
    int findmin(vector<int> temp){
        int minnum = temp[0];
        for(int i=1;i < temp.size();i++){
            if(minnum > temp[i]) minnum = temp[i];
        }
        return minnum;
    }
};
```

——题意没写清楚，相邻是左中右还是中右，WA了一发

```
class Solution {
public:
    int minimumTotal(vector<vector<int>>& triangle) {
        if(triangle.size() == 1) return triangle[0][0];
        triangle[1][0] += triangle[0][0];triangle[1][1] += triangle[0][0];
        for(int i=2;i < triangle.size();i++){
            triangle[i][0] += triangle[i-1][0];
            for(int j=1;j < triangle[i].size()-1;j++){
                triangle[i][j] += min(triangle[i-1][j-1],triangle[i-1][j]);
            }
            triangle[i][triangle[i].size()-1] += triangle[i-1][triangle[i].size()-2];
        }
        return findmin(triangle[triangle.size()-1]);
    }
    int findmin(vector<int> temp){
        int minnum = temp[0];
        for(int i=1;i < temp.size();i++){
            if(minnum > temp[i]) minnum = temp[i];
        }
        return minnum;
    }
};
```

_











