
# Clojure 学习入门（16）—— 正则表达式 - 阳光岛主 - CSDN博客

2014年01月12日 07:38:23[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：11466所属专栏：[Clojure 学习总结](https://blog.csdn.net/column/details/learn-clojure.html)



一、创建**正则表达式**
re-pattern函数：
函数 (re-pattern) 接受一个字符串参数，返回一个正则表达式样式(java.util.regex.Pattern类的实例）。这个样式能用于正则表达式匹配user=> (re-pattern"\\d+")
\#"\d+"

也可以使用读取宏来直接用文本的方式输入正则表达式：在字符串前使用\#符号。和用re-pattern函数生成的一样，例如，下面的表示方式和前面的例子是相同的：user=> \#"\\d+"
\#"\d+"

re-matcher函数：
re-matcher函数接受两个参数：一个正则表达式样式和一个字符串。返回一个有状态的"matcher"对象，提供给其它正则函数而不是直接提供样式。Matchers是java.util.regex.Matcher.类的实例。user=> (re-matcher \#"\d+""abc12345def")
\#<Matcher java.util.regex.Matcher[pattern=\d+ region=0,11lastmatch=]>
user=> (def matcher-num (re-matcher \#"\d+""abc12345def"))
\#'user/matcher-num
user=> (re-find matcher-num)
"12345"
这里的def函数用于定义变量，后面会描述
**二、使用正则表达式：**
re-find函数：
re-find接受一个样式与一个字符串或者一个matcher。每次调用，返回matcher中下一个符合正则匹配的结果。user=> (re-find \#"\d+""abc123")
"123"

re-matches函数：
re-matches接受两个参数：一个正则表达式样式和一个字符串。返回任何和正则表达式样式匹配的字符串，如果没有匹配则返回nil。例如下面的代码：user=> (re-matches \#"hello""hello,world")
nil
user=> (re-matches \#"hello.*""hello,world")
"hello,world"
user=> (re-matches \#"hello,(.*)""hello,world")
["hello,world""world"]

re-seq函数：
re-seq接受一个样式与一个字符串。它返回一个使用永久匹配（matcher）的lazy sequence(懒序列)（这个sequence在一个连续的样式匹配的字符串中）user=> (re-seq \#"\d""clojure 1.1.0")
("1""1""0")
user=> (re-seq \#"\w+""mary had a little lamb")
("mary""had""a""little""lamb")

re-groups函数：
接受一个matcher，返回从接近的发现与匹配的集合。如果没有嵌套集合，则返回一个完全匹配的字符串。如果有嵌套集合，则返回vector集合，第一个元素是完全匹配的（非嵌套）user=> (def phone-number"672-345-456-3212")
\#'user/phone-number
user=> (def matcher (re-matcher \#"((\d+)-(\d+))"phone-number))
\#'user/matcher
user=> (re-find matcher)
["672-345""672-345""672""345"]
user=> (re-groups matcher)
["672-345""672-345""672""345"]





