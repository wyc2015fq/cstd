# leetcode-139. Word Break 单词拆分 - 别说话写代码的博客 - CSDN博客





2019年03月18日 15:15:45[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：18
所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)








|Given a **non-empty** string *s* and a dictionary *wordDict* containing a list of **non-empty** words, determine if *s* can be segmented into a space-separated sequence of one or more dictionary words.**Note:**- The same word in the dictionary may be reused multiple times in the segmentation.- You may assume the dictionary does not contain duplicate words.**Example 1:****Input:** s = "leetcode", wordDict = ["leet", "code"]**Output:** true**Explanation:** Return true because `"leetcode"` can be segmented as `"leet code"`.**Example 2:****Input:** s = "applepenapple", wordDict = ["apple", "pen"]**Output:** true**Explanation:** Return true because `"`applepenapple`"` can be segmented as `"`apple pen apple`"`.             Note that you are allowed to reuse a dictionary word.**Example 3:****Input:** s = "catsandog", wordDict = ["cats", "dog", "sand", "and", "cat"]**Output:** false|给定一个**非空**字符串 *s* 和一个包含**非空**单词列表的字典 *wordDict*，判定 *s* 是否可以被空格拆分为一个或多个在字典中出现的单词。**说明：**- 拆分时可以重复使用字典中的单词。- 你可以假设字典中没有重复的单词。**示例 1：****输入:** s = "leetcode", wordDict = ["leet", "code"]**输出:** true**解释:** 返回 true 因为 "leetcode" 可以被拆分成 "leet code"。**示例 2：****输入:** s = "applepenapple", wordDict = ["apple", "pen"]**输出:** true**解释:** 返回 true 因为 `"`applepenapple`"` 可以被拆分成 `"`apple pen apple`"`。     注意你可以重复使用字典中的单词。**示例 3：****输入:** s = "catsandog", wordDict = ["cats", "dog", "sand", "and", "cat"]**输出:** false|
|----|----|

思路：求字符串子串类型的题目以及与字符串相关的题目大多都可以 用dp。只用想一下dp如何 求解即可，找到dp的递推公式。

定一个dp数组，长度为字符串长度+1，多一个表示 空串的情况。其中dp[i]表示[0,i)范围内子串是否可以拆分，初始化dp[0]=true;开始遍历有两个循环，表示将(0,i)拆分为(0,j)和(j,i)，并分别查找拆分后的数组是否在worddict中，如果都在，则只需将dp[i]=true，然后break；遍历完dp数组，返回dp数组最后一个是true还是false即可 

```cpp
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> wordset(wordDict.begin(),wordDict.end());
        vector<bool> dp(s.size()+1);
        dp[0]=true;
        for(int i=0;i<dp.size();++i)
        {
            for(int j=0;j<i;++j)
            {
                if(dp[j]&&wordset.count(s.substr(j,i-j))) //(0,i)可以分为(0,j)和(j,i).其中(0,j)与(j,i)都在wordset中
                {
                    dp[i]=true;
                    break;
                }
            }
        }return dp.back();
    }
};
```





