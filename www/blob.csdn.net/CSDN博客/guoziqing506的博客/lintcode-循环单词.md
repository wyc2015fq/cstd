# lintcode - 循环单词 - guoziqing506的博客 - CSDN博客





2018年03月21日 17:08:05[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：168
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：The words are same rotate words if rotate the word to the right by loop, and get another. Count how many different rotate word sets in dictionary.

E.g. picture and turepic are same rotate words.

样例： 

Given dict = [“picture”, “turepic”, “icturep”, “word”, “ordw”, “lint”] 

return 3.
“picture”, “turepic”, “icturep” are same ratote words. 

“word”, “ordw” are same too. 

“lint” is the third word that different from the previous two words.
解题思路：

说来惭愧，这道题我解了好久。一直在88%出遇到时间瓶颈。我反复优化了代码，也在网上查阅了大家的解法。无奈，本来用python解题的人就少，少数的答案又多少有点问题。所以一直难以解决。不过好在是最终还是AC了。

首先说明一个问题：如何判断两个单词是否一个是另一个旋转得到。基本思路是要让两个单词（记为w1, w2吧）依次满足如下2个条件：
- 长度相等
- w2在单词(w1 + w1)当中。这是一个非常重要的性质，用Python中的`in`操作符可直接完成这种包含关系的判断，从而省去了循环遍历所消耗的时间。举个例子来说，”turepic” + “turepic” = “turepicturepic”，而单词”picture”正好在”turepicturepic”中。

解决了两个单词是否为旋转关系的问题，剩下的就是对单词列表遍历检查了。当然我们需要存储记录已经遍历过的单词信息。这里我用Python中字典的数据结构，key为单词，值为该词的长度。之所以记录长度是为了后面提高运行的效率，避免大量的计算单词长度的运算。给出代码如下：

```python
class Solution:
    """
    @param: words: A list of words
    @return: Return how many different rotate words
    """

    def countRotateWords(self, words):
        if len(words) == 0:
            return 0
        record = {words[0]:len(words[0])}
        for word in words[1:]:
            exist = False
            temp = len(word)
            newWord = word + word
            for key in record:
                if len(key) == temp:
                    if key in newWord:
                        exist = True
                        break
            if not exist:
                record[word] = len(word)
        return len(record)
```

观察到第13，14两行引用变量的目的都是节省运行时间的措施。但是上面的代码虽然运行结果正确，却不能在lintcode上AC!!!在此，我想了很久，我觉得代码效率和算法的时间复杂度确实不能再提高了。后来看到一个博主的文章，其算法和我的一致，代码写的也差不多，但是少了第16行判断长度的语句，导致的结果是运行时间大幅缩减，在我的笔记本上跑的结果大约为加上判断长度时的一半。

当然他的这种算法是错的，但是却启发了我。也就是说主要的耗时在于频繁判断长度字符长度是否相等。在lintcode的测试数据中，我发现我不能AC的测试数据有着长度相等，数量多的特点，因此，我在这里稍作调整：

```
for key in record:
    if key in newWord and len(key) == temp:
        exist = True
        break
```

这样时间上就对了。因此我基本可以判断Python中，if a and b这种语句当是先判断a，倘若a为False，就不再判断b了。最后给出可以AC的代码：

```python
class Solution:
    """
    @param: words: A list of words
    @return: Return how many different rotate words
    """

    def countRotateWords(self, words):
        if len(words) == 0:
            return 0
        record = {words[0]:len(words[0])}
        for word in words[1:]:
            exist = False
            temp = len(word)
            newWord = word + word
            for key in record:
                if key in newWord and len(key) == temp:
                    exist = True
                    break
            if not exist:
                record[word] = len(word)
        return len(record)
```

有关于lintcode题目的答案代码，我打算整理一个完整的版本，并放在我的github上。已经在博客中写过的，我也会在后期慢慢加入。另外，一些我觉得过于简单的题目就不再博客中特别记录了。不过github上会有相关的代码。欢迎大家批评指正。网址：[Lintcode-Answer](https://github.com/guoziqingbupt/Lintcode-Answer)





