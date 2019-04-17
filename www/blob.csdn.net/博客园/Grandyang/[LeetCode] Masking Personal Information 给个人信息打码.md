# [LeetCode] Masking Personal Information 给个人信息打码 - Grandyang - 博客园







# [[LeetCode] Masking Personal Information 给个人信息打码](https://www.cnblogs.com/grandyang/p/10340260.html)









We are given a personal information string `S`, which may represent either an email address or a phone number.

We would like to mask this personal information according to the following rules:


1. Email address:

We define a name to be a string of `length ≥ 2`consisting of only lowercase letters `a-z` or uppercase letters `A-Z`.

An email address starts with a name, followed by the symbol `'@'`, followed by a name, followed by the dot `'.'` and followed by a name. 

All email addresses are guaranteed to be valid and in the format of `"name1@name2.name3".`

To mask an email, all names must be converted to lowercase and all letters between the first and last letter of the first name must be replaced by 5 asterisks `'*'`.


2. Phone number:

A phone number is a string consisting of only the digits `0-9`or the characters from the set `{'+', '-', '(', ')', ' '}.` You may assume a phone number contains 10 to 13 digits.

The last 10 digits make up the local number, while the digits before those make up the country code. Note that the country code is optional. We want to expose only the last 4 digits and mask all other digits.

The local number should be formatted and masked as `"***-***-1111", `where `1` represents the exposed digits.

To mask a phone number with country code like `"+111 111 111 1111"`, we write it in the form `"+***-***-***-1111".`  The `'+'` sign and the first `'-'` sign before the local number should only exist if there is a country code.  For example, a 12 digit phone number mask should start with `"+**-"`.

Note that extraneous characters like `"(", ")", " "`, as well as extra dashes or plus signs not part of the above formatting scheme should be removed.



Return the correct "mask" of the information provided.



Example 1:
Input: "LeetCode@LeetCode.com"
Output: "l*****e@leetcode.com"
Explanation: All names are converted to lowercase, and the letters between the
             first and last letter of the first name is replaced by 5 asterisks.
             Therefore, "leetcode" -> "l*****e".

Example 2:
Input: "AB@qq.com"
Output: "a*****b@qq.com"
Explanation: There must be 5 asterisks between the first and last letter 
             of the first name "ab". Therefore, "ab" -> "a*****b".

Example 3:
Input: "1(234)567-890"
Output: "***-***-7890"
Explanation: 10 digits in the phone number, which means all digits make up the local number.

Example 4:
Input: "86-(10)12345678"
Output: "+**-***-***-5678"
Explanation: 12 digits, 2 digits for country code and 10 digits for local number. 

Notes:
- `S.length <= 40`.
- Emails have length at least 8.
- Phone numbers have length at least 10.





这道题让我们给个人信息打码，在这个注重保护隐私的时代，打码什么的都是家常便饭了，想看高清无码的，那就得掏钱。这里对邮箱和电话分别进行了不同的打码方式，对于邮箱来说，只保留用户名的首尾两个字符，然后中间固定加上五个星号，还有就是所有的字母转小写。对于电话来说，有两种方式，有和没有国家代号，有的话其前面必须有加号，跟后面的区域号用短杠隔开，后面的10个电话号分为三组，个数分别为3，3，4。每组之间还是用短杠隔开，除了最后一组的数字保留之外，其他的都用星号代替。弄清楚了题意，就开始解题吧。既然是字符串的题目，那么肯定要遍历这个字符串了，我们关心的主要是数字和字母，所以要用个变量str来保存遍历到的数字和字母，所以判断，如果是数字或者小写字母的话，直接加入str中，若是大写字母的话，转成小写字母再加入str，如果遇到了 ‘@’ 号，那么表示当前处理的是邮箱，而此时的用户已经全部读入str了，那直接就取出首尾字符，然后中间加五个星号，并再加上 ‘@’ 号存入结果res中，并把str清空。若遇到了点，说明此时是邮箱的后半段，因为题目中限制了用户名中不会有点存在，那么我们将str和点一起加入结果res，并将str清空。当遍历结束时，若此时结果res不为空，说明我们处理的是邮箱，那么返回结果res加上str，因为str中存的是 "com"，还没有来得及加入结果res。若res为空，说明处理的是电话号码，所有的数字都已经加入了str，由于国家代号可能有也可能没有，所以判断一下存入的数字的个数，如果超过10个了，说明有国家代号，那么将超过的部分取出来拼装一下，前面加 ‘+’ 号，后面加短杠。然后再将10位号码的前六位的星号格式加上，并加上最后四个数字即可，参见代码如下：



解法一：

```
class Solution {
public:
    string maskPII(string S) {
        string res = "", str = "";
        for (char c : S) {
            if (c >= 'a' && c <= 'z') str.push_back(c);
            else if (c >= 'A' && c <= 'Z') str.push_back(c + 32);
            else if (c >= '0' && c <= '9') str.push_back(c);
            else if (c == '@') {
                res += string(1, str[0]) + "*****" + string(1, str.back()) + "@";
                str.clear();
            } else if (c == '.') {
                res += str + ".";
                str.clear();
            }
        }
        if (!res.empty()) return res + str;
        int n = str.size();
        if (n > 10) res += "+" + string(n - 10, '*') + "-";
        res += "***-***-" + str.substr(n - 4);
        return res;
    }
};
```



我们还可以换一种写法，首先在字符串S中找 ‘@’ 号，这是区分邮箱地址和电话号码的关键所在。若没找到，则按电话号码的方法处理，跟上面的操作几乎相同。若找到了，则直接取出第一个字母，加五个星号，然后将 ‘@’ 号位置及其后面所有的字符都加入结果res，然后再统一转为小写即可，参见代码如下：



解法二：

```
class Solution {
public:
    string maskPII(string S) {
        string res = "", str = "";
        auto pos = S.find('@');
        if (pos == string::npos) {
            for (char c : S) {
                if (c >= '0' && c <= '9') str.push_back(c);
            }
            int n = str.size();
            if (n > 10) res += "+" + string(n - 10, '*') + "-";
            res += "***-***-" + str.substr(n - 4);
        } else {
            res = S.substr(0, 1) + "*****" + S.substr(pos - 1);
            transform(res.begin(), res.end(), res.begin(), ::tolower);
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/masking-personal-information/](https://leetcode.com/problems/masking-personal-information/)

[https://leetcode.com/problems/masking-personal-information/discuss/129400/Straightforward-C%2B%2B](https://leetcode.com/problems/masking-personal-information/discuss/129400/Straightforward-C%2B%2B)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












