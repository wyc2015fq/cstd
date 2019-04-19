# python列表常用内建方法 - _天枢 - 博客园
## [python列表常用内建方法](https://www.cnblogs.com/yhleng/p/9512839.html)
2018-08-21 17:41 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9512839)
python列表常用内建方法:
```
abc = ['a',1,3,'a']
#abc.pop(1) #删除索引1的值。结果['a', 3]
#abc.append([123]) #结果:['a', 1, 3, [123]]
#abc.extend([1234,456]) #在列表未尾一次性增加另一个列表内所有值 结果：['a', 1, 3, 1234, 456]
#print(abc.count('a')) #统计该值在列表中存在相同的个数 结果：2
#abc.remove('a') #从侧开始查找，找到该值删除。只删除第一个找到的值。 结果：[1, 3, 'a']
#abc.clear() #清除列表内容，结果为:[]
#########copy()#############
#d = abc #直接赋值，d 和abc使用的是一个内存地址 可以使用id(abc)来查看内存地址
# d=abc.copy() #复制一个列表，新开扩一段内存。
#
# print(d)
#abc.reverse() #列表内的值，进行反转['a', 3, 1, 'a']
#print(abc.index(3)) #返回元素所在位置的索引值
print(abc)
```
