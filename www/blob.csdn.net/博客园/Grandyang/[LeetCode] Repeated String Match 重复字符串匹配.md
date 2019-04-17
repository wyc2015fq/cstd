# [LeetCode] Repeated String Match 重复字符串匹配 - Grandyang - 博客园







# [[LeetCode] Repeated String Match 重复字符串匹配](https://www.cnblogs.com/grandyang/p/7631434.html)







Given two strings A and B, find the minimum number of times A has to be repeated such that B is a substring of it. If no such solution, return -1.

For example, with A = "abcd" and B = "cdabcdab".

Return 3, because by repeating A three times (“abcdabcdabcd”), B is a substring of it; and B is not a substring of A repeated two times ("abcdabcd").

Note:
The length of `A` and `B` will be between 1 and 10000.



这道题让我们用字符串B来匹配字符串A，问字符串A需要重复几次，如果无法匹配，则返回-1。那么B要能成为A的字符串，那么A的长度肯定要大于等于B，所以当A的长度小于B的时候，我们可以先进行重复A，直到A的长度大于等于B，并且累计次数cnt。那么此时我们用find来找，看B是否存在A中，如果存在直接返回cnt。如果不存在，我们再加上一个A，再来找，这样可以处理这种情况A="abc", B="cab"，如果此时还找不到，说明无法匹配，返回-1，参见代码如下：



解法一：

```
class Solution {
public:
    int repeatedStringMatch(string A, string B) {
        int n1 = A.size(), n2 = B.size(), cnt = 1;
        string t = A;
        while (t.size() < n2) {
            t += A;
            ++cnt;
        }
        if (t.find(B) != string::npos) return cnt;
        t += A;
        return (t.find(B) != string::npos) ? cnt + 1 : -1;
    }
};
```



下面这种解法就更简洁了，思路和上面的一样，都是每次给t增加一个字符串A，我们其实可以直接算出最多需要增加的个数，即B的长度除以A的长度再加上2，当B小于A的时候，那么可能需要两个A，所以i就是小于等于2，这样我们每次在t中找B，如果找到了，就返回i，没找到，就增加一个A，循环推出后返回-1即可，参见代码如下：



解法二：

```
class Solution {
public:
    int repeatedStringMatch(string A, string B) {
        string t = A;
        for (int i = 1; i <= B.size() / A.size() + 2; ++i) {
            if (t.find(B) != string::npos) return i;
            t += A;
        }
        return -1;
    }
};
```



下面这种解法还是由热心网友[edyyy](http://home.cnblogs.com/u/1090659/)提供，没有用到字符串自带的find函数，而是逐个字符进行比较，循环字符串A中的所有字符，然后用个变量j，初始化为0，用来循环字符串B中的字符，每个字符和A中对应的字符进行比较，此时从A中取字符就要把A当作一个循环数组来处理，用(i+j)%m来取字符，还要确保j小于n，避免越界，如果字符匹配的话，j自增1。while 循环结束后，如果j等于n了，说明B中所有的字符都成功匹配了，那么我们来计算A的重复次数，通过(i+j-1)/m + 1来得到，注意i+j要减1再除以m，之后再加上一次。因为当i+j正好等于m时，说明此时不用重复A，那么(i+j-1)/m + 1还是等于1，当i+j>m的时候，需要重复A了，(i+j-1)/m + 1也可以得到正确的结构，参见代码如下：



解法三：

```
class Solution {
public:
    int repeatedStringMatch(string A, string B) {
        int m = A.size(), n = B.size();
        for (int i = 0; i < m; ++i) {
            int j = 0;
            while (j < n && A[(i + j) % m] == B[j]) ++j;
            if (j == n) return (i + j - 1) / m + 1;
        }
        return -1;
    }
};
```



类似题目：

[Repeated Substring Pattern](http://www.cnblogs.com/grandyang/p/6087347.html)



参考资料：

[https://discuss.leetcode.com/topic/105579/c-4-lines-o-m-n-o-1-and-8-lines-kmp-o-m-n-o-n](https://discuss.leetcode.com/topic/105579/c-4-lines-o-m-n-o-1-and-8-lines-kmp-o-m-n-o-n)

[https://discuss.leetcode.com/topic/105566/java-solution-just-keep-building-oj-missing-test-cases](https://discuss.leetcode.com/topic/105566/java-solution-just-keep-building-oj-missing-test-cases)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












