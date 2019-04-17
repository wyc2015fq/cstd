# python RE模块的使用 - lincappu - 博客园







# [python RE模块的使用](https://www.cnblogs.com/lincappu/p/8242460.html)





**摘要：**

      re模块包括操作正则表达式的函数，一些工作中都需要用到，现在说明下使用方法。

**使用说明：**

**一，re模块下的函数：**
|函数|描述|
|----|----|
|compile(pattern)|创建模式对象|
|search(pattern,string)|在字符串中寻找模式|
|match(pattern,string)|在字符串开始处匹配模式|
|split(pattern,string)|根据模式分割字符串|
|findall(pattern,string)|列表形式返回匹配项|
|sub(pat,repl,string)|pat匹配想用repl替换|
|escape(string)|特殊字符转义|















**1，compile：**

```
>>> import re
>>> pat=**re.compile**('A')
>>> m=**pat.search**('CBA')
>>> print m
<_sre.SRE_Match object at 0x9d690c8>   #匹配到了，返回MatchObject（True）

>>> m=pat.search('CBD')
>>> print m
None                                   #没有匹配到，返回None（False）

#上面的等价于

>>> **re.search**('A','CBA')
<_sre.SRE_Match object at 0xb72cd170>

#推荐都用第一种方法
```

说明：将正则表达式转换为模式对象，实现更有效率的匹配，因为其他的函数会在内部进行转换。

**2，search：**

```
>>> m = re.search('asd','ASDasd')
>>> print m
<_sre.SRE_Match object at 0xb72cd6e8>  #匹配到了，返回MatchObject（True）
>>> m = re.search('asd','ASDASD')
>>> print m
None                                   #没有匹配到，返回None（False）
```

说明：在给定的字符串中寻找第一个匹配给正则表达式的子字符串，有多个也只返回第一个出现的。

**3，match：**

```
>>> m = re.match('a','Aasd')
>>> print m
None                                  #没有匹配到，返回None（False

>>> m = re.match('a','aASD')
>>> print m
<_sre.SRE_Match object at 0xb72cd6e8> #匹配到了，返回MatchObject（True）

可以用第一个方法：**compile**

>>> pat=re.compile('a')
>>> printpat.match('Aasd')
None

>>> printpat.match('aASD')
<_sre.SRE_Match object at 0xb72cd6e8>
```

说明：在给定的字符串的开头匹配正则表达式。

上面的函数返回都可以在if条件语句中进行判断：

```
>>> if pat.search('asd'):
...     print 'OK'
... 
OK        #找到返回
>>> if re.search('a','ASD'):
...     print "OK"
...       #没有找到
```

**4，split：**

```
>>> re.split(',','a,s,d,asd')
['a', 's', 'd', 'asd']          #返回列表
>>> pat = re.compile(',')
>>> pat.split('a,s,d,asd')
['a', 's', 'd', 'asd']          #返回列表

>>> re.split('[, ]+','a ,  s  ,d     ,,,,,asd')   #正则匹配：[, ]+，后面说明
['a', 's', 'd', 'asd']
>>> re.split('[, ]+','a ,  s  ,d     ,,,,,asd',maxsplit=2) # maxsplit 最多分割次数
['a', 's', 'd     ,,,,,asd']
>>> pat = re.compile('[, ]+')                     #正则匹配：[, ]+，后面说明
>>> pat.split('a ,  s  ,d     ,,,,,asd',maxsplit=2)        # maxsplit 最多分割次数
['a', 's', 'd     ,,,,,asd']
```

说明：根据模式的匹配项来分割字符串，类似字符串的split 的方法，但是字符串的split只能用固定长度的分割符，而re.split允许用任意长度和个数的分割符。

**5，findall：**

```
>>> re.findall('a','ASDaDFGAa') 
['a', 'a']                           #列表形式返回匹配到的字符串
>>> pat = re.compile('a')
>>> pat.findall('ASDaDFGAa')
['a', 'a']                           #列表形式返回匹配到的字符串

>>> pat = re.compile('[A-Z]+')       #正则匹配：'[A-Z]+' 后面有说明
>>> pat.findall('ASDcDFGAa')
['ASD', 'DFGA']                      #找到匹配到的字符串
>>> pat = re.compile('[A-Z]')
>>> pat.findall('ASDcDFGAa')         #正则匹配：'[A-Z]+' 后面有说明
['A', 'S', 'D', 'D', 'F', 'G', 'A']  #找到匹配到的字符串

>>> pat = re.compile('[A-Za-z]')     #正则匹配：'[A-Za-z]+' 匹配所有单词，后面有说明
>>> pat.findall('ASDcDFGAa')
['A', 'S', 'D', 'c', 'D', 'F', 'G', 'A', 'a']
```

说明：列表形式返回给定模式的匹配项。返回所有匹配的字符串。

**6，sub：**

```
>>> re.sub('a','A','abcasd')   #找到a用A替换，后面见和**group的配合使用**
'AbcAsd'

>>> pat = re.compile('a')
>>> pat.sub('A','abcasd')
'AbcAsd'

**#通过组进行更新替换：**
>>> pat=re.compile(r'www\.(.*)\..{3}') #正则表达式
>>> pat.match('www.dxy.com').group(1)
'dxy'
>>> pat.sub(r'\1','hello,www.dxy.com') #通过正则匹配找到符合规则的”www.dxy.com“ ，取得组1字符串 去替换 整个匹配得到字符串。dxy -> www.dxy.com
'hello,dxy'

>>> pat=re.compile(r'(\w+) (\w+)') #正则表达式
>>> s='hello world ! hello hz !'
>>> pat.findall('hello world ! hello hz !')
[('hello', 'world'), ('hello', 'hz')]
>>> pat.sub(r'\2 \1',s) #通过正则得到组1(hello)，组2(world)，再通过sub去替换。即组1替换组2，组2替换组1，**调换位置**。 
'world hello!hz hello!'
```

说明：使用给定的内容替换掉找到的字符串。a->A

```
>>> dir(m)
['__class__', '__copy__', '__deepcopy__', '__delattr__', '__doc__', '__format__', '__getattribute__', '__hash__', '__init__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', 'end', 'endpos', 'expand', 'group', 'groupdict', 'groups', 'lastgroup', 'lastindex', 'pos', 're', 'regs', 'span', 'start', 'string']
```

**7，escape：**

```
>>> re.escape('www.dxy.cn')
'www\\.dxy\\.cn'                   #转义
```

说明：对字符串里面的特殊字符串进行转义。

上面的函数中，只有match、search有group方法，其他的函数没有。



**二，函数的方法：**

```
>>> dir(m)
[
'__class__', '__copy__', '__deepcopy__', '__delattr__', '__doc__', '__format__', '__getattribute__', '__hash__', '__init__', '__new__', 
'__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', 
'end', 'endpos', 'expand', 'group', 'groupdict', 'groups', 'lastgroup', 'lastindex', 'pos', 're', 'regs', 'span', 'start', 'string'
]
```

这里介绍几个：
|方法|描述|
|----|----|
|group|获取子模式(组)的匹配项|
|start|给定组匹配项的开始位置|
|end|给定组匹配项的结束位置|
|span|给定组匹配项的开始结束位置|











```
>>> pat = re.compile(r'www\.(.*)\.(.*)')       #用()表示一个组，2个组
>>> m = pat.**match**('www.dxy.com')
>>> m.group()                                  #默认为0，表示匹配整个字符串   
'www.dxy.com'
>>> m.group(1)                                 #返回给定组1匹配的子字符串
'dxy'
>>> m.group(2)
'com'
>>> m.start(2)                                 #组2开始的索引
8
>>> m.end(2)                                   #组2结束的索引
11
>>> m.span(2)                                  #组2开始、结束的索引
(8, 11)
>>> m1 = pat.**search**('www.dxy.com')
>>> m1.group()
'www.dxy.com'
>>> m1.group(1)
'dxy'
>>> m1.group(2)
'com'
>>> m1.start(2)
8
```


**三，正则表达式:![](https://images0.cnblogs.com/blog/163084/201306/30012544-bdf943c0d7f24d798a8964162de05f4b.png)**



**1，元字符：**
|字符|描述|
|----|----|
|.|除换行符外的任意字符|
|\|转义字符|
|[...]|字符集合|
|\d|数字:[0-9]|
|\D|非数字[^0-9]|
|\s|空白字符[\t\r\n\f、空格]|
|\S|非空白字符[^\s]|
|\w|单词字符[A-Za-z0-9]|
|\W|非单词字符[^\w]|



















**使用：**

```
# "." ：通配符
>>> pat=re.compile('.')
>>> pat.match('abc')
<_sre.SRE_Match object at 0xb72b6170>
>>> pat.match('abc').group()
'a'                                #匹配到了首个字符
>>> pat.search('abc').group() 
'a'
>>> pat.match('\n').group()        #换行符匹配出错
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
AttributeError: 'NoneType' object has no attribute 'group'

# "\" : 转义符
>>> pat=re.compile('\.')
>>> pat.search('abc.efg').group()  #匹配到.
'.'
>>> pat.findall('abc.efg')         #不用group,返回列表
['.']

# "[...]" : 集合，匹配里面的任意一个元素
>>> pat=re.compile('[abc]')
>>> pat.match('axbycz').group()
'a'
>>> pat.search('axbycz').group()
'a'
>>> pat.findall('axbycz')
['a', 'b', 'c']

# "\d" : 数字
>>> pat=re.compile('\d')           
>>> pat.search('ax1by2cz3').group()   #匹配到第一个数字:1，返回
'1'
>>> pat.match('ax1by2cz3').group()    #匹配不到（首个不是）返回None，报错，match匹配字符串头
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
AttributeError: 'NoneType' object has no attribute 'group'
>>> pat.findall('ax1by2cz3')          #匹配所有的数字，列表返回
['1', '2', '3']

# "\D" : 非数字
>>> pat=re.compile('\D')
>>> pat.match('ax1by2cz3').group()
'a'
>>> pat.search('ax1by2cz3').group()
'a'
>>> pat.findall('ax1by2cz3')
['a', 'x', 'b', 'y', 'c', 'z']

# "\s" :空白字符 : \t、\r、\n、空格
>>> pat=re.compile('\s')
>>> pat.findall('\rax1 \nby2 \tcz3')
['\r', ' ', '\n', ' ', '\t']
>>> pat.search('\rax1 \nby2 \tcz3').group()
'\r'
>>> pat.match('\rax1 \nby2 \tcz3').group()
'\r'

# "\S" :非空白字符
>>> pat=re.compile('\S')
>>> pat.search('\rax1 \nby2 \tcz3').group()
'a'
>>> pat.findall('\rax1 \nby2 \tcz3')
['a', 'x', '1', 'b', 'y', '2', 'c', 'z', '3']

# "\w" :单词字符,数字和字母:[A-Za-z0-9]
>>> pat=re.compile('\w')
>>> pat.search('1a2b3c').group()
'1'
>>> pat.findall('1a2b3c')
['1', 'a', '2', 'b', '3', 'c']
>>> pat.match('1a2b3c').group()
'1'

# "\W":非单词字符,除数字和字母外
>>> pat=re.compile('\W')
>>> pat.findall('1a2我b3c')
['\xe6', '\x88', '\x91']
>>> pat.search('1a2我b3c').group()
'\xe6'
```

**2，数量词:会出现贪婪模式**
|数量词|描述|
|----|----|
|*|匹配前一个字符0次或则多次|
|+|匹配前一个字符1次或则多次|
|？|匹配前一个字符0次或则1次|
|{m}|匹配前一个字符m次|
|{m,n}|匹配前一个字符m次到n次|
|数量词?|非贪婪模式|













**使用：**

```
# "*" ：0次或则多次
>>> pat = re.compile('[abc]*')
>>> pat.match('abcabcdefabc').group() 
'abcabc'                              #2次
>>> pat.search('abcabcdefabc').group()
'abcabc'                              #2次
>>> pat.findall('abcabcdefabc')
['abcabc', '', '', '', 'abc', '']     #2次和1次,因为有0次，所以匹配了''

# "+" ：1次或则多次
>>> pat = re.compile('[abc]+')
>>> pat.match('abcdefabcabc').group()
'abc'
>>> pat.search('abcdefabcabc').group()
'abc'
>>> pat.findall('abcdefabcabc')
['abc', 'abcabc']

# "?" ：0次或则1次，match,search 不会出现none，会出现''
>>> pat = re.compile('[abc]?')
>>> pat.match('defabc').group()     #0次
''
>>> pat.match('abcdefabc').group()
'a'
>>> pat.search('defabc').group()    #0次
''
>>> pat.findall('defabc')           #0次和1次
['', '', '', 'a', 'b', 'c', '']

# "{m}" ：匹配字符串出现m次
>>> pat = re.compile('[op]{2}')     #o或则p出现2次
>>> pat.search('abcooapp').group()  #匹配第一次出现的字符串,o比p先出现
'oo'
>>> pat.findall('abcooapp')         #匹配出现的所有字符串，列表形式返回
['oo', 'pp']

# "{m,n}" ：匹配字符串出现m到n次
>>> pat = re.compile('[op]{2,4}')     #o或则p出现2到4次
>>> pat.match('pppabcooapp').group()  #匹配开头
'ppp'
>>> pat.search('pppabcooapp').group() #匹配第一次出现
'ppp'
>>> pat.findall('pppabcooapp')        #匹配所有
['ppp', 'oo', 'pp']


# "数量词?" ：非贪婪模式：只匹配最少的（尽可能少）；默认贪婪模式：匹配最多的（尽可能多）
>>> pat = re.compile('[abc]+')         #贪婪模式
>>> pat.match('abcdefabcabc').group()  #匹配尽可能多的：abc
'abc'
>>> pat.match('bbabcdefabcabc').group()
'bbabc'
>>> pat.search('dbbabcdefabcabc').group()
'bbabc'
>>> pat.findall('abcdefabcabc')
['abc', 'abcabc']

>>> pat = re.compile('[abc]+?')        #非贪婪模式：+?
>>> pat.match('abcdefabcabc').group()  #匹配尽可能少的：a、b、c
'a'
>>> pat.search('dbbabcdefabcabc').group()
'b'
>>> pat.findall('abcdefabcabc')
['a', 'b', 'c', 'a', 'b', 'c', 'a', 'b', 'c']
```

**3，边界**
|边界词|描述|
|----|----|
|^|匹配字符串开头和行首|
|$|匹配字符串结尾和行尾|
|\A|匹配字符串开头|
|\Z|匹配字符串结尾|
|\b|匹配\w 和 \W 之间|













**使用：**

```
# "^" ：匹配字符串开头或则行头
>>> pat = re.compile('^[abc]')     #开头是abc
>>> pat.search('defabc').group()     
>>> pat.match('defabc').group()    #均找不到
>>> pat.findall('defabc')
[]
>>> pat.search('adefabc').group()
'a'
>>> pat.match('adefabc').group()   #开头是a、b、c中的任意一个
'a'
>>> pat.findall('adefabc')
['a']

>>> pat = re.compile('^[abc]+')    #开头是a、b、c中的任意一个的一次或则多次，贪婪：匹配多个
>>> pat.findall('cbadefab')
['cba']
>>> pat = re.compile(r'^[abc]+?')  #开头是a、b、c中的任意一个的一次或则多次，非贪婪：匹配一个
>>> pat.findall('cbadefab')
['c']

# "$" ：匹配字符串结尾或则行尾
>>> pat = re.compile('[abc]$')
>>> pat.match('adefAbc').group()   #match匹配的是字符串开头，所以查找$的时，总是返回None
>>> pat.search('adefAbc').group()  #结尾是a、b、c中的任意一个
'c'
>>> pat.findall('adefAbc')         
['c']

>>> pat = re.compile('[abc]+$')
>>> pat.search('adefAbc').group()  #结尾是a、b、c中的任意一个的一次或则多次，贪婪：匹配多个
'bc'
>>> pat.findall('adefAbc')
['bc']

# "\A"：匹配字符串开头
>>> pat = re.compile('\A[abc]+')
>>> pat.findall('cbadefab')
['cba']
>>> pat.search('cbadefab').group()
'cba'

# "\Z"：匹配字符串结尾
>>> pat = re.compile('[abc]+\Z')
>>> pat.search('cbadefab').group()
'ab'
>>> pat.findall('cbadefab')
['ab']
```

**4，分组：**这里有个例子：[http://www.cnblogs.com/zhoujinyi/archive/2012/10/29/2745004.html](http://www.cnblogs.com/zhoujinyi/archive/2012/10/29/2745004.html)
|分组方法|描述|
|----|----|
|||左右表达式任意匹配一个，先匹配左边一旦成功则跳过匹配右边，如果|没有包含在()中,匹配整个正则表达式|
|(...)|分组匹配,从左到右,每遇到一个 ( 编号+1，分组后面可加数量词|
|(?P<name>...)|除了分组序号外,指定一个 name的别名|
|\<number>|引用编号为<number>的分组匹配到的字符串|
|(?P=name)|引用别名为<name>的分组匹配到的串|















**使用：**

```
# "(...)" ：在模式里面用()来表示分组（匿名分组）,适用于提取目标字符串中的某一些部位。
>>> pat=re.compile(r'(a)\w(c)')  #\w:单词字符[A-Za-z0-9]
>>> pat.match('abcdef').group()
'abc'
>>> pat=re.compile('(a)b(c)')    #分2组，匿名分组
>>> pat.match('abcdef').group()  #默认返回匹配的字符串
'abc'
>>> pat.match('abcdef').group(1) #取分组1，适用于search
'a'
>>> pat.match('abcdef').group(2) #取分组2，适用于search
'c'
>>> pat.match('abcdef').groups() #取所有分组，元组形式返回
('a', 'c')
>>> pat=re.compile(r'www\.(.*)\..{3}')
>>> pat.match('www.dxy.com').group(1)
'dxy'

# "(?P<name>...)" ：在模式里面用()来表示分组（命名分组）,适用于提取目标字符串中的某一些部位。
>>> pat=re.compile(r'(?P<K>a)\w(c)')    #分2组：命名分组+匿名分组
>>> pat.search('abcdef').groups()       #取所有分组，元组形式返回
('a', 'c')
>>> pat.search('abcdef').group(1)       #取分组1，适用于match
'a'
>>> pat.search('abcdef').group(2)       #取分组2，适用于match
'c'
>>> pat.search('abcdef').group()        #默认返回匹配的字符串
'abc'
>>> pat.search('abcdef').groupdict()    #命名分组可以返回一个字典【专有】，匿名分组也没有
{'K': 'a'}

# "(?P=name)" ：引用命名分组(别名)匹配：
>>> pat=re.compile(r'(?P<K>a)\w(c)(?P=K)')    #(?P=K)引用分组1的值，就是a
>>> pat.search('abcdef').group()              #匹配不到，因为完整'a\wca',模式的第4位是a
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
AttributeError: 'NoneType' object has no attribute 'group'
>>> pat.search('abcadef').group()             #匹配到，模式的第4位和组1一样,值是c
'abca'
>>> pat.search('abcadef').groups()
('a', 'c')
>>> pat.search('abcadef').group(1)
'a'
>>> pat.search('abcadef').group(2)
'c

# "\<number>" ：引用分组编号匹配：
>>> pat=re.compile(r'(?P<K>a)\w(c)(?P=K)\2')  #\2引用分组2的值，就是c
>>> pat.findall('Aabcadef')                   #匹配不到，因为完整'a\wcac',模式的第5位是c
[]
>>> pat.findall('Aabcacdef')                  #匹配到，模式的第5位和组2一样,值是c
[('a', 'c')]
>>> pat.search('Aabcacdef').groups()
('a', 'c')
>>> pat.search('Aabcacdef').group()
'abcac'
>>> pat.search('Aabcacdef').group(1)
'a'
>>> pat.search('Aabcacdef').group(2)
'c'
```

**5，特殊构造**
|方法|描述|
|----|----|
|(?:...)|(...)不分组版本,用于使用 | 或者后接数量词|
|(?iLmsux)|iLmsux的每个字符代表一个匹配模式,只能用在正则表达式的开头,可选多个|
|(?#...)|#号后的内容将作为注释|
|(?=...)|之后的字符串内容需要匹配表达式才能成功匹配|
|(?!...)|之后的字符串不匹配表达式才能成功|
|(?<=...)|之前的字符串需要匹配表达式才能成功|
|(?<!...)|之前的字符串需要不匹配表达式才能成功|
|(?(id/name) yes |no)|如果编号为id/名字为name的组匹配到字符串,则需要匹配yes,否则匹配no,no可以省略|

















**使用：**

```
# "(?:...)" ：()里面有?:表示该()不是分组
>>> pat=re.compile(r'a(?:bc)')
>>> pat.findall('abc')
['abc']
>>> pat.match('abc').groups()      #显示不出分组
()

# "(?=...)"：匹配...表达式，返回。对后进行匹配，总是对后面进行匹配
>>> pat=re.compile(r'\w(?=\d)')    #匹配表达式\d，返回数字的前一位，\w：单词字符[A-Za-z0-9]
>>> pat.findall('abc1 def1 xyz1')
['c', 'f', 'z']
>>> pat.findall('zhoujy20130628hangzhou')  #匹配数字的前一位，列表返回
['y', '2', '0', '1', '3', '0', '6', '2']

>>> pat=re.compile(r'\w+(?=\d)')
>>> pat.findall('abc1,def1,xyz1')          #匹配最末数字的前字符串，列表返回
['abc', 'def', 'xyz']
>>> pat.findall('abc21,def31,xyz41')
['abc2', 'def3', 'xyz4']
>>> pat.findall('zhoujy20130628hangzhou')
['zhoujy2013062']

>>> pat=re.compile(r'[A-Za-z]+(?=\d)')       #[A-Za-z],匹配字母,可以用其他的正则方法
>>> pat.findall('zhoujy20130628hangzhou123') #匹配后面带有数字的字符串，列表返回
['zhoujy', 'hangzhou']
>>> pat.findall('abc21,def31,xyz41')
['abc', 'def', 'xyz']


# "(?!...)" 不匹配...表达式，返回。对后进行匹配
>>> pat=re.compile(r'[A-Za-z]+(?!\d)')       #[A-Za-z],匹配字母,可以用其他的正则方法
>>> pat.findall('zhoujy20130628hangzhou123,12,binjiang310')  #匹配后面不是数字的字符串，列表返回
['zhouj', 'hangzho', 'binjian']
>>> pat.findall('abc21,def31,xyz41')
['ab', 'de', 'xy']

# "(?<=...)"：匹配...表达式，返回。对前进行匹配,总是对前面进行匹配
>>> pat=re.compile(r'(?<=\d)[A-Za-z]+')      #匹配前面是数字的字母
>>> pat.findall('abc21,def31,xyz41')
[]
>>> pat.findall('1abc21,2def31,3xyz41')
['abc', 'def', 'xyz']
>>> pat.findall('zhoujy20130628hangzhou123,12,binjiang310')
['hangzhou']

# "(?<!...)"：不匹配...表达式，返回。对前进行匹配,总是对前面进行匹配
>>> pat=re.compile(r'(?<!\d)[A-Za-z]+')      #匹配前面不是数字的字母
>>> pat.findall('abc21,def31,xyz41')
['abc', 'def', 'xyz']
>>> pat.findall('zhoujy20130628hangzhou123,12,binjiang310')
['zhoujy', 'angzhou', 'binjiang']

#"(?(id/name) yes |no)": 组是否匹配，匹配返回
>>> pat=re.compile(r'a(\d)?bc(?(1)\d)')   #no省略了，完整的是a\dbc\d ==> a2bc3,总共5位，第2位是可有可无的数字，第5为是数字
>>> pat.findall('abc9')                   #返回组1，但第2位（组1）没有，即返回了''
['']
>>> pat.findall('a8bc9')                  #完整的模式，返回组1
['8']
>>> pat.match('a8bc9').group()
'a8bc9'
>>> pat.match('a8bc9').group(1)
'8'
>>> pat.findall('a8bc')                   #第5位不存在，则没有匹配到
[]

#"(?iLmsux)":这里就介绍下i参数：大小写区分匹配
>>> pat=re.compile(r'abc')
>>> pat.findall('abc')
['abc']
>>> pat.findall('ABC')
[]
>>> pat=re.compile(r'(?i)abc')            #(?i) 不区分大小写
>>> pat.findall('ABC')
['ABC']
>>> pat.findall('abc')
['abc']
>>> pat.findall('aBc')
['aBc']
>>> pat.findall('aBC')
['aBC']

>>> pat=re.compile(r'abc',re.I)           #re.I 作为参数使用，推荐
>>> pat.findall('aBC')
['aBC']
>>> pat.findall('abc')
['abc']
>>> pat.findall('ABC')
['ABC']
```












