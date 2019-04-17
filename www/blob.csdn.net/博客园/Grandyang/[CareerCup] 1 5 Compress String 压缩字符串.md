# [CareerCup] 1.5 Compress String 压缩字符串 - Grandyang - 博客园







# [[CareerCup] 1.5 Compress String 压缩字符串](https://www.cnblogs.com/grandyang/p/4653200.html)







1.5 Implement a method to perform basic string compression using the counts of repeated characters. For example, the string aabcccccaaa would become a2blc5a3. If the "compressed" string would not become smaller than the original string, your method should return the original string.



这道题让我们压缩给定的字符串，压缩方法是对于重复的字符，用数字来表示重复的个数，这种压缩方法对于有很多重复字符具有很高的压缩效率，但是对于不重复的字符串，压缩后的表示方法反而比不压缩占空间大。所以我们首先要先来计算下压缩后的字符串的长度，和原字符串长度比较，如果大的话，则直接返回原字符串，如果小的话，则我们就开始压缩。那么我们需要建立一个新字符串来保存压缩后的字符串，这里书中特别提到了用字符串的相加的方法是很没有效率的，下面英文部分摘自Cracking the coding interview 5th edition的第72页：



Imagine you were concatenating a list of strings, as shown below. What would the running time of this code be? For simplicity, assume that the strings are all the same length (call this x) and that there are n strings.

```
public String joinWords(String[] words) {
    String sentence = "";
    for (String w : words) {
        sentence = sentence + w;
    }
    return sentence;
}
```

On each concatenation, a new copy of the string is created, and the two strings are copied over, character by character. The first iteration requires us to copy x characters. The second iteration requires copying 2x characters.The third iteration requires 3x, and
so on.The total time therefore is 0(x + 2x + ... + nx). This reduces to 0(xn2). (Why isn't it 0(xnn)? Because 1 + 2 + ... + nequals n(n+l)/2,orO(n2).)



根据上面所说，字符串的拼接余姚拷贝拼接的两个字符串，当字符串长度很长的时候，这种方法非常没有效率，所以我们要避免使用拼接的方法。那么我们的替代方法是先声明好一个定长的字符串，然后给每个位置赋值。压缩后的字符串长度我们开始计算过了，所以只需要给每个位置赋值即可，跟之前那道[1.4 Replace Spaces 替换空格](http://www.cnblogs.com/grandyang/p/4650780.html)有些相似，参见代码如下：



```
class Solution {
public:
    string compress(string s) {
        int newLen = countCompression(s);
        if (newLen >= s.size()) return s;string res(newLen, ' ');
        char c = s[0];
        int cnt = 1, idx = 0;
        for (int i = 1; i < s.size(); ++i) {
            if (s[i] == c) ++cnt;
            else {
                res[idx++] = c;
                for (auto a : to_string(cnt)) res[idx++] = a;
                c = s[i];
                cnt = 1;
            }
        }
        res[idx++] = c;
        for (auto a : to_string(cnt)) res[idx++] = a;
        return res;
    }
    int countCompression(string s) {
        if (s.empty()) return 0;
        int res = 0, cnt = 1;
        char c = s[0];
        for (int i = 1; i < s.size(); ++i) {
            if (s[i] == c) ++cnt;
            else {
                c = s[i];
                res += 1 + to_string(cnt).size();
                cnt = 1;
            }
        }
        res += 1 + to_string(cnt).size();
        return res;
    }
};
```














