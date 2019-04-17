# leetcode-96. Unique Binary Search Trees 不同的二叉搜索树 - 别说话写代码的博客 - CSDN博客





2019年01月24日 12:29:29[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：41标签：[Unique Binary Search Trees																[不同的二叉搜索树																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=不同的二叉搜索树&t=blog)



|Given *n*, how many structurally unique **BST's** (binary search trees) that store values 1 ... *n*?**Example:****Input:** 3**Output:** 5**Explanation:**Given *n* = 3, there are a total of 5 unique BST's:   1         3     3      2      1    \       /     /      / \      \     3     2     1      1   3      2    /     /       \                 \   2     1         2                 3|给定一个整数 *n*，求以 1 ... *n* 为节点组成的二叉搜索树有多少种？**示例:****输入:** 3**输出:** 5**解释:**给定 *n* = 3, 一共有 5 种不同结构的二叉搜索树:   1         3     3      2      1    \       /     /      / \      \     3     2     1      1   3      2    /     /       \                 \   2     1         2                 3|
|----|----|

思路：卡特兰树，这种题我觉得只要对答案有印象就行了。具体推导参见[这里](http://www.cnblogs.com/grandyang/p/4299608.html)。

我们先来看当 n = 1的情况，只能形成唯一的一棵二叉搜索树，n分别为1,2,3的情况如下所示：

![](https://img-blog.csdnimg.cn/20190124122839977.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

就跟斐波那契数列一样，我们把n = 0 时赋为1，因为空树也算一种二叉搜索树，那么n = 1时的情况可以看做是其左子树个数乘以右子树的个数，左右字数都是空树，所以1乘1还是1。那么n = 2时，由于1和2都可以为跟，分别算出来，再把它们加起来即可。n = 2的情况可由下面式子算出：

![](https://img-blog.csdnimg.cn/2019012412290163.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

```cpp
class Solution {
public:
    int numTrees(int n) {
        vector<int> vec(n+1,0);
        vec[0]=1;vec[1]=1;
        for(int i=2;i<=n;++i)
            for(int j=0;j<i;++j)
                vec[i] += vec[j]*vec[i-j-1];
        return vec[n];
    }
};
```](https://so.csdn.net/so/search/s.do?q=Unique Binary Search Trees&t=blog)




