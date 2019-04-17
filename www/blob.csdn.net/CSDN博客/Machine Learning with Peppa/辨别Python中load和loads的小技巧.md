# 辨别Python中load和loads的小技巧 - Machine Learning with Peppa - CSDN博客





2018年03月15日 20:57:18[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：5946










一句话，load通过json.load(open('*.json'))这样的格式，从文件句柄中打开文件，加载到Python的变量中，并以字典的格式转换。而loads必须对于Python内存中的序列化对象转换成字符串。

因此，load和loads都是实现“反序列化”，区别在于（以Python为例）。loads针对内存对象，即将Python内置数据序列化为字串。如使用json.dumps序列化的对象d_json=json.dumps({‘a‘:1, ‘b‘:2})，在这里d_json是一个字串‘{"b": 2, "a": 1}‘

d=json.loads(d_json)  #{ b": 2, "a": 1}，使用load重新反序列化为dict，load针对文件句柄，如本地有一个json文件a.json则可以d=json.load(open(‘a.json‘))相应的，dump就是将内置类型序列化为json对象后写入文件。




测试：

```cpp
l1 = '[1,2,3,4]'
d1 = '{"k1":"v1"}'
print(type(l1))
print(type(d1))
```

输出：

```cpp
<class 'str'>
<class 'str'>
```

```cpp

```
ret = json.loads(l1)
print(ret,type(ret))
ret = json.loads(d1)
print(ret,type(ret))
输出：

```cpp
[1, 2, 3, 4] <class 'list'>
{'k1': 'v1'} <class 'dict'>
```



