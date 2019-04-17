# [FollowUp] Combinations 组合项 - Grandyang - 博客园







# [[FollowUp] Combinations 组合项](https://www.cnblogs.com/grandyang/p/4358831.html)







这是[Combinations 组合项](http://www.cnblogs.com/grandyang/p/4332522.html) 的延伸，在这里，我们允许不同的顺序出现，那么新的题目要求如下：

Given two integers *n* and *k*, return all possible combinations of *k* numbers out of 1 ... *n*.

For example,
If *n* = 4 and *k* = 2, a solution is:
[
  [1,2],
  [1,3],
  [1,4],
  [2,1],
  [2,3],
  [2,4],
  [3,1],
  [3,2],
  [3,4],
  [4,1],
  [4,2],
  [4,3],
]
这题的解法其实只是在原题[Combinations 组合项](http://www.cnblogs.com/grandyang/p/4332522.html) 的基础上做很小的改动即可，这里我们为了避免重复项，引入了visited数字来标记某个数组是否出现过，然后就是递归中的循环不是从level开始，改为每次从0开始，这样就能把所有不同的排列方式都找出来，代码如下：



```
class Solution {
public:
    vector<vector<int> > combine(int n, int k) {
        vector<vector<int> > res;
        vector<int> out;
        vector<int> visited(n, 0);
        combineDFS(n, k, 0, visited, out, res);
        return res;
    }
    void combineDFS(int n, int k, int level, vector<int> &visited, vector<int> &out, vector<vector<int> > &res) {
        if (out.size() == k) res.push_back(out);
        else {
            for (int i = 0; i < n; ++i) {
                if (visited[i] == 0) {
                    visited[i] = 1;
                    out.push_back(i + 1);
                    combineDFS(n, k, level + 1, visited, out, res);
                    out.pop_back();
                    visited[i] = 0;
                }
            }
        }
    }
};
```














