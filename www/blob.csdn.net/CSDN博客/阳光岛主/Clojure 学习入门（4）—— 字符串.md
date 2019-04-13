
# Clojure 学习入门（4）—— 字符串 - 阳光岛主 - CSDN博客

2013年12月22日 20:21:42[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：6311所属专栏：[Clojure 学习总结](https://blog.csdn.net/column/details/learn-clojure.html)



**一、创建**
函数str： 函数 (str) 接受任意数量的参数。如果参数不是字符串则将其转换为字符串，返回创建的新字符串。如果没有参数或为nil，则返回空字符串""
```python
user=> 
user=> (str 1)
"1"
user=> (str -2.5)
"-2.5"
user=> (str "a")
"a"
user=> (str "abc" 123)
"abc123"
user=> (str)
""
user=> (str '(1 2 3))
"(1 2 3)"
user=> (str nil)
""
user=> (str null)
java.lang.Exception: Unable to resolve symbol: null in this context (NO_SOURCE_FILE:149)
user=> (str "null")
"null"
user=> (str "abc " 123)
"abc 123"
user=> (str 123 345)
"123345"
```
其它比较少用的创建函数
print-str、println-str、pr-str、prn-str、with-out-str
```python
user=> (print-str "abc" 234)
"abc 234"
user=> (println-str "abc" 234)
"abc 234\n"
user=> (pr-str "abc" 234)
"\"abc\" 234"
user=> (prn-str "abc" 234)
"\"abc\" 234\n"
user=> (with-out-str "abc" 234)
""
```
**二、string操作**
count函数： count函数接受字符串参数时，返回字符串的字符数。
```python
user=> (count)
java.lang.IllegalArgumentException: Wrong number of args (0) passed to: core$count (NO_SOURCE_FILE:175)
user=> (count nil)
0
user=> (count "abc123")
6
```
subs函数： subs函数接受两个或三个参数, 第一个是字符串，第二个是一个整数偏移量，第三个（可选）是另一个整数偏移量。函数返回从第一个偏移量（含），到第二个（不含）偏移量或者结尾（如果没有第二个偏移量）截取的子字符串。
```python
user=> (subs "ithomer" 1)
"thomer"
user=> (subs "ithomer" 1 3)
"th"
user=> (subs "ithomer" 1 (count "ithomer"))
"thomer"
user=> (subs "ithomer" 1 20)
java.lang.StringIndexOutOfBoundsException: String index out of range: 20 (NO_SOURCE_FILE:0)
user=> (subs "ithomer")
java.lang.IllegalArgumentException: Wrong number of args (1) passed to: core$subs (NO_SOURCE_FILE:0)
```
format函数： format函数用于格式化字符串。使用jdk的java.util.Formatter类完成格式化。
```python
user=> (format "hello %s" "ithomer.net")
"hello ithomer.net"
user=> (format "%5d" 3)
"    3"
user=> (format "%-5d" 3)
"3    "
user=> (format "%05d" 3)
"00003"
```
检查函数：字符串检查函数 (string?) 接受一个参数，如果是字符串返回true，否则返回false
```python
user=> (string? "abc")
true
user=> (string? "123")
true
user=> (string? 123)
false
user=> (string? nil)
false
```
字符检查函数(char?)接受一个参数，如果是字符类型返回true，否则返回false
```python
user=> (char? "abc")
false
user=> (char? \a)
true
user=> (char? 'a')
java.lang.Exception: Unmatched delimiter: )
user=> (char? a)
java.lang.Exception: Unable to resolve symbol: a in this context (NO_SOURCE_FILE:222)
user=> (char? 1)
false
user=> (char? \1)
true
user=> (char? nil)
false
```



