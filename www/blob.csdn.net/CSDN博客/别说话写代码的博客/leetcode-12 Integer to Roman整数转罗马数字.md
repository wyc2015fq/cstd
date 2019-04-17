# leetcode-12:Integer to Roman整数转罗马数字 - 别说话写代码的博客 - CSDN博客





2018年12月17日 21:38:55[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：31标签：[leetcode 12																[整数转罗马数字																[Integer to Roman](https://so.csdn.net/so/search/s.do?q=Integer to Roman&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=整数转罗马数字&t=blog)




题目：
|Roman numerals are represented by seven different symbols: `I`, `V`, `X`, `L`, `C`, `D` and `M`.**Symbol****Value**I             1V             5X             10L             50C             100D             500M             1000For example, two is written as `II` in Roman numeral, just two one's added together. Twelve is written as, `XII`, which is simply `X` + `II`. The number twenty seven is written as `XXVII`, which is `XX` + `V` + `II`.Roman numerals are usually written largest to smallest from left to right. However, the numeral for four is not `IIII`. Instead, the number four is written as `IV`. Because the one is before the five we subtract it making four. The same principle applies to the number nine, which is written as `IX`. There are six instances where subtraction is used:- `I` can be placed before `V` (5) and `X` (10) to make 4 and 9. - `X` can be placed before `L` (50) and `C` (100) to make 40 and 90. - `C` can be placed before `D` (500) and `M` (1000) to make 400 and 900.Given an integer, convert it to a roman numeral. Input is guaranteed to be within the range from 1 to 3999.**Example 1:****Input:** 3**Output:** "III"**Example 2:****Input:** 4**Output:** "IV"**Example 3:****Input:** 9**Output:** "IX"**Example 4:****Input:** 58**Output:** "LVIII"**Explanation:** L = 50, V = 5, III = 3.**Example 5:****Input:** 1994**Output:** "MCMXCIV"**Explanation:** M = 1000, CM = 900, XC = 90 and IV = 4.|罗马数字包含以下七种字符： `I`， `V`， `X`， `L`，`C`，`D` 和 `M`。**字符****数值**I             1V             5X             10L             50C             100D             500M             1000例如， 罗马数字 2 写做 `II` ，即为两个并列的 1。12 写做 `XII` ，即为 `X` + `II` 。 27 写做  `XXVII`, 即为 `XX` + `V` + `II` 。通常情况下，罗马数字中小的数字在大的数字的右边。但也存在特例，例如 4 不写做 `IIII`，而是 `IV`。数字 1 在数字 5 的左边，所表示的数等于大数 5 减小数 1 得到的数值 4 。同样地，数字 9 表示为 `IX`。这个特殊的规则只适用于以下六种情况：- `I` 可以放在 `V` (5) 和 `X` (10) 的左边，来表示 4 和 9。- `X` 可以放在 `L` (50) 和 `C` (100) 的左边，来表示 40 和 90。 - `C` 可以放在 `D` (500) 和 `M` (1000) 的左边，来表示 400 和 900。给定一个整数，将其转为罗马数字。输入确保在 1 到 3999 的范围内。**示例 1:****输入:** 3**输出:** "III"**示例 2:****输入:** 4**输出:** "IV"**示例 3:****输入:** 9**输出:** "IX"**示例 4:****输入:** 58**输出:** "LVIII"**解释:** L = 50, V = 5, III = 3.**示例 5:****输入:** 1994**输出:** "MCMXCIV"**解释:** M = 1000, CM = 900, XC = 90, IV = 4.|
|----|----|

思路：用两个表，分别从大到小排列，将数字从大到小减下去，减的位置的字符串就是最终罗马数字。

```cpp
class Solution {
public:
    string intToRoman(int num) {
        vector<int> n{1000,900,500,400,100,90,50,40,10,9,5,4,1};
        vector<string> s{"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV",
                         "I"};
        string ret;
        for(int i=0;i<n.size();++i)
        {
            while(num>=n[i])
            {
                num-=n[i];
                ret+=s[i];
            }
        }return ret;
    }
};
```](https://so.csdn.net/so/search/s.do?q=leetcode 12&t=blog)




