# leetcode13：Roman to Integer罗马数字转整形 - 别说话写代码的博客 - CSDN博客





2018年12月18日 10:29:51[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：34标签：[罗马数字转整形																[Roman to Integer																[leetcode 13](https://so.csdn.net/so/search/s.do?q=leetcode 13&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Roman to Integer&t=blog)




题目：
|Roman numerals are represented by seven different symbols: `I`, `V`, `X`, `L`, `C`, `D` and `M`.**Symbol****Value**I             1V             5X             10L             50C             100D             500M             1000For example, two is written as `II` in Roman numeral, just two one's added together. Twelve is written as, `XII`, which is simply `X` + `II`. The number twenty seven is written as `XXVII`, which is `XX` + `V` + `II`.Roman numerals are usually written largest to smallest from left to right. However, the numeral for four is not `IIII`. Instead, the number four is written as `IV`. Because the one is before the five we subtract it making four. The same principle applies to the number nine, which is written as `IX`. There are six instances where subtraction is used:- `I` can be placed before `V` (5) and `X` (10) to make 4 and 9. - `X` can be placed before `L` (50) and `C` (100) to make 40 and 90. - `C` can be placed before `D` (500) and `M` (1000) to make 400 and 900.Given a roman numeral, convert it to an integer. Input is guaranteed to be within the range from 1 to 3999.**Example 1:****Input:** "III"**Output:** 3**Example 2:****Input:** "IV"**Output:** 4**Example 3:****Input:** "IX"**Output:** 9**Example 4:****Input:** "LVIII"**Output:** 58**Explanation:** L = 50, V= 5, III = 3.**Example 5:****Input:** "MCMXCIV"**Output:** 1994**Explanation:** M = 1000, CM = 900, XC = 90 and IV = 4.|罗马数字包含以下七种字符: `I`， `V`， `X`， `L`，`C`，`D` 和 `M`。**字符****数值**I             1V             5X             10L             50C             100D             500M             1000例如， 罗马数字 2 写做 `II` ，即为两个并列的 1。12 写做 `XII` ，即为 `X` + `II` 。 27 写做  `XXVII`, 即为 `XX` + `V` + `II` 。通常情况下，罗马数字中小的数字在大的数字的右边。但也存在特例，例如 4 不写做 `IIII`，而是 `IV`。数字 1 在数字 5 的左边，所表示的数等于大数 5 减小数 1 得到的数值 4 。同样地，数字 9 表示为 `IX`。这个特殊的规则只适用于以下六种情况：- `I` 可以放在 `V` (5) 和 `X` (10) 的左边，来表示 4 和 9。- `X` 可以放在 `L` (50) 和 `C` (100) 的左边，来表示 40 和 90。 - `C` 可以放在 `D` (500) 和 `M` (1000) 的左边，来表示 400 和 900。给定一个罗马数字，将其转换成整数。输入确保在 1 到 3999 的范围内。**示例 1:****输入:** "III"**输出:** 3**示例 2:****输入:** "IV"**输出:** 4**示例 3:****输入:** "IX"**输出:** 9**示例 4:****输入:** "LVIII"**输出:** 58**解释:** L = 50, V= 5, III = 3.**示例 5:****输入:** "MCMXCIV"**输出:** 1994**解释:** M = 1000, CM = 900, XC = 90, IV = 4.|
|----|----|

思路：用哈希表 映射字符与对应的值，从前往后依次扫描。当从刚开始或者左边的字母大于等于右边的字符，则直接加上对应的数，否则i不为0且左边的字母小于右边的字母，则需要加上右边字母的值减去左边字母的值， 这里注意我们从前往后扫描，会多加一次较小值，所以在此多减去1个 较小的值，比如"IV"，先ret=ret+1，然后V比I大，ret+=5-1,然后相当于多加了第一个1，再减去就可以了ret=4;

```cpp
class Solution {
public:
    int romanToInt(string s) {
        int ret=0;
        unordered_map<char,int> m{{'I',1},{'V',5},{'X',10},{'L',50},{'C',100},
                                  {'D',500},{'M',1000}};
        for(int i=0;i<s.size();++i)
        {
            if(i==0 || m[s[i]]<=m[s[i-1]]) ret += m[s[i]];
            else ret += m[s[i]] - 2*m[s[i-1]];
        }return ret;
    }
};
```](https://so.csdn.net/so/search/s.do?q=罗马数字转整形&t=blog)




