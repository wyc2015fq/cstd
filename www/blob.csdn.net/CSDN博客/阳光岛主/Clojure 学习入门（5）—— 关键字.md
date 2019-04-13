
# Clojure 学习入门（5）—— 关键字 - 阳光岛主 - CSDN博客

2013年12月22日 21:23:53[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：7245所属专栏：[Clojure 学习总结](https://blog.csdn.net/column/details/learn-clojure.html)



一、创建：
Keyword：关键字是一个内部字符串; 两个同样的关键字指向同一个对象; 通常被用来作为map的key。

```python
user=> (keyword "foo")
:foo
user=> (keyword 'foo)
:foo
user=> (keyword "abc def")
:abc def
user=> (keyword "abcdef")
:abcdef
user=> (keyword "/abc/def/ghi")
:/abc/def/ghi
user=> (keyword "user" "foo")
:user/foo
user=> (keyword nil)
nil
user=> (keyword 123)
nil
user=> (keyword 0)
nil
```

Symbol：Symbol是用来给东西命名的. 这些名字是被限制在名字空间里面的，要么是指定的名字空间，要么是当前的名字空间.

```python
user=> (symbol 'foo)
foo
user=> (symbol "foo")
foo
user=> (symbol "clojure.core" "foo")
clojure.core/foo
user=> (symbol "abc def")
abc def
user=> (symbol "abcdef")
abcdef
user=> (symbol nil)
java.lang.NullPointerException (NO_SOURCE_FILE:0)
user=> (symbol 0)
java.lang.ClassCastException: java.lang.Integer cannot be cast to java.lang.String (NO_SOURCE_FILE:0)
```

二、用法：
name：name函数接受一个参数，如果该参数为字符串，则直接返回该参数。如果不是字符串，则返回名称值。代码如下：

```python
user=> (name :foo)
"foo"
user=> (name 'foo)
"foo"
user=> (name "foo")
"foo"
user=> (name nil)
java.lang.NullPointerException (NO_SOURCE_FILE:0)
user=> (name 0)
java.lang.ClassCastException: java.lang.Integer cannot be cast to clojure.lang.Named (NO_SOURCE_FILE:0)
```

intern：intern函数有两个实现：(intern ns name)、(intern ns name val)。
根据命名空间ns中查找是否存在符号name，不存在，则创建；最有一个参数val作为值与符号进行绑定，代码如下：

```python
user=> (intern 'user 'x)
#'user/x
user=> x
java.lang.IllegalStateException: Var user/x is unbound. (NO_SOURCE_FILE:0)
user=> (intern 'user 'x "1")
#'user/x
user=> x
"1"
```

namespace：namespace函数根据给定的符号、或者关键字返回所在的命名空间名称，如果当前没有，则返回nil，代码如下：

```python
user=> (symbol 'xx)
xx
user=> (namespace 'user/xxx)
"user"
user=> (namespace 'xxx)
nil
user=> (def x :val)
#'user/x
user=> (namespace 'x)
nil
user=> (namespace 'user/x)
"user"
```

三、检查：
keyword?、symbol?用于检查指定的参数是否为关键字、或者是符号，是则返回true，否则返回false。

```python
user=> (symbol? 'x)
true
user=> (symbol? "x")
false
user=> (keyword? 'x)
false
user=> (keyword? :x)
true
user=> (keyword? "x")
false
```


