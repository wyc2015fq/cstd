# 【Python】.format()函数 - YZXnuaa的博客 - CSDN博客
2018年03月30日 18:20:12[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：60
个人分类：[Python库](https://blog.csdn.net/YZXnuaa/article/category/7389269)
用法：
　　它通过{}和:来代替传统%方式
1、使用位置参数
要点：从以下例子可以看出位置参数不受顺序约束，且可以为{},只要format里有相对应的参数值即可,参数索引从0开，传入位置参数列表可用*列表
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
>>> li = ['hoho',18]
>>> 'my name is {} ,age {}'.format('hoho',18)
'my name is hoho ,age 18'
>>> 'my name is {1} ,age {0}'.format(10,'hoho')
'my name is hoho ,age 10'
>>> 'my name is {1} ,age {0} {1}'.format(10,'hoho')
'my name is hoho ,age 10 hoho'
>>> 'my name is {} ,age {}'.format(*li)
'my name is hoho ,age 18'
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
2、使用关键字参数
要点：关键字参数值要对得上，可用字典当关键字参数传入值，字典前加**即可
```
>>> hash = {'name':'hoho','age':18}
>>> 'my name is {name},age is {age}'.format(name='hoho',age=19)
'my name is hoho,age is 19'
>>> 'my name is {name},age is {age}'.format(**hash)
'my name is hoho,age is 18'
```
3、填充与格式化
:[填充字符][对齐方式 <^>][宽度]
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
>>> '{0:*>10}'.format(10)  ##右对齐
'********10'
>>> '{0:*<10}'.format(10)  ##左对齐
'10********'
>>> '{0:*^10}'.format(10)  ##居中对齐
'****10****'
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
4、精度与进制
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
>>> '{0:.2f}'.format(1/3)
'0.33'
>>> '{0:b}'.format(10)    #二进制
'1010'
>>> '{0:o}'.format(10)     #八进制
'12'
>>> '{0:x}'.format(10)     #16进制
'a'
>>> '{:,}'.format(12369132698)  #千分位格式化
'12,369,132,698'
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
5、使用索引
```
>>> li
['hoho', 18]
>>> 'name is {0[0]} age is {0[1]}'.format(li)
'name is hoho age is 18
```
