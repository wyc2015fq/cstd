# leetcode-3:Longest Substring Without Repeating Characters 最长无重复字符的子串 - 别说话写代码的博客 - CSDN博客





2018年12月09日 15:42:28[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：48
所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)









题目：
|Given a string, find the length of the **longest substring** without repeating characters.**Example 1:****Input: **"abcabcbb"**Output: **3 **Explanation:** The answer is `"abc"`, with the length of 3. **Example 2:****Input: **"bbbbb"**Output: **1**Explanation: **The answer is `"b"`, with the length of 1.**Example 3:****Input: **"pwwkew"**Output: **3**Explanation: **The answer is `"wke"`, with the length of 3.              Note that the answer must be a **substring**, `"pwke"` is a *subsequence* and not a substring.|给定一个字符串，请你找出其中不含有重复字符的 **最长子串 **的长度。**示例 1:****输入: **"abcabcbb"**输出: **3 **解释:** 因为无重复字符的最长子串是 `"abc"，所以其`长度为 3。**示例 2:****输入: **"bbbbb"**输出: **1**解释: **因为无重复字符的最长子串是 `"b"`，所以其长度为 1。**示例 3:****输入: **"pwwkew"**输出: **3**解释: **因为无重复字符的最长子串是 `"wke"`，所以其长度为 3。     请注意，你的答案必须是 **子串 **的长度，`"pwke"` 是一个*子序列，*不是子串。|
|----|----|

思路：给了我们一个字符串，让我们求最长的无重复字符的子串，注意这里是子串，不是子序列，所以必须是连续的。建立一个256位大小的整型数组来代替HashMap，这样做的原因是ASCII表共能表示256个字符，所以可以记录所有字符，然后我们需要定义两个变量maxlen和left，其中maxlen用来记录最长无重复子串的长度，left指向该无重复子串左边的起始位置，然后我们遍历整个字符串，对于每一个遍历到的字符，如果哈希表中该字符串对应的值为0，说明没有遇到过该字符，则此时计算最长无重复子串，i - left +１，其中ｉ是最长无重复子串最右边的位置，left是最左边的位置，还有一种情况也需要计算最长无重复子串，就是当哈希表中的值小于left，这是由于此时出现过重复的字符，left的位置更新了，如果又遇到了新的字符，就要重新计算最长无重复子串。最后每次都要在哈希表中将当前字符对应的值赋值为i+1。（利用滑动窗口思想）

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int m[256]={0},maxlen=0,left=0;
        for(int i=0;i<s.length();++i)
        {
            if(m[s[i]]==0 || m[s[i]]<left)   //从未出现过和未在当前滑动窗口内
                maxlen = max(maxlen,i-left+1);
            else 
            {
                left = m[s[i]];
            }
            m[s[i]] = i+1;
        }
        return maxlen;
    }
};
```





