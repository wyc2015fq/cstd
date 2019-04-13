
# Java 源码 —— List - Zhang's Wikipedia - CSDN博客


2018年09月03日 08:18:36[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：256个人分类：[源码阅读](https://blog.csdn.net/lanchunhui/article/category/8002247)



## 0. 成员属性
```python
transient
```
```python
Object[] elementData;
```
```python
// non-private to simplify nested class access
```
```python
private
```
```python
static
```
```python
final
```
```python
Object[] EMPTY_ELEMENTDATA = {};
```
```python
private
```
```python
static
```
```python
final
```
```python
Object[] DEFAULTCAPACITY_EMPTY_ELEMENTDATA = {};
```
```python
private
```
```python
static
```
```python
final
```
```python
int
```
```python
DEFAULT_CAPACITY =
```
```python
10
```
```python
;
```
## 1. 泛型
ArrayList 的泛型实现依赖内部的 Object 对象的数组；
```python
public
```
```python
class
```
```python
ArrayList
```
```python
<
```
```python
E
```
```python
> {
```
```python
transient
```
```python
Object
```
```python
[] elementData;
```
```python
//
```
```python
non-private to simplify nested
```
```python
class
```
```python
access
```
```python
@SuppressWarnings
```
```python
(
```
```python
"unchecked"
```
```python
)
```
```python
E
```
```python
elementData(int index) {
```
```python
return
```
```python
(
```
```python
E
```
```python
) elementData[index];
    }
}
```
## 2. contains
同理可知 indexOf、lastIndexOf 的实现；
```python
public
```
```python
boolean
```
```python
contains
```
```python
(Object o) {
```
```python
return
```
```python
indexOf(o) >=
```
```python
0
```
```python
;
}
```
```python
public
```
```python
int
```
```python
indexOf
```
```python
(Object o) {
```
```python
if
```
```python
(o ==
```
```python
null
```
```python
) {
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < size; i++)
```
```python
if
```
```python
(elementData[i]==
```
```python
null
```
```python
)
```
```python
return
```
```python
i;
    }
```
```python
else
```
```python
{
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < size; i++)
```
```python
if
```
```python
(o.equals(elementData[i]))
```
```python
return
```
```python
i;
    }
```
```python
return
```
```python
-
```
```python
1
```
```python
;
}
```

