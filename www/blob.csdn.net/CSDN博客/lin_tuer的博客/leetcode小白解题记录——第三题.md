
# leetcode小白解题记录——第三题 - lin_tuer的博客 - CSDN博客


2016年08月18日 16:14:18[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：1200


3. Longest Substring Without Repeating Characters
Given a string, find the length of thelongest substringwithout repeating characters.
Examples:
Given`"abcabcbb"`, the answer is`"abc"`,
 which the length is 3.
Given`"bbbbb"`, the answer is`"b"`,
 with the length of 1.
Given`"pwwkew"`, the answer is`"wke"`,
 with the length of 3. Note that the answer must be asubstring,`"pwke"`is
 asubsequenceand not a substring.
主要考察字符串操作。

# 解题思路1：
> 用int型的start和end来记录目前考察的字符串的开头和结尾
> 用布尔型的数组exist[26]（因为一共有26个字母）来记录目前字符串中出现过的字母
> 用int型数组position[26]来记录出现过的字符串的字母的位置
每往后走一位，和exist来比较看这个字母是否已经出现过了。
1 如果出现过了，那么我们把start移动到之前那个字母出现的位置的后一位，
end往后移动一位；
2 如果没有出现过，那么我们就把end往后移动一位；


*01***int**lengthOfLongestSubstring(string s) {
*02***int**max =0, start =0;
*03***bool**exist[26];
*04***int**position[26];
05
*06***for**(**int**i =0; i <26; i++) {
*07*exist[i] =**false**;
*08*position[i] =0;
*09*}
10
*11***for**(**int**i =0; i < s.size(); i++) {
*12***if**(exist[s[i] -'a']) {
 //如果当前字母已存在，子字符串整体后移一位
*13***for**(**int**j = start; j <= position[s[i] -'a']; j++) {
*14*exist[s[j] -'a'] =**false**;
15}
*16*start = position[s[i] -'a'] +1;
*17*exist[s[i] -'a'] =**true**;
*18*position[s[i] -'a'] = i;
*19*}
20**else**{
*21*exist[s[i] -'a'] =**true**;
*22*position[s[i] -'a'] = i;
*23*max = max > (i - start +1) ? max : (i - start +1);
*24*}
25}
*26*
*27***return**max;
*28*}


//  测试显示，这种方法是有缺陷的，它只能处理内容为字母的字符串，对于其他的，比如标点，数字，等，完全不适合

# 解题思路2：
>遍历字符串，当当前字符出现过的时候，子串开始位置+1，
>否则更新locs数组中的hash值为当前位置


//我的水平还差的很多，这里面的memset()完全不知道是什么东西，更不要说用了-_-
```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        int locs[256];//保存字符上一次出现的位置
        memset(locs, -1, sizeof(locs));
        int idx = -1, max = 0;//idx为当前子串的开始位置-1
        for (int i = 0; i < s.size(); i++)
        {
            if (locs[s[i]] > idx)//如果当前字符出现过，那么当前子串的起始位置为这个字符上一次出现的位置+1
            {
                idx = locs[s[i]];
            }
            if (i - idx > max)
            {
                max = i - idx;
            }
            locs[s[i]] = i;
        }
        return max;
    }
};
```

//要学习的东西还有很多啊！


# 解题思路3：

》使用i和j两个指针进行搜索，i代表候选的最长子串的开头，j代表候选的最长子串的结尾。
》先假设i不动，那么在理想的情况下，我们希望可以一直右移j，直到j到达原字符串的结尾，此时j-i就构成了一个候选的最长子串。每次都维护一个max_length，就可以选出最长子串了。
》实际情况是，不一定可以一直右移j，如果字符j已经重复出现过（假设在位置k），就需要停止右移了。记录当前的候选子串并和max_length做比较。
接下来为下一次搜寻做准备。
》在下一次搜寻中，i应该更新到k+1。这句话的意思是，用这个例子来理解，abcdef是个不重复的子串，abcdefc中（为了方便记录为abc1defc2）,c1和c2重复了。那么下一次搜寻，应该跨过出现重复的地方进行，否则找出来的候选串依然有重复字符，且长度还不如上次的搜索。所以下一次搜索，直接从c1的下一个字符d开始进行，也就是说，下一次搜寻中，i应该更新到k+1。
LeetCode给出的参考答案：


```cpp
int lengthOfLongestSubstring(string s) {  
  int n = s.length();  
  int i = 0, j = 0;  
  int maxLen = 0;  
  bool exist[256] = { false };  //各种字符的ASCII码范围是从0-255，共256个
  while (j < n) {  
    if (exist[s[j]]) {  //如果现在检测的字符已经存在过了
      maxLen = max(maxLen, j-i);  //更新最大不重复长度
      while (s[i] != s[j]) {  //为下一轮不重复子字符串检测做准备，将之前存的已存在字符设为不存在
        exist[s[i]] = false;  
        i++;  
      }  
      i++;//子字符串开始指针后移一位  
      j++;  //尾指针后移，继续检测
    } else {  //如果当前的字符还没有出现过，那么，把它加入已存在字符中，更新已存在字符表
      exist[s[j]] = true;  
      j++;  //并将尾指针后移
    }  
  }  
  maxLen = max(maxLen, n-i);  
  return maxLen;  
}
```

时间复杂度是O(N)。虽然有两个while的嵌套，但是时间复杂度依然是O(N)，
因为i和j都只把这个string从开始到结束遍历了一遍。







