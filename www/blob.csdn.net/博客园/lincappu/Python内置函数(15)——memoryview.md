# Python内置函数(15)——memoryview - lincappu - 博客园







# [Python内置函数(15)——memoryview](https://www.cnblogs.com/lincappu/p/8144738.html)





**英文文档：**

`*class *`memoryview`(*obj*)`
- `memoryview` objects allow Python code to access the internal data of an object that supports the buffer protocol without copying.
- 
- Create a `memoryview` that references *obj*. *obj* must support the buffer protocol. Built-in objects that support the buffer protocol include `bytes` and `bytearray`.

 　　根据输的内容创建一个内存查看对象。

**说明：**

　　1. 函数功能返回内存查看对象，实际上是内存查看对象(Momory view)的构造函数。

　　2. 所谓内存查看对象，是指对支持缓冲区协议的数据进行包装，在不需要复制对象基础上允许Python代码访问。

　　3. Python内置对象中支持缓冲区协议的对象有bytes和bytearray。

```
>>> v = memoryview(b'abcefg')
>>> v[1]
98
>>> v[-1]
103
>>> v[1:4]
<memory at 0x7f3ddc9f4350>
>>> bytes(v[1:4])
b'bce'
```












