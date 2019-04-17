# Leetcode  1003. 检查替换后的词是否有效 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






### 1003. 检查替换后的词是否有效

 显示英文描述
[我的提交](https://leetcode-cn.com/contest/weekly-contest-126/problems/check-if-word-is-valid-after-substitutions/submissions/)[返回竞赛](https://leetcode-cn.com/contest/weekly-contest-126/)







- 用户通过次数245
- 用户尝试次数273
- 通过次数249
- 提交次数500
- 题目难度Medium



给定有效字符串 `"abc"`。

对于任何有效的字符串 `V`，我们可以将 `V` 分成两个部分 `X` 和 `Y`，使得 `X + Y`（`X` 与 `Y` 连接）等于 `V`。（`X` 或 `Y` 可以为空。）那么，`X + "abc" + Y` 也同样是有效的。

例如，如果 `S = "abc"`，则有效字符串的示例是：`"abc"`，`"aabcbc"`，`"abcabc"`，`"abcabcababcc"`。无效字符串的示例是：`"abccba"`，`"ab"`，`"cababc"`，`"bac"`。

如果给定字符串 `S` 有效，则返回 `true`；否则，返回 `false`。



示例 1：
输入："aabcbc"
输出：true
解释：
从有效字符串 "abc" 开始。
然后我们可以在 "a" 和 "bc" 之间插入另一个 "abc"，产生 "a" + "abc" + "bc"，即 "aabcbc"。

示例 2：
输入："abcabcababcc"
输出：true
解释：
"abcabcabc" 是有效的，它可以视作在原串后连续插入 "abc"。
然后我们可以在最后一个字母之前插入 "abc"，产生 "abcabcab" + "abc" + "c"，即 "abcabcababcc"。

示例 3：
输入："abccba"
输出：false

示例 4：
输入："cababc"
输出：false


提示：
- `1 <= S.length <= 20000`
- `S[i]` 为 `'a'`、`'b'`、或 `'c'`

```
class Solution {
public:
    bool isValid(string S) {
        while(S.size()){
            int pos = S.find("abc");
            if(pos == -1) break;
            string s1 = S.substr(0,pos);
            string s2 = S.substr(pos+3,S.size()-pos-3);
            S = s1+s2;
            // cout << S << endl;
        }
        return S.size()==0;
    }
};
```

_更简单的

```
class Solution {
public:
    bool isValid(string S) {
        int a=0, b=0, c=0;
        for(int i=0; i<S.length(); i++) {
            if (S[i]=='a') a++;
            if (S[i]=='b') b++;
            if (S[i]=='c') c++;
            if (a<b||b<c) return false;
        }
        return a==b&&b==c;
    }
};
```
















