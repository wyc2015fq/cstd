# python︱处理与使用json格式的数据（json/UltraJSON/Demjson）、pickle模块 - 素质云笔记/Recorder... - CSDN博客





2017年08月23日 18:31:09[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：4828








python种关于json有很多，simplejson，cjson，还有ujson（详细内容可见：[http://blog.csdn.net/gzlaiyonghao/article/details/6567408）](http://blog.csdn.net/gzlaiyonghao/article/details/6567408%EF%BC%89).

cjson模块只支持string/unicode的key

JSON(JavaScript Object Notation)编码格式的数据。- 1、变量解码、编码为Json格式
- 2、文件读出、导入json格式

注意：

使用json时需要注意的地方：python中字典的key在经过json转化后都变成了string类型
.

## 1、变量解码、编码为Json格式

来看看py3，.dump的[主函数](https://docs.python.org/3.5/library/json.html)：

```
json.dumps(obj, skipkeys=False, ensure_ascii=True, check_circular=True, allow_nan=True, cls=None, indent=None, separators=None, default=None, sort_keys=False, **kw)
```

ensure_ascii比较关键，True代表显示为编码形式，这个一般在中文里面特别不好用，所以建议关掉。

indent代表缩进，sort_keys代表会否按照某些顺序排列

```python
import json

data = {
    'name' : 'ACME',
    'shares' : 100,
    'price' : 542.23
}

json_str = json.dumps(data)  # 编码
json_str
```

```
'{"price": 542.23, "shares": 100, "name": "ACME"}'
```

```python
data = json.loads(json_str)  # 解码
data
```

```
{'name': 'ACME', 'price': 542.23, 'shares': 100}
```

## 2、文件读出、导入json格式

```python
# Writing JSON data
with open('data.json', 'w') as f:
    json.dump(data, f)

# Reading data back
with open('data.json', 'r') as f:
    data = json.load(f)
```

还有一种导入、导出的写法（限py3）：

```python
open("myfile.json", "w", encoding="utf-8" ).write( json_data )
json_data = open("myfile.json", "r", encoding="utf8").read()
```

## .3、其他用法

（1）分离器，用来分割。

```
>>> import json
>>> json.dumps([1,2,3,{'4': 5, '6': 7}], separators=(',', ':'))
'[1,2,3,{"4":5,"6":7}]'
```

（2）缩进，更好看。

```
>>> import json
>>> print(json.dumps({'4': 5, '6': 7}, sort_keys=True, indent=4))
{
    "4": 5,
    "6": 7
}
```

（3）与StringIO合用。

```
>>> import json
>>> json.loads('["foo", {"bar":["baz", null, 1.0, 2]}]')
['foo', {'bar': ['baz', None, 1.0, 2]}]
>>> json.loads('"\\"foo\\bar"')
'"foo\x08ar'
>>> from io import StringIO
>>> io = StringIO('["streaming API"]')
>>> json.load(io)
['streaming API']
```

.

## 延伸一：python3中读保存成中文

发现在py3中保存成.json见很容易变成：`"{\"new\": [\"\u5927\u90fd\u4f1a\u827a\u672f\u535a\u7269\u9986\"`

我想直接保存成中文。

```
with open('data.json', 'w') as f:
    json.dump(json.dump(data,ensure_ascii = False), f)
```

这段代码中`ensure_ascii = False`是可以在py3里面打印出中文，而不是编码形式的。但是保存不了。

需要以下的代码：

```
open('../../data.json', 'w').write(json.dumps(json_data,ensure_ascii = False))
```

若依旧出现问题，报错：`UnicodeEncodeError: 'gbk' codec can't encode character '\u02c6' in position 54928648: illegal multibyte sequence`，可能还需要加上encoding选项，`open('../../data.json', 'w',encoding='utf-8')`

读入也可以用：

```
eval(open('../../data.json', "r").read())
```

.read()读入为字符串的形式，eval可以把字符串变成原来的格式，也就是{}

网上还有用codecs库，好像也是不行的

```
import codecs
codecs.open('../../data.json', 'w', "utf-8") as f:
	json.dump
```

# Demjson

> 
Demjson 是 python 的第三方模块库，可用于编码和解码 JSON 数据，包含了 JSONLint 的格式化及校验功能。


Github 地址：[https://github.com/dmeranda/demjson](https://github.com/dmeranda/demjson)

官方地址：[http://deron.meranda.us/python/demjson/](http://deron.meranda.us/python/demjson/)
- encode	将 Python 对象编码成 JSON 字符串
- decode	将已编码的 JSON 字符串解码为 Python 对象

```python
import demjson

data = [ { 'a' : 1, 'b' : 2, 'c' : 3, 'd' : 4, 'e' : 5 } ]

json = demjson.encode(data)
print(json)
```

```
[{"a":1,"b":2,"c":3,"d":4,"e":5}]
```

```python
text = demjson.decode(json)
print (text)
```

```
[{'c': 3, 'e': 5, 'b': 2, 'a': 1, 'd': 4}]
```

.

# UltraJSON

[https://github.com/esnme/ultrajson](https://github.com/esnme/ultrajson)

```
>>> import ujson
>>> ujson.dumps([{"key": "value"}, 81, True])
'[{"key":"value"},81,true]'
>>> ujson.loads("""[{"key": "value"}, 81, true]""")
[{u'key': u'value'}, 81, True]
```

编码encode_html_chars：

```
>>> ujson.dumps("<script>John&Doe", encode_html_chars=True)
'"\\u003cscript\\u003eJohn\\u0026Doe"'
```

ensure_ascii：

将输出限制为ASCII，并将所有扩展字符转义为127以上。默认值为true。如果您的最终格式支持UTF-8设置此选项为false，强烈建议节省空间：

```
>>> ujson.dumps(u"\xe5\xe4\xf6")
'"\\u00e5\\u00e4\\u00f6"'
>>> ujson.dumps(u"\xe5\xe4\xf6", ensure_ascii=False)
'"\xc3\xa5\xc3\xa4\xc3\xb6"'
```

escape_forward_slashes：

控制是否正斜杠（/）逃脱了。

```
>>> ujson.dumps("http://esn.me")
'"http:\/\/esn.me"'
>>> ujson.dumps("http://esn.me", escape_forward_slashes=False)
'"http://esn.me"'
```

缩进indent：

```
>>> ujson.dumps({"foo": "bar"})
'{"foo":"bar"}'
>>> ujson.dumps({"foo": "bar"}, indent=4)
{
    "foo":"bar"
}
```

# pickle模块

```
import pickle
pickle.dump(output,open('xxx.pkl','wb')   )  
pickle.load(open('../data/hist_data', 'rb'))
```

## 参考文献：

[Python JSON](http://www.runoob.com/python/python-json.html)
[Using demjson with Python 3](http://deron.meranda.us/python/demjson/python3)
[Python:读取和处理json数据](http://gohom.win/2016/01/04/py-json/)

.
## 延伸：用json解析网页

使用urlopen方法打开网址后, 使用json.load(u)以文件方法来读取.

也可以先u.read().decode(‘utf-8’)读取转为字符串后再传给json.loads(str).

```python
import urllib2,json
u=urllib2.urlopen("http://api.crossref.org/works/10.1021/jm0203783")
json.load(u)
```

注意, 因为是文件形式, 读取一次后即到底, 不能再次调用json.load(u)获取内容.

使用requests，抓取json数据后, 可以直接使用相应对象的json()方法获得json数据, 也可以使用r.text调用字符串数据传递给json获取.

```python
import json,requests
r=requests.get("http://api.crossref.org/works/10.1021/jm0203783")
r.json()
json.loads(r.text)
```

## 报错：OverflowError: Maximum recursion level reached

```
>>> import ujson
>>> import numpy
>>>
>>> ujson.dumps(numpy.min([1,2]))
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
OverflowError: Maximum recursion level reached
```

出现的报错，那么这里就是数值numpy有问题，需要改成Int即可






