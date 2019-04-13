
# scala 正则表达式 - bitcarmanlee的博客 - CSDN博客


2017年05月23日 11:32:16[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：2955


正则表达式是所有攻城狮尤其是算法，数据相关攻城狮必备的技能。日常工作中免不了处理各种字符串与字符串操作，写好正则表达式能大幅度提高工作效率，提升工作愉悦度。现在就简单总结一下scala中常见的正则表达式用法。
如果有对正则表达式不熟悉的同志们，可以查看[正则表达式30分钟入门教程](https://deerchao.net/tutorials/regex/regex.htm)。
## 1.匹配电话号码
匹配电话号码是我们常见的需求之一。代码如下：
```python
def
```
```python
reg() = {
```
```python
val
```
```python
PHONE_PATTERN =
```
```python
"""1(([3,5,8]\d{9})|(4[5,7]\d{8})|(7[0,6-8]\d{8}))"""
```
```python
.r
```
```python
val
```
```python
num =
```
```python
"13566513245"
```
```python
val
```
```python
res = PHONE_PATTERN.findFirstIn(num).get
        println(res)
    }
```
在这里，”“”表示raw string，三引号括起来的字符串表示不需要转义。因为正则表达式本身比较长，可读性也比较差，所以用三引号可以降低阅读难度，并且缩短字符串长度。
## 2.匹配小写字母
```python
def
```
```python
reg2() = {
```
```python
val
```
```python
str =
```
```python
"abc"
```
```python
val
```
```python
pattern =
```
```python
new
```
```python
Regex(
```
```python
"[a-z]+"
```
```python
)
        println(pattern.findFirstIn(str).get)
    }
```
## 3.匹配所有英文字母
```python
def
```
```python
reg3() = {
```
```python
val
```
```python
str =
```
```python
"abcDEFg"
```
```python
//val pattern = new Regex("(?i)[a-z]+")
```
```python
val
```
```python
pattern =
```
```python
new
```
```python
Regex(
```
```python
"[a-zA-Z]"
```
```python
)
        println(pattern.findAllIn(str).mkString(
```
```python
""
```
```python
))
    }
```
注意pattern的两种写法都可以。
## 4.匹配email
```python
def
```
```python
reg4() = {
```
```python
val
```
```python
str =
```
```python
"email:leilei@mi.com.cn"
```
```python
val
```
```python
pattern =
```
```python
new
```
```python
Regex(
```
```python
"""(?i)[a-z0-9._-]+@[a-z0-9._-]+(\.[a-z0-9._-]+)+"""
```
```python
)
        println(pattern.findAllIn(str).mkString(
```
```python
""
```
```python
))
    }
```
## 5.匹配日期，并提取年月日
```python
def
```
```python
reg5() = {
```
```python
val
```
```python
str =
```
```python
"2017-01-01"
```
```python
val
```
```python
datePattern =
```
```python
new
```
```python
Regex(
```
```python
"""(\d{4})-(\d{2})-(\d{2})"""
```
```python
)
```
```python
val
```
```python
datePattern(year, month, day) = str
        println(year, month, day)
        str
```
```python
match
```
```python
{
```
```python
case
```
```python
datePattern(year, month, day) => println(s
```
```python
"year is $year.\n"
```
```python
+
                f
```
```python
"month is $month.\n"
```
```python
+ raw
```
```python
"day is $day"
```
```python
)
        }
    }
```
正则表达式中，小括号表示分组的概念。从上面的例子可以看出，scala正则中提取分组可以采用两种方式，都很方便。模式匹配的方式可能更直观，更符合scala的习惯。
## 6.非铆定
```python
def
```
```python
reg7() = {
```
```python
val
```
```python
datePattern =
```
```python
new
```
```python
Regex(
```
```python
"""(\d{4})-(\d{2})-(\d{2})"""
```
```python
)
```
```python
val
```
```python
pattern = datePattern.unanchored
```
```python
val
```
```python
str =
```
```python
"Date: 2017-05-21 19:25:18"
```
```python
str
```
```python
match
```
```python
{
```
```python
case
```
```python
pattern(year, month, day) => println(s
```
```python
"year is $year.\n"
```
```python
+
                f
```
```python
"month is $month.\n"
```
```python
+ raw
```
```python
"day is $day"
```
```python
)
        }
    }
```
如果要非铆定，可以使用上面的用法。

