# python修改文件后缀名 - V__KING__的专栏 - CSDN博客





2013年12月21日 14:01:49[v__king__](https://me.csdn.net/V__KING__)阅读数：4082








不要小看这段代码哦。我还从中收获不少呢。



```
```python
import
```

```python
os
```



```python
import
```

```python
string
```



```python
dirName
```

```python
=
```

```python
"./code/demo/"
```



```python
li
```

```python
=
```

```python
os.listdir(dirName)
```



```python
for
```

```python
filename
```

```python
in
```

```python
li:
```



```python

```

```python
newname
```

```python
=
```

```python
filename
```



```python

```

```python
newname
```

```python
=
```

```python
newname.split(
```

```python
"."
```

```python
)
```



```python

```

```python
if
```

```python
newname[
```

```python
-
```

```python
1
```

```python
]
```

```python
=
```

```python
=
```

```python
"txt"
```

```python
:
```



```python

```

```python
newname[
```

```python
-
```

```python
1
```

```python
]
```

```python
=
```

```python
"html"
```



```python

```

```python
newname
```

```python
=
```

```python
string.join(newname,
```

```python
"."
```

```python
)
```



```python

```

```python
filename
```

```python
=
```

```python
dirName
```

```python
+
```

```python
filename
```



```python

```

```python
newname
```

```python
=
```

```python
dirName
```

```python
+
```

```python
newname
```



```python

```

```python
os.rename(filename,newname)
```



```python

```

```python
print
```

```python
newname,
```

```python
"updated successfully"
```
```



如果你这样写：



```
```python
import
```

```python
os
```



```python
import
```

```python
string
```



```python
dirName
```

```python
=
```

```python
"./code/demo/"
```



```python
li
```

```python
=
```

```python
os.listdir(dirName)
```



```python
for
```

```python
filename
```

```python
in
```

```python
li:
```



```python

```

```python
#newname = filename
```



```python

```

```python
newname
```

```python
=
```

```python
newname.split(
```

```python
"."
```

```python
)
```



```python

```

```python
newname
```

```python
=
```

```python
filename
```

```python
#在这后面修改newname也直接修改了filename
```



```python

```

```python
if
```

```python
newname[
```

```python
-
```

```python
1
```

```python
]
```

```python
=
```

```python
=
```

```python
"txt"
```

```python
:
```



```python

```

```python
newname[
```

```python
-
```

```python
1
```

```python
]
```

```python
=
```

```python
"html"
```



```python

```

```python
newname
```

```python
=
```

```python
string.join(newname,
```

```python
"."
```

```python
)
```



```python

```

```python
filename
```

```python
=
```

```python
dirName
```

```python
+
```

```python
filename
```



```python

```

```python
newname
```

```python
=
```

```python
dirName
```

```python
+
```

```python
newname
```



```python

```

```python
os.rename(filename,newname)
```



```python

```

```python
print
```

```python
newname,
```

```python
"updated successfully"
```
```



因为python里面对传值和引用传值有非常诡异的“搞法”

如果函数收到的是一个可变对象（比如字典或者列表）的引用，就能修改对象的原始值——相当于通过“传引用”来传递对象。如果函数收到的是一个不可变对象（比如数字、字符或者元组）的引用，就不能直接修改原始对象——相当于通过“传值'来传递对象。






```
```python
files
```

```python
=
```

```python
os.listdir(
```

```python
"."
```

```python
)
```



```python

```

```python
for
```

```python
filename
```

```python
in
```

```python
files:
```



```python

```

```python
li
```

```python
=
```

```python
os.path.splitext(filename)
```



```python

```

```python
if
```

```python
li[
```

```python
1
```

```python
]
```

```python
=
```

```python
=
```

```python
".html"
```

```python
:
```



```python

```

```python
newname
```

```python
=
```

```python
li[
```

```python
0
```

```python
]
```

```python
+
```

```python
".htm"
```



```python

```

```python
os.rename(filename,newname)
```
```


原来系统有自带的os.path.splitext( )来获取后缀名


