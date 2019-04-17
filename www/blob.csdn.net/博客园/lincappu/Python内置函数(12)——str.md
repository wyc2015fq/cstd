# Python内置函数(12)——str - lincappu - 博客园







# [Python内置函数(12)——str](https://www.cnblogs.com/lincappu/p/8144699.html)





**英文文档：**

*class *`str`(*object=''*) *class*

`str`(*object=b''*, *encoding='utf-8'*, *errors='strict'*)

Return a string version of *object*. If *object* is not provided, returns the empty string. Otherwise, the behavior of `str()`depends on whether *encoding* or *errors* is given, as follows.

If neither *encoding* nor *errors* is given, `str(object)` returns `object.__str__()`, which is the “informal” or nicely printable string representation of *object*. For string objects, this is the string itself. If *object* does not have a `__str__()`method, then `str()` falls back to returning `repr(object)`.

If at least one of *encoding* or *errors* is given, *object* should be a bytes-like object (e.g. `bytes` or `bytearray`). In this case, if *object* is a `bytes` (or `bytearray`) object, then `str(bytes, encoding, errors)` is equivalent to `bytes.decode(encoding, errors)`. Otherwise, the bytes object underlying the buffer object is obtained before calling `bytes.decode()`. See Binary Sequence Types — bytes, bytearray, memoryview and Buffer Protocol for information on buffer objects.

 　　返回一个对象的字符串格式给用户

**说明：**

　　1. str函数功能时将对象转换成其字符串表现形式，如果不传入参数，将返回空字符串。

```
>>> str()
''
>>> str(None)
'None'
>>> str('abc')
'abc'
>>> str(123)
'123'
```

　　2. 当转换二进制流时，可以传入参数encoding，表示读取字节数组所使用的编码格式；参数errors，表示读取二进制的错误级别。(这两个参数和open方法中的同名参数有相同取值和类似的含义，详见[Python内置函数(47)——open](http://www.cnblogs.com/sesshoumaru/p/6047046.html))。

```
>>> file = open('test.txt','rb') # 打开文件
>>> fileBytes = file.read() # 读取二进制流
>>> fileBytes
b'\xe6\x88\x91\xe6\x98\xaf\xe7\xac\xac1\xe8\xa1\x8c\xe6\x96\x87\xe6\x9c\xac\xef\xbc\x8c\xe6\x88\x91\xe5\xb0\x86\xe8\xa2\xab\xe6\x98\xbe\xe7\xa4\xba\xe5\x9c\xa8\xe5\xb1\x8f\xe5\xb9\x95\r\n\xe6\x88\x91\xe6\x98\xaf\xe7\xac\xac2\xe8\xa1\x8c\xe6\x96\x87\xe6\x9c\xac\xef\xbc\x8c\xe6\x88\x91\xe5\xb0\x86\xe8\xa2\xab\xe6\x98\xbe\xe7\xa4\xba\xe5\x9c\xa8\xe5\xb1\x8f\xe5\xb9\x95\r\n\xe6\x88\x91\xe6\x98\xaf\xe7\xac\xac3\xe8\xa1\x8c\xe6\x96\x87\xe6\x9c\xac\xef\xbc\x8cr\xe6\x88\x91\xe5\xb0\x86\xe8\xa2\xab\xe6\x98\xbe\xe7\xa4\xba\xe5\x9c\xa8\xe5\xb1\x8f\xe5\xb9\x95'
>>> str(fileBytes) # 默认将二进制流转换成字符串表现形式
"b'\\xe6\\x88\\x91\\xe6\\x98\\xaf\\xe7\\xac\\xac1\\xe8\\xa1\\x8c\\xe6\\x96\\x87\\xe6\\x9c\\xac\\xef\\xbc\\x8c\\xe6\\x88\\x91\\xe5\\xb0\\x86\\xe8\\xa2\\xab\\xe6\\x98\\xbe\\xe7\\xa4\\xba\\xe5\\x9c\\xa8\\xe5\\xb1\\x8f\\xe5\\xb9\\x95\\r\\n\\xe6\\x88\\x91\\xe6\\x98\\xaf\\xe7\\xac\\xac2\\xe8\\xa1\\x8c\\xe6\\x96\\x87\\xe6\\x9c\\xac\\xef\\xbc\\x8c\\xe6\\x88\\x91\\xe5\\xb0\\x86\\xe8\\xa2\\xab\\xe6\\x98\\xbe\\xe7\\xa4\\xba\\xe5\\x9c\\xa8\\xe5\\xb1\\x8f\\xe5\\xb9\\x95\\r\\n\\xe6\\x88\\x91\\xe6\\x98\\xaf\\xe7\\xac\\xac3\\xe8\\xa1\\x8c\\xe6\\x96\\x87\\xe6\\x9c\\xac\\xef\\xbc\\x8cr\\xe6\\x88\\x91\\xe5\\xb0\\x86\\xe8\\xa2\\xab\\xe6\\x98\\xbe\\xe7\\xa4\\xba\\xe5\\x9c\\xa8\\xe5\\xb1\\x8f\\xe5\\xb9\\x95'"
>>> str(fileBytes,'utf-8') # 传入encoding参数，函数将以此编码读取二进制流的内容
'我是第1行文本，我将被显示在屏幕\r\n我是第2行文本，我将被显示在屏幕\r\n我是第3行文本，r我将被显示在屏幕'
>>> str(fileBytes,'gbk') # 当传入encoding不能解码时，会报错(即errors参数默认为strict)
Traceback (most recent call last):
  File "<pyshell#46>", line 1, in <module>
    str(fileBytes,'gbk')
UnicodeDecodeError: 'gbk' codec can't decode byte 0xac in position 8: illegal multibyte sequence
>>> str(fileBytes,'gbk','ignore') # 'ignore' 忽略级别，字符编码有错，忽略掉.
'鎴戞槸绗1琛屾枃鏈锛屾垜灏嗚鏄剧ず鍦ㄥ睆骞\r\n鎴戞槸绗2琛屾枃鏈锛屾垜灏嗚鏄剧ず鍦ㄥ睆骞\r\n鎴戞槸绗3琛屾枃鏈锛宺鎴戝皢琚鏄剧ず鍦ㄥ睆骞'
>>> str(fileBytes,'gbk','replace') # 'replace' 替换级别，字符编码有错的，替换成？. 
'鎴戞槸绗�1琛屾枃鏈�锛屾垜灏嗚��鏄剧ず鍦ㄥ睆骞�\r\n鎴戞槸绗�2琛屾枃鏈�锛屾垜灏嗚��鏄剧ず鍦ㄥ睆骞�\r\n鎴戞槸绗�3琛屾枃鏈�锛宺鎴戝皢琚�鏄剧ず鍦ㄥ睆骞�'
```












