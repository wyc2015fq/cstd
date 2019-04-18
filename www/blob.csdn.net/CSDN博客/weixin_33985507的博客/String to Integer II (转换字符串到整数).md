# String to Integer II (转换字符串到整数) - weixin_33985507的博客 - CSDN博客
2017年09月21日 23:33:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
# 问题
Implement function atoi to convert a string to an integer.
If no valid conversion could be performed, a zero value is returned.
If the correct value is out of the range of representable values, INT_MAX (2147483647) or INT_MIN (-2147483648) is returned.
Have you met this question in a real interview? Yes
Example
"10" => 10
"-1" => -1
"123123123123123" => 2147483647
"1.0" => 1
# 分析
基本没有什么难度，根据他的结果反馈，多对临界条件进行判断就可以了。
# 代码
```
public class Solution {
    /*
     * @param str: A string
     * @return: An integer
     */
    public int atoi(String str) {
        // write your code here
        if (str == null || str.length() == 0) {
            return 0;
        }
        str = str.trim();
        boolean isPositive = true;
        String src = "";
        for (int i = 0; i < str.length(); i++) {
            char c = str.charAt(i);
            if (i == 0) {
                if (c == '-') {
                    isPositive = false;
                    continue;
                } else if (c == '+') {
                    continue;
                }
            }
            if (c == '.') {
                break;
            }
            if (Character.isDigit(c)) {
                src += c;
            } else {
                break;
            }
        }
        if (src.length() == 0) {
            return 0;
        }
        try {
            int d = Integer.parseInt(src);
            if (isPositive) {
                return d;
            } else {
                return -d;
            }
        } catch (NumberFormatException e) {
            if (isPositive) {
                return Integer.MAX_VALUE;
            } else {
                return Integer.MIN_VALUE;
            }
        }
    }
}
```
