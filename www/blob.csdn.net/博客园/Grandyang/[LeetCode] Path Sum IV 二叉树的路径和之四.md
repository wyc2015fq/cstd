# [LeetCode] Path Sum IV 二叉树的路径和之四 - Grandyang - 博客园







# [[LeetCode] Path Sum IV 二叉树的路径和之四](https://www.cnblogs.com/grandyang/p/7570954.html)







If the depth of a tree is smaller than `5`, then this tree can be represented by a list of three-digits integers.

For each integer in this list:
- The hundreds digit represents the depth `D` of this node, `1 <= D <= 4.`
- The tens digit represents the position `P` of this node in the level it belongs to, `1 <= P <= 8`. The position is the same as that in a full binary tree.
- The units digit represents the value `V` of this node, `0 <= V <= 9.`



Given a list of `ascending` three-digits integers representing a binary with the depth smaller than 5. You need to return the sum of all paths from the root towards the leaves.

Example 1:
Input: [113, 215, 221]
Output: 12
Explanation: 
The tree that the list represents is:
    3
   / \
  5   1

The path sum is (3 + 5) + (3 + 1) = 12.



Example 2:
Input: [113, 221]
Output: 4
Explanation: 
The tree that the list represents is: 
    3
     \
      1

The path sum is (3 + 1) = 4.


这道题还是让我们求二叉树的路径之和，但是跟之前不同的是，树的存储方式比较特别，并没有专门的数结点，而是使用一个三位数字来存的，百位数是该结点的深度，十位上是该结点在某一层中的位置，个位数是该结点的结点值。为了求路径之和，我们肯定还是需要遍历树，但是由于没有树结点，所以我们可以用其他的数据结构代替。比如我们可以将每个结点的位置信息和结点值分离开，然后建立两者之间的映射。比如我们可以将百位数和十位数当作key，将个位数当作value，建立映射。由于题目中说了数组是有序的，所以首元素就是根结点，然后我们进行先序遍历即可。在递归函数中，我们先将深度和位置拆分出来，然后算出左右子结点的深度和位置的两位数，我们还要维护一个变量cur，用来保存当前路径之和。如果当前结点的左右子结点不存在，说明此时cur已经是一条完整的路径之和了，加到结果res中，直接返回。否则就是对存在的左右子结点调用递归函数即可，参见代码如下：



解法一：

```
class Solution {
public:
    int pathSum(vector<int>& nums) {
        if (nums.empty()) return 0;
        int res = 0;
        unordered_map<int, int> m;
        for (int num : nums) {
            m[num / 10] = num % 10;
        }
        helper(nums[0] / 10, m, 0, res);
        return res;
    }
    void helper(int num, unordered_map<int, int>& m, int cur, int& res) {
        int level = num / 10, pos = num % 10;
        int left = (level + 1) * 10 + 2 * pos - 1, right = left + 1;
        cur += m[num];
        if (!m.count(left) && !m.count(right)) {
            res += cur;
            return;
        }
        if (m.count(left)) helper(left, m, cur, res);
        if (m.count(right)) helper(right, m, cur, res);
    }
};
```



下面这种方法是迭代的形式，我们使用的层序遍历，与先序遍历不同的是，我们不能维护一个当前路径之和的变量，这样会重复计算结点值，而是在遍历每一层的结点时，加上其父结点的值，如果某一个结点没有子结点了，才将累加起来的结点值加到结果res中，参见代码如下：



解法二：

```
class Solution {
public:
    int pathSum(vector<int>& nums) {
        if (nums.empty()) return 0;
        int res = 0, cur = 0;
        unordered_map<int, int> m;
        queue<int> q{{nums[0] / 10}};
        for (int num : nums) {
            m[num / 10] = num % 10;
        }
        while (!q.empty()) {
            int t = q.front(); q.pop();
            int level = t / 10, pos = t % 10;
            int left = (level + 1) * 10 + 2 * pos - 1, right = left + 1;
            if (!m.count(left) && !m.count(right)) {
                res += m[t];
            }
            if (m.count(left)) {
                m[left] += m[t];
                q.push(left);
            }
            if (m.count(right)) {
                m[right] += m[t];
                q.push(right);
            }
        }
        return res;
    }
};
```



类似题目：

[Path Sum III](http://www.cnblogs.com/grandyang/p/6007336.html)

[Binary Tree Maximum Path Sum](http://www.cnblogs.com/grandyang/p/4280120.html)

[Path Sum II](http://www.cnblogs.com/grandyang/p/4042156.html)

[Path Sum](http://www.cnblogs.com/grandyang/p/4036961.html)



参考资料：

[https://discuss.leetcode.com/topic/101111/java-solution-represent-tree-using-hashmap](https://discuss.leetcode.com/topic/101111/java-solution-represent-tree-using-hashmap/2)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












