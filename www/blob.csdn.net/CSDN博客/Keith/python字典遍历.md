# python字典遍历 - Keith - CSDN博客





2016年12月16日 13:28:19[ke1th](https://me.csdn.net/u012436149)阅读数：696








# python字典遍历

1.

```python
a = {'a':1, 'b':2}
for key in a.keys():
    print key,
    print a[key]
#输出
#a 1
#b 2
```

2.

```python
a = {'a':1, 'b':2}

for val in a.values():
    print val
#输出
#1
#2
```

3.

```python
a = {'a':1, 'b':2}

for key,value in a.items():
    print key,value
#输出
#a 1
#b 2
```



