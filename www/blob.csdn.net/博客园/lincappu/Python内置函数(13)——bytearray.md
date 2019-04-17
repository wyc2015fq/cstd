# Python内置函数(13)——bytearray - lincappu - 博客园







# [Python内置函数(13)——bytearray](https://www.cnblogs.com/lincappu/p/8144712.html)





**英文文档：**

*class *`bytearray`([*source*[, *encoding*[, *errors*]]])

Return a new array of bytes. The [`bytearray`](mk:@MSITStore:C:\Python35\Doc\Python352.chm::/library/functions.html#bytearray) class is a mutable sequence of integers in the range 0 <= x < 256. It has most of the usual methods of mutable sequences, described in Mutable Sequence Types, as well as most methods that the [`bytes`](mk:@MSITStore:C:\Python35\Doc\Python352.chm::/library/functions.html#bytes) type has, see Bytes and Bytearray Operations.

The optional *source* parameter can be used to initialize the array in a few different ways:
- If it is a *string*, you must also give the *encoding* (and optionally, *errors*) parameters; [`bytearray()`](mk:@MSITStore:C:\Python35\Doc\Python352.chm::/library/functions.html#bytearray) then converts the string to bytes using `str.encode()`.
- If it is an *integer*, the array will have that size and will be initialized with null bytes.
- If it is an object conforming to the *buffer* interface, a read-only buffer of the object will be used to initialize the bytes array.
- If it is an *iterable*, it must be an iterable of integers in the range `0 <= x < 256`, which are used as the initial contents of the array.

Without an argument, an array of size 0 is created.

　　根据传入的参数创建一个新的字节数组

说明：

    1. 返回值为一个新的字节数组

    2. 当3个参数都不传的时候，返回长度为0的字节数组

```
>>> b = bytearray()
>>> b
bytearray(b'')
>>> len(b)
0
```

   3. 当source参数为字符串时，encoding参数也必须提供，函数将字符串使用str.encode方法转换成字节数组

```
>>> bytearray('中文')
Traceback (most recent call last):
  File "<pyshell#48>", line 1, in <module>
    bytearray('中文')
TypeError: string argument without an encoding
>>> bytearray('中文','utf-8')
bytearray(b'\xe4\xb8\xad\xe6\x96\x87')
```

    4. 当source参数为整数时，返回这个整数所指定长度的空字节数组

```
>>> bytearray(2)
bytearray(b'\x00\x00')
>>> bytearray(-2) #整数需大于0，使用来做数组长度的
Traceback (most recent call last):
  File "<pyshell#51>", line 1, in <module>
    bytearray(-2)
ValueError: negative count
```

    5. 当source参数为实现了buffer接口的object对象时，那么将使用只读方式将字节读取到字节数组后返回

    6. 当source参数是一个可迭代对象，那么这个迭代对象的元素都必须符合0 <= x < 256，以便可以初始化到数组里

```
>>> bytearray([1,2,3])
bytearray(b'\x01\x02\x03')
>>> bytearray([256,2,3]) #不在0-255范围内报错
Traceback (most recent call last):
  File "<pyshell#53>", line 1, in <module>
    bytearray([256,2,3])
ValueError: byte must be in range(0, 256)
```












