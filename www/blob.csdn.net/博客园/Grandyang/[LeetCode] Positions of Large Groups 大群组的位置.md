# [LeetCode] Positions of Large Groups 大群组的位置 - Grandyang - 博客园







# [[LeetCode] Positions of Large Groups 大群组的位置](https://www.cnblogs.com/grandyang/p/10332063.html)







In a string `S` of lowercase letters, these letters form consecutive groups of the same character.

For example, a string like `S = "abbxxxxzyy"` has the groups `"a"`, `"bb"`, `"xxxx"`, `"z"` and `"yy"`.

Call a group *large* if it has 3 or more characters.  We would like the starting and ending positions of every large group.

The final answer should be in lexicographic order.



Example 1:
Input: "abbxxxxzzy"
Output: [[3,6]]
Explanation: `"xxxx" is the single `large group with starting  3 and ending positions 6.

Example 2:
Input: "abc"
Output: []
Explanation: We have "a","b" and "c" but no large group.

Example 3:
Input: "abcdddeeeeaabbbcd"
Output: [[3,5],[6,9],[12,14]]


Note:  `1 <= S.length <= 1000`



这道题给了我们一个全小写的字符串，说是重复出现的字符可以当作一个群组，如果重复次数大于等于3次，可以当作一个大群组，让我们找出所有大群组的起始和结束位置。那么实际上就是让我们计数连续重复字符的出现次数，由于要连续，所以我们可以使用双指针来做，一个指针指向重复部分的开头，一个往后遍历计数，只要不相同了就停止，然后看次数是否大于等3，是的话就将双指针位置存入结果res中，并更新指针，参见代码如下：



解法一：

```
class Solution {
public:
    vector<vector<int>> largeGroupPositions(string S) {
        vector<vector<int>> res;
        int n = S.size(), i = 0, j = 0;
        while (j < n) {
            while (j < n && S[j] == S[i]) ++j;
            if (j - i >= 3) res.push_back({i, j - 1});
            i = j;
        }
        return res;
    }
};
```



我们也可以换一种写法，不用while循环，而是使用for循环，但本质上还是双指针的思路，并没有什么太大的区别，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<int>> largeGroupPositions(string S) {
        vector<vector<int>> res;
        int n = S.size(), start = 0;
        for (int i = 1; i <= n; ++i) {
            if (i < n && S[i] == S[start]) continue;
            if (i - start >= 3) res.push_back({start, i - 1});
            start = i;
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/positions-of-large-groups/](https://leetcode.com/problems/positions-of-large-groups/)

[https://leetcode.com/problems/positions-of-large-groups/discuss/128961/Java-Solution-Two-Pointers](https://leetcode.com/problems/positions-of-large-groups/discuss/128961/Java-Solution-Two-Pointers)

[https://leetcode.com/problems/positions-of-large-groups/discuss/128942/My-Easy-7-Lines-C%2B%2B-Solution](https://leetcode.com/problems/positions-of-large-groups/discuss/128942/My-Easy-7-Lines-C%2B%2B-Solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












