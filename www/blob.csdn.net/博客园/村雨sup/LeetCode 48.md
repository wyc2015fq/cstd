# LeetCode 48 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





这种方法首先对原数组取其转置矩阵，然后把每行的数字翻转可得到结果，如下所示(其中蓝色数字表示翻转轴)：

1  2  3　　　 　　 1  4  7　　　　　  7  4  1

4  5  6　　-->　　 2  5  8　　 -->  　  8  5  2　　

7  8  9 　　　 　　3  6  9　　　　      9  6  3

```
class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        for(int i=0;i < matrix[0].size();i++){
            for(int j=i+1;j < matrix[0].size();j++){
                swap(matrix[i][j],matrix[j][i]);
            }
            reverse(matrix[i].begin(),matrix[i].end());
        }
    }
};
```

_很好的数学思想，不是单纯的蛮力旋转











