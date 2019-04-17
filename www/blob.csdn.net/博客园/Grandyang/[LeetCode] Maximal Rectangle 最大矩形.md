# [LeetCode] Maximal Rectangle 最大矩形 - Grandyang - 博客园







# [[LeetCode] Maximal Rectangle 最大矩形](https://www.cnblogs.com/grandyang/p/4322667.html)







Given a 2D binary matrix filled with 0's and 1's, find the largest rectangle containing all ones and return its area.



此题是之前那道的[Largest Rectangle in Histogram 直方图中最大的矩形](http://www.cnblogs.com/grandyang/p/4322653.html) 的扩展，这道题的二维矩阵每一层向上都可以看做一个直方图，输入矩阵有多少行，就可以形成多少个直方图，对每个直方图都调用[Largest Rectangle in Histogram 直方图中最大的矩形](http://www.cnblogs.com/grandyang/p/4322653.html) 中的方法，就可以得到最大的矩形面积。那么这道题唯一要做的就是将每一层构成直方图，由于题目限定了输入矩阵的字符只有 '0' 和 '1' 两种，所以处理起来也相对简单。方法是，对于每一个点，如果是‘0’，则赋0，如果是 ‘1’，就赋 之前的height值加上1。具体参见代码如下：



解法一：

```
class Solution {
public:
    int maximalRectangle(vector<vector<char> > &matrix) {
        int res = 0;
        vector<int> height;
        for (int i = 0; i < matrix.size(); ++i) {
            height.resize(matrix[i].size());
            for (int j = 0; j < matrix[i].size(); ++j) {
                height[j] = matrix[i][j] == '0' ? 0 : (1 + height[j]);
            }
            res = max(res, largestRectangleArea(height));
        }
        return res;
    }
    int largestRectangleArea(vector<int> &height) {
        int res = 0;
        stack<int> s;
        height.push_back(0);
        for (int i = 0; i < height.size(); ++i) {
            if (s.empty() || height[s.top()] <= height[i]) s.push(i);
            else {
                int tmp = s.top();
                s.pop();
                res = max(res, height[tmp] * (s.empty() ? i : (i - s.top() - 1)));
                --i;
            }
        }
        return res;
    }
};
```



我们也可以在一个函数内完成，这样代码看起来更加简洁一些：



解法二：

```
class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;
        int res = 0, m = matrix.size(), n = matrix[0].size();
        vector<int> height(n + 1, 0);
        for (int i = 0; i < m; ++i) {
            stack<int> s;
            for (int j = 0; j < n + 1; ++j) {
                if (j < n) {
                    height[j] = matrix[i][j] == '1' ? height[j] + 1 : 0;
                }
                while (!s.empty() && height[s.top()] >= height[j]) {
                    int cur = s.top(); s.pop();
                    res = max(res, height[cur] * (s.empty() ? j : (j - s.top() - 1)));
                }
                s.push(j);
            }
        }
        return res;
    }
};
```



下面这种方法的思路很巧妙，height数组和上面一样，这里的left数组表示左边界是1的位置，right数组表示右边界是1的位置，那么对于任意一行的第j个位置，矩形为(right[j] - left[j]) * height[j]，我们举个例子来说明，比如给定矩阵为：

```
[
  [1, 1, 0, 0, 1],
  [0, 1, 0, 0, 1],
  [0, 0, 1, 1, 1],
  [0, 0, 1, 1, 1],
  [0, 0, 0, 0, 1]
]
```

第0行：

```
h: 1 1 0 0 1
l: 0 0 0 0 4
r: 2 2 5 5 5
```



第1行：

```
h: 1 1 0 0 1
l: 0 0 0 0 4
r: 2 2 5 5 5
```



第2行：

```
h: 0 0 1 1 3
l: 0 0 2 2 4
r: 5 5 5 5 5
```



第3行：

```
h: 0 0 2 2 4
l: 0 0 2 2 4
r: 5 5 5 5 5
```



第4行：

```
h: 0 0 0 0 5
l: 0 0 0 0 4
r: 5 5 5 5 5
```



解法三：

```
class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;
        int res = 0, m = matrix.size(), n = matrix[0].size();
        vector<int> height(n, 0), left(n, 0), right(n, n);
        for (int i = 0; i < m; ++i) {
            int cur_left = 0, cur_right = n;
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] == '1') ++height[j];
                else height[j] = 0;
            }
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] == '1') left[j] = max(left[j], cur_left);
                else {left[j] = 0; cur_left = j + 1;}
            }
            for (int j = n - 1; j >= 0; --j) {
                if (matrix[i][j] == '1') right[j] = min(right[j], cur_right);
                else {right[j] = n; cur_right = j;}
            }
            for (int j = 0; j < n; ++j) {
                res = max(res, (right[j] - left[j]) * height[j]);
            }
        }
        return res;
    }
};
```



我们也可以通过合并一些for循环，使得运算速度更快一些：



解法四：

```
class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;
        int res = 0, m = matrix.size(), n = matrix[0].size();
        vector<int> height(n, 0), left(n, 0), right(n, n);
        for (int i = 0; i < m; ++i) {
            int cur_left = 0, cur_right = n;
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] == '1') {
                    ++height[j];
                    left[j] = max(left[j], cur_left);
                } else {
                    height[j] = 0;
                    left[j] = 0;
                    cur_left = j + 1;
                }
            }
            for (int j = n - 1; j >= 0; --j) {
                if (matrix[i][j] == '1') {
                    right[j] = min(right[j], cur_right);
                } else {
                    right[j] = n;
                    cur_right = j;
                }
                res = max(res, (right[j] - left[j]) * height[j]);
            }
        }
        return res;
    }
};
```



类似题目：

[Maximal Square](http://www.cnblogs.com/grandyang/p/4550604.html)

[Largest Rectangle in Histogram](http://www.cnblogs.com/grandyang/p/4322653.html)



参考资料：

[https://leetcode.com/discuss/20240/share-my-dp-solution](https://leetcode.com/discuss/20240/share-my-dp-solution)

[https://leetcode.com/discuss/5198/a-o-n-2-solution-based-on-largest-rectangle-in-histogram](https://leetcode.com/discuss/5198/a-o-n-2-solution-based-on-largest-rectangle-in-histogram)

[https://leetcode.com/discuss/17993/sharing-straightforward-solution-with-time-with-explanation](https://leetcode.com/discuss/17993/sharing-straightforward-solution-with-time-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












