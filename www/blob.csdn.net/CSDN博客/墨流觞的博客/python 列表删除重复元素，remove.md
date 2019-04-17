# python 列表删除重复元素，remove - 墨流觞的博客 - CSDN博客





2018年09月09日 20:49:06[墨氲](https://me.csdn.net/dss_dssssd)阅读数：622








```python
ls = [1, 1, 2, 3, 4]
for i in ls:
    if i == 1: 
        ls.remove(i)
print(ls)
 # output: [1, 2, 3, 4]
```

原因是remove改变列表长度和元素的位置，第二次进入for循环时，指向ls[1],而此时ls长度已经改变，ls[1]是2而不是指向元素1了 

看一下以下例子的输出就明白了

```python
ls = [1, 1, 2, 3, 4]
for i in ls:
    ls.remove(i)

print(ls)
```

例子的输出不是[],而是[1, 3]

解决方案： 

先将所有的元素取出，然后循环删除

```python
rem = [i for i in ls if i == 1]
    _ = (ls.remove(i) for i in rem )
    print(ls)
```

看一个小问题：

```python
rem = (i for i in ls if i == 1)
    # _ = (ls.remove(i) for i in rem )
    for i in rem:
        print(i)
        ls.remove(i)
    print(ls)
    # output: 
    # 1
    # [1, 2, 3,4]
```

虽然变成生成器，但是也是集合，所以rem中有一个元素1，集合自动去掉重复元素了。



