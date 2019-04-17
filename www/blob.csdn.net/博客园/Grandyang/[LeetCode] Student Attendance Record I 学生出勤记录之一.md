# [LeetCode] Student Attendance Record I 学生出勤记录之一 - Grandyang - 博客园







# [[LeetCode] Student Attendance Record I 学生出勤记录之一](https://www.cnblogs.com/grandyang/p/6736484.html)







You are given a string representing an attendance record for a student. The record only contains the following three characters:


- 'A' : Absent.
- 'L' : Late.
- 'P' : Present.



A student could be rewarded if his attendance record doesn't contain more than one 'A' (absent) or more than two continuous 'L' (late).

You need to return whether the student could be rewarded according to his attendance record.

Example 1:
Input: "PPALLP"
Output: True



Example 2:
Input: "PPALLL"
Output: False


这道题让我们判断学生的出勤率是否是优秀，判断标准是不能缺勤两次和不能连续迟到三次，那么最直接的方法就是分别记录缺勤和连续迟到的次数，如果当前遇到缺勤，那么缺勤计数器自增1，如果此时次数大于1了，说明已经不是优秀了，直接返回false，否则连续迟到计数器清零。如果当前遇到迟到，那么连续迟到计数器自增1，如果此时连续迟到计数器大于1了，说明已经不是优秀了，直接返回false。如果遇到正常出勤了，那么连续迟到计数器清零，参见代码如下：



解法一：

```
class Solution {
public:
    bool checkRecord(string s) {
        int cntA = 0, cntL = 0;
        for (char c : s) {
            if (c == 'A') {
                if (++cntA > 1) return false;
                cntL = 0;
            } else if (c == 'L') {
                if (++cntL > 2) return false;
            } else {
                cntL = 0;
            }
        }
        return true;
    }
};
```



那么这种方法利用到了string的查找函数，由于本题的逻辑并不复杂，所以我们可以直接对字符串进行操作，利用STL提供的find函数，方法是同时满足下面两个条件就是优秀，第一个条件是找不到A，或者正着找A和逆着找A在同一个位置(说明只有一个A)；第二个条件是找不到LLL，说明不能连续迟到三次，参见代码如下：



解法二：

```
class Solution {
public:
    bool checkRecord(string s) {
        return (s.find("A") == string::npos || s.find("A") == s.rfind("A")) && s.find("LLL") == string::npos;
    }
};
```



再来看使用正则匹配来做的解法，我们找出不合题意的情况，然后取反即可，正则匹配式是A.*A|LLL，其中.*表示有零个或者多个，那么A.*A就是至少有两A的情况，LLL是三个连续的迟到，|表示两个是或的关系，只要能匹配出任意一种情况，就会返回false，参见代码如下：



解法三：

```
class Solution {
public:
    bool checkRecord(string s) {
        return !regex_search(s, regex("A.*A|LLL"));
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/86651/c-1-liner](https://discuss.leetcode.com/topic/86651/c-1-liner)

[https://discuss.leetcode.com/topic/86571/one-line-java-mixed-solution](https://discuss.leetcode.com/topic/86571/one-line-java-mixed-solution)

[https://discuss.leetcode.com/topic/86534/tiny-ruby-short-python-java-c/2](https://discuss.leetcode.com/topic/86534/tiny-ruby-short-python-java-c/2)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












