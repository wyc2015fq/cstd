# LeetCode Longest Substring Without Repeating Characters 最长不重复子串 - xcw0754 - 博客园
# [LeetCode Longest Substring Without Repeating Characters 最长不重复子串](https://www.cnblogs.com/xcw0754/p/4399601.html)
![](https://images0.cnblogs.com/blog2015/641737/201504/072139433999650.png)
题意：给一字符串，求一个子串的长度，该子串满足所有字符都不重复。字符可能包含标点之类的，不仅仅是字母。按ASCII码算，就有2^8=128个。
思路：从左到右扫每个字符，判断该字符距离上一次出现的距离是多少，若大于max，则更新max。若小于，则不更新。每扫到一个字符就需要更新他的出现位置了。这里边还有个注意点，举例说明：
假如有长为16串 s="arbtbqwecpoiuyca" 
当扫到第2个b时，距离上一个b的距离是2；（直接减）
当扫到第2个c时，距离上一个c的距离是6；（直接减）
但是！当扫到第2个a时，距离上一个a的距离是15，可是这串里面已经有b和c都有重复的了，是不符合的。真正的长 = 第2个a的位置 - 第1个c的位置。
假设当前扫到的字符为'A'，其实求长的式子应该这样的：len = i - max（cur，pos[A]）    
这里的cur是指一个字符的位置，该字符是距离A最近的，并且在该字符与A之间还会出现该字符一次，（也就是在两个A之间，如果有出现次数为两次的字符，记录下第1个字符的位置，若多次出现，记录从右数第2次出现该字符的位置）这个cur的值要随时更新。
注：坑！这个算法的复杂度完全的O(n)，实在强大。想了2天，我本来想到用哈希来做，感觉有点麻烦，一直在想更简单的，下面这个别人的代码实在简洁到没办法了，佩服。
```
1 class Solution {
 2 public:
 3     int lengthOfLongestSubstring(string s) {
 4         // Start typing your C/C++ solution below
 5         // DO NOT write int main() function
 6         int locs[256];//保存字符上一次出现的位置
 7         memset(locs, -1, sizeof(locs));
 8 
 9         int idx = -1, max = 0;//idx为当前子串的开始位置-1
10         for (int i = 0; i < s.size(); i++)
11         {
12             if (locs[s[i]] > idx)//如果当前字符出现过，那么当前子串的起始位置为这个字符上一次出现的位置+1
13             {
14                 idx = locs[s[i]];
15             }
16 
17             if (i - idx > max)
18             {
19                 max = i - idx;
20             }
21 
22             locs[s[i]] = i;
23         }
24         return max;
25     }
26 };
Longest Substring Without Repeating Characters
```
上面代码一字不差复制过来了。

