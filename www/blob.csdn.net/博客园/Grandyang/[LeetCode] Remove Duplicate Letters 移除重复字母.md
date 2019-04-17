# [LeetCode] Remove Duplicate Letters 移除重复字母 - Grandyang - 博客园







# [[LeetCode] Remove Duplicate Letters 移除重复字母](https://www.cnblogs.com/grandyang/p/5085379.html)







Given a string which contains only lowercase letters, remove duplicate letters so that every letter appear once and only once. You must make sure your result is the smallest in lexicographical order among all possible results.

Example 1:
Input: `"bcabc"`
Output: `"abc"`
Example 2:
Input: `"cbacdcbc"`
Output: `"acdb"`
Credits:
Special thanks to [@dietpepsi](https://leetcode.com/discuss/user/dietpepsi) for adding this problem and creating all test cases.



这道题让我们移除重复字母，使得每个字符只能出现一次，而且结果要按字母顺序排，前提是不能打乱其原本的相对位置。我们的解题思路是：先建立一个哈希表来统计每个字母出现的次数，还需要一个visited数字来纪录每个字母是否被访问过，我们遍历整个字符串，对于遍历到的字符，先在哈希表中将其值减一，然后看visited中是否被访问过，若访问过则继续循环，说明该字母已经出现在结果中并且位置已经安排妥当。如果没访问过，我们和结果中最后一个字母比较，如果该字母的ASCII码小并且结果中的最后一个字母在哈希表中的值不为0(说明后面还会出现这个字母)，那么我们此时就要在结果中删去最后一个字母且将其标记为未访问，然后加上当前遍历到的字母，并且将其标记为已访问，以此类推直至遍历完整个字符串s，此时结果里的字符串即为所求。这里有个小技巧，我们一开始给结果字符串res中放个"0"，就是为了在第一次比较时方便，如果为空就没法和res中的最后一个字符比较了，而‘0’的ASCII码要小于任意一个字母的，所以不会有问题。最后我们返回结果时再去掉开头那个‘0’即可，参见代码如下：



```
class Solution {
public:
    string removeDuplicateLetters(string s) {
        int m[256] = {0}, visited[256] = {0};
        string res = "0";
        for (auto a : s) ++m[a];
        for (auto a : s) {
            --m[a];
            if (visited[a]) continue;
            while (a < res.back() && m[res.back()]) {
                visited[res.back()] = 0;
                res.pop_back();
            }
            res += a;
            visited[a] = 1;
        }
        return res.substr(1);
    }
};
```



这道题如果用Java解题的话可以使用一种递归的方法，参见[这里](https://leetcode.com/discuss/73761/a-short-o-n-recursive-greedy-solution)。思路是：先用哈希表记录每个字母出现的次数，再遍历给定字符串s，找出最小的字母，每比较一个字母，在哈希表中的值减1，如果此时为0了，则不继续遍历了，此时我们记录了一个位置，把字符串s中该位置左边的字符都删掉，右边的所有再出现的该字母也删掉，递归调用此函数即可，在Java中可以使用replaceAll函数，我用STL自己写了一个无法通过OJ的大数据，可能实现方法不对吧。。。



参考资料：

[https://leetcode.com/problems/remove-duplicate-letters/](https://leetcode.com/problems/remove-duplicate-letters/)

[https://leetcode.com/discuss/75529/c-simple-solution-easy-understanding](https://leetcode.com/discuss/75529/c-simple-solution-easy-understanding)

[https://leetcode.com/discuss/73761/a-short-o-n-recursive-greedy-solution](https://leetcode.com/discuss/73761/a-short-o-n-recursive-greedy-solution)


[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












