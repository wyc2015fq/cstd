# python cookbook 2.7 最短匹配模式 - 别说话写代码的博客 - CSDN博客





2019年04月11日 09:54:34[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：13
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```
#用正则匹配时可能是最长匹配模式，而你想修改它匹配最短模式。一般用来匹配一对分隔符，比如一对括号和一对引号等
import re
strpat= re.compile(r'\"(.*)\"')
#此正则的意图是匹配被双引号包含的文本。但是*操作符是贪婪的，因此匹配操作会查找最长的可能匹配
#也就是在text2中会不满足我们想要的
text1='computer says "no."'
print(strpat.findall(text1))
text2='computer says "no.",phone says "yes."'
print(strpat.findall(text2))
```

![](https://img-blog.csdnimg.cn/20190411095249149.png)

```python
#基于这张情况，可以在模式中*操作符后面加上？修饰符，像这样：
strpat=re.compile(r'\"(.*?)\"')
print(strpat.findall(text2))
#这样就使得匹配变成非贪婪模式，从而得到最短的匹配，也就是我们想要的结果
```

![](https://img-blog.csdnimg.cn/2019041109530731.png)

```
#但是在这种情况下就不灵了，不能递归匹配
text3='computer says (no.(fine) ),phone says (yes.)'
strpat=re.compile(r'\((.*?)\)')
print(strpat.findall(text3))
```

![](https://img-blog.csdnimg.cn/20190411095321787.png)

在一个模式字符串中，点(.)匹配除了换行外的任何字符。若将点（.）号放在开始于结束符（比如引号）之间的时候，那么匹配操作会查找符合模式的最长的可能匹配。这样通常会导致很多中间的被开始与结束符包含的文本被忽略掉，并最终被包含在匹配结果字符串中返回。通过在*或者+这样的操作符后面添加一个？可以强制匹配算法改成寻找最短的可能匹配。



