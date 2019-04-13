
# 2.4 Python标识符 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月30日 10:02:45[seven-soft](https://me.csdn.net/softn)阅读数：217个人分类：[Python																](https://blog.csdn.net/softn/article/category/6290759)



标识符是电脑语言中允许作为名字的有效字符串集合。其中，有一部分是关键字，构成语言的标识符。这样的标识符是不能做它用的标识符的，否则会引起语法错误（SyntaxError 异常）。
## 合法的Python 标识符
Python 标识符字符串规则和其他大部分用C 编写的高级语言相似：第一个字符必须是字母或下划线（_）；
剩下的字符可以是字母和数字或下划线；
大小写敏感。
标识符不能以数字开头；除了下划线，其他的符号都不允许使用。处理下划线最简单的方法是把它们当成字母字符。大小写敏感意味着标识符foo 不同于Foo，而这两者也不同于FOO。
## Python关键字
一般来说，任何语言的关键字应该保持相对的稳定，但是因为Python 是一门不断成长和进化的语言，关键字列表和iskeyword()函数都放入了keyword模块以便查阅。
Python关键字|and|as|1|assert|2|break|class|continue|def|
|del|elif|else|except|exec|finally|for|
|from|global|if|import|in|is|lambda|
|not|or|pass|print|raise|return|
|try|while|with|3|yield|4|None|5|
注意：从Python1.4 开始关键字access 就被废除了；
Python2.6 时加入；
Python1.5 时加入；
Python2.3 时加入；
Python2.4 中非关键字常量。
## built-in 标识符
Python 还有称为 built-in 标识符集合，这些名字可以由解释器设置或使用，虽然它们不是保留字，但是不推荐使用这些特别的名字。
虽然 built-in 标识符不是关键字，但是应该把它当作“系统保留字”，不做他用。

