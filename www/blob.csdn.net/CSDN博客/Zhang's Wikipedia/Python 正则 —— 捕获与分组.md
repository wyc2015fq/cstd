
# Python 正则 —— 捕获与分组 - Zhang's Wikipedia - CSDN博客


2018年06月20日 00:11:25[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：1541


\n：表示第 n 个捕获：
>> s ="<html><h1>what the fuck!</h1></html>">> p =r"<(.+)><(.+)>(.+)</\2></\1>"\# \2 对应第二个捕获，也即 h1，则 </\2> 为：</h1>\# \1 对应第一个捕获，也即 html，则 </\1> 为：</html>>> re.match(p, s).group(3)'what the fuck!'
## 1. 匹配邮箱与html标签
匹配邮箱：
>> mail ='zch921005@126.com'>> reg =r"(\w{4,20})@(126|qq|gmail|163|outlook)\.(com)"\# 正则表达式中不要出现无意义的空格>> re.match(reg, mail).group(1)'zch921005'>> re.match(reg, mail).group(2)'126'>>
匹配 html 标签：
>> s='<div><a href="https://support.google.com/chrome/?p=ui_hotword_search" rel="external nofollow" target="_blank">更多</a><p>dfsl</p></div>'>> re.search(r'<a.*>(.*)</a>', s).group(1)'更多'
## 2. 起别名
[https://blog.csdn.net/HeatDeath/article/details/70171569](https://blog.csdn.net/HeatDeath/article/details/70171569)
```python
>>>
```
```python
s =
```
```python
'<html><h1>what the fuck!</h1></html>'
```
```python
>>>
```
```python
p =
```
```python
r"<(?P<key1>.+)><(?P<key2>.+)>(.+)</(?P=key2)></(?P=key1)>"
```
```python
>> re.match(p, s).group(
```
```python
1
```
```python
)
```
```python
'html'
```
```python
>> re.match(p, s).group(
```
```python
2
```
```python
)
```
```python
'h1'
```
```python
>> re.match(p, s).group(
```
```python
3
```
```python
)
```
```python
'what the fuck!'
```

