# Python入门---易错已错易混淆----知识点 - 默槑 - 博客园







# [Python入门---易错已错易混淆----知识点](https://www.cnblogs.com/modaidai/p/6745389.html)





1、not 1 or 0 and 1 or 3 and 4 or 5 and 6 or 7 and 8 and 9 结果会输出啥？

根据优先级：(not 1) or (0 and 1) or (3 and 4) or (5 and 6) or (7 and 8 and 9)  

之后剩下：0 or 0 or 4 or 6 or 9

之后根据短路逻辑： 3 and 4  == 4 ；3 or 4 == 3

所以最后答案是 4



2、元字符“.”在默认模式下，匹配除换行符外的所有字符。在DOTALL模式下，匹配所有字符，包括换行符。

 　　regexgenes=re.compile(r'GENES(.*?)REFERENCE',re.DOTALL)



3、lower()

　　将字符串中的大写变小写

　　strip()

　　将字符串前面和后面的换指定字符去掉，不指定则默认为空格



4、re.compile()

使用re的一般的步骤是先使用re.compile()函数，将正则表达式的字符串形式编译为Pattern实例，然后使用Pattern实例处理文本并获得匹配结果（下面举一个match的例子），最后使用Match实例获得信息，进行其他操作


```python
import
```

```python
re
```



```python
pattern
```

```python
=
```

```python
re.
```

```python
compile
```

```python
(
```

```python
'[a-zA-Z]'
```

```python
)
```



```python
result
```

```python
=
```

```python
pattern.findall(
```

```python
'as3SiOPdj#@23awe'
```

```python
)
```



```python
print
```

```python
result
```



```python
# ['a', 's', 'S', 'i', 'O', 'P', 'd', 'j', 'a', 'w', 'e']
```




5、str()与repr()的区别

　　　　其实没什么区别，基本是一样的，只是一些细节不同，str让用户舒服，repr让python舒服

>>> s = 'Hello, world.'
>>> str(s)
'Hello, world.'
>>> repr(s)
"'Hello, world.'"












