# Python笔记_第一篇_童子功_5.Python数据类型之元组类型(tuple) - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Python笔记_第一篇_童子功_5.Python数据类型之元组类型(tuple)](https://www.cnblogs.com/noah0532/p/8419201.html)





　　元组！在Python中元组是属于列表的一种延伸，也是一种有序集合，成为一种只读列表，即数据可以被查找，不能被修改，列表的切片操作同样适用于元组。

　　特点：1. 与列表非常相似。

　　　　　2. 一旦初始化就不能修改（这句话的意思是：元素不可变，但是元素中的值可以改）。

　　          3. 使用小括号。

　　　　   4. 如果元素当中只有一个元素(12, )，书写习惯上也应该加上一个逗号。原因和列表这个地方的原因是一样的。



**1.   元组的简单应用**

**1.1   创建元组**

　　格式：元组名 = (元组选项1，元组选项2，元组选项n)

　　创建一个空元组和带有元素的元组。

```
1 # 创建空的元组
 2 tuple1 = ()
 3 print(tuple1)
 4 # 输出结果：()
 5 
 6 # 创建带有元素的元组
 7 # 元组中的元素的类型可以不同
 8 tuple2 = (1, 2, 3, "good", True)
 9 print(tuple2)
10 # 输出结果：(1, 2, 3, 'good', True)
11 
12 # 定义只有一个元素的元祖
13 tuple3 = (1, )
14 print(tuple3)
15 print(type(tuple3))
16 # 输出结果：
17 # (1,)
18 # <class 'tuple'>
```



**1.2   元组的常规操作**

1.2.1   元素元素的访问

```
1 # 元组元素的访问
2 # 格式：元组名[下标]
3 tuple4 = (1, 2, 3, 4, 5)
4 print(tuple4[0])
5 # print(tuple4[5])  # 下标越界报错
6 print(tuple4[-1])  # 获取最后一个元素
7 # 输出结果：
8 # 1
9 # 5
```

1.2.2   元组元素的修改

　　元组不允许修改元素，但是元组当中元素的值（二级目录一下的值可以修改）

```
1 # 修改元组
 2 tuple5 = (1, 2, 3, 4, [5, 6, 7])
 3 tuple5[-1][0] = 500  # 改变里面的list可以变
 4 print(tuple5)
 5 # 输出结果：(1, 2, 3, 4, [500, 6, 7])
 6 
 7 # 元组的元素不能修改
 8 tuple6 = (1, 2, 3, "good", True)
 9 tuple6[3] = "bad"  # 改变里面的list可以变
10 print(tuple6)
11 # 输出结果：
12 # Traceback (most recent call last):
13 #   File "C:/Users/Administrator/Desktop/Python-1704/day04/tuple(元组).py", line 51, in <module>
14 #     tuple6[3] = "bad"  # 改变里面的list可以变
15 # TypeError: 'tuple' object does not support item assignment
```

1.2.3   元组元素的删除

```
1 # 删除元组
 2 tuple6 = (1, 2, 3)
 3 del tuple6[0]
 4 del tuple6
 5 print(tuple6)
 6 # 输出结果：
 7 # 不允许删除元组的元素
 8 # Traceback (most recent call last):
 9 #   File "C:/Users/Administrator/Desktop/Python-1704/day04/tuple(元组).py", line 67, in <module>
10 #     del tuple6[0]
11 # TypeError: 'tuple' object doesn't support item deletion
12 # 删除整个元组
13 # Traceback (most recent call last):
14 #   File "C:/Users/Administrator/Desktop/Python-1704/day04/tuple(元组).py", line 69, in <module>
15 #     print(tuple6)
16 # NameError: name 'tuple6' is not defined
```

1.2.4   元组的组合

```
1 # 元组的组合
 2 t7 = (1, 2, 3)
 3 t8 = (4, 5, 6)
 4 t9 = t7 + t8
 5 print(t7 + t8)
 6 print(t7, t8)
 7 print(t9)
 8 # 输出结果：
 9 # (1, 2, 3, 4, 5, 6)
10 # (1, 2, 3) (4, 5, 6)
11 # (1, 2, 3, 4, 5, 6)
```

1.2.5   元组的重复

```
1 # 元组重复
2 t10 = (1, 2, 3)
3 print(t10 * 3)
4 # 输出结果：(1, 2, 3, 1, 2, 3, 1, 2, 3)
```

1.2.6   判断元素是否在元组中，返回值为False 或 True，成员运算符（in，not in）；身份运算符同理不再举例。

```
1 # 判断元素是否在元组中
2 t11 = (1, 2, 3)
3 print(1 in t11)
4 # 输出结果：True
```

1.2.7   元组的截取（切片）

　　格式：[[start] [,end] [,step]]，开始位置，结束位置，步长

**注意：几到几的指针是放到元素的前面。**

```
1 # 元组的截取
 2 # 格式：元组名[开始下标，结束下标]
 3 # 从开始下标开始截取，截取到结束下标之前
 4 t12 = (1, 2, 3, 4, 5, 6, 7, 8, 9)
 5 print(t12[3:7])
 6 print(t12[3:])
 7 print(t12[:7])
 8 # 输出结果：
 9 # (4, 5, 6, 7)
10 # (4, 5, 6, 7, 8, 9)
11 # (1, 2, 3, 4, 5, 6, 7)
```

1.2.8   二维元组和三维元组

　　二维元组和三维元组，以及其他高维元组的逻辑是在元组内在表示一组元组元素。访问也是通过双下标、三下标或高下标的方式访问，操作高维元组和操作一维元组的逻辑是一样的。

```
1 # 二位元组，及其以上元组,元素为一维元组的元组
2 t13 = ((1, 2, 3), (4, 5, 6), (7, 8, 9))
3 print(t13[1][1])
4 # 输出结果：访问第二个元组中的第二个元素
5 # 5
```



**2.   元组的常见内建函数的应用**

**　　函：len　　**

**　　　　语：len(tuple)**

**　　　　　　用：返回元组中元素的个数**

```
1 # 元组的方法
2 # len 返回元组中元素的个数
3 t14 = (1, 2, 3, 4, 5)
4 print(len(t14))
5 # 输出结果：
6 # 5
```

**　　函：max()、min()**

**　　　　语：max(tuple)、min(tuple)**

**　　　　　　用：返回元组中的最大值、返回元组中的最小值**

```
1 # max()返回元组中的最大值
2 # min()返回元组中的最小值
3 print(max((5, 6, 7, 8, 9)))
4 print(min((5, 6, 7, 8, 9)))
5 # 输出结果：9   5
```



**3.   元组常用技巧**

3.1   将列表转换为元组

```
1 # 将列表转为元组
2 list = [1, 2, 3]
3 t15 = tuple(list)
4 print(t15)
5 print(type(t15))
6 # 输出结果：
7 # (1, 2, 3)
8 # <class 'tuple'>
```

**3.2   元组的遍历**

**　　函：enumerate**

**　　　　语：enumerate(e, start = )**

**　　　　　　用：枚举函数，start= 表示从数字几开始记录枚举的起始，如果是0，就为0,1,2,3,4......为序号开始标记；如果为1，就为1,2,3,4,5，为序号开始标记；默认为从0开始。**

```
1 # 元组的历编
 2 for i in (1, 2, 3, 4, 5):
 3     print(i, end="")
 4 print("\n")
 5 # 输出结果：12345
 6 
 7 # 通过枚举的方式遍历元组元素
 8 for i, v in enumerate((1, 2, 3, 4, 5), 1):
 9     print(i, v)
10 # 输出结果：
11 # 1 1
12 # 2 2
13 # 3 3
14 # 4 4
15 # 5 5
```














