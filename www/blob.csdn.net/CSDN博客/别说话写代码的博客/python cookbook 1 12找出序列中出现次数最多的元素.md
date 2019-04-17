# python cookbook 1.12找出序列中出现次数最多的元素 - 别说话写代码的博客 - CSDN博客





2019年01月12日 15:13:39[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：33标签：[python cookbook																[找出序列中出现次数最多的元素](https://so.csdn.net/so/search/s.do?q=找出序列中出现次数最多的元素&t=blog)](https://so.csdn.net/so/search/s.do?q=python cookbook&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)








```python
#1.12找出序列中出现次数最多的元素
#collections模块中的Counter类正式这样的用法，most_common()可以直接统计
words = [
'look', 'into', 'my', 'eyes', 'look', 'into', 'my', 'eyes',
'the', 'eyes', 'the', 'eyes', 'the', 'eyes', 'not', 'around', 'the',
'eyes', "don't", 'look', 'around', 'the', 'eyes', 'look', 'into',
'my', 'eyes', "you're", 'under'
]
from collections import Counter
words_counts = Counter(words)    #统计所有元素以及出现的次数
print(words_counts)
top_tree = words_counts.most_common(3)   #统计出现次数最多的元素
print(top_tree)
print(words_counts['eyes'])   #查看某元素出现的次数
```

![](https://img-blog.csdnimg.cn/20190112151223710.png)

```python
morewords =  ['why','are','you','not','looking','in','my','eyes']   #新的数据
for word in morewords:
    words_counts[word]+=1    #更新新数据
print(words_counts['eyes'])

words_counts.update(morewords)
print(words_counts)   #或者用这种方法，因为上面更新了一次，再更新相当于更新了两次
```

![](https://img-blog.csdnimg.cn/20190112151249331.png)



```python
a = Counter(words)
b = Counter(morewords)
print(a)
print(b)
c=a+b   #将两个合起来
print(c)
d=a-b
print(d)   #从a中去除b
```

![](https://img-blog.csdnimg.cn/20190112151316547.png)




