# python cookbook 2.1 使用多个字符分割字符串 - 别说话写代码的博客 - CSDN博客





2019年03月29日 21:17:45[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：4标签：[使用多个字符分割字符串																[python分割多个字符																[python分割字符串](https://so.csdn.net/so/search/s.do?q=python分割字符串&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)](https://so.csdn.net/so/search/s.do?q=python分割多个字符&t=blog)




python的string中split函数只能按照一个字符分割字符串，不允许多个分隔符或者分隔符周围有不确定的空格。re.split()就可以解决这个问题。

```python
#2.1针对任意多的分隔符拆分字符串，split可以处理简单情况，且不支持多个分隔符，当需要更加灵活功能时，用re.split()

line = 'asdf fjdk; afed, fjek,asdf, foo'
import re
print(re.split(r'[;,\s]\s*',line))   #\s是空格   r是防止转义
#例子中分隔符可以是逗号，分号或者空格，用多个字符分割写在[]中，并且后面紧跟着任意个空格
fields = re.split(r'(;|,|\s)\s*', line)
#用捕获分组()，可以获取分割字符。可以保留分割字符串，用在后面重新构造一个新的输出字符串
print(fields)
```

![](https://img-blog.csdnimg.cn/20190329211626548.png)

```python
values = fields[::2]
delimiters = fields[1::2]+['']
print(values)
print(delimiters)
print(''.join(v+d for v,d in zip(values,delimiters)))#复原字符串，但是很多空格会丢掉
```

![](https://img-blog.csdnimg.cn/20190329211647613.png)

```python
#若不想保留分割字符串到结果列表中，但 仍然需要使用到括号来分组正则表达式。确保你的分组是非捕获分组，形如(?:...)
print(re.split(r'(?:,|;|\s)\s*',line))
```

![](https://img-blog.csdnimg.cn/20190329211712951.png)](https://so.csdn.net/so/search/s.do?q=使用多个字符分割字符串&t=blog)




