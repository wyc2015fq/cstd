# [LeetCode] Flipping an Image 翻转图像 - Grandyang - 博客园







# [[LeetCode] Flipping an Image 翻转图像](https://www.cnblogs.com/grandyang/p/10349485.html)









Given a binary matrix `A`, we want to flip the image horizontally, then invert it, and return the resulting image.

To flip an image horizontally means that each row of the image is reversed.  For example, flipping `[1, 1, 0]` horizontally results in `[0, 1, 1]`.

To invert an image means that each `0` is replaced by `1`, and each `1` is replaced by `0`. For example, inverting `[0, 1, 1]` results in `[1, 0, 0]`.

Example 1:
Input: [[1,1,0],[1,0,1],[0,0,0]]
Output: [[1,0,0],[0,1,0],[1,1,1]]
Explanation: First reverse each row: [[0,1,1],[1,0,1],[0,0,0]].
Then, invert the image: [[1,0,0],[0,1,0],[1,1,1]]

Example 2:
Input: [[1,1,0,0],[1,0,0,1],[0,1,1,1],[1,0,1,0]]
Output: [[1,1,0,0],[0,1,1,0],[0,0,0,1],[1,0,1,0]]
Explanation: First reverse each row: [[0,0,1,1],[1,0,0,1],[1,1,1,0],[0,1,0,1]].
Then invert the image: [[1,1,0,0],[0,1,1,0],[0,0,0,1],[1,0,1,0]]

Notes:
- `1 <= A.length = A[0].length <= 20`
- `0 <= A[i][j] <= 1`





这道题让我们翻转图像，翻转的方法是对于二维数组的每一行，先将所有元素位置翻转一下，然后再按顺序将每个像素值取个反。既然要求这么直接明了，那么就按照其说的一步一步来呗，首先翻转每一行，记得一定要加 ‘&’ 号，不然原数组不会被修改。然后在遍历每个数字，让其或上1，达到取反的目的，当然还是必须要加 ‘&’ 号，最后返回修改后的A数组即可，参见代码如下：



解法一：

```
class Solution {
public:
    vector<vector<int>> flipAndInvertImage(vector<vector<int>>& A) {
        for (auto &row : A) reverse(row.begin(), row.end());
        for (auto &row : A) {
            for (int &num : row) num ^= 1;
        }
        return A;
    }
};
```



上面的方法虽然直接了当，但是毕竟修改了原数组A，再来看一种不修改的方法，这里我们新建一个跟A一样长的二维数组，只不过里面的各行还是空的。然后我们遍历A数组的各行，但在遍历各行上的数字时，我们采用从后往前的遍历顺序，然后对于每个数字取反在加入结果res中，这样直接将翻转和取反同时完成了，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<int>> flipAndInvertImage(vector<vector<int>>& A) {
        vector<vector<int>> res(A.size());
        for (int i = 0; i < A.size(); ++i) {
            for (int j = (int)A[i].size() - 1; j >= 0; --j) {
                res[i].push_back(!A[i][j]);
            }
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/flipping-an-image/](https://leetcode.com/problems/flipping-an-image/)

[https://leetcode.com/problems/flipping-an-image/discuss/130590/C%2B%2BJavaPython-Reverse-and-Toggle](https://leetcode.com/problems/flipping-an-image/discuss/130590/C%2B%2BJavaPython-Reverse-and-Toggle)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












