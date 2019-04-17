# [LeetCode] Permutation in String 字符串中的全排列 - Grandyang - 博客园







# [[LeetCode] Permutation in String 字符串中的全排列](https://www.cnblogs.com/grandyang/p/6815227.html)







Given two strings s1 and s2, write a function to return true if s2 contains the permutation of s1. In other words, one of the first string's permutations is the substring of the second string.

Example 1:
Input:s1 = "ab" s2 = "eidbaooo"
Output:True
Explanation: s2 contains one permutation of s1 ("ba").



Example 2:
Input:s1= "ab" s2 = "eidboaoo"
Output: False



Note:
- The input strings only contain lower case letters.
- The length of both given strings is in range [1, 10,000].



这道题给了两个字符串s1和s2，问我们s1的全排列的字符串任意一个是否为s2的字串。虽然题目中有全排列的关键字，但是跟之前的全排列的题目的解法并不一样，如果受思维定势影响比较深的话，很容易遍历s1所有全排列的情况，然后检测其是否为s2的子串，这种解法是非常不高效的，估计OJ不会答应。 这道题的正确做法应该是使用滑动窗口Sliding Window的思想来做，可以使用两个哈希表来做，或者是使用一个哈希表配上双指针来做。我们先来看使用两个哈希表来做的情况，我们先来分别统计s1和s2中前n1个字符串中各个字符出现的次数，其中n1为字符串s1的长度，这样如果二者字符出现次数的情况完全相同，说明s1和s2中前n1的字符互为全排列关系，那么符合题意了，直接返回true。如果不是的话，那么我们遍历s2之后的字符，对于遍历到的字符，对应的次数加1，由于窗口的大小限定为了n1，所以每在窗口右侧加一个新字符的同时就要在窗口左侧去掉一个字符，每次都比较一下两个哈希表的情况，如果相等，说明存在，参见代码如下：



解法一：

```
class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        int n1 = s1.size(), n2 = s2.size();
        vector<int> m1(128), m2(128);
        for (int i = 0; i < n1; ++i) {
            ++m1[s1[i]]; ++m2[s2[i]];
        }
        if (m1 == m2) return true;
        for (int i = n1; i < n2; ++i) {
            ++m2[s2[i]];
            --m2[s2[i - n1]];
            if (m1 == m2) return true;
        }
        return false;
    }
};
```



下面这种解法是利用一个哈希表加上双指针，我们还是先统计s1中字符的出现次数，然后遍历s2中的字符，对于每个遍历到的字符，我们在哈希表中对应的字符次数减1，如果次数次数小于0了，说明该字符在s1中不曾出现，或是出现的次数超过了s1中的对应的字符出现次数，那么我们此时移动滑动窗口的左边界，对于移除的字符串，哈希表中对应的次数要加1，如果此时次数不为0，说明该字符不在s1中，继续向右移，直到更新后的次数为0停止，此时到达的字符是在s1中的。如果次数大于等于0了，我们看此时窗口大小是否为s1的长度，若二者相等，由于此时窗口中的字符都是在s1中存在的字符，而且对应的次数都为0了，说明窗口中的字符串和s1互为全排列，返回true即可，参见代码如下：



解法二：

```
class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        int n1 = s1.size(), n2 = s2.size(), left = 0;
        vector<int> m(128);
        for (char c : s1) ++m[c];
        for (int right = 0; right < n2; ++right) {
            if (--m[s2[right]] < 0) {
                while (++m[s2[left++]] != 0) {}
            } else if (right - left + 1 == n1) return true;
        }
        return n1 == 0;
    }
};
```



下面这种解法也是用一个哈希表外加双指针来做的，跟上面的解法思路大体相同，写法有些不同，不变的还是统计s1中字符出现的次数，不一样的是我们用一个变量cnt来表示还需要匹配的s1中的字符的个数，初始化为s1的长度，然后遍历s2中的字符，如果该字符在哈希表中存在，说明匹配上了，cnt自减1，哈希表中的次数也应该自减1，然后如果cnt减为0了，说明s1的字符都匹配上了，如果此时窗口的大小正好为s1的长度，那么说明找到了s1的全排列，返回true，否则说明窗口过大，里面有一些非s1中的字符，我们将左边界右移，同时将移除的字符串在哈希表中的次数自增1，如果增加后的次数大于0了，说明该字符是s1中的字符，我们将其移除了，那么cnt就要自增1，参见代码如下：



解法三：

```
class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        int n1 = s1.size(), n2 = s2.size(), cnt = n1, left = 0;
        vector<int> m(128);
        for (char c : s1) ++m[c];
        for (int right = 0; right < n2; ++right) {
            if (m[s2[right]]-- > 0) --cnt;
            while (cnt == 0) {
                if (right - left + 1 == n1) return true;
                if (++m[s2[left++]] > 0) ++cnt;
            }
        }
        return false;
    }
};
```



类似题目：

[Minimum Window Substring](http://www.cnblogs.com/grandyang/p/4340948.html)

[Find All Anagrams in a String](http://www.cnblogs.com/grandyang/p/6014408.html)



参考资料：

[https://discuss.leetcode.com/topic/87856/sliding-window-o-n-c](https://discuss.leetcode.com/topic/87856/sliding-window-o-n-c)

[https://discuss.leetcode.com/topic/87845/java-solution-sliding-window](https://discuss.leetcode.com/topic/87845/java-solution-sliding-window)

[https://discuss.leetcode.com/topic/87861/c-java-clean-code-with-explanation](https://discuss.leetcode.com/topic/87861/c-java-clean-code-with-explanation)

[https://discuss.leetcode.com/topic/87884/8-lines-slide-window-solution-in-java](https://discuss.leetcode.com/topic/87884/8-lines-slide-window-solution-in-java)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












