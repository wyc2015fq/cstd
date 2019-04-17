# House Robber III - 李鑫o_O - CSDN博客





2016年03月15日 23:00:14[hustlx](https://me.csdn.net/HUSTLX)阅读数：319







问题：
The thief has found himself a new place for his thievery again. There is only one entrance to this area, called the "root." Besides the root, each house has one and only one parent house. After a tour, the smart thief realized that
 "all houses in this place forms a binary tree". It will automatically contact the police if two directly-linked houses were broken into on the same night.

Determine the maximum amount of money the thief can rob tonight without alerting the police.

这个问题可以分解为子问题，因为对于二叉树其左右子树也是二叉树，我们定义一个函数rob(root)让它返回根节点开始小偷能偷到的最大金钱数额，我们可以由rob(root.left)和rob(root.right)来计算rob(root).所以原问题可以递归求解：



```java
<span style="font-size:14px;">public int rob(TreeNode root) {
    if (root == null) {
        return 0;
    }

    int val = 0;

    if (root.left != null) {
        val += rob(root.left.left) + rob(root.left.right);
    }

    if (root.right != null) {
        val += rob(root.right.left) + rob(root.right.right);
    }

    return Math.max(val + root.val, rob(root.left) + rob(root.right));
}</span>
```

然而这样做很慢。因为深度越深的节点多次做重复计算，因此考虑用一个hash表存储每个节点能偷到的最大金额，从而问题变为一个DP问题。






```java
<span style="font-size:14px;">public int rob(TreeNode root) {
    Map<TreeNode, Integer> map = new HashMap<>();
    return robSub(root, map);
}
private int robSub(TreeNode root, Map<TreeNode, Integer> map) {
    if (root == null) return 0;
    if (map.containsKey(root)) return map.get(root);
    int val = 0;
    if (root.left != null) {
        val += robSub(root.left.left, map) + robSub(root.left.right, map);
    }
    if (root.right != null) {
        val += robSub(root.right.left, map) + robSub(root.right.right, map);
    }
    val = Math.max(val + root.val, robSub(root.left, map) + robSub(root.right, map));
    map.put(root, val);
    return val;
}</span>
```
然而这是一个O(n)空间复杂度，然而这个复杂度还可以降低，对于每个节点有偷或者不偷两种情况，我们可以给每个节点两个数表示偷和不偷分别获得的最大金额，而父节点的两个数又可以根据子节点的两个数得到，所以我们把空间复杂度可以降低为O（1）：


```java
<span style="font-size:14px;">public int rob(TreeNode root) {
    int[] res = robSub(root);
    return Math.max(res[0], res[1]);
}
private int[] robSub(TreeNode root) {
    if (root == null) {
        return new int[2];
    }
    int[] left = robSub(root.left);
    int[] right = robSub(root.right);
    int[] res = new int[2];
    res[0] = Math.max(left[0], left[1]) + Math.max(right[0], right[1]);
    res[1] = root.val + left[0] + right[0];
    return res;
}</span>
```

下面是C++版本：

```cpp
<span style="font-size:14px;">rob(TreeNode* root) {
    vector<int> res = robSub(root);
    return max(res[0], res[1]);
}
vector<int> robSub(TreeNode* root) {
    if (root == NULL) {
        return vector<int>(2,0);
    }
    vector<int> left = robSub(root->left);
    vector<int> right = robSub(root->right);
    vector<int> res(2,0);
    res[0] = max(left[0], left[1]) + max(right[0], right[1]);
    res[1] = root->val + left[0] + right[0];
    return res;
}</span>
```








