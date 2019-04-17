# Left Pad - guoziqing506的博客 - CSDN博客





2016年04月22日 17:01:06[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：774
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：You know what, left pad is javascript package and referenced by React: 

Github link. One day his author unpublished it, then a lot of javascript projects in the world broken. You can see from github it's only 11 lines. You job is to implement the left pad function. If you do not know what left pad does, see examples below and guess.

样例：

leftpad("foo", 5)

>> "  foo"

leftpad("foobar", 6)

>> "foobar"

leftpad("1", 2, "0")

>> "01"

这道题我主要把他的意思说清楚一点：就是说一个字符串，我现在给定一个长度（默认这个长度一定是大于或等于字符串长度的），如果字符串长度比这个给定的长度小了，就在字符串前面添加一些符号，使得生成的新字符串长度都相等。

样例：‘abc’，我现在给定长度5，填充的符号为‘ ’（空字符），显然，'abc'的长度比5小，那就在‘abc’前面添加两个空字符，变成‘  abc’就行了。需要注意的是，这里的pad这个词是“填充的”意思，如果你们阅读科研文献的话，会经常遇到这个词，我们搞安全搜索的时候，就经常采取这种办法，令敌手无法区分搜索词串的长度，从而得不到有用信息。这是题外话了，这个题代码我放下面，因为太简单，不做解释了。



```python
class StringUtils:
    # @param {string} originalStr the string we want to append to
    # @param {int} size the target length of the string
    # @param {string} padChar the character to pad to the left side of the string
    # @return {string} a string
    @classmethod
    def leftPad(self, originalStr, size, padChar=' '):
        return padChar * (size - len(originalStr)) + originalStr
        # Write your code here
```










