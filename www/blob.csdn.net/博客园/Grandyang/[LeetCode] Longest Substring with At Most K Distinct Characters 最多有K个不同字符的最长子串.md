# [LeetCode] Longest Substring with At Most K Distinct Characters 最多有K个不同字符的最长子串 - Grandyang - 博客园







# [[LeetCode] Longest Substring with At Most K Distinct Characters 最多有K个不同字符的最长子串](https://www.cnblogs.com/grandyang/p/5351347.html)







Given a string, find the length of the longest substring T that contains at most *k* distinct characters.

For example, Given s = `“eceba”` and k = 2,

T is "ece" which its length is 3.



这道题是之前那道[Longest Substring with At Most Two Distinct Characters](http://www.cnblogs.com/grandyang/p/5185561.html)的拓展，而且那道题中的解法一和解法二直接将2换成k就行了，具体讲解请参考之前那篇博客：



解法一：

```
class Solution {
public:
    int lengthOfLongestSubstringKDistinct(string s, int k) {
        int res = 0, left = 0;
        unordered_map<char, int> m;
        for (int i = 0; i < s.size(); ++i) {
            ++m[s[i]];
            while (m.size() > k) {
                if (--m[s[left]] == 0) m.erase(s[left]);
                ++left;
            }
            res = max(res, i - left + 1);
        }
        return res;
    }
};
```



具体讲解请参考之前那篇博客[Longest Substring with At Most Two Distinct Characters](http://www.cnblogs.com/grandyang/p/5185561.html)，参见代码如下：



解法二：

```
class Solution {
public:
    int lengthOfLongestSubstringKDistinct(string s, int k) {
        int res = 0, left = 0;
        unordered_map<char, int> m;
        for (int i = 0; i < s.size(); ++i) {
            m[s[i]] = i;
            while (m.size() > k) {
                if (m[s[left]] == left) m.erase(s[left]);
                ++left;
            }
            res = max(res, i - left + 1);
        }
        return res;
    }
};
```



类似题目：

[Longest Substring with At Most Two Distinct Characters](http://www.cnblogs.com/grandyang/p/5185561.html)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)














