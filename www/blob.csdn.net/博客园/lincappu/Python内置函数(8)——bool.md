# Python内置函数(8)——bool - lincappu - 博客园







# [Python内置函数(8)——bool](https://www.cnblogs.com/lincappu/p/8144631.html)





**英文文档：**

*class *`bool`([*x*])

    Return a Boolean value, i.e. one of `True` or `False`. *x* is converted using the standard truth testing procedure. If *x* is false or omitted, this returns `False`; otherwise it returns `True`. The `bool` class is a subclass of `int` (see Numeric Types — int, float, complex). It cannot be subclassed further. Its only instances are `False` and `True` (see Boolean Values).



**说明：**

**　　1.返回值为 True 或者 False 的布尔值**

**　　2.参数如果缺省则返回 False**

>>> bool(True)
True
>>> bool(False)
False
>>> bool(false)  # 大小写敏感。
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 'false' is not defined

>>> bool() #
False


　　3.逻辑测试单元表达式：

传入 bool 类型的值：按原值返回

数值类型：非0 为 True，0为 False，

可迭代对象：字符串，类表，元组，集合，空则返回 False，非空则返回 True。

***None ：返回 False***

>>> bool()  
False
>>> bool(True)
True
>>> bool(False)
False
>>>
>>> bool(None)
*False*
















