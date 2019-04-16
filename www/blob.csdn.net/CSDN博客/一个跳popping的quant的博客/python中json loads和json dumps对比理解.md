# python中json.loads和json.dumps对比理解 - 一个跳popping的quant的博客 - CSDN博客





2018年02月10日 21:55:27[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：6049
所属专栏：[学python的路上](https://blog.csdn.net/column/details/20561.html)









json.loads和json.dumps最近经常用到，就稍微整理了下二者的关系和区别。

一开始用到的时候是把json.loads理解为把json字符串转换为python对象；而json.dumps是把python对象转换为json字符串的。这么理解感觉就挺合适的，为了加深理解，下面再进一步来剖析二者。

看一下json.loads和json.dumps源码中的注释。

```python
"""Deserialize ``s`` (a ``str`` or ``unicode`` instance containing a JSON
    document) to a Python object.

    If ``s`` is a ``str`` instance and is encoded with an ASCII based encoding
    other than utf-8 (e.g. latin-1) then an appropriate ``encoding`` name
    must be specified. Encodings that are not ASCII based (such as UCS-2)
    are not allowed and should be decoded to ``unicode`` first.
```


```python
"""Serialize ``obj`` to a JSON formatted ``str``.
```

二者分别是序列化和反序列化的两个作用。

先说一下什么是序列化和反序列化。

**把对象转换为字节序列的过程成为对象的序列化；把字节序列恢复为对象的过程称为对象的反序列化。**

在使用序列化的时候，通常有两个场景：

1）、把对象的字节序列保存在硬盘上，通常存放在一个文件中。就像打单机游戏时，现在多少级多少装备然后进行存档一样。

2）、在网络上传送对象的字节序列时，就要先把对象进行序列化操作，接受端接收后再进行反序列化。




举个例子：

下面代码是通过新浪接口获取期货数据。


```python
class SinaDemo(object):
    def __init__(self):
        super(SinaDemo, self).__init__()

    def getData(self):
        id = 'rb1901'
        url = 'http://stock2.finance.sina.com.cn/futures/api/json.php/IndexService.getInnerFuturesMiniKLine5m?symbol='+ id
        req = urllib2.Request(url)
        rsp = urllib2.urlopen(req)
        res = rsp.read()
        res_json = json.loads(res)
     
        print res
        print res_json

if __name__ == '__main__':
    sd = SinaDemo()
    sd.getData()
```

返回的结果是：

```python
[["2018-02-09 23:00:00","3681.000","3683.000","3680.000","3680.000","32"]]
```

```python
[[u'2018-02-09 23:00:00', u'3681.000', u'3683.000', u'3680.000', u'3680.000', u'32']]
```


res是一个json数据，数组中的元素都是str，其中根据注释只要是str或者unicode编码的实例都可以被反序列化为python对象。

经过json.loads()后得到的每个元素都是python字符对象了。前面的u是声明是unicode编码。

json.dumps()的作用正好相反。

```python
def sendData(self):
        dict = {'size':10, 'length':10.0, 'color':'blue'}
        json_dict = json.dumps(dict)
        print dict
        print json_dict
```

```python
{'color': 'blue', 'length': 10.0, 'size': 10}
{"color": "blue", "length": 10.0, "size": 10}
```


将python中的dict序列化为了json格式的字符串。







