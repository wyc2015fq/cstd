# LeetCode 罗马数字与阿拉伯数字的转换 - 紫芝的博客 - CSDN博客





2018年11月22日 22:37:25[紫芝](https://me.csdn.net/qq_40507857)阅读数：78








# 罗马数字->阿拉伯数字

## [Roman to Integer](https://leetcode.com/problems/roman-to-integer/)

```cpp
class Solution {
public:
    int romanToInt(string s) {
        map<char, int> mymap = {
            {'I', 1}, {'V', 5}, {'X', 10}, {'L', 50}, 
            {'C', 100}, {'D', 500}, {'M', 1000}
        };
        int result = 0, i = 0, temp;
        for (i = 0; i < s.length() - 1; i++) {
            temp = mymap[s[i]];
            if (temp < mymap[s[i + 1]]) {
                result -= temp;
            } else {
                result += temp;
            }
        }
        result += mymap[s[i]];
        return result;
    }
};
```

# 阿拉伯数字->罗马数字

## [Integer to Roman](https://leetcode.com/problems/integer-to-roman/)

```cpp
class Solution {
public:
    string intToRoman(int num) {
        // table表存储十进制数上每个位所表示的罗马数字
        // 例如， 365 中 百位上的 3 大小为 table[2][3] = CCC
        string table[4][10] = {
            {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"},
            {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"},
            {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"},
            {"", "M", "MM", "MMM", "", "", "", "", "", ""}
        };
        string roman_numeral = "";
        roman_numeral += table[3][num / 1000];
        num %= 1000;
        roman_numeral += table[2][num / 100];
        num %= 100;
        roman_numeral += table[1][num / 10];
        num %= 10;
        roman_numeral += table[0][num];
        return roman_numeral;
    }
};
```



