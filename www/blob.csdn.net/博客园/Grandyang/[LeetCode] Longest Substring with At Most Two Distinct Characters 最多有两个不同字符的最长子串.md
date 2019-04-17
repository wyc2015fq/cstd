# [LeetCode] Longest Substring with At Most Two Distinct Characters 最多有两个不同字符的最长子串 - Grandyang - 博客园







# [[LeetCode] Longest Substring with At Most Two Distinct Characters 最多有两个不同字符的最长子串](https://www.cnblogs.com/grandyang/p/5185561.html)







Given a string S, find the length of the longest substring T that contains at most two distinct characters.
For example,
Given S = “eceba”,
T is “ece” which its length is 3. 



这道题给我们一个字符串，让我们求最多有两个不同字符的最长子串。那么我们首先想到的是用哈希表来做，哈希表记录每个字符的出现次数，然后如果哈希表中的映射数量超过两个的时候，我们需要删掉一个映射，比如此时哈希表中e有2个，c有1个，此时把b也存入了哈希表，那么就有三对映射了，这时我们的left是0，先从e开始，映射值减1，此时e还有1个，不删除，left自增1。这是哈希表里还有三对映射，此时left是1，那么到c了，映射值减1，此时e映射为0，将e从哈希表中删除，left自增1，然后我们更新结果为i - left + 1，以此类推直至遍历完整个字符串，参见代码如下：



解法一：

```
class Solution {
public:
    int lengthOfLongestSubstringTwoDistinct(string s) {
        int res = 0, left = 0;
        unordered_map<char, int> m;
        for (int i = 0; i < s.size(); ++i) {
            ++m[s[i]];
            while (m.size() > 2) {
                if (--m[s[left]] == 0) m.erase(s[left]);
                ++left;
            }
            res = max(res, i - left + 1);
        }
        return res;
    }
};
```



我们除了用哈希表来映射字符出现的个数，我们还可以映射每个字符最新的坐标，比如题目中的例子"eceba"，遇到第一个e，映射其坐标0，遇到c，映射其坐标1，遇到第二个e时，映射其坐标2，当遇到b时，映射其坐标3，每次我们都判断当前哈希表中的映射数，如果大于2的时候，那么我们需要删掉一个映射，我们还是从left=0时开始向右找，我们看每个字符在哈希表中的映射值是否等于当前坐标left，比如第一个e，哈希表此时映射值为2，不等于left的0，那么left自增1，遇到c的时候，哈希表中c的映射值是1，和此时的left相同，那么我们把c删掉，left自增1，再更新结果，以此类推直至遍历完整个字符串，参见代码如下：



解法二：

```
class Solution {
public:
    int lengthOfLongestSubstringTwoDistinct(string s) {
        int res = 0, left = 0;
        unordered_map<char, int> m;
        for (int i = 0; i < s.size(); ++i) {
            m[s[i]] = i;
            while (m.size() > 2) {
                if (m[s[left]] == left) m.erase(s[left]);
                ++left;
            }
            res = max(res, i - left + 1);
        }
        return res;
    }
};
```



后来又在网上看到了一种解法，这种解法是维护一个sliding window，指针left指向起始位置，right指向window的最后一个位置，用于定位left的下一个跳转位置，思路如下：

1. 若当前字符和前一个字符相同，继续循环。

2. 若不同，看当前字符和right指的字符是否相同

    (1) 若相同，left不变，右边跳到i - 1

    (2) 若不同，更新结果，left变为right+1，right变为i - 1

最后需要注意在循环结束后，我们还要比较res和s.size() - left的大小，返回大的，这是由于如果字符串是"ecebaaa"，那么当left=3时，i=5,6的时候，都是继续循环，当i加到7时，跳出了循环，而此时正确答案应为"baaa"这4个字符，而我们的res只更新到了"ece"这3个字符，所以我们最后要判断s.size() - left和res的大小。

另外需要说明的是这种解法仅适用于于不同字符数为2个的情况，如果为k个的话，还是需要用上面两种解法。



解法三：

```
class Solution {
public:
    int lengthOfLongestSubstringTwoDistinct(string s) {
        int left = 0, right = -1, res = 0;
        for (int i = 1; i < s.size(); ++i) {
            if (s[i] == s[i - 1]) continue;
            if (right >= 0 && s[right] != s[i]) {
                res = max(res, i - left);
                left = right + 1;
            }
            right = i - 1;
        }
        return max(s.size() - left, res);
    }
};
```



类似题目：

[Longest Substring Without Repeating Characters](http://www.cnblogs.com/grandyang/p/4480780.html)

[Sliding Window Maximum](http://www.cnblogs.com/grandyang/p/4656517.html)

[Longest Substring with At Most K Distinct Characters](http://www.cnblogs.com/grandyang/p/5351347.html)



参考资料：

[https://discuss.leetcode.com/topic/21645/c-one-scan-solution-easy-understanding](https://discuss.leetcode.com/topic/21645/c-one-scan-solution-easy-understanding)





[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












