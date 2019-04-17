# Python内置函数(41)——id - lincappu - 博客园







# [Python内置函数(41)——id](https://www.cnblogs.com/lincappu/p/8145005.html)





**英文文档：**
- `id`(*object*)
- 
- Return the “identity” of an object. This is an integer which is guaranteed to be unique and constant for this object during its lifetime. Two objects with non-overlapping lifetimes may have the same `id()` value.
- **CPython implementation detail:** This is the address of the object in memory.
- 
- 
- 
- 
- **　　返回对象的唯一标识**
- **说明：**
- 
- 　　1. 返回对象的唯一标识符，用整数表示。在程序生命周期内，这个标识符常量是唯一的。

```
>>> a = 1
>>> id(a)
1652447504
>>> id(1.0)
66116592
>>> a = 'some text'
>>> id(a)
69228568
```












