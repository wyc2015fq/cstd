# [LeetCode] Compare Version Numbers 版本比较 - Grandyang - 博客园







# [[LeetCode] Compare Version Numbers 版本比较](https://www.cnblogs.com/grandyang/p/4244123.html)







Compare two version numbers *version1* and *version2*.
If `*version1* > *version2*` return `1;` if `*version1* <*version2*` return `-1;`otherwise return `0`.

You may assume that the version strings are non-empty and contain only digits and the `.` character.
The `.` character does not represent a decimal point and is used to separate number sequences.
For instance, `2.5` is not "two and a half" or "half way to version three", it is the fifth second-level revision of the second first-level revision.

Example 1:
Input: `*version1*` = "0.1", `*version2*` = "1.1"
Output: -1
Example 2:
Input: `*version1*` = "1.0.1", `*version2*` = "1"
Output: 1
Example 3:
Input: `*version1*` = "7.5.2.4", `*version2*` = "7.5.3"
Output: -1
Credits:
Special thanks to [@ts](https://oj.leetcode.com/discuss/user/ts) for adding this problem and creating all test cases.



这道题调试了好久，一直不想上网搜别人的解法，因为感觉自己可以做出来，改来改去最后终于通过了，再上网一搜，发现果然和别人的方法不同，小有成就感。我的思路是：由于两个版本号所含的小数点个数不同，有可能是1和1.1.1比较，还有可能开头有无效0，比如01和1就是相同版本，还有可能末尾无效0，比如1.0和1也是同一版本。对于没有小数点的数字，可以默认为最后一位是小数点，而版本号比较的核心思想是相同位置的数字比较，比如题目给的例子，1.2和13.37比较，我们都知道应该显示1和13比较，13比1大，所以后面的不用再比了，再比如1.1和1.2比较，前面都是1，则比较小数点后面的数字。那么算法就是每次对应取出相同位置的小数点之前所有的字符，把他们转为数字比较，若不同则可直接得到答案，若相同，再对应往下取。如果一个数字已经没有小数点了，则默认取出为0，和另一个比较，这样也解决了末尾无效0的情况。代码如下：



解法一：

```
class Solution {
public:
    int compareVersion(string version1, string version2) {
        int n1 = version1.size(), n2 = version2.size();
        int i = 0, j = 0, d1 = 0, d2 = 0;
        string v1, v2;
        while (i < n1 || j < n2) {
            while (i < n1 && version1[i] != '.') {
                v1.push_back(version1[i++]);
            }
            d1 = atoi(v1.c_str());
            while (j < n2 && version2[j] != '.') {
                v2.push_back(version2[j++]);
            }
            d2 = atoi(v2.c_str());
            if (d1 > d2) return 1;
            else if (d1 < d2) return -1;
            v1.clear(); v2.clear();
            ++i; ++j;
        }
        return 0;
    }
};
```


当然我们也可以不使用将字符串转为整型的atoi函数，我们可以一位一位的累加，参加如下代码：



解法二：

```
class Solution {
public:
    int compareVersion(string version1, string version2) {
        int n1 = version1.size(), n2 = version2.size();
        int i = 0, j = 0, d1 = 0, d2 = 0;
        while (i < n1 || j < n2) {
            while (i < n1 && version1[i] != '.') {
                d1 = d1 * 10 + version1[i++] - '0';
            }
            while (j < n2 && version2[j] != '.') {
                d2 = d2 * 10 + version2[j++] - '0';
            }
            if (d1 > d2) return 1;
            else if (d1 < d2) return -1;
            d1 = d2 = 0;
            ++i; ++j;
        }
        return 0;
    }
};
```



由于这道题我们需要将版本号以’.'分开，那么我们可以借用强大的字符串流stringstream的功能来实现分段和转为整数，使用这种方法写的代码很简洁，如下所示：



解法三：

```
class Solution {
public:
    int compareVersion(string version1, string version2) {
        istringstream v1(version1 + "."), v2(version2 + ".");
        int d1 = 0, d2 = 0;
        char dot = '.';
        while (v1.good() || v2.good()) {
            if (v1.good()) v1 >> d1 >> dot;
            if (v2.good()) v2 >> d2 >> dot;
            if (d1 > d2) return 1;
            else if (d1 < d2) return -1;
            d1 = d2 = 0;
        }
        return 0;
    }
};
```



最后我们来看一种用C语言的字符串指针来实现的方法，这个方法的关键是用到将字符串转为长整型的strtol函数，关于此函数的用法可以参见我的另一篇博客[strtol 函数用法](http://www.cnblogs.com/grandyang/p/4966132.html)。参见代码如下：



解法四：

```
class Solution {
public:
    int compareVersion(string version1, string version2) {
        int res = 0;
        char *v1 = (char*)version1.c_str(), *v2 = (char*)version2.c_str();
        while (res == 0 && (*v1 != '\0' || *v2 != '\0')) {
            long d1 = *v1 == '\0' ? 0 : strtol(v1, &v1, 10);
            long d2 = *v2 == '\0' ? 0 : strtol(v2, &v2, 10);
            if (d1 > d2) return 1;
            else if (d1 < d2) return -1;
            else {
                if (*v1 != '\0') ++v1;
                if (*v2 != '\0') ++v2;
            }
        }
        return res;
    }
};
```



类似题目：

[First Bad Version](http://www.cnblogs.com/grandyang/p/4790469.html)



参考资料：

[https://leetcode.com/problems/compare-version-numbers/discuss/?orderBy=most_votes](https://leetcode.com/problems/compare-version-numbers/discuss/?orderBy=most_votes)

[https://leetcode.com/problems/compare-version-numbers/discuss/50774/Accepted-small-Java-solution.](https://leetcode.com/problems/compare-version-numbers/discuss/50774/Accepted-small-Java-solution.)

[https://leetcode.com/problems/compare-version-numbers/discuss/50788/My-JAVA-solution-without-split](https://leetcode.com/problems/compare-version-numbers/discuss/50788/My-JAVA-solution-without-split)

[https://leetcode.com/problems/compare-version-numbers/discuss/50804/10-line-concise-solution.-(C%2B%2B)](https://leetcode.com/problems/compare-version-numbers/discuss/50804/10-line-concise-solution.-(C%2B%2B))

[https://leetcode.com/problems/compare-version-numbers/discuss/50767/My-2ms-easy-solution-with-CC%2B%2B](https://leetcode.com/problems/compare-version-numbers/discuss/50767/My-2ms-easy-solution-with-CC%2B%2B)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












