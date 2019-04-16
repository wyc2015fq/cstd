# Python笔记_第一篇_童子功_5.Python数据类型之集合类型(set) - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Python笔记_第一篇_童子功_5.Python数据类型之集合类型(set)](https://www.cnblogs.com/noah0532/p/8419210.html)





　　集合！Python中的集合数据基本上是为了方便数学计算使用的。

　　什么是集合？

　　集合就是“确定的一堆东西”。集合里面的东西叫做元素。

　　特点：1. 集合里面是没有重复的元素的。

　　           2. 集合的创建也是通过( )的方式进行创建，里面可以是字符串的形式，也可以是列表等其他形式。

　　　　    3. 集合和字典一样也是没有顺序的概念。

**  4. 集合每次显示结果的顺序都是随机的！**

　　分类：在Python中对于集合分可变集合也就是最常用的set，还有一种叫frozenset（冻结集合）有不可编辑的性质。最主要的还是set集合，他们有一些区别。

　　区别：1. set:无序排列且不重复，不存在哈希值，元素是可变的，有add()、remove()等方法。

　　　　　　frozenset：是不可变的，存在哈希值，好处是他可以作为字典的key，也可以作为其他集合的元素。但是一旦创建没有add()、remove()等方法。

　　　　　2. set:基本功能包括关系测试和消除重复元素，集合对象还支持union（联合、并集）、intersection（交集）、difference（差集）和sysmmetric difference（对称差集）等数学运算。



**1.   集合的简单应用**

1.1   创建集合

　　语法：集合名 = set ("元素1", "元素2", "元素n")

　　创建一个空集合和带有元素的集合

```
1 # 创建集合 #
 2 set0 = set()
 3 set1 = set("boyaaaaa")  # 字符串按照单个元素进行拆分
 4 set2 = set(["y", "b", "o"]) # 也可以额用列表方式进行创建
 5 set3 = set([1, 2, 3])
 6 set4 = set([123,])
 7 print(set0)
 8 print(type(set0))
 9 print(set1)
10 print(set2)
11 print(type(set2))
12 print(set3)
13 print(set4)
14 # 输出结果：
15 # set()
16 # <class 'set'>
17 # {'y', 'a', 'o', 'b'}  # 注意一下，这里集合的显示结果是用花括号列出的
18 # {'b', 'o', 'y'}  # 注意一下，这里集合的显示结果是用花括号列出的
19 # <class 'set'>  # 注意一下，这里集合的显示结果是用花括号列出的
20 # {1, 2, 3}  # 注意一下，这里集合的显示结果是用花括号列出的
21 # {123}  # 注意一下，这里集合的显示结果是用花括号列出的
```

1.2   集合的常规操作

　　有一个函数没见过，列出一下：

**　　函：set.add**

**　　　　语：set.add("元素")**

**　　　　　　用：把元素添加到set集合里面**

```
1 # 集合添加、删除 #
 2 # 添加
 3 a = set("set")
 4 a.add("python") # 任意位置添加
 5 print(a)
 6 # 输出结果：
 7 # {'s', 'e', 'python', 't'}
 8 
 9 b = set("boy")
10 b.update("python") # 按照字符串的字符进行打散添加
11 print(b)
12 # 输出结果：
13 # {'y', 'p', 'h', 'b', 'o', 'n', 't'}
14 
15 # 删除
16 c = set(["y", "python", "b", "o"])
17 c.remove("python")
18 print(c)
19 # 输出结果：{'o', 'b', 'y'}
```



**2.   集合的数学运算**

　　集合的数学运算主要包括：

**-** 差集、相对补集运算,谁相对于谁没有的元素

**&** 并集；两个集合当中相同的元素，或者函数union(s)

**|**合集；a和b两个集合进行组合

**!=** 不等于；判断两个集合的元素是否相等，如果不相等返回为True，否则为False

**==** 相等；判断两个集合的元素是否相等，如果相等返回为True，否则为True

**in** 包含；判断集合a是否包含在集合b当中，如果包含返回为Ture，否则为False

**not in** 不包含；判断集合a是否不包含在集合b当中，如果不包含返回为True，否则为False

```
1 # 集合的数学运算 #
 2 a = set("abc")
 3 b = set("cdef")
 4 # - 差集、相对补集运算,谁相对于谁没有的元素
 5 print(a - b, b - a)   # 输出结果：{'b', 'a'} {'d', 'f', 'e'}
 6 
 7 # & 并集；两个集合当中相同的元素
 8 print(a & b)  # 输出结果：{'c'}
 9 
10 # | 合集；a和b两个集合进行组合
11 print(a | b)  # 输出结果：{'b', 'e', 'c', 'a', 'd', 'f'}
12 
13 # != 不等于；判断两个集合的元素是否相等，如果不相等返回为True，否则为False
14 print(a != b)  # 输出结果：True
15 
16 # == 相等；判断两个集合的元素是否相等，如果相等返回为True，否则为True
17 print(a == b)  # 输出结果：False
18 
19 # in 包含；判断集合a是否包含在集合b当中，如果包含返回为Ture，否则为False
20 print(a in b)  # 输出结果：False
21 
22 # not in 不包含；判断集合a是否不包含在集合b当中，如果不包含返回为True，否则为False
23 print(a not in b)  # 输出结果：True
```














