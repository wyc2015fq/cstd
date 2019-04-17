# [CareerCup] 1.3 Permutation String 字符串的排列 - Grandyang - 博客园







# [[CareerCup] 1.3 Permutation String 字符串的排列](https://www.cnblogs.com/grandyang/p/4650206.html)







1.3 Given two strings, write a method to decide if one is a permutation of the other.



这道题给定我们两个字符串，让我们判断一个是否为另一个的全排列字符串。在LeetCode中，关于排列的题有如下几道，[Permutation Sequence 序列排序](http://www.cnblogs.com/grandyang/p/4358678.html)，[Permutations 全排列](http://www.cnblogs.com/grandyang/p/4358848.html)， [Permutations II 全排列之二](http://www.cnblogs.com/grandyang/p/4359825.html) 和 [Next Permutation 下一个排列](http://www.cnblogs.com/grandyang/p/4428207.html)。这道题跟它们比起来，算是很简单的了。我们先来看一种O(n)的解决方法，跟之前那道[1.1 Unique Characters of a String 字符串中不同的字符](http://www.cnblogs.com/grandyang/p/4648044.html) 使用的方法相同，还是用整型数组来代替哈希表，首先遍历一遍s1中的字符，将其对应位置增1，然后再遍历s2，将其对应位置减1，如果此时减成了负数，说明s2和s1的字符及其数量不完全相等，返回false。若s2遍历完成，返回true。代码如下：



```
class Solution {
public:
    bool isPermutation(string s1, string s2) {
        if (s1.size() != s2.size()) return false;
        int m[256] = {0};
        for (int i = 0; i < s1.size(); ++i) ++m[s1[i]];
        for (int i = 0; i < s2.size(); ++i) {
            --m[s2[i]];
            if (m[s2[i]] < 0) return false; 
        }
        return true;
    }
};
```



当然如果不考虑运行效率的话，也可以分别对两个字符串进行排序，排序后的两个字符串如果完全相等则返回true，反之返回false。代码如下：



```
class Solution {
public:
    bool isPermutation(string s1, string s2) {
        if (s1.size() != s2.size()) return false;
        sort(s1.begin(), s1.end());
        sort(s2.begin(), s2.end());
        return s1 == s2;
    }
};
```














