# [LeetCode] String Compression 字符串压缩 - Grandyang - 博客园







# [[LeetCode] String Compression 字符串压缩](https://www.cnblogs.com/grandyang/p/8742564.html)







Given an array of characters, compress it [in-place](https://en.wikipedia.org/wiki/In-place_algorithm).

The length after compression must always be smaller than or equal to the original array.

Every element of the array should be a character (not int) of length 1.

After you are done modifying the input array [in-place](https://en.wikipedia.org/wiki/In-place_algorithm), return the new length of the array.



Follow up:
Could you solve it using only O(1) extra space?



Example 1:
Input:
["a","a","b","b","c","c","c"]

Output:
Return 6, and the first 6 characters of the input array should be: ["a","2","b","2","c","3"]

Explanation:
"aa" is replaced by "a2". "bb" is replaced by "b2". "ccc" is replaced by "c3".



Example 2:
Input:
["a"]

Output:
Return 1, and the first 1 characters of the input array should be: ["a"]

Explanation:
Nothing is replaced.



Example 3:
Input:
["a","b","b","b","b","b","b","b","b","b","b","b","b"]

Output:
Return 4, and the first 4 characters of the input array should be: ["a","b","1","2"].

Explanation:
Since the character "a" does not repeat, it is not compressed. "bbbbbbbbbbbb" is replaced by "b12".
Notice each digit has it's own entry in the array.



Note:
- All characters have an ASCII value in `[35, 126]`.
- `1 <= len(chars) <= 1000`.



这道题给了我们一个字符串，让我们进行压缩，即相同的字符统计出个数，显示在该字符之后，根据例子分析不难理解题意。这道题要求我们进行in place操作，即不使用额外空间，最后让我们返回修改后的新数组的长度。我们首先想，数组的字符不一定是有序的，如果我们用Map来建立字符和出现次数之间的映射，不管是用HashMap还是TreeMap，一定无法保证原有的顺序。所以不能用Map，而我们有需要统计个数，那么双指针就是不二之选啦。既然双指针，其中一个指针指向重复字符串的第一个，然后另一个指针向后遍历并计数，就能得到重复的个数。我们仔细研究例子3，可以发现，当个数是两位数的时候，比如12，这里是将12拆分成1和2，然后存入数组的。那么比较简便的提取出各个位上的数字的办法就是转为字符串进行遍历。另外，由于我们需要对原数组进行修改，则需要一个指针cur来标记下一个可以修改的位置，那么最终cur的值就是新数组的长度，直接返回即可。

具体来看代码，我们用i和j表示双指针，开始循环后，我们用j来找重复的字符串的个数，用一个while循环，最终j指向的是第一个和i指向字符不同的地方，此时我们需要先将i位置的字符写进chars中，然后我们判断j是否比i正好大一个，因为只有一个字符的话，后面是不用加个数的，所以直接跳过。否则我们将重复个数转为字符串，然后提取出来修改chars数组即可，注意每次需要将i赋值为j，从而开始下一个字符的统计，参见代码如下：



```
class Solution {
public:
    int compress(vector<char>& chars) {
        int n = chars.size(), cur = 0;
        for (int i = 0, j = 0; i < n; i = j) {
            while (j < n && chars[j] == chars[i]) ++j;
            chars[cur++] = chars[i];
            if (j - i == 1) continue;
            for (char c : to_string(j - i)) chars[cur++] = c;
        }
        return cur;
    }
};
```



类似题目：

[Count and Say](http://www.cnblogs.com/grandyang/p/4086299.html)

[Encode and Decode Strings](http://www.cnblogs.com/grandyang/p/5265628.html)

[Design Compressed String Iterator](http://www.cnblogs.com/grandyang/p/7026999.html)



参考资料：

[https://leetcode.com/problems/string-compression/discuss/118472/C++-simple-solution-(-EASY-to-understand-)-with-explanation](https://leetcode.com/problems/string-compression/discuss/118472/C++-simple-solution-(-EASY-to-understand-)-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












