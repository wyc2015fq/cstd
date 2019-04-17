# python cookbook 1.9 查找两个字典相同点（相同key，相同value） - 别说话写代码的博客 - CSDN博客





2019年01月08日 11:44:25[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：68
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```python
#1.9查找两个字典的相同点(相同的键key，相同的值value)
a = {
    'x':1,
    'y':2,
    'z':3
}
b={
    'w':11,
    'x':4,
    'z':6
} 
print(a.keys()-b.keys())   #字典中的key支持集合操作
print(a.keys() & b.keys()) 
print(a.keys() | b.keys()) 

#这些操作也可以修改或者过滤字典元素。若想以现有字典排除几个key的新字典
d={key:a[key] for key in a.keys() - {'x'}}   #排除掉key为'x'的键值对
print(d)
```

![](https://img-blog.csdnimg.cn/20190108114333911.png)

**dict的key和items都支持集合操作，求相同，不同键值对value不支持集合操作，可以先将其转为set，然后进行集合操作**



