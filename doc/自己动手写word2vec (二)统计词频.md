# 自己动手写word2vec (二):统计词频

2016年07月14日 10:55:24

 

multiangle

 

阅读数：15208

 

标签： [python](https://so.csdn.net/so/search/s.do?q=python&t=blog)[word2vec](https://so.csdn.net/so/search/s.do?q=word2vec&t=blog)[nlp](https://so.csdn.net/so/search/s.do?q=nlp&t=blog)[自然语言处理](https://so.csdn.net/so/search/s.do?q=%E8%87%AA%E7%84%B6%E8%AF%AD%E8%A8%80%E5%A4%84%E7%90%86&t=blog)更多

个人分类： [自然语言处理](https://blog.csdn.net/u014595019/article/category/6183383)[机器学习&深度学习](https://blog.csdn.net/u014595019/article/category/3051069)

所属专栏： [自然语言处理](https://blog.csdn.net/column/details/13461.html)



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/u014595019/article/details/51907294

------

**系列所有帖子** 
[自己动手写word2vec (一):主要概念和流程](http://blog.csdn.net/u014595019/article/details/51884529) 
[自己动手写word2vec (二):统计词频](http://blog.csdn.net/u014595019/article/details/51907294) 
[自己动手写word2vec (三):构建Huffman树 ](http://blog.csdn.net/u014595019/article/details/51925495)
[自己动手写word2vec (四):CBOW和skip-gram模型](http://blog.csdn.net/u014595019/article/details/51943428)

------

在我之前写的[word2vec的大概流程](http://blog.csdn.net/u014595019/article/details/51884529)中，第一步的分词使用jieba来实现，感觉效果还不错。

## 第二步. 统计词频

统计词频，相对来讲比较简单一些，主要在Python自带的Counter类基础上稍作改进。值得注意的是需要去掉停用词。所谓停用词，就是出现频率太高的词，如逗号，句号等等，以至于没有区分度。停用词可以在网上很轻易找到，我事先已经转化成二进制的格式存储下来了。这一部分的代码放在[WordCount.py](https://github.com/multiangle/pyword2vec/blob/master/WordCount.py)文件中

### 2.1 MulCounter

MulCounter完成的是根据单词数组来完成统计词频的工作。 
这是一个继承自Counter的类。之所以不直接用Counter是因为它虽然能够统计词频，但是无法完成过滤功能。而MulCounter可以通过larger_than和less_than这两个方法过滤掉出现频率过少和过多的词。

```
class MulCounter(Counter):
    # a class extends from collections.Counter
    # add some methods, larger_than and less_than
    def __init__(self,element_list):
        super().__init__(element_list)

    def larger_than(self,minvalue,ret='list'):
        temp = sorted(self.items(),key=_itemgetter(1),reverse=True)
        low = 0
        high = temp.__len__()
        while(high - low > 1):
            mid = (low+high) >> 1
            if temp[mid][1] >= minvalue:
                low = mid
            else:
                high = mid
        if temp[low][1]<minvalue:
            if ret=='dict':
                return {}
            else:
                return []
        if ret=='dict':
            ret_data = {}
            for ele,count in temp[:high]:
                ret_data[ele]=count
            return ret_data
        else:
            return temp[:high]

    def less_than(self,maxvalue,ret='list'):
        temp = sorted(self.items(),key=_itemgetter(1))
        low = 0
        high = temp.__len__()
        while ((high-low) > 1):
            mid = (low+high) >> 1
            if temp[mid][1] <= maxvalue:
                low = mid
            else:
                high = mid
        if temp[low][1]>maxvalue:
            if ret=='dict':
                return {}
            else:
                return []
        if ret=='dict':
            ret_data = {}
            for ele,count in temp[:high]:
                ret_data[ele]=count
            return ret_data
        else:
            return temp[:high]123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051
```

### 2.2 WordCounter

WordCounter完成的是根据文本来统计词频的工作。确切的来说，对完整的文本进行分词，过滤掉停用词，然后将预处理好的单词数组交给MulCounter去统计

```python
class WordCounter():
    # can calculate the freq of words in a text list

    # for example
    # >>> data = ['Merge multiple sorted inputs into a single sorted output',
    #           'The API below differs from textbook heap algorithms in two aspects']
    # >>> wc = WordCounter(data)
    # >>> print(wc.count_res)

    # >>> MulCounter({' ': 18, 'sorted': 2, 'single': 1, 'below': 1, 'inputs': 1, 'The': 1, 'into': 1, 'textbook': 1,
    #                'API': 1, 'algorithms': 1, 'in': 1, 'output': 1, 'heap': 1, 'differs': 1, 'two': 1, 'from': 1,
    #                'aspects': 1, 'multiple': 1, 'a': 1, 'Merge': 1})

    def __init__(self, text_list):
        self.text_list = text_list
        self.stop_word = self.Get_Stop_Words()
        self.count_res = None

        self.Word_Count(self.text_list)

    def Get_Stop_Words(self):
        ret = []
        ret = FI.load_pickle('./static/stop_words.pkl')
        return ret

    def Word_Count(self,text_list,cut_all=False):

        filtered_word_list = []
        count = 0
        for line in text_list:
            res = jieba.cut(line,cut_all=cut_all)
            res = list(res)
            text_list[count] = res
            count += 1
            filtered_word_list += res

        self.count_res = MulCounter(filtered_word_list)
        for word in self.stop_word:
            try:
                self.count_res.pop(word)
            except:
                pass
```