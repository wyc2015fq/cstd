# Python笔记_第一篇_童子功_5.Python数据类型之字符串类型(string) - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Python笔记_第一篇_童子功_5.Python数据类型之字符串类型(string)](https://www.cnblogs.com/noah0532/p/8418278.html)





　　关于Python的字符串处理也如其他语言一样属于重点中的重点，主要是牵扯到的函数和内容较为多和乱一些。关于什么是字符串，Python中的定义是：以单引号或者双引号括起来的任意文本。



**1.   字符串的简单应用**

1.1   创建字符串

```
1 str1 = "Thomas is a good man"
2 str2 = "Thomas is a nice man"
3 str3 = "Thomas is a handsome man"
4 print(str1, str2, str3)
5 print(type(str1), type(str2), type(str3))
6 # 输出结果：
7 # Thomas is a good man Thomas is a nice man Thomas is a handsome man
8 # <class 'str'> <class 'str'> <class 'str'>
```

1.2   转换成字符串

```
1 # 转换成字符串
2 a = 18
3 b = str(a)
4 print(b, type(b))
5 # 输出结果：18 <class 'str'>
```

1.3   字符串的运算（字符串的拼接和字符串的重复输出）

```
1 # 字符串的运算
 2 # 字符串的拼接
 3 str6 = "Thomas is a "
 4 str7 = "good man"
 5 str8 = str6 + str7
 6 print(str8)
 7 # 输出结果：Thomas is a good man
 8 
 9 # 字符串的重复输出
10 str9 = "good"
11 str10 = str9 * 3
12 print(str10)
13 # 输出结果：goodgoodgood
```

1.4   字符串大小的比较

　　字符串大小的比较和数字类型的大小比较最大的不同，由于是字符串不能直接的比较数字大小。因此规则为：从第一个字符开始比较大小，谁的ASCII码大谁就大，如果相等会比较下一个字符的ASCII码值大小，那么谁的值大就大。和数字类型大小比较相同，返回值也是True 或者 False。

```
1 # 字符串大小的比较
2 print("a" > "b")
3 # 输出结果：False
4 print("Thomas" == "Thomas")
5 # 输出结果：True
6 print("-azzzz" < "-")  # 第二个字符串的值为0比z小所以返回为假
7 # 输出结果：False
```

1.5   字符串的查找（访问）

　　字符串的访问最常用的方式是通过下标进行访问，下标的格式为：[ ]（[start] ：[end] ：[step]），下标可为正也可为负，为负为从右向左查找，默认某个具体的下标位置对应的字符，其中用：进行分割。另外说明一下，所有的计算机计数都是从0开始的。在字符串中，0表示字符串的第一个字符位置，空格也算一个字符。

　　字符串的访问，也可以叫“切片”，截取字符串中的一部分。

```
1 # 通过下标的方式进行访问
 2 str11 = "Thomas is a good man"
 3 print(str11[12])  # 输出结果：g，第11个字符
 4 # 截取字符串中的一部分（切片）
 5 str13 = "Thomas is a good man"
 6 str15 = str13[6:16]  # 空格也是一个字符。从第七个字符位置到第十七个字符位置
 7 print(str15)  # 输出结果：is a good
 8 
 9 str16 = str13[0:6]  # str16 = str13[:5]这样也可以，不写起始位置默认都是从0开始
10 print(str16)  # 输出结果：Thomas
11 
12 str17 = str13[7:]
13 print(str17)  # 输出结果：is a good man
14 
15 str18 = str13[-1]
16 print(str18)  # 输出结果：n  从右往左数第一个字符
17 
18 str19 = str13[::-1]
19 print(str19)  # 输出结果：nam doog a si samohT，通过倒叙方式输出一遍，最有一个位置步长为1
20 
21 str20 = str13[0::2]
22 print(str20)  # 输出结果：Toa sago a ，从开始到最后，按照2个字符步长进行切片
```

1.6   判断某个字符是否是字符串的一部分。

　　这里就用到了2个成员运算符和另外2个身份运算符。

**　　符："in"、"not in" 和 "is"、"is not"**

**　　　　用：成员运算符：判断左边的成员是否属于右边的集合，如果是返回为True；如果非，返回为False。相反的是not in，如果是返回为False，如果非返回为True。**

**　　　　　　身份运算符：判断左边的成员是否和右边成员相等，如果是返回为True；如果非，返回为False。相反的是is not，如果是返回为False，如果非返回为True。**

```
1 # 成员运算符
2 str21 = "Thoams is a good man"
3 print("good" in str21)  # 输出结果：True
4 print("good1" not in str21)  # 输出结果：True
5 
6 # 成员运算符
7 print("good" is "good")  # 输出结果：True
8 print("good1" is not "good")  # 输出结果：True
```



**2. 字符串常用的内建函数**

　　这部分的内容是非常重要的，通过内建函数（built-in func）可以实现字符串的增删改查。增删改查是对于各种数据类型常用的操作手段。

2.1   函数：eval，这部分内容在数字型数据有所阐述，这里指举几个例子：

```
1 # eval（str）
 2 # 作用1：将字符串str当成有效的表达式来求值，并返回计算结果
 3 num1 = eval("123")
 4 print(num1)
 5 print(type(num1))
 6 print(eval("+123"))
 7 print(eval("-123"))
 8 print(eval("12-3"))
 9 # 作用2：实现list、dict、tuple与str之间的转化
10 # 字符串转换成列表
11 a = "[[1,2], [3,4], [5,6], [7,8], [9,0]]"
12 print(type(a))
13 b = eval(a)
14 print(b)
15 # 字符串转换成字典
16 a = "{1: 'a', 2: 'b'}"
17 print(type(a))
18 b = eval(a)
19 print(type(b))
20 print(b)
21 # 字符串转换成元组
22 a = "([1,2], [3,4], [5,6], [7,8], (9,0))"
23 print(type(a))
24 b=eval(a)
25 print(type(b))
26 print(b)
```



2.2    其他内建函数

**　　函：len**

**　　　　语：len(string)**

**　　　　　　用：返回字符串的长度，一般用于查找字符串的循环中**

```
1 # len(str)
2 # 作用：返回字符串的长度，一般用于找字符的循环中
3 print(len("sunck is a good man"))
4 # 输出结果：19
```

**　　函：lower、upper**

**　　　　语：lower(string)、upper(string)**

**　　　　　　用：将字符串中的大写字母转换为小写字母，中文不变；将字符串中的小写字母转换成大写字母，中文不变。**

```
1 # lower()
 2 # 作用：将字符串中的大写字母转换为小写字母，中文不变
 3 str20 = "SUNCK is a Good Man"
 4 print(str20.lower())
 5 # 输出结果：sunck is a good man
 6 
 7 # upper()
 8 # 作用：将字符串中的小写字母转换成大写字母
 9 str21 = "SUNCK is a Good Man"
10 print(str21.upper())
11 # 输出结果：SUNCK IS A GOOD MAN
```

**　　函：swapace**

**　　　　语：swapace(string)**

**　　　　　　用：字符串中大小写的互相转换，将大写字母转换成小写字母，将小写字母转换成大写字母。**

```
1 # swapcase()
2 # 作用：字符串中的大小写的相互转换
3 str22 = "SUNCK is a Good Man"
4 print(str22.swapcase())
5 # 输出结果：sunck IS A gOOD mAN
```

**　　函：capitalize、title**

**　　　　语：capitalize(string)、title(string)**

**　　　　　　用：将字符串中的首句首字母大写，其他字母位置转换成小写；将每个单词（或者字母）第一个单词大写。**

```
1 # capitalize()
 2 # 作用：把首字母变成大写，其他字母位置都是小写
 3 str23 = "sunck is a good man"
 4 print(str23.capitalize())
 5 # 输出结果：Sunck is a good man
 6 
 7 # title()
 8 # 作用：把每个单词的首字母大写
 9 str24 = "sunck is a good man"
10 print(str24.title())
11 # 输出结果：Sunck Is A Good Man
```

**　　函：center**

**　　　　语：center(self,width,fillchar)  fillchar = character**

**　　　　　　用：返回一个指定宽度的字符串（居中的字符串），fillchar为填充的字符，默认为空格填充。**

```
1 # center(self,width,fillchar) char = character
2 # 作用：返回一个指定宽度的字符串（居中的字符串），fillchar为填充的字符，默认是空格填充
3 str25 = "kaige is a nice man"
4 print(str25.center(40, "*")) # 输出结果：**********kaige is a nice man***********
5 print(str25.center(40))
6 # 输出结果：          kaige is a nice man
```

**　　函：ljust、rjust**

**　　　　语：ljust(width, fillchar) fillchar = character、rjust(width,fillchar) fillchar = character**

**　　　　　　用：l=left，返回一个指定宽度的左对齐字符串，fillchar为填充字符，默认为空格。**

**　　　　　　　　r=right，返回一个指定宽度的右对齐字符串，fillchar为填充字符，默认为空格。**

```
1 # ljust(width [,fillchar])
2 # rjust(width [,fillchar])
3 # 作用：l = left，返回一个指定宽度的左对齐字符串，fillchar为填充字符，默认空格填充
4 #      r = right，返回一个指定宽度的左对齐字符串，fillchar为填充字符，默认空格填充
5 str26 = "Kaige is a nice man"
6 print(str26.ljust(40, "%"))
7 print(str26.rjust(40, "%"))
8 # 输出结果：Kaige is a nice man%%%%%%%%%%%%%%%%%%%%%
9 # 输出结果：%%%%%%%%%%%%%%%%%%%%%Kaige is a nice man
```

**　　函：zfill**

**　　　　语：zfill(width)**

**　　　　　　用：z = zero，返回一个长度为width的字符串，元字符右对齐，左侧填充0。**

```
1 # zfill(width)
2 # 作用：返回一个长度为width的字符串，原字符串右对齐，前面补0
3 str28 = "kaige is a nice man"
4 print(str28.zfill(40))
5 # 输出结果：000000000000000000000kaige is a nice man
```

**　　函：count**

**　　　　语：count(str,[,start] [,end])**

**　　　　　　用：返回字符串中str出现的次数，可以指定一个start和end字符串中的范围，默认从头到尾。**

```
1 # count(str [,start] [,end])
2 # 作用：返回字符串中str出现的次数，可以指定一个start和end字符串中的范围，默认从头到尾
3 str29 = "kaige is a very very good man"
4 print(str29.count("very", 11, len(str29)))  # 从第几个字符位置开始查找
5 print(str29.count("very", 12, len(str29)))  # 从第几个字符位置开始查找
6 # 输出结果：2
7 # 输出结果：1
```

**　　函：find、rfind**

**　　　　语：find(str, [,start] [,end])、rfind(str, [,start] [,end])**

**　　　　　　用：从左向右，检测str字符串是否在包含的字符串中，可以指定范围，默认从头到尾。得到的是第一次出现的开始下标，只不过如果str不存的，返回为-1。**

**　　　　　　　　从右向左，检测str字符串是否在包含的字符串中，可以指定范围，默认从尾到头。得到的是第一次出现的开始下标，只不过如果str不存的，返回为-1。**

```
1 # find(str [,start] [,end])
 2 # 作用：从左向右，检测str字符串是否包含在字符串中,可以指定范围，默认从头到尾。得到的是第一次出现的开始下标
 3 # 只不过如果str不存在的时候，没有返回为 -1
 4 str30 = "kaige is a very very nice man"
 5 print(str30.find("very"))
 6 print(str30.find("good"))
 7 print(str30.find("very", 16, len(str30)))
 8 # 输出结果：11
 9 # 输出结果：-1
10 # 输出结果：16
11 
12 # rfind(str[,start][,end])
13 # 作用：从右向左，检测str字符串是否包含在字符串中,可以指定范围，默认从尾到头。得到的是第一次出现的开始下标
14 # 只不过如果str不存在的时候，没有返回为 -1
15 print(str30.rfind("very"))
16 print(str30.rfind("good"))
17 print(str30.rfind("very", 16, len(str30)))
18 # 输出结果：16
19 # 输出结果：-1
20 # 输出结果：16
```

**　　函：index、rindex**

**　　　　语：index(str,, [,start] [,end])、rindex(str,, [,start] [,end])**

**　　　　　　用：用法与find和rfind一样，但是如果str不存的话，会报告一个异常。**

```
1 # index(str,start=0, end=len(str)
 2 # rindex(str,start=0, end=len(str)
 3 # 作用：跟find方法一样，只不过如果str不存在的时候，他会报一个异常
 4 str31 = "kaige is a very very nice man"
 5 str32 = "kaige is a very very nice man"
 6 # print(str31.index("good"))
 7 # 输出结果：
 8 # Traceback (most recent call last):
 9 #   File "C:/Users/Administrator/Desktop/Python-1704/day03/String(字符串).py", line 162, in <module>
10 #     print(str31.index("good"))
11 # ValueError: substring not found
12 print(str31.index("very"))
13 # 输出结果：11
14 print(str32.rindex("very"))
15 # 输出结果：16
```

**　　函：operator.eq(cmp)**

**　　　　语：operator.eq(str1, str2)**

**　　　　　　用：比较两个字符串的大小，在Python3中，原先的cmp函数比较字符大小的方式被取代，调用operator库的方式来比较两个字符串的大小**

```
1 # operator.eq = cmp
2 # 字符串的比较，在python3中已经没有cmp这个命令了，要调用operator库
3 import operator
4 print(operator.eq("hello", "name"))
5 print(operator.eq("hello", "hello"))
6 # 输出结果：False
7 # True
```

**　　函：lstrip、rstrip、strip**

**　　　　语：lstrip(char)、rstrip(char)、strip(char)**

**　　　　　　用：高级切片功能。从左侧开始截取指定的字符、从右侧开始截取指定的字符、从任意位置截取指定的字符（默认为空格，包括换行符\n，也包括制表符\t等）**

```
1 # lstrip()、rstrip()
 2 # 作用：截掉字符串左侧指定的字符，默认为空格
 3 # 作用：截掉字符串右侧指定的字符，默认为空格
 4 str33 = "**********kaige is a nice man"
 5 str34 = "kaige is a nice man%*%*******"
 6 print(str33.lstrip("*"))
 7 # 输出结果：kaige is a nice man
 8 print(str34.rstrip("*"))  # 备注：这两个命令是按照顺序截取，遇到kaige is a nice man%*%一个非截取，返回
 9 # 输出结果：kaige is a nice man%*%
10 
11 # strip()
12 # 作用：任意截取字符串指定的字符，默认为空格
13 str35 = "*********%kaige is a nice man*%%%%******"
14 print(str35.strip("%*"))
15 # 输出结果：kaige is a nice man
```

**　　函：join**

**　　　　语："(拼接内容)".join([拼接变量A,拼接变量B])**

**　　　　　　用：字符串拼接函数**

```
1 # join
 2 # ""(按照什么拼接内容进行拼接).join([拼接变量1，拼接变量n])
 3 # ""(按照什么拼接内容进行拼接).join((拼接变量1，拼接变量n))
 4 # 作用：字符串拼接
 5 a = "123"
 6 b = "abc"
 7 str36 = "****".join([a, b])
 8 # str36 = a.join(b) # a123b123c123
 9 print(str36)
10 print(type(str36))
11 # 输出结果：
12 # 123****abc
13 # <class 'str'>
```

**　　函：startswith、endswith**

**　　　　语：startswith(str [,start] [,end])、endswith(str [,start]  [,end])**

**　　　　　　用：判断以什么字符串（字符）作为开头、判断以什么字符串（字符）作为结尾。**

```
1 # startswith(str,[,start] [,end])
2 # endswith(str,[,start] [,end])
3 # 作用：以什么字符串（字符）作为开头；以什么字符串（字符）作为结尾，返回为真假，有大小写
4 str37 = "hello kitty"
5 print(str37.endswith("tty3"))
6 print(str37.startswith("he"))
7 # 输出结果：
8 # False
9 # True
```

**　　函：expandtabs**

**　　　　语：expandtabs(tabsize = 10)**

**　　　　　　用：与\t结合使用，在字符串中\t的位置加入空格。**

```
1 # expandtabs(tabsize = 10)
2 # 作用：与\t结合使用，在字符串中\t的位置空加空格
3 str38 = "h\tello kitty"
4 print(str38.expandtabs(tabsize = 20))
5 # 输出结果：h                   ello kitty
```

**　　函：replace**

**　　　　语：replace(“被替换”,“替换内容”)**

**　　　　　　用：用一个字符串（字符）去替换字符串（字符）当中的内容。**

```
1 # replace("替换"，"替换内容")
2 # 作用：替换字符串内容
3 print("My title title".replace("title", "Good"))
4 # 输出结果：My Good Good
```

**　　函：split、rsplit**

**　　　　语：split("", section) section = 0、split("", section) section = 0**

**　　　　　　用：按照什么分割, 按照第几次出现分割字符的位置开始分割，section=分割成次数、从右侧开始按照什么分割, 按照第几次出现分割字符的位置开始分割，section=分割成次数。**

```
1 # split()、rsplit()
 2 # split(按照什么分割, 按照第几次出现分割字符的位置开始分割)
 3 # 作用：按照什么进行分割，生成一个列表；从右侧
 4 print("My title title".split("t", 2)) #分割的位置将没有
 5 print("My title title".split(" ")) # 按照空格进行分割
 6 print("My title title".rsplit("t", 3)) # 按照空格进行分割
 7 # 输出结果：
 8 # ['My ', 'i', 'le title']
 9 # ['My', 'title', 'title']
10 # ['My ti', 'le ', 'i', 'le']
```



2.3   is... 几种判断字符串的函数（返回的都是True和False两种结果）

**　　函：isnumeric、isalpha、isalnum**

**　　　　语：isnumeric(str)、isalpha(str)、isalum(str)**

**　　　　　　用：判断是否是包含或全包含数字，是返回为真，否则为假。**

**　　　　　　　　判断是否是包含或者全包含字母，是返回为真，否则为假。**

**　　　　　　　　拆分：is alpha number，如果字符串中包含或全包含字母、数字、中文返回为真，否则返回为假。**

**　　　　　　　　其他诸如里面有空格或者其他字符，返回都为假。**

```
1 # isnumeric()
 2 # 作用：判断是否是一个数字（必须是整型）
 3 print("123.00".isnumeric())
 4 # 输出结果：False
 5 print("123".isnumeric())
 6 # 输出结果：True
 7 print("123  ".isnumeric())  # 因为有一个空格
 8 # 输出结果：False
 9 
10 # isalpha()
11 # 作用：判断是否是一个字母
12 print("abc".isalpha())
13 # 输出结果：True
14 print("123".isalpha())
15 # 输出结果：False
16 print("abc ".isalpha())  # 因为有一个空格
17 # 输出结果：False
18 
19 # isalnum()
20 # 作用：如果 string 至少有一个字符并且所有字符都是字母或数字则返回 True,否则返回 False
21 print("abc123中国".isalnum())
22 # 输出结果：True
23 print(" abc123中国".isalnum())  # 有一个空格
24 # 输出结果：False
```

**　　函：isdecimal**

**　　　　语：isdecimal(str)**

**　　　　　　用：判断字符串中是否是一个十进制数**

```
1 # isdecimal()
2 # 作用：判断是否是一个十进制数
3 print("0010".isdecimal())
4 print("0b0010".isdecimal())  # 里面有个b表示二进制
5 # 输出结果：
6 # True
7 # False
```

**　　函：isdigit**

**　　　　语：isdigit(str)**

**　　　　　　用：判断字符串中是否是一个数字（必须是数字，和isnumeric用法一样）**

```
1 # isdigit()
2 # 作用：判断是否是一个数字（必须是整型）
3 print("126.9999".isdigit())
4 print("126".isdigit())
5 # 输出结果：
6 # False
7 # True
```

**　　函：islower、isupper**

**　　　　语：islower(str)、isupper(str)**

**　　　　　　用：判断字符串中是否都是小写（忽略里面有空格情况）；判断字符串中是否都是大写（忽略里面有空格情况）**

```
1 # islower()、isupper
2 # 作用：判断是否是全小写；断点是否为全大写
3 print("adf  ".islower(), "ABc".isupper())  # 忽略空格
4 # 输出结果：True  False
```

**　　函：isspace**

**　　　　语：isspace(str)**

**　　　　　　用：判断字符串是否是一个只包含空格的字符串，如果里面存在字符返回为假，否则为真。**

```
1 # isspace()
2 # 作用：判断是否是一个空格
3 print("  ".isspace())  # 只要里面有一个字符就是假
4 # 输出结果：True
```

**　　函：istitle**

**　　　　语：istitle(str)**

**　　　　　　用：判断字符串中是否每个单词都是大写，如果是返回为真，否则为假。**

```
1 # istitle()
2 # 作用：判断标题是否是首字母大写
3 print("My Title".istitle())
4 # 输出结果：True
```

**　　函：isidentifier　　　　**

**　　　　语：isidentifier()**

**　　　　　　用：判断字符串是否一个变量名（按照变量命名规则来，首字母不允许数字等等）**

```
1 # isidentifier()
2 # 作用：判断是否是一个变量名
3 print("34abc".isidentifier())
4 # 输出结果：False
```

**　　函：isinstance**

**　　　　语：isinstance(object, (classinfo1,classinfo2,classinfon))**

**　　　　　　用：判断一个对象是否是已知的类型，类似于type()。其中object---实例对象；classinfo---可以是直接或者间接类名、基本类型或者有他们组成的元组。如果object判断为真，返回为True，如果object判断为假，返回为False。其中classinfo可以是一个单独进行判断，如果是多个表示object的类型是括号里面的其中一个（用括号括起来是表示由classinfo组成的元组）。**

　　另外：isinstance和type的区别如下：

　　1. type可以只接收一个参数，打印其位置所属的类型；而isinstance智能是否属于某个已知的类型。

　　2. isinstance 可以判断子类对象是否继承了父类；而type不可以。

```
1 # isinstance
 2 a = 2
 3 isinstance(a, int)
 4 print(isinstance(a, int))
 5 isinstance(a, str)
 6 print(isinstance(a, str))
 7 isinstance(a, (int, str, list))
 8 print(isinstance(a, (int, str, list)))  # 是元组中的一个，返回为True
 9 # 输出结果：
10 # True
11 # False
12 # True
13 
14 # type和isinstance的区别
15 class A:
16     pass
17 class B(A):
18     pass
19 
20 isinstance(A(), A)
21 print(isinstance(A(), A))  # returns True
22 
23 type(A()) == A
24 print(type(A()) == A)  # returns True
25 
26 isinstance(B(), A)
27 print(isinstance(B(), A) )  # returns True
28 
29 type(B()) == A
30 print(type(B()) == A )  # returns False
```

** 　　函：isatty**

**　　　　语：isatty()**

**　　　　　　用：返回是否是一个终端设备（不常用）**

　　举例：略














