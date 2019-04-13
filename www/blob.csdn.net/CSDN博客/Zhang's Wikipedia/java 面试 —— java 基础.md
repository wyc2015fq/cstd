
# java 面试 —— java 基础 - Zhang's Wikipedia - CSDN博客


2018年09月01日 20:30:29[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：210



## 1. char => int
char 类型转换为 int 类型时，是转换为其 ascii 码或 unicode 码（比如中文）
charch ='A';inti = ch;// (int)ch// i = 65;intch ='香';inti = ch;// (int)ch// i = 39321;   \u9999;
## 2. float
Infinity 与 NaN
publicfinalclassFloatextendsNumberimplementsComparable<Float> {publicstaticfinalfloat POSITIVE_INFINITY =1.0f /0.0f;publicstaticfinalfloat NEGATIVE_INFINITY = -1.0f /0.0f;publicstaticfinalfloat NaN =0.0f /0.0f;publicstaticbooleanisNaN(float v) {return(v != v);
    }
}1/0java.lang.ArithmeticException thrown: / by zero// 除 0 异常，只有在除数为 0 时才会发生；1.0f/0.0f==1.0f/0.0f;// true;1.0f/0.0f==2.0f/0.0f;// true;0.0f/0.0f==0.0f/0.0f;// false;
## 3. String
```python
jshell>
```
```python
new
```
```python
String
```
```python
() ==
```
```python
new
```
```python
String
```
```python
()
$
```
```python
7
```
```python
==>
```
```python
false
```
```python
jshell>
```
```python
String
```
```python
s =
```
```python
"abc"
```
```python
;
s ==>
```
```python
"abc"
```
```python
jshell>
```
```python
String
```
```python
s2 =
```
```python
new
```
```python
String
```
```python
(
```
```python
"abc"
```
```python
)
s2 ==>
```
```python
"abc"
```
```python
jshell> s2 == s
$
```
```python
10
```
```python
==>
```
```python
false
```
```python
jshell> s2.intern() == s
$
```
```python
11
```
```python
==>
```
```python
true
```
java String pool：字符串常量池（在堆空间）
String str1 = “ABC”;可能创建一个对象或者不创建对象。
如果”ABC” 这个字符串在java String池中不存在，会在java String池中创建一个String str1= “ABC”的对象。然后把str1指向这个内存地址。之后用这种方式创建多少个值为”ABC”的字符串对象。始终只有一个内存地址被分配，之后都是String的copy。这种被称为‘字符串驻留’，所有的字符串都会在编译之后自动驻留。

![这里写图片描述](https://img-blog.csdn.net/2018090120393214?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[](https://img-blog.csdn.net/2018090120393214?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

源码
@Stableprivatefinalbyte[] value;/** Cache the hash code for the string */// Java的字符串的hash做了缓存，第一次才会真正算，以后都是取缓存值。privateinthash;// Default to 0

