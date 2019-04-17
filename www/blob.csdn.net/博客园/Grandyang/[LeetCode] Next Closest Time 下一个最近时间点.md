# [LeetCode] Next Closest Time 下一个最近时间点 - Grandyang - 博客园







# [[LeetCode] Next Closest Time 下一个最近时间点](https://www.cnblogs.com/grandyang/p/7623614.html)








Given a time represented in the format "HH:MM", form the next closest time by reusing the current digits. There is no limit on how many times a digit can be reused.

You may assume the given input string is always valid. For example, "01:34", "12:09" are all valid. "1:34", "12:9" are all invalid.

Example 1:
Input: "19:34"
Output: "19:39"
Explanation: The next closest time choosing from digits 1, 9, 3, 4, is 19:39, which occurs 5 minutes later.  It is not 19:33, because this occurs 23 hours and 59 minutes later.



Example 2:
Input: "23:59"
Output: "22:22"
Explanation: The next closest time choosing from digits 2, 3, 5, 9, is 22:22. It may be assumed that the returned time is next day's time since it is smaller than the input time numerically.



这道题给了我们一个时间点，让我们求最近的下一个时间点，规定了不能产生新的数字，当下个时间点超过零点时，就当第二天的时间。为了找到下一个时间点，我们肯定是从分钟开始换数字，而且换的数字要是存在的数字，那么我们最先要做的就是统计当前时间点中的数字，由于可能有重复数字的存在，我们把数字都存入集合set中，这样可以去除重复数字，并且可以排序，然后再转为vector。下面就从低位分钟开始换数字了，如果低位分钟上的数字已经是最大的数字了，那么说明要转过一轮了，就要把低位分钟上的数字换成最小的那个数字；否则就将低位分钟上的数字换成下一个数字。然后再看高位分钟上的数字，同理，如果高位分钟上的数字已经是最大的数字，或则下一个数字大于5，那么直接换成最小值；否则就将高位分钟上的数字换成下一个数字。对于小时位上的数字也是同理，对于小时低位上的数字情况比较复杂，当小时高位不为2的时候，低位可以是任意数字，而当高位为2时，低位需要小于等于3。对于小时高位，其必须要小于等于2，参见代码如下：



解法一：

```
class Solution {
public:
    string nextClosestTime(string time) {
        string res = time;
        set<int> s{time[0], time[1], time[3], time[4]};
        string str(s.begin(), s.end());
        for (int i = res.size() - 1; i >= 0; --i) {
            if (res[i] == ':') continue;
            int pos = str.find(res[i]);
            if (pos == str.size() - 1) {
                res[i] = str[0];
            } else {
                char next = str[pos + 1];
                if (i == 4) {
                    res[i] = next;
                    return res;
                } else if (i == 3 && next <= '5') {
                    res[i] = next;
                    return res; 
                } else if (i == 1 && (res[0] != '2' || (res[0] == '2' && next <= '3'))) {
                    res[i] = next;
                    return res;
                } else if (i == 0 && next <= '2') {
                    res[i] = next;
                    return res;
                }
                res[i] = str[0];
            }
        }
        return res;
    }
};
```



下面这种方法的写法比较简洁，实际上用了暴力搜索，由于按分钟算的话，一天只有1440分钟，也就是1440个时间点，我们可以从当前时间点开始，遍历一天的时间，也就是接下来的1440个时间点，得到一个新的整型时间点后，我们按位来更新结果res，对于每个更新的数字字符，看其是否在原时间点字符中存在，如果不存在，直接break，然后开始遍历下一个时间点，如果四个数字都成功存在了，那么将当前时间点中间夹上冒号返回即可，参见代码如下：



解法二：

```
class Solution {
public:
    string nextClosestTime(string time) {
        string res = "0000";
        vector<int> v{600, 60, 10, 1};
        int found = time.find(":");
        int cur = stoi(time.substr(0, found)) * 60 + stoi(time.substr(found + 1));
        for (int i = 1, d = 0; i <= 1440; ++i) {
            int next = (cur + i) % 1440;
            for (d = 0; d < 4; ++d) {
                res[d] = '0' + next / v[d]; 
                next %= v[d];
                if (time.find(res[d]) == string::npos) break;
            }
            if (d >= 4) break;
        }
        return res.substr(0, 2) + ":" + res.substr(2);
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/104692/c-java-clean-code](https://discuss.leetcode.com/topic/104692/c-java-clean-code)

[https://discuss.leetcode.com/topic/104736/concise-java-solution](https://discuss.leetcode.com/topic/104736/concise-java-solution)

[https://discuss.leetcode.com/topic/105411/short-simple-java-using-regular-expression](https://discuss.leetcode.com/topic/105411/short-simple-java-using-regular-expression)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












