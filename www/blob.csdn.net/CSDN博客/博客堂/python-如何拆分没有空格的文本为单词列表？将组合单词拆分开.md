# python - 如何拆分没有空格的文本为单词列表？将组合单词拆分开 - 博客堂 - CSDN博客





2018年02月28日 16:35:17[最小森林](https://me.csdn.net/u012052268)阅读数：2339








# python - 如何拆分没有空格的文本为单词列表？将组合单词拆分开

很多时候，我们需要把一长串单词字符拆分开来，比如：
- 输入：“tableapplechairtablecupboard …”很多字。我们希望有一个有效的算法来分割这样的文本到单词列表，并得到：
- 输出：[“table”，“apple”，“chair”，“table”，“cup”，“board”]

或者将一个很长的函数名拆分成小单词： WbxNewBrowserInstance 变为 [ wbx new browser instance ]

## 1算法的思想

算法的思想是：通过所有可能的单词(从第一个字母开始)，并找到最长的单词可能 ，插入空格。

所以我们需要一个 “常用单词列表”。

后面的方法是对输出的分布进行建模。良好的第一近似是假设所有单词是独立分布的。然后你只需要知道所有单词的相对频率。可以合理地假定它们遵循Zipf定律，即在单词列表中具有秩n的单词具有大约1 /(n log N)的概率，其中N是字典中的单词数。

一旦你修正了模型，你可以使用动态规划来推断空格的位置。最可能的句子是最大化每个单词的概率的乘积，并且很容易用动态规划来计算它。不是直接使用概率，而是使用定义为概率的倒数的对数的成本来避免溢出。

## 2代码

```python
def cutLongNameFun(self,s):
        '''
        longWords变为 long word：log里面有很多长函数名，比如WbxMeeting_VerifyMeetingIsExist。
        将其拆成小单词wbx meeting verify meeting is exist，更有意义。若有大写则分割。
        '''
        # Build a cost dictionary, assuming Zipf's law and cost = -math.log(probability).
        # 建立一个成本字典，假设Zipf定律和成本= -math.log（概率）。
        words = open("words-by-frequency.txt").read().split() # 有特殊字符的话直接在其中添加
        wordcost = dict((k, log((i+1)*log(len(words)))) for i,k in enumerate(words))
        maxword = max(len(x) for x in words)

        def infer_spaces(s):
            '''Uses dynamic programming to infer the location of spaces in a string without spaces.
            .使用动态编程来推断不带空格的字符串中空格的位置。'''
            # Find the best match for the i first characters, assuming cost has
            # been built for the i-1 first characters.
            # Returns a pair (match_cost, match_length).
            def best_match(i):
                candidates = enumerate(reversed(cost[max(0, i-maxword):i]))
                return min((c + wordcost.get(s[i-k-1:i], 9e999), k+1) for k,c in candidates)

            # Build the cost array.
            cost = [0]
            for i in range(1,len(s)+1):
                c,k = best_match(i)
                cost.append(c)

            # Backtrack to recover the minimal-cost string.
            out = []
            i = len(s)
            while i>0:
                c,k = best_match(i)
                assert c == cost[i]
                out.append(s[i-k:i])
                i -= k

            return " ".join(reversed(out))
```

使用实例：

```php
s = 'thumbgreenappleactiveassignmentweeklymetaphor'
print(infer_spaces(s))

# 结果为 thumb green apple active assignment weekly metaphor.
```

```
Before: thereismassesoftextinformationofpeoplescommentswhichisparsedfromhtmlbuttherearen
odelimitedcharactersinthemforexamplethumbgreenappleactiveassignmentweeklymetapho
rapparentlytherearethumbgreenappleetcinthestringialsohavealargedictionarytoquery
whetherthewordisreasonablesowhatsthefastestwayofextractionthxalot.

After: there is masses of text information of peoples comments which is parsed from html but there are no delimited characters in them for example thumb green apple active assignment weekly metaphor apparently there are thumb green apple etc in the string i also have a large dictionary to query whether the word is reasonable so what s the fastest way of extraction thx a lot.
```

正如你可以看到，这个算法基本上是完美的。最**重要的部分**是确保你的单词列表训练到一个类似于你实际遇到的语料库，否则结果将是非常糟糕。所以你应该手工完善自己的单词库。

## 3下载

words-by-frequency.txt 这个文件下载： 
[http://download.csdn.net/download/u012052268/10263440](http://download.csdn.net/download/u012052268/10263440)



