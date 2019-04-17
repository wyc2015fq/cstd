# [LeetCode] Add Bold Tag in String 字符串中增添加粗标签 - Grandyang - 博客园







# [[LeetCode] Add Bold Tag in String 字符串中增添加粗标签](https://www.cnblogs.com/grandyang/p/7043394.html)







Given a string s and a list of strings dict, you need to add a closed pair of bold tag `<b>` and `</b>` to wrap the substrings in s that exist in dict. If two such substrings overlap, you need to wrap them together by only one pair of closed bold tag. Also, if two substrings wrapped by bold tags are consecutive, you need to combine them.

Example 1:
Input: 
s = "abcxyz123"
dict = ["abc","123"]
Output:
"<b>abc</b>xyz<b>123</b>"



Example 2:
Input: 
s = "aaabbcc"
dict = ["aaa","aab","bc"]
Output:
"<b>aaabbc</b>c"



Note:
- The given dict won't contain duplicates, and its length won't exceed 100.
- All the strings in input have length in range [1, 1000].



这道题给我们了一个字符串，还有一个字典，让我们把字符串中在字典中的单词加粗，注意如果两个单词有交集或者相接，就放到同一个加粗标签中。博主刚开始的想法是，既然需要匹配字符串，那么就上KMP大法，然后得到每个单词在字符串匹配的区间位置，然后再合并区间，再在合并后的区间两头加标签。但是一看题目难度，Medium，中等难度的题不至于要祭出KMP大法吧，于是去网上扫了一眼众神们的解法，发现大多都是暴力匹配啊，既然OJ能过去，那么就一起暴力吧。这题参考的是高神[shawngao](https://discuss.leetcode.com/user/shawngao)的解法，高神可是集了一千五百多个赞的男人，叼到飞起！思路是建一个和字符串s等长的bold布尔型数组，表示如果该字符在单词里面就为true，那么最后我们就可以根据bold数组的真假值来添加标签了。我们遍历字符串s中的每一个字符，把遍历到的每一个字符当作起始位置，我们都匹配一遍字典中的所有单词，如果能匹配上，我们就用i + len来更新end，len是当前单词的长度，end表示字典中的单词在字符串s中结束的位置，那么如果i小于end，bold[i]就要赋值为true了。最后我们更新完bold数组了，就再遍历一遍字符串s，如果bold[i]为false，直接将s[i]加入结果res中；如果bold[i]为true，那么我们用while循环来找出所有连续为true的个数，然后在左右两端加上标签，参见代码如下：



解法一：

```
class Solution {
public:
    string addBoldTag(string s, vector<string>& dict) {
        string res = "";
        int n = s.size(), end = 0;
        vector<bool> bold(n, false);
        for (int i = 0; i < n; ++i) {
            for (string word : dict) {
                int len = word.size();
                if (i + len <= n && s.substr(i, len) == word) {
                    end = max(end, i + len);
                }
            }
            bold[i] = end > i;
        }
        for (int i = 0; i < n; ++i) {
            if (!bold[i]) {
                res.push_back(s[i]);
                continue;
            }
            int j = i;
            while (j < n && bold[j]) ++j;
            res += "<b>" + s.substr(i, j - i) + "</b>";
            i = j - 1;
        }
        return res;
    }
};
```



这道题跟之后的那道[Bold Words in String](http://www.cnblogs.com/grandyang/p/8531642.html)是一模一样的题，那么解法当然是可以互通的了，这里我们把那道题中解法二也贴过来吧，由于解法一和解法二实在是太相似了，就贴一个吧，具体讲解可以参见[Bold Words in String](http://www.cnblogs.com/grandyang/p/8531642.html)这篇帖子，参见代码如下：



解法二：

```
class Solution {
public:
    string addBoldTag(string s, vector<string>& dict) {
        string res = "";
        int n = s.size();
        unordered_set<int> bold;
        for (string word : dict) {
            int len = word.size();
            for (int i = 0; i <= n - len; ++i) {
                if (s[i] == word[0] && s.substr(i, len) == word) {
                    for (int j = i; j < i + len; ++j) bold.insert(j);
                }
            }
        }
        for (int i = 0; i < n; ++i) {
            if (bold.count(i) && !bold.count(i - 1)) res += "<b>";
            res += s[i];
            if (bold.count(i) && !bold.count(i + 1)) res += "</b>";
        }
        return res;
    }
};
```



类似题目：

[Tag Validator](http://www.cnblogs.com/grandyang/p/7016476.html)

[Merge Intervals](http://www.cnblogs.com/grandyang/p/4370601.html)

[Bold Words in String](http://www.cnblogs.com/grandyang/p/8531642.html)



参考资料：

[https://discuss.leetcode.com/topic/92112/java-solution-boolean-array](https://discuss.leetcode.com/topic/92112/java-solution-boolean-array)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












