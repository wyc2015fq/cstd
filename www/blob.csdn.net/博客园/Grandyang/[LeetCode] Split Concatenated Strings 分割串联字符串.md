# [LeetCode] Split Concatenated Strings 分割串联字符串 - Grandyang - 博客园







# [[LeetCode] Split Concatenated Strings 分割串联字符串](https://www.cnblogs.com/grandyang/p/6887140.html)







Given a list of strings, you could concatenate these strings together into a loop, where for each string you could choose to reverse it or not. Among all the possible loops, you need to find the lexicographically biggest string after cutting the loop, which will make the looped string into a regular one.

Specifically, to find the lexicographically biggest string, you need to experience two phases:
- Concatenate all the strings into a loop, where you can reverse some strings or not and connect them in the same order as given.
- Cut and make one breakpoint in any place of the loop, which will make the looped string into a regular one starting from the character at the cutpoint.



And your job is to find the lexicographically biggest one among all the possible regular strings.

Example:
Input: "abc", "xyz"
Output: "zyxcba"
Explanation: You can get the looped string "-abcxyz-", "-abczyx-", "-cbaxyz-", "-cbazyx-", 
where '-' represents the looped status. 
The answer string came from the fourth looped one, 
where you could cut from the middle character 'a' and get "zyxcba".



Note:
- The input strings will only contain lowercase letters.
- The total length of all the strings will not over 1,000.



这道题给了我们一些字符串，让我们将其连接起来，连接的时候对于每个字符串我们可以选择翻转或者不翻转，在行程的大的字符串上找一个位置cut掉，将该位置当作首字符，前面的字符串移动到末尾去，问怎么cut能使字符串的字母顺序大。刚开始博主想，既然要让最终字符串字母顺序最大，那么每一个字符串当然要尽可能的大了，所以如果其翻转字符串的字母顺序大的话，就要对字符串进行翻转。然后在组成的字符串中找最大的字符进行cut，然而这种思路不一定能得到正确的结果。比如字符串数组["lc", "love", "ydc"]，如果按照博主之前的思路得到的字符串应该为"ydclclove"，但正确结果应该是"ylclovecd"。我们可出来正确的答案中cut位置所在的字符串ydc，虽然cdy小于ydc，但还是翻转了。但是其他的字符都是按照字母顺序来确定要不要翻转的，那么我们可以得出这样的结论，只有cut所在的字符串的翻转可能不按规律。那么我们如何确定cut位置呢，其实没有太好的办法，只能遍历每一个字母。我们首先来根据字母顺序确定要不要翻转每一个字符串，将字母顺序大的连成一个字符串，然后遍历每一个字符串，在每一个字符串中遍历每一个位置，将当前遍历的字符串后面的所有字符串跟前面所有字符串先连起来，存入mid中，然后取出当前遍历的字符串中当前遍历的位置及其后面的字符取出，连上mid，然后再连上当前位置前面的字符，然后跟结果res比较，取较大者存入结果res。这里我们可以进行小优化，如果cut位置的字母大于等于结果res的首字母，我们才进行对比更新。注意我们在遍历每个字符串时，要将其翻转字符串的每一位也遍历了，这样才能涵盖所有的情况，参见代码如下：



```
class Solution {
public:
    string splitLoopedString(vector<string>& strs) {
        if (strs.empty()) return "";
        string s = "", res = "a";
        int n = strs.size(), cur = 0;
        for (string str : strs) {
            string t = string(str.rbegin(), str.rend());
            s += str > t ? str : t;
        }
        for (int i = 0; i < n; ++i) {          
            string t1 = strs[i], t2 = string(t1.rbegin(), t1.rend());
            string mid = s.substr(cur + t1.size()) + s.substr(0, cur);
            for (int j = 0; j < strs[i].size(); ++j) {
                if (t1[j] >= res[0]) res = max(res, t1.substr(j) + mid + t1.substr(0, j));
                if (t2[j] >= res[0]) res = max(res, t2.substr(j) + mid + t2.substr(0, j));
            }
            cur += strs[i].size();
        }
        return res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/86545/c-9ms-12-lines/2](https://discuss.leetcode.com/topic/86545/c-9ms-12-lines/2)

[https://discuss.leetcode.com/topic/86509/easy-understanding-c-solution-with-detailed-explnation](https://discuss.leetcode.com/topic/86509/easy-understanding-c-solution-with-detailed-explnation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












