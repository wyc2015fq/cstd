# [LeetCode] Goat Latin 山羊拉丁文 - Grandyang - 博客园







# [[LeetCode] Goat Latin 山羊拉丁文](https://www.cnblogs.com/grandyang/p/10236802.html)







A sentence `S` is given, composed of words separated by spaces. Each word consists of lowercase and uppercase letters only.

We would like to convert the sentence to "*Goat Latin"* (a made-up language similar to Pig Latin.)

The rules of Goat Latin are as follows:
- If a word begins with a vowel (a, e, i, o, or u), append `"ma"` to the end of the word.
For example, the word 'apple' becomes 'applema'.

- If a word begins with a consonant (i.e. not a vowel), remove the first letter and append it to the end, then add `"ma"`.
For example, the word `"goat"` becomes `"oatgma"`.

- Add one letter `'a'` to the end of each word per its word index in the sentence, starting with 1.
For example, the first word gets `"a"` added to the end, the second word gets `"aa"` added to the end and so on.

Return the final sentence representing the conversion from `S` to Goat Latin. 



Example 1:
Input: "I speak Goat Latin"
Output: "Imaa peaksmaaa oatGmaaaa atinLmaaaaa"

Example 2:
Input: "The quick brown fox jumped over the lazy dog"
Output: "heTmaa uickqmaaa rownbmaaaa oxfmaaaaa umpedjmaaaaaa overmaaaaaaa hetmaaaaaaaa azylmaaaaaaaaa ogdmaaaaaaaaaa"



Notes:
- `S` contains only uppercase, lowercase and spaces. Exactly one space between each word.
- `1 <= S.length <= 150`.



这道题让我们将一句话转为山羊拉丁文，有几个规则，如果单词是元音开头的，那么直接在但此后加ma，（为啥不是咩mie，西方的羊就是叫ma么，不知为何想起了老友记中Janice的笑声-.-|||，但其实Janice在剧中的声音是有意为之的，看过演员本人的访谈节目，相当正常，不得不说演员啊，声优啊，都是怪物，扯远了～），如果是非元音开头的单词，那么把首字母移到末尾，并且加ma。还有就是根据当前是第几个单词，后面加几个a，估计是为了模仿羊叫声，拉长音，感觉更像绵羊音一样。此题难度不是很大，就照题目要求来做，不需要啥特别的技巧。首先为了快速检测开头字母是否为元音，我们将所有元音加入一个HashSet，注意大小写的元音都要加进去。然后要一个单词一个单词的处理，这里我们使用C++的字符串流类来快速的提取单词，对于每个提取出的单词，我们先加上一个空格，然后判断开头字母是否为元音，是的话直接加上，不然就去子串去掉首字母，然后将首字母加到末尾。后面再加上ma，还有相对应数目个a。这里我们定义一个变量cnt，初始化为1，每处理一个单词，cnt自增1，这样我们就知道需要加的a的个数了，最后别忘了把结果res的首空格去掉，参见代码如下：



```
class Solution {
public:
    string toGoatLatin(string S) {
        unordered_set<char> vowel{'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};
        istringstream ss(S);
        string res, t;
        int cnt = 1;
        while (ss >> t) {
            res += ' ' + (vowel.count(t[0]) ? t : t.substr(1) + t[0]) + "ma" + string(cnt, 'a');
            ++cnt;
        }
        return res.substr(1);
    }
};
```



参考资料：

[https://leetcode.com/problems/goat-latin/](https://leetcode.com/problems/goat-latin/)

[https://leetcode.com/problems/goat-latin/discuss/126973/Short-C%2B%2B-solution-using-io-stringstream](https://leetcode.com/problems/goat-latin/discuss/126973/Short-C%2B%2B-solution-using-io-stringstream)

[https://leetcode.com/problems/goat-latin/discuss/127024/C%2B%2BJavaPython-Straight-Forward-Solution](https://leetcode.com/problems/goat-latin/discuss/127024/C%2B%2BJavaPython-Straight-Forward-Solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












