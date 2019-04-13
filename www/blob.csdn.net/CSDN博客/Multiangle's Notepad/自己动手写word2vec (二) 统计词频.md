
# 自己动手写word2vec (二):统计词频 - Multiangle's Notepad - CSDN博客


2016年07月14日 10:55:24[multiangle](https://me.csdn.net/u014595019)阅读数：15260标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[word2vec																](https://so.csdn.net/so/search/s.do?q=word2vec&t=blog)[nlp																](https://so.csdn.net/so/search/s.do?q=nlp&t=blog)[自然语言处理																](https://so.csdn.net/so/search/s.do?q=自然语言处理&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=nlp&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=word2vec&t=blog)个人分类：[自然语言处理																](https://blog.csdn.net/u014595019/article/category/6183383)[机器学习&深度学习																](https://blog.csdn.net/u014595019/article/category/3051069)[
							](https://blog.csdn.net/u014595019/article/category/6183383)
所属专栏：[自然语言处理](https://blog.csdn.net/column/details/13461.html)
[
																	](https://so.csdn.net/so/search/s.do?q=word2vec&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=python&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=python&t=blog)


---
**系列所有帖子**
[自己动手写word2vec (一):主要概念和流程](http://blog.csdn.net/u014595019/article/details/51884529)
[自己动手写word2vec (二):统计词频](http://blog.csdn.net/u014595019/article/details/51907294)
[自己动手写word2vec (三):构建Huffman树 ](http://blog.csdn.net/u014595019/article/details/51925495)
[自己动手写word2vec (四):CBOW和skip-gram模型 ](http://blog.csdn.net/u014595019/article/details/51943428)
---
在我之前写的[word2vec的大概流程](http://blog.csdn.net/u014595019/article/details/51884529)中，第一步的分词使用jieba来实现，感觉效果还不错。
## 第二步. 统计词频
统计词频，相对来讲比较简单一些，主要在Python自带的Counter类基础上稍作改进。值得注意的是需要去掉停用词。所谓停用词，就是出现频率太高的词，如逗号，句号等等，以至于没有区分度。停用词可以在网上很轻易找到，我事先已经转化成二进制的格式存储下来了。这一部分的代码放在[WordCount.py](https://github.com/multiangle/pyword2vec/blob/master/WordCount.py)文件中
### 2.1 MulCounter
MulCounter完成的是根据单词数组来完成统计词频的工作。
这是一个继承自Counter的类。之所以不直接用Counter是因为它虽然能够统计词频，但是无法完成过滤功能。而MulCounter可以通过larger_than和less_than这两个方法过滤掉出现频率过少和过多的词。
```python
class
```
```python
MulCounter
```
```python
(Counter)
```
```python
:
```
```python
# a class extends from collections.Counter
```
```python
# add some methods, larger_than and less_than
```
```python
def
```
```python
__init__
```
```python
(self,element_list)
```
```python
:
```
```python
super().__init__(element_list)
```
```python
def
```
```python
larger_than
```
```python
(self,minvalue,ret=
```
```python
'list'
```
```python
)
```
```python
:
```
```python
temp = sorted(self.items(),key=_itemgetter(
```
```python
1
```
```python
),reverse=
```
```python
True
```
```python
)
        low =
```
```python
0
```
```python
high = temp.__len__()
```
```python
while
```
```python
(high - low >
```
```python
1
```
```python
):
            mid = (low+high) >>
```
```python
1
```
```python
if
```
```python
temp[mid][
```
```python
1
```
```python
] >= minvalue:
                low = mid
```
```python
else
```
```python
:
                high = mid
```
```python
if
```
```python
temp[low][
```
```python
1
```
```python
]<minvalue:
```
```python
if
```
```python
ret==
```
```python
'dict'
```
```python
:
```
```python
return
```
```python
{}
```
```python
else
```
```python
:
```
```python
return
```
```python
[]
```
```python
if
```
```python
ret==
```
```python
'dict'
```
```python
:
            ret_data = {}
```
```python
for
```
```python
ele,count
```
```python
in
```
```python
temp[:high]:
                ret_data[ele]=count
```
```python
return
```
```python
ret_data
```
```python
else
```
```python
:
```
```python
return
```
```python
temp[:high]
```
```python
def
```
```python
less_than
```
```python
(self,maxvalue,ret=
```
```python
'list'
```
```python
)
```
```python
:
```
```python
temp = sorted(self.items(),key=_itemgetter(
```
```python
1
```
```python
))
        low =
```
```python
0
```
```python
high = temp.__len__()
```
```python
while
```
```python
((high-low) >
```
```python
1
```
```python
):
            mid = (low+high) >>
```
```python
1
```
```python
if
```
```python
temp[mid][
```
```python
1
```
```python
] <= maxvalue:
                low = mid
```
```python
else
```
```python
:
                high = mid
```
```python
if
```
```python
temp[low][
```
```python
1
```
```python
]>maxvalue:
```
```python
if
```
```python
ret==
```
```python
'dict'
```
```python
:
```
```python
return
```
```python
{}
```
```python
else
```
```python
:
```
```python
return
```
```python
[]
```
```python
if
```
```python
ret==
```
```python
'dict'
```
```python
:
            ret_data = {}
```
```python
for
```
```python
ele,count
```
```python
in
```
```python
temp[:high]:
                ret_data[ele]=count
```
```python
return
```
```python
ret_data
```
```python
else
```
```python
:
```
```python
return
```
```python
temp[:high]
```
### 2.2 WordCounter
WordCounter完成的是根据文本来统计词频的工作。确切的来说，对完整的文本进行分词，过滤掉停用词，然后将预处理好的单词数组交给MulCounter去统计
```python
class
```
```python
WordCounter
```
```python
()
```
```python
:
```
```python
# can calculate the freq of words in a text list
```
```python
# for example
```
```python
# >>> data = ['Merge multiple sorted inputs into a single sorted output',
```
```python
#           'The API below differs from textbook heap algorithms in two aspects']
```
```python
# >>> wc = WordCounter(data)
```
```python
# >>> print(wc.count_res)
```
```python
# >>> MulCounter({' ': 18, 'sorted': 2, 'single': 1, 'below': 1, 'inputs': 1, 'The': 1, 'into': 1, 'textbook': 1,
```
```python
#                'API': 1, 'algorithms': 1, 'in': 1, 'output': 1, 'heap': 1, 'differs': 1, 'two': 1, 'from': 1,
```
```python
#                'aspects': 1, 'multiple': 1, 'a': 1, 'Merge': 1})
```
```python
def
```
```python
__init__
```
```python
(self, text_list)
```
```python
:
```
```python
self.text_list = text_list
        self.stop_word = self.Get_Stop_Words()
        self.count_res =
```
```python
None
```
```python
self.Word_Count(self.text_list)
```
```python
def
```
```python
Get_Stop_Words
```
```python
(self)
```
```python
:
```
```python
ret = []
        ret = FI.load_pickle(
```
```python
'./static/stop_words.pkl'
```
```python
)
```
```python
return
```
```python
ret
```
```python
def
```
```python
Word_Count
```
```python
(self,text_list,cut_all=False)
```
```python
:
```
```python
filtered_word_list = []
        count =
```
```python
0
```
```python
for
```
```python
line
```
```python
in
```
```python
text_list:
            res = jieba.cut(line,cut_all=cut_all)
            res = list(res)
            text_list[count] = res
            count +=
```
```python
1
```
```python
filtered_word_list += res
        self.count_res = MulCounter(filtered_word_list)
```
```python
for
```
```python
word
```
```python
in
```
```python
self.stop_word:
```
```python
try
```
```python
:
                self.count_res.pop(word)
```
```python
except
```
```python
:
```
```python
pass
```

