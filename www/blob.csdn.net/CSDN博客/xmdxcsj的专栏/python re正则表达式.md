# python re正则表达式 - xmdxcsj的专栏 - CSDN博客





2016年04月02日 21:42:39[xmucas](https://me.csdn.net/xmdxcsj)阅读数：633








**语法和函数参考[官方文档](https://docs.python.org/2/library/re.html)和[使用方法](https://docs.python.org/2/howto/regex.html#regex-howto)**

## 常用语法

参考[博客](http://www.cnblogs.com/huxi/archive/2010/07/04/1771073.html)的表格整理

## 常用函数
- compile 

将正则表达式编译成对象，可以被其他正则匹配函数多次调用，无需再重新编译正则表达式。

```python
prog = re.compile(pattern)
result = prog.match(string)
```

等同于
`result = re.match(pattern, string)`- match(): Determine if the RE matches at the beginning of the string.

```python
>>> p = re.compile('[a-z]+')
>>> m = p.match('tempo')
>>> m  
<_sre.SRE_Match object at 0x...>
>>> m.group()
'tempo'
>>> m.start(), m.end()
(0, 5)
>>> m.span()
(0, 5)
```
- search(): Scan through a string, looking for any location where this RE matches.

```python
#match从字符串的起始位置开始匹配
>>> print p.match('::: message')
None
#search从字符串的任意位置开始匹配
>>> m = p.search('::: message'); print m  
<_sre.SRE_Match object at 0x...>
>>> m.group()
'message'
>>> m.span()
(4, 11)
```
- findall(): Find all substrings where the RE matches, and returns them as a list.

```python
>>> p = re.compile('\d+')
>>> p.findall('12 drummers drumming, 11 pipers piping, 10 lords a-leaping')
['12', '11', '10']
```
- finditer(): Find all substrings where the RE matches, and returns them as an iterator.

```python
>>> iterator = p.finditer('12 drummers drumming, 11 ... 10 ...')
>>> iterator  #迭代器
<callable-iterator object at 0x...>
>>> for match in iterator:
...     print match.span()
...
(0, 2)
(22, 24)
(29, 31)
```
- split(): Split the string into a list, splitting it wherever the RE matches

```python
>>> p = re.compile(r'\W+')
>>> p2 = re.compile(r'(\W+)') #()可以将分隔符作为结果返回
>>> p.split('This... is a test.')
['This', 'is', 'a', 'test', '']
>>> p2.split('This... is a test.')
['This', '... ', 'is', ' ', 'a', ' ', 'test', '.', '']
```
- sub(): Find all substrings where the RE matches, and replace them with a different string

```python
>>> p = re.compile( '(blue|white|red)')
>>> p.sub( 'colour', 'blue socks and red shoes')
'colour socks and colour shoes'
>>> p.sub( 'colour', 'blue socks and red shoes', count=1)
'colour socks and red shoes
#空匹配符，只有不邻接匹配的地方都会添加
>>> p = re.compile('x*')
>>> p.sub('-', 'abxd')
'-a-b-d-'
#使用\num \g<num> \g<number> 来代表一部分字符串
>>> p = re.compile('section{ (?P<name> [^}]* ) }', re.VERBOSE)
>>> p.sub(r'subsection{\1}','section{First}')
'subsection{First}'
>>> p.sub(r'subsection{\g<1>}','section{First}')
'subsection{First}'
>>> p.sub(r'subsection{\g<name>}','section{First}')
'subsection{First}'
#使用函数作为替换字符
>>> def hexrepl(match):
...     "Return the hex string for a decimal number"
...     value = int(match.group())
...     return hex(value)
...
>>> p = re.compile(r'\d+')
>>> p.sub(hexrepl, 'Call 65490 for printing, 49152 for user code.')
'Call 0xffd2 for printing, 0xc000 for user code.'
```
- subn(): Does the same thing as sub(), but returns the new string and the number of replacements

```python
>>> p = re.compile( '(blue|white|red)')
>>> p.subn( 'colour', 'blue socks and red shoes')
('colour socks and colour shoes', 2)
>>> p.subn( 'colour', 'no colours at all')
('no colours at all', 0)
```

## 使用须知
- **能用string的函数解决的问题不要使用正则**

因为string的函数是利用优化过的c循环实现，而正则的实现相对低效。 

比如替换字符串的字符，可以使用replace函数，不要使用re.sub- **查找字符串是否出现使用search**

使用re.search(r’hello’, str)，避免使用re.match(r’.*hello.*’, str)，因为增加的.*会将match函数的优化失效。
- **贪婪模式和非贪婪模式**

正则匹配默认会使用贪婪模式，尽可能多的匹配字符。可以使用?将贪婪模式修改为非贪婪模式。*？、+?、??、{m,n}?将会尽量少的进行匹配

```python
>>> s = '<html><head><title>Title</title>'
#贪婪模式
>>> print re.match('<.*>', s).group()
<html><head><title>Title</title>
#非贪婪模式
>>> print re.match('<.*?>', s).group()
<html>
```
- **使用re.VERBOSE**

在compile函数使用该标记，将会忽略掉whitespace，可以对写出的正则表达式进行注释

```python
pat = re.compile(r"""
 \s*                 # Skip leading whitespace
 (?P<header>[^:]+)   # Header name
 \s* :               # Whitespace, and a colon
 (?P<value>.*?)      # The header's value -- *? used to
                     # lose the following trailing whitespace
 \s*$                # Trailing whitespace to end-of-line
""", re.VERBOSE)
#等价于
pat = re.compile(r"\s*(?P<header>[^:]+)\s*:(?P<value>.*?)\s*$")
```




