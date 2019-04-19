# beautiful python - Arthur的随笔 - CSDN博客
2012年04月18日 23:44:38[largetalk](https://me.csdn.net/largetalk)阅读数：1269标签：[features																[python																[lambda																[import																[file																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=lambda&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)](https://so.csdn.net/so/search/s.do?q=features&t=blog)
个人分类：[NLP																[python](https://blog.csdn.net/largetalk/article/category/715661)](https://blog.csdn.net/largetalk/article/category/1127651)
使用短短20几行代码实现一个单词拼写纠正程序，python确实太漂亮了
```python
import re, collections
def words(text): return re.findall('[a-z]+', text.lower()) 
def train(features):
    model = collections.defaultdict(lambda: 1)
    for f in features:
        model[f] += 1
    return model
NWORDS = train(words(file('big.txt').read()))
alphabet = 'abcdefghijklmnopqrstuvwxyz'
def edits1(word):
   splits     = [(word[:i], word[i:]) for i in range(len(word) + 1)]
   deletes    = [a + b[1:] for a, b in splits if b]
   transposes = [a + b[1] + b[0] + b[2:] for a, b in splits if len(b)>1]
   replaces   = [a + c + b[1:] for a, b in splits for c in alphabet if b]
   inserts    = [a + c + b     for a, b in splits for c in alphabet]
   return set(deletes + transposes + replaces + inserts)
def known_edits2(word):
    return set(e2 for e1 in edits1(word) for e2 in edits1(e1) if e2 in NWORDS)
def known(words): return set(w for w in words if w in NWORDS)
def correct(word):
    candidates = known([word]) or known(edits1(word)) or known_edits2(word) or [word]
    return max(candidates, key=NWORDS.get)
```
原文地址：[http://norvig.com/spell-correct.html](http://norvig.com/spell-correct.html)
