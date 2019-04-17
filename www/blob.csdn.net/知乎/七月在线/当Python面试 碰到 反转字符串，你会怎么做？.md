# 当Python面试 碰到 反转字符串，你会怎么做？ - 知乎
# 



> 来源：公众号-哎妈呀Bug  
链接：
[https://mp.weixin.qq.com/s/wC5x8fRAwVvZYs3YGVr5iw](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/wC5x8fRAwVvZYs3YGVr5iw)


**按单词反转字符串**是一道很常见的面试题。在Python中实现起来非常简单。


```
def reverse_string_by_word(s):
    lst = s.split()  # split by blank space by default
    return ' '.join(lst[::-1])

s = 'Power of Love'
print reverse_string_by_word(s)
# Love of Power

s = 'Hello    World!'
print reverse_string_by_word(s)
# World! Hello
```


上面的实现其实已经能满足大多数情况，但是并不完美。比如第二个字符串中的感叹号并没有被翻转，而且原字符串中的空格数量也没有保留。（在上面的例子里其实Hello和World之间不止一个空格）

我们期望的结果应该是这样子的。


```
print reverse_string_by_word(s)
# Expected: !World  Hello
```


要改进上面的方案还不把问题复杂化，推荐使用`re`模块。你可以查阅`re.split()` 的官方文档。我们看一下具体例子。


```
>>> import re
>>> s = 'Hello  World!'

>>> re.split(r'\s+', s)    # will discard blank spaces
['Hello', 'World!']

>>> re.split(r'(\s+)', s)  # will keep spaces as a group
['Hello', '  ', 'World!']

>>> s = '< Welcome to EF.COM! >'

>>> re.split(r'\s+', s)  # split by spaces
['<', 'Welcome', 'to', 'EF.COM!', '>']

>>> re.split(r'(\w+)', s)  # exactly split by word
['< ', 'Welcome', ' ', 'to', ' ', 'EF', '.', 'COM', '! >']

>>> re.split(r'(\s+|\w+)', s)  # split by space and word
['<', ' ', '', 'Welcome', '', ' ', '', 'to', '', ' ', '', 'EF', '.', 'COM', '!', ' ', '>']

>>> ''.join(re.split(r'(\s+|\w+)', s)[::-1])
'> !COM.EF to Welcome <'

>>> ''.join(re.split(r'(\s+)', s)[::-1])
'> EF.COM! to Welcome <'

>>> ''.join(re.split(r'(\w+)', s)[::-1])
'! >COM.EF to Welcome< '
```


如果你觉得用切片将序列倒序可读性不高，那么其实也可以这样写。


```
>>> ''.join(reversed(re.split(r'(\s+|\w+)', s)))
'> !COM.EF to Welcome <'
```


一句话搞定，so easy!



![](https://pic2.zhimg.com/v2-7eea49f979c026756fabc2855377fecd_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='22' height='10'></svg>)



## **《机器学习 第九期》从零到机器学习实战项目，提供GPU&CPU双云平台，作业考试1V1批改（优秀学员内推BAT等）；点击文末“阅读原文”了解详情。**



![](https://pic2.zhimg.com/v2-66d900b19153e4c2698efb71e9ca954d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1066' height='296'></svg>)






[http://weixin.qq.com/r/NDjo8E3EtRDKrQmT920m](https://link.zhihu.com/?target=http%3A//weixin.qq.com/r/NDjo8E3EtRDKrQmT920m) (二维码自动识别)




