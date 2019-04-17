# Leetcode 74 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int m = matrix.size();
        if(!m) return false;
        int n = matrix[0].size();
        if(!n) return false;
        if(target < matrix[0][0] || target > matrix[m-1][n-1]) return false;
        int a = m-1;
        for(int i=1;i < m;i++){
            if(target < matrix[i][0]){
                a = i-1;
                break;
            }
        }
        int j = 0;
        int k = n-1;
        while(j <= k){
            int mid = (j+k)/2;
            if(matrix[a][mid] < target){j = mid+1;}
            else if(matrix[a][mid] > target){k = mid - 1;}
            else{
                return true;
            }
        }
        return false;
    }
};
```













