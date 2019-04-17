# leetcode-28. Implement strStr() 实现strStr() - 别说话写代码的博客 - CSDN博客





2019年01月07日 13:23:05[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：14标签：[28. Implement strStr()																[leecode																[实现strStr()](https://so.csdn.net/so/search/s.do?q=实现strStr()&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leecode&t=blog)




题目：
|Implement [strStr()](http://www.cplusplus.com/reference/cstring/strstr/).Return the index of the first occurrence of needle in haystack, or **-1** if needle is not part of haystack.**Example 1:****Input:** haystack = "hello", needle = "ll"**Output:** 2**Example 2:****Input:** haystack = "aaaaa", needle = "bba"**Output:** -1**Clarification:**What should we return when `needle` is an empty string? This is a great question to ask during an interview.For the purpose of this problem, we will return 0 when `needle` is an empty string. This is consistent to C's [strstr()](http://www.cplusplus.com/reference/cstring/strstr/) and Java's [indexOf()](https://docs.oracle.com/javase/7/docs/api/java/lang/String.html#indexOf(java.lang.String)).|实现 [strStr()](https://baike.baidu.com/item/strstr/811469) 函数。给定一个 haystack 字符串和一个 needle 字符串，在 haystack 字符串中找出 needle 字符串出现的第一个位置 (从0开始)。如果不存在，则返回  **-1**。**示例 1:****输入:** haystack = "hello", needle = "ll"**输出:** 2**示例 2:****输入:** haystack = "aaaaa", needle = "bba"**输出:** -1**说明:**当 `needle` 是空字符串时，我们应当返回什么值呢？这是一个在面试中很好的问题。对于本题而言，当 `needle` 是空字符串时我们应当返回 0 。这与C语言的 [strstr()](https://baike.baidu.com/item/strstr/811469) 以及 Java的 [indexOf()](https://docs.oracle.com/javase/7/docs/api/java/lang/String.html#indexOf(java.lang.String)) 定义相符。|
|----|----|

思路：haystack从前往后查找。

```cpp
class Solution {
public:
    int strStr(string haystack, string needle) {
        int m=haystack.size(),n=needle.size(),i,j,p=0;
        if(n==0) return 0;
        if(m<n) return -1;
        for(i=0;i<=m-n;++i)
        {
            p=0;
            for(j=0;j<n;++j)
            {
                if(haystack[i+j]==needle[j])
                    p++;
            }
            if(p==n) return i;
        }
        return -1;
    }
};
```](https://so.csdn.net/so/search/s.do?q=28. Implement strStr()&t=blog)




