# [LeetCode] Tag Validator 标签验证器 - Grandyang - 博客园







# [[LeetCode] Tag Validator 标签验证器](https://www.cnblogs.com/grandyang/p/7016476.html)







Given a string representing a code snippet, you need to implement a tag validator to parse the code and return whether it is valid. A code snippet is valid if all the following rules hold:


- The code must be wrapped in a valid closed tag. Otherwise, the code is invalid.
- A closed tag (not necessarily valid) has exactly the following format : `<TAG_NAME>TAG_CONTENT</TAG_NAME>`. Among them, `<TAG_NAME>` is the start tag, and `</TAG_NAME>` is the end tag. The TAG_NAME in start and end tags should be the same. A closed tag is valid if and only if the TAG_NAME and TAG_CONTENT are valid.
- A valid `TAG_NAME` only contain upper-case letters, and has length in range [1,9]. Otherwise, the `TAG_NAME`is invalid.
- A valid `TAG_CONTENT` may contain other valid closed tags, cdata and any characters (see note1) EXCEPTunmatched `<`, unmatched start and end tag, and unmatched or closed tags with invalid TAG_NAME. Otherwise, the `TAG_CONTENT` is invalid.
- A start tag is unmatched if no end tag exists with the same TAG_NAME, and vice versa. However, you also need to consider the issue of unbalanced when tags are nested.
- A `<` is unmatched if you cannot find a subsequent `>`. And when you find a `<` or `</`, all the subsequent characters until the next `>` should be parsed as TAG_NAME (not necessarily valid).
- The cdata has the following format : `<![CDATA[CDATA_CONTENT]]>`. The range of `CDATA_CONTENT` is defined as the characters between `<![CDATA[` and the first subsequent `]]>`.
- `CDATA_CONTENT` may contain any characters. The function of cdata is to forbid the validator to parse `CDATA_CONTENT`, so even it has some characters that can be parsed as tag (no matter valid or invalid), you should treat it as regular characters.

Valid Code Examples:
Input: "<DIV>This is the first line <![CDATA[<div>]]></DIV>"

Output: True

Explanation: 

The code is wrapped in a closed tag : <DIV> and </DIV>. 

The TAG_NAME is valid, the TAG_CONTENT consists of some characters and cdata. 

Although CDATA_CONTENT has unmatched start tag with invalid TAG_NAME, it should be considered as plain text, not parsed as tag.

So TAG_CONTENT is valid, and then the code is valid. Thus return true.


Input: "<DIV>>>  ![cdata[]] <![CDATA[<div>]>]]>]]>>]</DIV>"

Output: True

Explanation:

We first separate the code into : start_tag|tag_content|end_tag.

start_tag -> "<DIV>"

end_tag -> "</DIV>"

tag_content could also be separated into : text1|cdata|text2.

text1 -> ">>  ![cdata[]] "

cdata -> "<![CDATA[<div>]>]]>", where the CDATA_CONTENT is "<div>]>"

text2 -> "]]>>]"


The reason why start_tag is NOT "<DIV>>>" is because of the rule 6.
The reason why cdata is NOT "<![CDATA[<div>]>]]>]]>" is because of the rule 7.



Invalid Code Examples:
Input: "<A>  <B> </A>   </B>"
Output: False
Explanation: Unbalanced. If "<A>" is closed, then "<B>" must be unmatched, and vice versa.

Input: "<DIV>  div tag is not closed  <DIV>"
Output: False

Input: "<DIV>  unmatched <  </DIV>"
Output: False

Input: "<DIV> closed tags with invalid tag name  <b>123</b> </DIV>"
Output: False

Input: "<DIV> unmatched tags with invalid tag name  </1234567890> and <CDATA[[]]>  </DIV>"
Output: False

Input: "<DIV>  unmatched start tag <B>  and unmatched end tag </C>  </DIV>"
Output: False



Note:
- For simplicity, you could assume the input code (including the any characters mentioned above) only contain `letters`, `digits`, `'<'`,`'>'`,`'/'`,`'!'`,`'['`,`']'` and `' '`.



这道题让我们给了我们一个字符串，其实是html的代码，让我们验证其写法是否正确。规定了八条规则，比如说必须是封闭的，标签名必须都是大写，并且不能超过9个字符，还规定了CDATA的一些格式规范，并且给了一些实例，但是说实话，题目中给的这些例子完全不能覆盖OJ中的各种情况，博主这次完全被OJ教育了，每次submit都被OJ打回来，然后分析未通过的test case，修改代码，再提交，再打回，折腾了十几次，终于通过OJ，绿色的**Accepted**出现的那一刹那，无比的快感，这也是博主能坚持到现在的动力之一吧，当然最主要的动力还是大家的支持与鼓励，博主很喜欢跟大家留言互动哈。下面呈上博主fail过的case，并来分析原因：



```
"<![CDATA[wahaha]]]><![CDATA[]> wahaha]]>"  -> False
```

错误原因：没有以Tag开头



```
"<A><A>/A></A></A>"  -> True
```

多余的'>'不会影响



```
"<A>  <B> </A>   </B>"  -> False
```

错误原因：最后一个闭标签只能闭合首标签



```
"<A></A><B></B>"  -> False
```

错误原因：最后一个闭标签只能闭合首标签



```
"<A><B></B></A>"  -> True
```

没有content data也没关系



```
"<A>![CDATA[/A>]]></A>"  -> True
```

CDATA中间的内容可以是任意字符



```
"123456"  -> False
```

错误原因：没有标签存在



```
"<A></A>"  -> True
```

没有content data也没关系



```
"<A></A>>"  -> False
```

错误原因：末尾存在多余的'>'



```
"<DIV>>>>>>>  ![cdata[]] <![CDATA[<div>]>]]>]]>>]</DIV>"  -> True
```

注意content data的干扰字符



```
"<AAAAAAAAAA></AAAAAAAAAA>"  -> False
```

错误原因：标签的字符长度不能超过9个



```
"<a><a></a></a>"  ->  False
```

错误原因：标签字符必须都是大写



```
"<A><!CDATAA[[123]]></A>"  -> False
```

错误原因：没有正确的match上"<![CDATA["，也不能当做标签



```
"!!!<A>123</A>123"  -> False
```

错误原因：不能以非标签开头



```
"<A>123</A>123"  -> False
```

错误原因：不能以非标签结尾



由于博主的修修补补使得博主自己的代码看起来不elegant，所以博主借鉴了论坛上[dengzhizhang](https://discuss.leetcode.com/user/dengzhizhang)大神的解法，果然还是用find函数写起来简洁，而不是用while函数来一个一个的找。下面的代码结构还是比较清晰的，首先对于这种成对匹配的问题肯定是要用栈stack的，就像之前的匹配括号的问题。那么我们来遍历code字符串，首先是判断，如果当前遍历到的字符非首字符，并且栈为空，那么直接返回false。虽然只是短短的一句但其实非常的重要，这句就排除了很多错误情况，比如开头结尾不是标签的情况，以及没有标签的情况，和开头的标签在中间就闭合了情况等等，非常powerful的一句判断。然后我们来处理包含CDATA的情况，当然是要先匹配到"<![CDATA["，然后我们用find来找结束标志"]]>"，如果没找到，直接返回false，找到了点话就继续遍历，顺便把当前遍历的位置移到结束标志符的最后一位上。

如果我们只匹配到了"</"，说明是个结束标签，那么我们用find来找到右尖括号'>'，如果没找到直接返回false，找到了就把tag到内容提出来，然后看此时的stack，如果stack为空，或者栈顶元素不等于tag，直接返回false，否则就将栈顶元素取出。

如果我们只匹配到了"<"，说明是个起始标签，还是要找右尖括号，如果找不到，或者标签的长度为0，或者大于9了，直接返回true。然后遍历标签的每一位，如果不全是大些字母，返回false，否则就把tag压入栈。那么你可能会有疑问，为啥在处理结束标签时，没有这些额外的判断呢，因为结束标签要和栈顶元素比较，栈里的标签肯定都是合法的，所以如果结束标签不合法，那么肯定不相等，也就直接返回false了。最后我们看栈是否为空，如果不为空，说明有未封闭的标签，返回false。参见代码如下：



```
class Solution {
public:
    bool isValid(string code) {
        stack<string> st;
        for (int i = 0; i < code.size(); ++i) {
            if (i > 0 && st.empty()) return false;
            if (code.substr(i, 9) == "<![CDATA[") {
                int j = i + 9;
                i = code.find("]]>", j);
                if (i < 0) return false;
                i += 2;
            } else if (code.substr(i, 2) == "</") {
                int j = i + 2;
                i = code.find(">", j);
                if (i < 0) return false;
                string tag = code.substr(j, i - j);
                if (st.empty() || st.top() != tag) return false;
                st.pop();
            } else if (code.substr(i, 1) == "<") {
                int j = i + 1;
                i = code.find(">", j);
                if (i < 0 || i == j || i - j > 9) return false;
                for (int k = j; k < i; ++k) {
                    if (code[k] < 'A' || code[k] > 'Z') return false;
                }
                string tag = code.substr(j, i - j);
                st.push(tag);
            }
        }
        return st.empty();
    }
};
```



这道题只是判断html里最简单的标签，并没有加上js和css的东西，就已经是个Hard的题目了。论坛看到有大神们用正则匹配来做，代码太简洁了，但是博主看不太懂那个正则表达式，囧～所以没有把那种解法贴上来，大家可以看看[zqfan大神的帖子](https://discuss.leetcode.com/topic/91505/6-lines-c-solution-using-regex)，如果哪位看官大神看懂了，请给博主讲一下，博主可以贴上来供大家参考，多谢～



类似题目：

[Add Bold Tag in String](http://www.cnblogs.com/grandyang/p/7043394.html)



参考资料：

[https://discuss.leetcode.com/topic/91473/clean-c-solution](https://discuss.leetcode.com/topic/91473/clean-c-solution)

[https://discuss.leetcode.com/topic/91446/c-clean-code-recursive-parser](https://discuss.leetcode.com/topic/91446/c-clean-code-recursive-parser)

[https://discuss.leetcode.com/topic/91505/6-lines-c-solution-using-regex](https://discuss.leetcode.com/topic/91505/6-lines-c-solution-using-regex)

[https://discuss.leetcode.com/topic/91300/java-solution-use-startswith-and-indexof](https://discuss.leetcode.com/topic/91300/java-solution-use-startswith-and-indexof)

[https://discuss.leetcode.com/topic/91406/java-solution-7-lines-regular-expression/2](https://discuss.leetcode.com/topic/91406/java-solution-7-lines-regular-expression/2)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












