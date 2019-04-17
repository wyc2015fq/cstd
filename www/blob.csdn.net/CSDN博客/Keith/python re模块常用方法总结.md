# python re模块常用方法总结 - Keith - CSDN博客





2017年03月14日 23:48:18[ke1th](https://me.csdn.net/u012436149)阅读数：5866








```python
res = re.match(pattern, string, flags=0)
#字符串的开头是否能匹配正则表达式。返回_sre.SRE_Match对象，如果
#不能匹配返回None。
# 如果匹配的话，res.string可以获得原始的字符串，并不是匹配的字符串
```

```python
re.sub(pattern, repl, string, count=0, flags=0)

#找到 RE 匹配的所有子串，并将其用repl替换。可选参数 
#count 是模式匹配後替换的最大次数；count 必须是非负整数。缺省值
#是 0 表示替换所有的匹配。如果无匹配，字符串将会无改变地返回。如
#果有匹配,则返回替换后的字符串
# pattern='he$' 尾部匹配
# pattern='^he' 头部匹配，等价于match
```

```python
re.findall(pattern,string)
# 从 string中找到所有 匹配 pattern的子串，作为列表返回
#如果没有匹配的话，返回空数组，可用来当做if的判断条件
#空数组为False
# pattern='he$' 尾部匹配
# pattern='^he' 头部匹配，等价于match
```

```python
re.search(pattern, string)
#顾名思义，查找，如果找到返回一个match对象，找不到，返回None。
# pattern='he$' 尾部匹配
# pattern='^he' 头部匹配，等价于match
```

## 参考资料

[http://www.cnblogs.com/PythonHome/archive/2011/11/19/2255459.html](http://www.cnblogs.com/PythonHome/archive/2011/11/19/2255459.html)
[https://zh.wikipedia.org/wiki/%E6%AD%A3%E5%88%99%E8%A1%A8%E8%BE%BE%E5%BC%8F](https://zh.wikipedia.org/wiki/%E6%AD%A3%E5%88%99%E8%A1%A8%E8%BE%BE%E5%BC%8F)
[http://www.cnblogs.com/wei-li/archive/2012/05/16/2505503.html](http://www.cnblogs.com/wei-li/archive/2012/05/16/2505503.html)



