# Python笔记_第一篇_童子功_5.Python数据类型之字典类型(dict) - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Python笔记_第一篇_童子功_5.Python数据类型之字典类型(dict)](https://www.cnblogs.com/noah0532/p/8419204.html)





　　字典！在Python中字典是另一种可变容器模型，可以存储任意类型的对象。是Python存储数据类型的最高级（maybe）。

　　特点：1. 字典的存储方式和其他类型数据略有不同，是通过键(key)和值(value)的方式进行存储的，其特点很像平时去查找字典，键类似于字典的目录，值是想要查找的内容。

　　　　　2. 字典属于哈希表的一种应用。

　　　　   3. 字典是通过{ }来定义、存储、查找数据的。

　　　　　4. 字典元素的访问是通过键[" 键名 "]来访问元素的。

　　　　　5. 特别说明一下在字典的增删改查的操作中，**记住这个公式：一个item = key + values，一个项目 = 一个键 + 值，后面对函数的用途就一目了然了。**

**1.   字典的简单应用**

1.1   创建一个字典

　　语法：dict = {key1:value1, key2:value2, keyn:valuen}，字典的每一个键值(key = value)是一一对应的，且用冒号进行分割。

　　字典包括{ }内键必须是唯一的，但职责可不比，因此字典是没有顺序的。字典很像一种数据库或者成绩单花名册。

```
1 # 创建一个字典 #
 2 # 创建一个空字典
 3 dict0 = {}
 4 print(dict0)
 5 # 输出结果：{}
 6 dict1 = {"Name": "Zara", }
 7 print(dict1)
 8 # 输出结果：{'Name': 'Zara'}
 9 dict2 = {"Name": "Zara", "Age": 7, "Class": "First"}
10 print(dict2)
11 # 输出结果：{'Name': 'Zara', 'Age': 7, 'Class': 'First'}
```

 1.2  字典的访问、修改和删除

　　这里特别说明一下**pop函数**和**popitem函数**，因为字典没有顺序这么一说，所以pop(keyname)也就类似于找到序号了；另外popitem函数，因为字典没有append这么一说，所以popitem()方法随机返回并删除字典的一对键和值(一般删除末尾对)。如果字典已经为空，调用了此方法，就会报keyerror异常。

```
1 # 字典的访问和修改和删除 #（与列表、元组大同小异）
 2 # 字典的访问[]
 3 dict = {"Name": "Zara", "Age": 7, "Class": "First"}
 4 print(dict["Name"]) # 通过键值访问
 5 print(dict["Age"]) # 通过键值访问
 6 # 输出结果：
 7 # Zara
 8 # 7
 9 
10 # 字典的修改
11 # 通过键来修改
12 dict1 = {"Name": "Zara", "Age": 7, "Class": "First"}
13 dict1["Age"] = 8 # 通过键值修改内容
14 dict1["School"] = "Qingdao University"  # 添加键值增加内容
15 print(dict1)
16 # 输出结果：
17 # {'Name': 'Zara', 'Age': 8, 'Class': 'First', 'School': 'Qingdao University'}
18 
19 # 字典的删除 del函数、clear函数、pop函数、popitem函数
20 dict3 = {'Name': 'Zara', 'Age': 7, 'Class': 'First'}
21 del dict3["Name"] # 清除字典中的某个键值
22 print(dict3) # 键的删除其值也不复存在
23 # 输出结果：{'Age': 7, 'Class': 'First'}
24 
25 dict3.clear() # 清除字典中所有内容
26 print(dict3)
27 # 输出内容：{}
28 
29 dict3 = {'Name': 'Zara', 'Age': 7, 'Class': 'First'}
30 dict3.pop("Name") # 删除给定key所对应的值
31 print(dict3)
32 # 输出结果：{'Age': 7, 'Class': 'First'}
33 
34 dict3 = {'Name': 'Zara', 'Age': 7, 'Class': 'First', "Married": "None"}
35 dict3.popitem()
36 print(dict3) # 随机返回并删除字典中对应的键和值
37 # 输出结果：{'Name': 'Zara', 'Age': 7, 'Class': 'First'}
```



**2.   字典的常用内建函数的应用**

**　　函：dict.copy()**

**　　　　语：字典的拷贝**

**　　　　　　用：属于深拷贝**

```
1 # dict.copy（）
2 # 作用：字典的拷贝
3 dict4 = {'Name': 'Zara', 'Age': 7, 'Class': 'First'}
4 dict5 = dict4.copy()
5 print(dict4, dict5)
6 # 输出结果： {'Name': 'Zara', 'Age': 7, 'Class': 'First'} {'Name': 'Zara', 'Age': 7, 'Class': 'First'}
```

** 　　函：dict.fromkey()**

**　　　　语：dict.fromkey(["key1", "key2", "keyn"])**

**　　　　　　用：创建一个新字典，序列seq中元素作为字典的键，value中所有的值都初始化**

```
1 # dict.fromkeys
2 # 作用：创建一个新字典，一序列seq中元素做字典的键，val为字典所对应的初始化
3 dict6 = {'Name': 'Zara', 'Age': 7, 'Class': 'First'}
4 dict7 = dict6.fromkeys(["Name", "Age", "Class"])  # 创建一个新字典，键值引用原先的那几个，且值都为None
5 print(dict7)
6 # 输出结果：{'Name': None, 'Age': None, 'Class': None}
```

** 　　函：dict.get**

**　　　　语：dict.get("key")**

**　　　　　　用：获取某一个键的值，每次只能差一个，若没有次键值返回为None**

```
1 # dict.get
 2 # 作用：获取某一个键的值，每次只能查一个，若没有此键值返回为None
 3 dict8 = {'Name': 'Zara', 'Age': 7, 'Class': 'First'}
 4 dict9 = dict8.get("school")
 5 dict10 = dict8.get("Class")
 6 print(dict9, "来自于： ", dict8)
 7 print(dict10, "来自于： ", dict8)
 8 # 输出结果：
 9 # None 来自于：  {'Name': 'Zara', 'Age': 7, 'Class': 'First'}
10 # First 来自于：  {'Name': 'Zara', 'Age': 7, 'Class': 'First'}
```

** 　　函：dict.setdefault**

**　　　　语：dict.setdefault("key")**

**　　　　　　用：与dict.get的效果一样，如果键不存于字典中，将会添加键并将值设为默认的None**

```
1 # dict.setdefault与get效果一样，如果键不存在于字典中，将会添加键并将值设置为default默认None
2 # 作用：获取某一个键的值，每次只能查一个
3 dict8 = {'Name': 'Zara', 'Age': 7, 'Class': 'First'}
4 dict9 = dict8.setdefault("School")
5 print(dict9, dict8) # 将原先的赋值为None，重新提添加了一个键，值默认为None
6 # 输出结果：None {'Name': 'Zara', 'Age': 7, 'Class': 'First', 'School': None}
```

**　　函：dict.item**

**　　　　语：dict.item()**

**　　　　　　用：对整个字典进行遍历**

```
1 # dict.items()
 2 # 作用：以列表返回可遍历的（键、值）
 3 dict10 = {'Name': 'Zara', 'Age': 7, 'Class': 'First'}
 4 dict11 = dict10.items()
 5 print(dict11) # 显示字典中所有元素的全貌
 6 print(type(dict10))
 7 print(type(dict11)) # 我们观察到这个遍历函数返回的是一个dict_items类型，可用list函数把找到的东西强制转换成列表形式。
 8 # 输出结果：
 9 # dict_items([('Name', 'Zara'), ('Age', 7), ('Class', 'First')])
10 # <class 'dict'>
11 # <class 'dict_items'>
```

** 　　函：dict.key、dict.values**

**　　　　语：dict.key()、dict.values()**

**　　　　　　用：遍历整个字典中的键和值**

```
1 # dict.key()、dict.values()
2 # 作用：显示字典中所有的键值、显示字典中所有的值
3 dict11 = {'Name': 'Zara', 'Age': 7, 'Class': 'First'}
4 dict12 = dict11.keys()
5 print(list(dict12), dict11.values())  # 这里特别说明一下，可以用list函数把找到的东西强制转换成列表形式。
6 # 输出结果：['Name', 'Age', 'Class'] dict_values(['Zara', 7, 'First'])
```

** 　　函：dict.update**

**　　　　语：dict1.update(dict2)**

**　　　　　　用：把dict2的字典更新到dict1里面，dict1的内容顺延添加到dict1的字典中。**

```
1 # dict.update()
2 # 作用：将某个字典中的值，后续顺延更新到某个字典中
3 dict13 = {'Name': 'Zara', 'Age': 7}
4 dict14 = {'Sex': 'female' }
5 dict14.update(dict13)
6 print(dict14)
7 # 输出结果：{'Sex': 'female', 'Name': 'Zara', 'Age': 7}
```



**3.   字典常用的技巧**

3.1   字典的遍历

```
1 # 字典的遍历
 2 dict15 = {"name": "aliex", "age": 18}
 3 for i, v in enumerate(dict15):
 4     print(i, v)
 5 # 这里看枚举函数就不能使用了。
 6 # 输出结果：
 7 # 0 name
 8 # 1 age
 9 
10 dict15 = {"name": "aliex", "age": 18}
11 for i in dict15:
12     print(i, dict15[i])  # 首先找到i的键，然后通过dict[键]的方式再把值列出来
13 # 输出结果：
14 # name aliex
15 # age 18
16 
17 
18 for items in dict15.items():
19     print(items)
20 print(type(items))  
21 # 这样遍历的方式，是把内容全部以元组方式进行输出
22 # 输出结果：
23 # ('name', 'aliex')
24 # ('age', 18)
25 # <class 'tuple'>
26 
27 for key,values in dict15.items():
28     print(key,values)
29 # 通过分开写的形式，可以遍历出具体的数据
30 # 输出结果：
31 # name aliex
32 # age 18
```














