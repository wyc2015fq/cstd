# [LeetCode] Similar RGB Color 相似的红绿蓝颜色 - Grandyang - 博客园







# [[LeetCode] Similar RGB Color 相似的红绿蓝颜色](https://www.cnblogs.com/grandyang/p/9296602.html)







In the following, every capital letter represents some hexadecimal digit from `0` to `f`.

The red-green-blue color `"#AABBCC"` can be written as `"#ABC"` in shorthand.  For example, `"#15c"` is shorthand for the color `"#1155cc"`.

Now, say the similarity between two colors `"#ABCDEF"` and `"#UVWXYZ"` is `-(AB - UV)^2 - (CD - WX)^2 - (EF - YZ)^2`.

Given the color `"#ABCDEF"`, return a 7 character color that is most similar to `#ABCDEF`, and has a shorthand (that is, it can be represented as some `"#XYZ"`
Example 1:
Input: color = "#09f166"
Output: "#11ee66"
Explanation:  
The similarity is -(0x09 - 0x11)^2 -(0xf1 - 0xee)^2 - (0x66 - 0x66)^2 = -64 -9 -0 = -73.
This is the highest among any shorthand color.

Note:
- `color` is a string of length `7`.
- `color` is a valid RGB color: for `i > 0`, `color[i]` is a hexadecimal digit from `0` to `f`
- Any answer which has the same (highest) similarity as the best answer will be accepted.
- All inputs and outputs should use lowercase letters, and the output is 7 characters.



这道题定义了一种表示颜色的十六进制字符串，然后说是有一种两两字符相等的颜色可以缩写。然后又给了我们一个人一的字符串，让我们找出距离其最近的可以缩写的颜色串。题意不难理解，而且还是Easy标识符，所以我们要有信心可以将其拿下。那么通过分析题目中给的例子, 我们知道可以将给定的字符串拆成三个部分，每个部分分别来进行处理，比如对于字符串"#09f166"来说，我们就分别处理"09"，"f1"，"66"即可。我们的目标是要将每部分的两个字符变为相同，并且跟原来的距离最小，那么实际上我们并不需要遍历所有的组合，因为比较有参考价值的就是十位上的数字，因为如果十位上的数字不变，或者只是增减1，而让个位上的数字变动大一些，这样距离会最小，因为个位上的数字权重最小。就拿"09"来举例，这个数字可以变成"11"或者"00"，十六进制数"11"对应的十进制数是17，跟"09"相差了8，而十六进制数"00"对应的十进制数是0，跟"09"相差了9，显然我们选择"11"会好一些。所以我们的临界点是"8"，如果个位上的数字大于"8"，那么十位上的数就加1。

下面来看如何确定十位上的数字，比如拿"e1"来举例，其十进制数为225，其可能的选择有"ff"，"ee"，和"dd"，其十进制数分别为255，238，和221，我们目测很容易看出来是跟"dd"离得最近，但是怎么确定十位上的数字呢。我们发现"11"，"22"，"33"，"44"... 这些数字之间相差了一个"11"，十进制数为17，所以我们只要将原十六进制数除以一个"11"，就知道其能到达的位置，比如"e1"除以"11"，就只能到达"d"，那么十进制上就是"d"，至于个位数的处理情况跟上面一段讲解相同，我们对"11"取余，然后跟临界点"8"比较，如果个位上的数字大于"8"，那么十位上的数就加1。这样就可以确定正确的数字了，那么组成正确的十六进制字符串即可，参见代码如下：



解法一：

```
class Solution {
public:
    string similarRGB(string color) {
        return "#" + helper(color.substr(1, 2)) + helper(color.substr(3, 2)) + helper(color.substr(5, 2));
    }
    string helper(string str) {
        string dict = "0123456789abcdef";
        int num = stoi(str, nullptr, 16);
        int idx = num / 17 + (num % 17 > 8 ? 1 : 0);
        return string(2, dict[idx]);
    }
};
```



我们也可以不用helper函数，直接在一个函数中搞定即可，参见代码如下：



解法二：

```
class Solution {
public:
    string similarRGB(string color) {
        for (int i = 1; i < color.size(); i += 2) {
            int num = stoi(color.substr(i, 2), nullptr, 16);
            int idx = num / 17 + (num % 17 > 8 ? 1 : 0);
            color[i] = color[i + 1] = (idx > 9) ? (idx - 10 + 'a') : (idx + '0');
        }
        return color;
    }
};
```



参考资料：

[https://leetcode.com/problems/similar-rgb-color/solution/](https://leetcode.com/problems/similar-rgb-color/solution/)

[https://leetcode.com/problems/similar-rgb-color/discuss/120077/C++-Concise-Solution-with-explanation-6ms](https://leetcode.com/problems/similar-rgb-color/discuss/120077/C++-Concise-Solution-with-explanation-6ms)

[https://leetcode.com/problems/similar-rgb-color/discuss/120093/C++-O(1)-time-and-space-Easy-to-Understand](https://leetcode.com/problems/similar-rgb-color/discuss/120093/C++-O(1)-time-and-space-Easy-to-Understand)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












