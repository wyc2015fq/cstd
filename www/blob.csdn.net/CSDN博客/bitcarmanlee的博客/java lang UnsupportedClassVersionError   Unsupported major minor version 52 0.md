
# java.lang.UnsupportedClassVersionError : Unsupported major.minor version 52.0 - bitcarmanlee的博客 - CSDN博客


2018年01月11日 16:30:21[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：1004


在spark-shell中，遇到了如下的错误：
```python
Caused by: java
```
```python
.lang
```
```python
.UnsupportedClassVersionError
```
```python
: xxx/xxx/xxx/xxx : Unsupported major
```
```python
.minor
```
```python
version
```
```python
52.0
```
```python
at java
```
```python
.lang
```
```python
.ClassLoader
```
```python
.defineClass
```
```python
1(Native Method)
  at java
```
```python
.lang
```
```python
.ClassLoader
```
```python
.defineClass
```
```python
(ClassLoader
```
```python
.java
```
```python
:
```
```python
800
```
```python
)
  at java
```
```python
.security
```
```python
.SecureClassLoader
```
```python
.defineClass
```
```python
(SecureClassLoader
```
```python
.java
```
```python
:
```
```python
142
```
```python
)
  at java
```
```python
.net
```
```python
.URLClassLoader
```
```python
.defineClass
```
```python
(URLClassLoader
```
```python
.java
```
```python
:
```
```python
449
```
```python
)
...
```
经过一番思考与搜索，发现这种情况是java版本不一致造成的。参考`Wikipedia Java Class Reference`
```python
J2SE 9
```
```python
=
```
```python
53
J2SE 8 = 52
J2SE 7 = 51
J2SE 6.0 = 50
J2SE 5.0 = 49
JDK 1.4 = 48
JDK 1.3 = 47
JDK 1.2 = 46
JDK 1.1 = 45
```
以上是java的主要版本分配的数字。这个错误是由于编译的时候JDK版本比运行时候的JDK版本高造成的。
那解决的方法就很简单了，将运行时JDK版本指定为编译时的JDK版本，两者保持一致即可。
具体到spark-shell中，加入配置项：`--java 8`，指定jdk的版本为8，即可完美解决。
参考文献：
1.Wikipedia Java Class Reference[https://en.wikipedia.org/wiki/Java_class_file](https://en.wikipedia.org/wiki/Java_class_file)
2.[https://stackoverflow.com/questions/22489398/unsupported-major-minor-version-52-0](https://stackoverflow.com/questions/22489398/unsupported-major-minor-version-52-0)

