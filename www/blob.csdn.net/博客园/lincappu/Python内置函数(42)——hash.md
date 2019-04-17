# Python内置函数(42)——hash - lincappu - 博客园







# [Python内置函数(42)——hash](https://www.cnblogs.com/lincappu/p/8145045.html)





**英文文档：**

`hash`(*object*)Return the hash value of the object (if it has one). Hash values are integers. They are used to quickly compare dictionary keys during a dictionary lookup. Numeric values that compare equal have the same hash value (even if they are of different types, as is the case for 1 and 1.0).

 　　返回对象的哈希值

**说明：　　**

　　1. 返回对象的哈希值，用整数表示。哈希值在字典查找时，可用于快速比较键的值。


```
>>> hash('good good study')
1032709256
```



　　2. 相等的数值，即使类型不一致，计算的哈希值是一样的。

```
>>> 1.0 == 1
True
>>> hash(1.0)
1
>>> hash(1)
1
>>> hash(1.0000)
1
```













