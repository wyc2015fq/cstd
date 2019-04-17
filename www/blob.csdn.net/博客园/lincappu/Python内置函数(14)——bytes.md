# Python内置函数(14)——bytes - lincappu - 博客园







# [Python内置函数(14)——bytes](https://www.cnblogs.com/lincappu/p/8144727.html)





**英文文档：**

*class *`bytes`([*source*[, *encoding*[, *errors*]]])

    Return a new “bytes” object, which is an immutable sequence of integers in the range `0 <= x < 256`. `bytes` is an immutable version of `bytearray` – it has the same non-mutating methods and the same indexing and slicing behavior.

Accordingly, constructor arguments are interpreted as for `bytearray()`.

 　　根据传入的参数生成一个新的不可变的数组

**说明：**

    1. 返回值为一个新的不可修改字节数组，每个数字元素都必须在0 - 255范围内，和bytearray函数的具有相同的行为，差别仅仅是返回的字节数组不可修改。

    2. 当3个参数都不传的时候，返回长度为0的字节数组

```
>>> b = bytes()
>>> b
b''
>>> len(b)
0
```

    3. 当source参数为字符串时，encoding参数也必须提供，函数将字符串使用str.encode方法转换成字节数组

```
>>> bytes('中文') #需传入编码格式
Traceback (most recent call last):
  File "<pyshell#14>", line 1, in <module>
    bytes('中文')
TypeError: string argument without an encoding
>>> bytes('中文','utf-8')
b'\xe4\xb8\xad\xe6\x96\x87'
>>> '中文'.encode('utf-8')
b'\xe4\xb8\xad\xe6\x96\x87'
```

    4. 当source参数为整数时，返回这个整数所指定长度的空字节数组

```
>>> bytes(2)
b'\x00\x00'
>>> bytes(-2) #整数需大于0，用于做数组长度
Traceback (most recent call last):
  File "<pyshell#19>", line 1, in <module>
    bytes(-2)
ValueError: negative count
```

    5. 当source参数为实现了buffer接口的object对象时，那么将使用只读方式将字节读取到字节数组后返回

    6. 当source参数是一个可迭代对象，那么这个迭代对象的元素都必须符合0 <= x < 256，以便可以初始化到数组里

```
>>> bytes([1,2,3])
b'\x01\x02\x03'
>>> bytes([256,2,3])
Traceback (most recent call last):
  File "<pyshell#21>", line 1, in <module>
    bytes([256,2,3])
ValueError: bytes must be in range(0, 256)
```

    7. 返回数组不可修改

```
>>> b = bytes(10)
>>> b
b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
>>> b[0]
0
>>> b[1] = 1 #不可修改
Traceback (most recent call last):
  File "<pyshell#6>", line 1, in <module>
    b[1] = 1
TypeError: 'bytes' object does not support item assignment

>>> b = bytearray(10)
>>> b
bytearray(b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00')
>>> b[1] = 1 #可修改
>>> b
bytearray(b'\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00')
```












