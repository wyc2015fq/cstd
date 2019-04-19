# python challenge第3题 - Arthur的随笔 - CSDN博客
2012年02月07日 23:38:00[largetalk](https://me.csdn.net/largetalk)阅读数：2687
这道题目是 One small letter, surrounded by EXACTLY three big bodyguards on each of its sides.
直觉就是用正则来做
一开始使用得正则是
```python
reg = re.compile('[A-Z]{3}([a-z])[A-Z]{3}')
```
发现匹配出来得东西特别多， 又研究题目，觉得EXACTLY是不是指包括得字符得是一样得比如 AAA，BBB，CCC之类得，
又去查python正则分组写出来得第二个正则式
```python
reg = re.compile('((?P<hh>[A-Z])(?P=hh){2})([a-z])((?P<tt>[A-Z])(?P=tt){2})')
```
上面这句分组引用只能用别名，不能用编号，比如这样
```python
reg = re.compile('(([A-Z])(\1){2})')
```
是匹配不到AAA得，不知道为什么
用第二个正则发现什么也没匹配到，然后回过头来看第一个正则才发现应该是有些条件没有考虑到，EXACTLY是指大写字母个数一定精确为3， so 正确得正则是
```python
reg = re.compile('[^A-Z][A-Z]{3}([a-z])[A-Z]{3}[^A-Z]')
```
整个代码是
```python
import urllib2
f = urllib2.urlopen('http://www.pythonchallenge.com/pc/def/equality.html')
data = f.read()
import re
reg = re.compile('[^A-Z][A-Z]{3}([a-z])[A-Z]{3}[^A-Z]')
print ''.join(reg.findall(data))
```
ps. 这个题虽然花了一些时间， 但还是学了点东西，现在想想，以前做ETL用到得正则真是太简单了
