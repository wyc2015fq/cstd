# [LeetCode] Count of Smaller Numbers After Self 计算后面较小数字的个数 - Grandyang - 博客园







# [[LeetCode] Count of Smaller Numbers After Self 计算后面较小数字的个数](https://www.cnblogs.com/grandyang/p/5078490.html)







You are given an integer array *nums* and you have to return a new *counts* array. The *counts* array has the property where `counts[i]` is the number of smaller elements to the right of `nums[i]`.

Example:
Given *nums* = [5, 2, 6, 1]

To the right of 5 there are 2 smaller elements (2 and 1).
To the right of 2 there is only 1 smaller element (1).
To the right of 6 there is 1 smaller element (1).
To the right of 1 there is 0 smaller element.

Return the array `[2, 1, 1, 0]`.



这道题给定我们一个数组，让我们计算每个数字右边所有小于这个数字的个数，目测我们不能用brute force，OJ肯定不答应，那么我们为了提高运算效率，首先可以使用用二分搜索法，思路是将给定数组从最后一个开始，用二分法插入到一个新的数组，这样新数组就是有序的，那么此时该数字在新数组中的坐标就是原数组中其右边所有较小数字的个数，参见代码如下：



解法一:

```
// Binary Search
class Solution {
public:
    vector<int> countSmaller(vector<int>& nums) {
        vector<int> t, res(nums.size());
        for (int i = nums.size() - 1; i >= 0; --i) {
            int left = 0, right = t.size();
            while (left < right) {
                int mid = left + (right - left) / 2;
                if (t[mid] >= nums[i]) right = mid;
                else left = mid + 1;
            }
            res[i] = right;
            t.insert(t.begin() + right, nums[i]);
        }
        return res;
    }
};
```



上面使用二分搜索法是一种插入排序的做法，我们还可以用C++中的STL的一些自带的函数来帮助我们，比如求距离distance，或是求第一个不小于当前数字的函数lower_bound，这里利用这两个函数代替了上一种方法中的二分搜索的部分，两种方法的核心思想都是相同的，构造有序数组，找出新加进来的数组在有序数组中对应的位置存入结果中即可，参见代码如下： 



解法二：

```
// Insert Sort
class Solution {
public:
    vector<int> countSmaller(vector<int>& nums) {
        vector<int> t, res(nums.size());
        for (int i = nums.size() - 1; i >= 0; --i) {
            int d = distance(t.begin(), lower_bound(t.begin(), t.end(), nums[i]));
            res[i] = d;
            t.insert(t.begin() + d, nums[i]);
        }
        return res;
    }
};
```



再来看一种利用二分搜索树来解的方法，我们来构造一棵二分搜索树，稍有不同的地方是我们需要加一个变量smaller来记录比当前节点值小的所有节点的个数，我们每插入一个节点，会判断其和根节点的大小，如果新的节点值小于根节点值，则其会插入到左子树中，我们此时要增加根节点的smaller，并继续递归调用左子节点的insert。如果节点值大于根节点值，则需要递归调用右子节点的insert并加上根节点的smaller，并加1，参见代码如下：



解法三：

```
// Binary Search Tree
class Solution {
public:
    struct Node {
        int val, smaller;
        Node *left, *right;
        Node(int v, int s) : val(v), smaller(s), left(NULL), right(NULL) {}
    };
    int insert(Node *&root, int v) {
        if (!root) return (root = new Node(v, 0)), 0;
        if (root->val > v) return root->smaller++, insert(root->left, v);
        else return insert(root->right, v) + root->smaller + (root->val < v ? 1 : 0);
    }
    vector<int> countSmaller(vector<int>& nums) {
        vector<int> res(nums.size());
        Node *root = NULL;
        for (int i = nums.size() - 1; i >= 0; --i) {
            res[i] = insert(root, nums[i]);
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/discuss/73280/my-simple-ac-java-binary-search-code](https://leetcode.com/discuss/73280/my-simple-ac-java-binary-search-code)

[https://leetcode.com/discuss/73635/insert-sort-method-c-use-68ms-solution](https://leetcode.com/discuss/73635/insert-sort-method-c-use-68ms-solution)

[https://leetcode.com/discuss/75586/14-line-44ms-c-building-bst](https://leetcode.com/discuss/75586/14-line-44ms-c-building-bst)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)














