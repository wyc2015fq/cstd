# 一句python，一句R︱python中的字符串操作、中文乱码、NaN情况（split、zip...） - 素质云笔记/Recorder... - CSDN博客





2016年08月24日 17:03:54[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2543












先学了R，最近刚刚上手python,所以想着将python和R结合起来互相对比来更好理解python。最好就是一句python，对应写一句R。

pandas可谓如雷贯耳，数据处理神器。



以下符号：

**=R=**

代表着在R中代码是怎么样的。

**速查笔记：**



```python
string模块，还提供了很多方法，如  
S.find(substring, [start [,end]]) #可指范围查找子串，返回索引值，否则返回-1  
S.rfind(substring,[start [,end]]) #反向查找  
S.index(substring,[start [,end]]) #同find，只是找不到产生ValueError异常  
S.rindex(substring,[start [,end]])#同上反向查找  
S.count(substring,[start [,end]]) #返回找到子串的个数  

S.lowercase()  
S.capitalize()      #首字母大写  
S.lower()           #转小写  
S.upper()           #转大写  
S.swapcase()        #大小写互换  

S.split(str, ' ')   #将string转list，以空格切分  
S.join(list, ' ')   #将list转string，以空格连接  

处理字符串的内置函数  
len(str)                #串长度  
cmp("my friend", str)   #字符串比较。第一个大，返回1  
max('abcxyz')           #寻找字符串中最大的字符  
min('abcxyz')           #寻找字符串中最小的字符  

string的转换  

float(str) #变成浮点数，float("1e-1")  结果为0.1  
int(str)        #变成整型，  int("12")  结果为12  
int(str,base)   #变成base进制整型数，int("11",2) 结果为2  
long(str)       #变成长整型，  
long(str,base)  #变成base进制长整型，
```















————————————————————————————————————————————



# 字符编码



## encode 和 decode



Python2 默认的编码是 ascii，通过 encode 可以将对象的编码转换为指定编码格式（称作“编码”），而 decode 是这个过程的逆过程（称作“解码”）。





decode，将**字节串**转变为**字符串**，并且这个字符串是按照 unicode 编码的。在 unicode 编码中，一个汉字对应一个字符，这时候度量它的长度就是 1.

encode，一个 unicode 编码的字符串，也可以转换为字节串。



```
>>> a = "中"
>>> a
'\xe4\xb8\xad'

>>> b = a.decode()
>>> b
u'\u4e2d'
```

其中，a就是ASCII格式的编码，字节串；b就是unicode编码的字符串。当然有一个问题就是最后出来的还不是汉字。。。（醉！）

在py2中print的时候，把‘\\u6838\\u7535\\u5360’这类，unicode格式，转化为中文变得可视化：

```
str = '\u82f1\u8bed\u8001\u5e08'
tran = str.decode('unicode-escape')
pritn(tran)
#英语老师
```



————————————————————————————————————————————

# 一、字符形成、展示、拼接、切片



## 1、字符形成=R=paste



双引号包裹单引号



```
>>> "What's your name?"
"What's your name?"
```



单引号使用转义符









```
>>> 'What\'s your name?'
"What's your name?"
```











## 2、字符串展示print /raw_input





```
>>> name = raw_input("input your name:")
input your name:python
>>> name
'python'
```

其中raw_input有点交互关系，具体看案例，直接键入name就可以获得你输入的内容。



```
>>> print("hello, world")
hello, world
```















## 3、字符切片、选择、截取 =R=无



字符可以像一般的数据格式一样进行切片选择，有点像series：



```
>>> lang = "study Python"
>>> lang[0]
's'
>>> lang[2:9]
'udy pyt'
```



当然也包括lang[:]可以选中所有的。

其中index代表着某个字符的索引值。


`lang.index("p")`




## 4、内存编号 =R= 无







这个与R中不一样，当数据存入python之后，机器会自动给存入内存的数据编号，这个编号可以用id来查看。



```
>>> id(c)
3071934536L
>>> id(lang)
3071934536L
```







## 5、ASCII 值（是十进制的）





ord("a")   代表输入字符返回ASCII值

cha(97)  代表输入ASCII值返回字符





```
>>> cmp("a","b")    #a-->97, b-->98, 97 小于 98，所以 a 小于 b
-1
```

其中cmp()代表比较 a  b 两个字符的ASCII值的大小，返回值为1，0，-1



```
>>> max(str1)
'd'
>>> max(str2)
'e'
>>> min(str1)
'a'
```

返回字符的ASCII值的最大值。





————————————————————————————————————————————



# 二、字符串基本操作

## 1、字符串重复 =R=rep



```
>>> str1*3
'abcdabcdabcd'
```





其中变成字符串有两种方式：一种是str()或者是用单引号来表示。





## 2、字符串拼接



### ```方式一：单个字符相连用 + 就可以； 方式二：list组内相连，可以用join； 方式三：list与list相连，可以用zip。 方式四：占位符，“%s”字符串格式化的方式 方式五：.format()```

### （1）加号 =R= paste



```
>>> "Py" + "thon"
'Python'

>>> a = 1989
>>> b = "free"

>>> print b+“a”
>>> print b+str(a)
```



其中变成字符串有两种方式：一种是str()或者是用单引号来表示。

乘法，就是重复那个字符串的含义。







### （2）join =R= paste



```
['www', 'itdiffer', 'com']
>>> ".".join(c)
'www.itdiffer.com'
```



用 .  来填补间隔中的内容。





**（3）其他**





```python
#字符串连接   +
 #两两单字符相连
sStr1 = 'strcat'
sStr2 = 'append'
sStr1 += sStr2  #sStr1改变了
sStr1 + sStr2
```



```python
#list中相连
delimiter = ','
mylist = ['Brazil', 'Russia', 'India', 'China']
print delimiter.join(mylist)

>>>Brazil,Russia,India,China
```



其中这里，’,’.join(mylist)，书写格式是，把”,”这个符号，加到mylist这些list之间，连接起来。



```python
#list字符贴在一起
name=('jack','beginman','sony','pcky')
age=(2001,2003,2005,2000)
for a,n in zip(name,age):
  print a,n
```

```python
#list+list相连
  >>> z1=[1,2,3]
  >>> z2=[4,5,6]
  >>> result=zip(z1,z2)
  >>> result
  [(1, 4), (2, 5), (3, 6)]
```









```python
#占位符
#“%s”字符串格式化的方式，%s当占位符在前面的字符串中占一个位置，后面用百分号%来连接需要填进去的对象。
print 'Running solvers for %d iterations...' % niter
```



%d——表示那个位置是整数，%s——表示那个位置应该是字符串。



```python
#.format()
'{}{}{}'.format(a,b,c)
#当{}里面是空的时候，里面默认索引为0，1，2按format括号里的顺序依次填入。
'{1}{2}{0}'.format(a,b,c)
#当{}里面有索引值时，按前面的索引值将后面的每项依次填入。
'{n1}{n2}{n3}'.format(n1=a,n2=b,n3=c)
#大括号{}里面可以指定对象名称，后面通过赋值的方式给前面的相应的值，后面的对象是无序的。
```







**3、字符的交集**







```python
sStr1 = '12345678'
sStr2 = '456'
sStr1 and sStr2
```



跟set有点像，就可以得到字符级别的交集是什么





**4、翻转字符**



```python
#翻转字符
sStr1 = 'abcdefg'
sStr1 = sStr1[::-1]
```











**5、字符替换**


Python replace() 方法把字符串中的 old（旧字符串） 替换成 new(新字符串)，如果指定第三个参数max，则替换不超过 max 次。


```python
#!/usr/bin/python

str = "this is string example....wow!!! this is really string";
print str.replace("is", "was");
print str.replace("is", "was", 3);
```

以上实例输出结果如下：



```python
thwas was string example....wow!!! thwas was really string
thwas was string example....wow!!! thwas is really string
```











## 6、语句分割split =R= split

这个函数的作用是将字符串根据某个分割符进行分割。



```
>>> a = "I LOVE PYTHON"
>>> a.split(" ")
['I', 'LOVE', 'PYTHON']
```

其中split（“ ”）括号中，代表着依据什么样式来进行分割。





## 7、字符串去掉空格 = R=grep

方法是：
- S.strip() 去掉字符串的左右空格
- S.lstrip() 去掉字符串的左边空格
- S.rstrip() 去掉字符串的右边空格

```
>>> b=" hello "    # 两边有空格
>>> b.strip()
'hello'
```





## 8、字符串大小写







在 Python 中有下面一堆内建函数，用来实现各种类型的大小写转化
- S.upper() #S 中的字母大写
- S.lower() #S 中的字母小写
- S.capitalize() # 首字母大写
- S.isupper() #S 中的字母是否全是大写
- S.islower() #S 中的字母是否全是小写
- S.istitle()

S.title()  #把所有的单词的第一个字母转化为大写

S.istitle()  #判断每个单词的第一个字母是否为大写



## 9、in 包含关系 =R=%in%



类似集合计算，a in b代表a是否包含在b中，返回的是布尔值。







```
>>> "a" in str1
True
>>> "de" in str1
False
>>> "de" in str2
True
```



## 10、字符长度len  =R= length





```
>>> a="hello"
>>> len(a)
5
```





## ——————————————————————————————



**三、ZIP的用法**



## 1、zip字符贴在一起

```
#Zip
#list字符贴在一起
name=('jack','beginman','sony','pcky')
age=(2001,2003,2005,2000)
for a,n in zip(name,age):
  print a,n
```
- 1
- 2
- 3
- 4
- 5
- 6

## 2、list相连

```
#list+list相连
 1 >>> z1=[1,2,3]
 2 >>> z2=[4,5,6]
 3 >>> result=zip(z1,z2)
 4 >>> result
 5 [(1, 4), (2, 5), (3, 6)]
```
- 1
- 2
- 3
- 4
- 5
- 6

## 3、zip将数据变成list

```
#zip(*)解压，将[]变为{}
x=[1,2,3]
y=['a','b','c']
result=zip(x,y)      
zip(*result)    #直接转置
zip(*zip(x,y))  #接触转置

map(list,zip(x,y) ) #变成list
```











————————————————————————————————————————————



# 四、转义符、占用符列表



## 1、转义符列表



在字符串中，有时需要输入一些特殊的符号，但是，某些符号不能直接输出，就需要用转义符。所谓转义，就是不采用符号本来的含义，而采用另外一含义了。下面表格中列出常用的转义符：
|转义字符|描述|
|----|----|
|\|(在行尾时) 续行符|
|\|反斜杠符号|
|\'|单引号|
|\"|双引号|
|\a|响铃|
|\b|退格(Backspace)|
|\e|转义|
|\000|空|
|\n|换行|
|\v|纵向制表符|
|\t|横向制表符|
|\r|回车|
|\f|换页|
|\oyy|八进制数，yy 代表的字符，例如：\o12 代表换行|
|\xyy|十六进制数，yy 代表的字符，例如：\x0a 代表换行|
|\other|其它的字符以普通格式输出|

以上所有转义符，都可以通过交互模式下 print 来测试一下，感受实际上是什么样子的。



## 2、占位符



占位符在自动生成字符内容方面有很好的应用：



```
>>> print "我%s喜欢NLP" % "非常"
我非常喜欢NLP
```





### (1)老式占位符%s

另外，不同的占位符，会表示那个位置应该被不同类型的对象填充。下面列出许多，供参考。不过，不用记忆，常用的只有 `%s` 和 `%d`，或者再加上 `%f`，其它的如果需要了，到这里来查即可。
|占位符|说明|
|----|----|
|%s|字符串(采用 str()的显示)|
|%r|字符串(采用 repr()的显示)|
|%c|单个字符|
|%b|二进制整数|
|%d|十进制整数|
|%i|十进制整数|
|%o|八进制整数|
|%x|十六进制整数|
|%e|指数 (基底写为 e)|
|%E|指数 (基底写为 E)|
|%f|浮点数|
|%F|浮点数，与上相同|
|%g|指数(e) 或浮点数 (根据显示长度)|
|%G|指数(E)或浮点数 (根据显示长度)|

### （2）新式{}

```
>>> print "我{}喜欢NLP" .format("非常")
我非常喜欢NLP
```



 Python 非常提倡的 `string.format()`的格式化方法，其中 `{}` 作为占位符。



这种方法真的是非常好，而且非常简单，只需要将对应的东西，按照顺序在 format 后面的括号中排列好，分别对应占位符 `{}` 即可。我喜欢的方法。



**————————————————————————————————————————————**





# 四、Python 中如何避免中文是乱码



这个问题是一个具有很强操作性的问题。我这里有一个经验总结，分享一下，供参考：

首先，提倡使用 utf-8 编码方案，因为它跨平台不错。

经验一：在开头声明：
`# -*- coding: utf-8 -*-`
有朋友问我-*-有什么作用，那个就是为了好看，爱美之心人皆有，更何况程序员？当然，也可以写成：
`# coding:utf-8`
经验二：遇到字符（节）串，立刻转化为 unicode，不要用 str()，直接使用 unicode()

```
unicode_str = unicode('中文', encoding='utf-8')
print unicode_str.encode('utf-8')
```

经验三：如果对文件操作，打开文件的时候，最好用 codecs.open，替代 open(这个后面会讲到，先放在这里)

```
import codecs
codecs.open('filename', encoding='utf8')
```



大多来源于：极客学院的[《从零开始学 Python》(第二版)](http://wiki.jikexueyuan.com/project/start-learning-python)



# 五、[Python正则表达式:re的match方法](https://mp.weixin.qq.com/s?__biz=MzI0MDcyMzg5NQ==&mid=2247483733&idx=1&sn=930df75b24cd7f8184aea5c4019e48b2&chksm=e917338dde60ba9bc631e88a980120b88b80e488d4c25f8899d3ab657eacc45f641a7248eecc&mpshare=1&scene=1&srcid=0403wXGfPwk3UWkzMpHzjo9d#rd)（来源公众号人人可以学python）





Python 从1.5版本起添加了模块re ，提供 Perl 风格的正则表达式模式

我们经常用的有re.match( ),re.search( )，re.sub( )， 下面我们一个一个来讲一下。

其中注意它们各自的区别

re.match( )


- 
match( )从要匹配的字符串的起始位置开始匹配一个正则表达式。如果起始位置匹配失败，则返回None


re.match(pattern, string, flags=0)

参数：

pattern： 正则表达式

string：要匹配的字符串

flags：标志位，用来控制匹配模式


- 


```
举例：
#!/usr/bin/python
# -*- coding: UTF-8 -*- 
import re
str1 = "Allen is renren python"
print re.match('Bllen', str1)
print re.match('Allen', str1)
print re.match('.*renren.*', str1)
allenwoo@~/renren$ python test.py
```






第一个由于一开始匹配不成功，所以结果是None

第二个在字符串开始就找到了“Allen”所以匹配成功，返回结果

第三个，" . "表示任何字符，“ * ”表示前面一个修饰符有任何个（包括0个）

所以" .* "就是匹配任何个数的任何字符的意思。



```
#!/usr/bin/python
# -*- coding: UTF-8 -*- 
import re
str1 = "Allen is renren python"
r = re.match('Allen', str1)
# span是返回我们匹配到的字符串的开始和结束的下标
print r.span()
# group返回我们匹配到的字符串
print r.group()
print "\n"
r = re.match('.*renren.*', str1)
print r.span()
print r.group()
```


- 
数字匹配可以使用\w或者[0-9]


比如要匹配字符串"Jack age:18，sex:m"中的数字

```
#!/usr/bin/python
# -*- coding: UTF-8 -*- 
import re
str1 = "Jack age:18，sex:m"
r = re.match('.*\w+.*', str1)
if r:
    print r.group()
print "\n"
 
r = re.match('.*[0-9]+.*', str1)
if r:
    print r.group()
 
print("分组匹配输出")
r = re.match(r'(.*):([0-9]+).*', str1)
if r:
    print r.group()
    print r.group(1)
    print r.group(2)
```

还有：

```python
look_reply = '<span class="ch_r tzrht">查看：4489 | 回复：35</span>'
re.findall("\d+",str(look_reply))
>>>['4489', '35']
```






你会发现我们在正则表达式中使用()就能分组匹配，然后可以使用group返回每个括号中匹配的字符串。

注意: 在`re.match(r``'(.*):([0-9]+).*'``, str1)的表达式前面我们使用了一个 r， r是防止字符串转意，因为我们使用了（）,我们不希望它被当作要匹配的字符。`
- 
`附：那这就先讲一下转意：`


```
#!/usr/bin/python
# -*- coding: UTF-8 -*- 
# 大家都知道\n是换行符，
# 而\t 是tab空格，相当于我们键盘的tab键
print("We\tare\trenrenpython")
 
# 加上r防止转意后
print(r"We\tare\trenrenpython")
```



\是一个特殊符符号，用来转意一些字符，如\n换行符

使用了 r 后"\t"就是字符串"\t", 而不是tab


- 
然后又有新的问题来了，想"\w"," . ", " * "都被用作了正则表达式的修饰符，如果我们需要把它们当原本的字符匹配怎么办呢？使用" \" 






```
#!/usr/bin/python
# -*- coding: UTF-8 -*- 
import re
str1 = "python . renren"
str2 = "python : renren"
# 由于"." 是代表任何字符，所以下面表达式str1,str2都能匹配到
r = re.match('.* . .*', str1)
if r:
    print r.group()
else:
    print "未匹配" 
 
r = re.match('.* . .*', str2)
if r:
    print r.group()
else:
    print "未匹配" 
print "\n"
 
# 如果我们要匹配的是"."这个字符呢? 
r = re.match('.* \. .*', str1)
if r:
    print r.group()
else:
    print "未匹配" 
 
r = re.match('.* \. .*', str2)
if r:
    print r.group()
else:
    print "未匹配"
```



**跟match比较像的finditer：**

```
sentence = '大幅度发的史莱克,大多亏了三等奖克'
list(re.finditer('大(.*?)克',sentence))

>>>[<_sre.SRE_Match object; span=(0, 8), match='大幅度发的史莱克'>,
 <_sre.SRE_Match object; span=(9, 17), match='大多亏了三等奖克'>]
```







### 正则做替换：

```
import re
url = 'https://113.215.20.136:9011/113.215.6.77/c3pr90ntcya0/youku/6981496DC9913B8321BFE4A4E73/0300010E0C51F10D86F80703BAF2B1ADC67C80-E0F6-4FF8-B570-7DC5603F9F40.flv'
pattern = re.compile(r'(?<![\.\d])(?:\d{1,3}\.){3}\d{1,3}(?![\.\d])')
print pattern.findall(url)
out = re.sub(pattern, '127.0.0.1', url)
print out
```





## 延伸一：Nan

```python
def isnotNaN(num):
    return num == num
```




**延伸二：相关实践**



```python
一些实践案例：
# 通过正则，提取字符
<a target="img-window" class="col-sm-12 col-md-6 img-div" href="../../../img/56b311675fe3329a703cf9de.html">
查询:re.findall(r'\"(.*html)\"',str(  content  ))
结果:../../../img/56b311675fe3329a703cf9de.html

# 确定句子中是否存在某字符：
string.find('*')!= -1  # 结果为-1时,代表没有这个字符

# 通过正则，确定句子中是否存在某字符：
确定句子中是否存在中文：
	re.match('[\u4e00-\u9fa5]', string)
	(0,3)，说明存在中文
	''，说明不存在。
```

**提取括号中的元素：**

```python
import re

str = u'陈奕迅演唱（十年)）、(浮夸)、(不要说话)'
re.findall('\（(.*?)\）', str)
```



**提取.jpg文件：**

```python
http:.+\.jpg
```

**提取数字：**

```
电话号码：\d{3}-\d{8}|\d{4}-\d{7}

QQ号：[1-9][0-9]{4,}

中国邮政编码：[1-9]\d{5}(?!\d)
身份证：\d{15}|\d{18}

ip地址：\d+\.\d+\.\d+\.\d+

[1-9]\d*　     正整数
-[1-9]\d* 　 负整数
-?[1-9]\d*　整数
[1-9]\d*|0　 非负整数
-[1-9]\d*|0　　 非正整数
[1-9]\d*\.\d*|0\.\d*[1-9]\d*$　　 正浮点数
-([1-9]\d*\.\d*|0\.\d*[1-9]\d*)$　 负浮点数
-?([1-9]\d*\.\d*|0\.\d*[1-9]\d*|0?\.0+|0)$　 浮点数
```

```
price='25.34-34.55'
 
test=re.compile(r'[1-9]\d*\.\d*|0\.\d*[1-9]|[1-9]\d*').findall(price)[0]
```

匹配价格，并输出平均价格。

```
#五牛圖紙本20.8x139.8抽取
pattern = re.compile(r'([1-9]\d*)x([1-9]\d*)')
pattern.findall(text)
>>>  [('20.8', '139.8')]
```

抽取正整数，()*()这样的格式。



**删除字符串中的数字：**

```
from string import digits
 
s = 'abc123def456ghi789zero0'
remove_digits = str.maketrans('', '', digits)
res = s.translate(remove_digits)
# 'abcdefghizero'
```



**根据规则进行分割：**

```
p = re.compile(r'[\s\,\;]+')
print p.split('a,b;; c   d')
```

**提取限定长度的字符**

```
sentence = '大幅度发的史莱克,大多亏了等奖克，大克,大多亏了奖克'
re.findall('大(.{0,5})克',sentence)
>>>['多亏了等奖', '', '多亏了奖']


# 其中一种情况是在正则中使用Format
# 注意是使用{{}}来区分
re.findall('{}(.{{0,5}}){}'.format('大','克'),sentence)
>>> ['多亏了等奖', '', '多亏了奖']
```

等于再说，大-克，之间只能存在0<x<5个字（不包括5）



