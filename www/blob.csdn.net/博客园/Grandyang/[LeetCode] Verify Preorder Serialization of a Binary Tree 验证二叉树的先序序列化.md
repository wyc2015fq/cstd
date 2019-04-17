# [LeetCode] Verify Preorder Serialization of a Binary Tree 验证二叉树的先序序列化 - Grandyang - 博客园







# [[LeetCode] Verify Preorder Serialization of a Binary Tree 验证二叉树的先序序列化](https://www.cnblogs.com/grandyang/p/5174738.html)







One way to serialize a binary tree is to use pre-order traversal. When we encounter a non-null node, we record the node's value. If it is a null node, we record using a sentinel value such as `#`.
     _9_
    /   \
   3     2
  / \   / \
 4   1  #  6
/ \ / \   / \
# # # #   # #

For example, the above binary tree can be serialized to the string `"9,3,4,#,#,1,#,#,2,#,6,#,#"`, where `#` represents a null node.

Given a string of comma separated values, verify whether it is a correct preorder traversal serialization of a binary tree. Find an algorithm without reconstructing the tree.

Each comma separated value in the string must be either an integer or a character `'#'` representing `null` pointer.

You may assume that the input format is always valid, for example it could never contain two consecutive commas such as `"1,,3"`.

Example 1:
Input: `"9,3,4,#,#,1,#,#,2,#,6,#,#"`
Output: `true`
Example 2:
Input: `"1,#"`
Output: `false`
Example 3:
Input: `"9,#,#,1"`
Output: `false`
Credits:
Special thanks to [@dietpepsi](https://leetcode.com/discuss/user/dietpepsi) for adding this problem and creating all test cases.



这道题给了我们一个类似序列化二叉树后的字符串，关于二叉树的序列化和去序列化可以参见我之前的博客[Serialize and Deserialize Binary Tree](http://www.cnblogs.com/grandyang/p/4913869.html)，这道题让我们判断给定是字符串是不是一个正确的序列化的二叉树的字符串。那么根据之前那边博客的解法，我们还是要用istringsteam来操作字符串，C++里面没有像Java那样有字符串的split函数，可以直接分隔任意字符串，我们只能使用getline这个函数，来将字符串流的内容都存到一个vector数组中。我们通过举一些正确的例子，比如"9,3,4,#,#,1,#,#,2,#,6,#,#" 或者"9,3,4,#,#,1,#,#,2,#,6,#,#"等等，可以观察出如下两个规律：

1. 数字的个数总是比#号少一个

2. 最后一个一定是#号

那么我们加入先不考虑最后一个#号，那么此时数字和#号的个数应该相同，如果我们初始化一个为0的计数器，遇到数字，计数器加1，遇到#号，计数器减1，那么到最后计数器应该还是0。下面我们再来看两个返回False的例子，"#,7,6,9,#,#,#"和"7,2,#,2,#,#,#,6,#"，那么通过这两个反例我们可以看出，如果根节点为空的话，后面不能再有节点，而且不能有三个连续的#号出现。所以我们再加减计数器的时候，如果遇到#号，且此时计数器已经为0了，再减就成负数了，就直接返回False了，因为正确的序列里，任何一个位置i，在[0, i]范围内的#号数都不大于数字的个数的。当循环完成后，我们检测计数器是否为0的同时还要看看最后一个字符是不是#号。参见代码如下：



解法一：

```
class Solution {
public:
    bool isValidSerialization(string preorder) {
        istringstream in(preorder);
        vector<string> v;
        string t = "";
        int cnt = 0;
        while (getline(in, t, ',')) v.push_back(t);
        for (int i = 0; i < v.size() - 1; ++i) {
            if (v[i] == "#") {
                if (cnt == 0) return false;
                --cnt;
            } else ++cnt;
        }
        return cnt == 0 && v.back() == "#";
    }
};
```



下面这种解法由网友[edyyy](http://home.cnblogs.com/u/1090659/)提供，不需要建立解法一中的额外数组，而是边解析边判断，遇到不合题意的情况直接返回false，而不用全部解析完再来验证是否合法，提高了运算的效率。我们用一个变量degree表示能容忍的"#"的个数，degree初始化为1。再用一个布尔型变量degree_is_zero来记录degree此时是否为0的状态，这样的设计很巧妙，可以cover到"#"开头，但后面还跟有数字的情况，比如"#,1,2"这种情况，当检测到"#"时，degree自减1，此时若degree为0了，degree_is_zero赋值为true，那么如果后面还跟有其他东西的话，在下次循环开始开始前，先判断degree_is_zero，如果为true的话，直接返回false。而当首字符为数字的话，degree自增1，那么此时degree就成了2，表示后面可以再容忍两个"#"。当循环退出的时候，此时判断degree是否为0，因为我们要补齐"#"的个数，少了也是不对的，参见代码如下：



解法二：

```
class Solution {
public:
    bool isValidSerialization(string preorder) {
        istringstream in(preorder);
        string t = "";
        int degree = 1;
        bool degree_is_zero = false;;
        while (getline(in, t, ',')) {
            if (degree_is_zero) return false;
            if (t == "#") {
                if (--degree == 0) degree_is_zero = true;
            } else ++degree;
        }
        return degree == 0;
    }
};
```



下面这种解法就更加巧妙了，连字符串解析都不需要了，用一个变量capacity来记录能容忍"#"的个数，跟上面解法中的degree一个作用，然后我们给preorder末尾加一个逗号，这样可以处理末尾的"#"。我们遍历preorder字符串，如果遇到了非逗号的字符，直接跳过，否则的话capacity自减1，如果此时capacity小于0了，直接返回true。此时再判断逗号前面的字符是否为"#"，如果不是的话，capacity自增2。这种设计非常巧妙，如果逗号前面是"#"，我们capacity自减1没问题，因为容忍了一个"#"；如果前面是数字，那么先自减的1，可以看作是初始化的1被减了，然后再自增2，因为每多一个数字，可以多容忍两个"#"，最后还是要判断capacity是否为0，跟上面的解法一样，我们要补齐"#"的个数，少了也是不对的，参见代码如下：



解法三：

```
class Solution {
public:
    bool isValidSerialization(string preorder) {
        int capacity = 1;
        preorder += ",";
        for (int i = 0; i < preorder.size(); ++i) {
            if (preorder[i] != ',') continue;
            if (--capacity < 0) return false;
            if (preorder[i - 1] != '#') capacity += 2;
        }
        return capacity == 0;
    }
};
```



类似题目：

[Serialize and Deserialize Binary Tree](http://www.cnblogs.com/grandyang/p/4913869.html)



参考资料：

[https://leetcode.com/problems/verify-preorder-serialization-of-a-binary-tree/](https://leetcode.com/problems/verify-preorder-serialization-of-a-binary-tree/)

[https://discuss.leetcode.com/topic/36035/2-lines-java-using-regex](https://discuss.leetcode.com/topic/36035/2-lines-java-using-regex)

[https://discuss.leetcode.com/topic/35976/7-lines-easy-java-solution](https://discuss.leetcode.com/topic/35976/7-lines-easy-java-solution)

[https://discuss.leetcode.com/topic/45326/c-4ms-solution-o-1-space-o-n-time](https://discuss.leetcode.com/topic/45326/c-4ms-solution-o-1-space-o-n-time)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












