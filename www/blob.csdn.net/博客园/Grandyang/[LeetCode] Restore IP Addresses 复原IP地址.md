# [LeetCode] Restore IP Addresses  复原IP地址 - Grandyang - 博客园







# [[LeetCode] Restore IP Addresses  复原IP地址](https://www.cnblogs.com/grandyang/p/4305572.html)







Given a string containing only digits, restore it by returning all possible valid IP address combinations.

Example:
Input: "25525511135"
Output: `["255.255.11.135", "255.255.111.35"]`


这道题要求是复原[IP地址](http://zh.wikipedia.org/wiki/IP%E5%9C%B0%E5%9D%80)，IP地址对我们并不陌生，就算我们不是学CS的，只要我们是广大网友之一，就应该对其并不陌生。IP地址由32位[二进制](http://zh.wikipedia.org/wiki/%E4%BA%8C%E8%BF%9B%E5%88%B6)数组成，为便于使用，常以XXX.XXX.XXX.XXX形式表现，每组XXX代表小于或等于255的10进制数。所以说IP地址总共有四段，每一段可能有一位，两位或者三位，范围是[0, 255]，题目明确指出输入字符串只含有数字，所以当某段是三位时，我们要判断其是否越界（>255)，还有一点很重要的是，当只有一位时，0可以成某一段，如果有两位或三位时，像 00， 01， 001， 011， 000等都是不合法的，所以我们还是需要有一个判定函数来判断某个字符串是否合法。这道题其实也可以看做是字符串的分段问题，在输入字符串中加入三个点，将字符串分为四段，每一段必须合法，求所有可能的情况。根据目前刷了这么多题，得出了两个经验，**一是只要遇到字符串的子序列或配准问题首先考虑动态规划DP，二是只要遇到需要求出所有可能情况首先考虑用递归**。这道题并非是求字符串的子序列或配准问题，更符合第二种情况，所以我们要用递归来解。我们用k来表示当前还需要分的段数，如果k = 0，则表示三个点已经加入完成，四段已经形成，若这时字符串刚好为空，则将当前分好的结果保存。若k != 0, 则对于每一段，我们分别用一位，两位，三位来尝试，分别判断其合不合法，如果合法，则调用递归继续分剩下的字符串，最终和求出所有合法组合，代码如下：



C++ 解法一：

```
class Solution {
public:
    vector<string> restoreIpAddresses(string s) {
        vector<string> res;
        restore(s, 4, "", res);
        return res;
    }
    void restore(string s, int k, string out, vector<string> &res) {
        if (k == 0) {
            if (s.empty()) res.push_back(out);
        }
        else {
            for (int i = 1; i <= 3; ++i) {
                if (s.size() >= i && isValid(s.substr(0, i))) {
                    if (k == 1) restore(s.substr(i), k - 1, out + s.substr(0, i), res);
                    else restore(s.substr(i), k - 1, out + s.substr(0, i) + ".", res);
                }
            }
        }
    }
    bool isValid(string s) {
        if (s.empty() || s.size() > 3 || (s.size() > 1 && s[0] == '0')) return false;
        int res = atoi(s.c_str());
        return res <= 255 && res >= 0;
    }
};
```



我们也可以省掉isValid函数，直接在调用递归之前用if语句来滤掉不符合题意的情况，这里面用了k != std::to_string(val).size()，其实并不难理解，比如当k=3时，说明应该是个三位数，但如果字符是"010"，那么转为整型val=10，再转回字符串就是"10"，此时的长度和k值不同了，这样就可以找出不合要求的情况了，参见代码如下；



C++ 解法二：

```
class Solution {
public:
    vector<string> restoreIpAddresses(string s) {
        vector<string> res;
        helper(s, 0, "", res);
        return res;
    }
    void helper(string s, int n, string out, vector<string>& res) {
        if (n == 4) {
            if (s.empty()) res.push_back(out);
        } else {
            for (int k = 1; k < 4; ++k) {
                if (s.size() < k) break;
                int val = atoi(s.substr(0, k).c_str());
                if (val > 255 || k != std::to_string(val).size()) continue;
                helper(s.substr(k), n + 1, out + s.substr(0, k) + (n == 3 ? "" : "."), res);
            }
        }
    }
};
```



Java 解法二：

```
public class Solution {
    public List<String> restoreIpAddresses(String s) {
        List<String> res = new ArrayList<String>();
        helper(s, 0, "", res);
        return res;
    }
    public void helper(String s, int n, String out, List<String> res) {
        if (n == 4) {
            if (s.isEmpty()) res.add(out);
            return;
        }
        for (int k = 1; k < 4; ++k) {
            if (s.length() < k) break;
            int val = Integer.parseInt(s.substring(0, k));
            if (val > 255 || k != String.valueOf(val).length()) continue;
            helper(s.substring(k), n + 1, out + s.substring(0, k) + (n == 3 ? "" : "."), res);
        }
    }
}
```



由于每段数字最多只能有三位，而且只能分为四段，所以情况并不是很多，我们可以使用暴力搜索的方法，每一段都循环1到3，然后当4段位数之和等于原字符串长度时，我们进一步判断每段数字是否不大于255，然后滤去不合要求的数字，加入结果中即可，参见代码如下；



C++ 解法三：

```
class Solution {
public:
    vector<string> restoreIpAddresses(string s) {
        vector<string> res;
        for (int a = 1; a < 4; ++a) 
        for (int b = 1; b < 4; ++b) 
        for (int c = 1; c < 4; ++c) 
        for (int d = 1; d < 4; ++d) 
            if (a + b + c + d == s.size()) {
                int A = stoi(s.substr(0, a));
                int B = stoi(s.substr(a, b));
                int C = stoi(s.substr(a + b, c));
                int D = stoi(s.substr(a + b + c, d));
                if (A <= 255 && B <= 255 && C <= 255 && D <= 255) {
                    string t = to_string(A) + "." + to_string(B) + "." + to_string(C) + "." + to_string(D);
                    if (t.size() == s.size() + 3) res.push_back(t);
                }
            }
        return res;
    }
};
```



Java 解法三：

```
public class Solution {
    public List<String> restoreIpAddresses(String s) {
        List<String> res = new ArrayList<String>();
        for (int a = 1; a < 4; ++a) 
        for (int b = 1; b < 4; ++b) 
        for (int c = 1; c < 4; ++c)
        for (int d = 1; d < 4; ++d) 
            if (a + b + c + d == s.length()) {
                int A = Integer.parseInt(s.substring(0, a));
                int B = Integer.parseInt(s.substring(a, a + b));
                int C = Integer.parseInt(s.substring(a + b, a + b + c));
                int D = Integer.parseInt(s.substring(a + b + c));
                if (A <= 255 && B <= 255 && C <= 255 && D <= 255) {
                    String t = String.valueOf(A) + "." + String.valueOf(B) + "." + String.valueOf(C) + "." + String.valueOf(D);
                    if (t.length() == s.length() + 3) res.add(t);
                }
            }
        return res;
    }
}
```



类似题目：

[IP to CIDR](http://www.cnblogs.com/grandyang/p/8440087.html)



参考资料：

[https://leetcode.com/problems/restore-ip-addresses/](https://leetcode.com/problems/restore-ip-addresses/)

[https://leetcode.com/problems/restore-ip-addresses/discuss/30972/who-can-beat-this-code](https://leetcode.com/problems/restore-ip-addresses/discuss/30972/who-can-beat-this-code)

[https://leetcode.com/problems/restore-ip-addresses/discuss/31098/easy-java-code-of-backtracking-within-16-lines](https://leetcode.com/problems/restore-ip-addresses/discuss/31098/easy-java-code-of-backtracking-within-16-lines)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












