
# 正则表达式 —— Cases 与 Tricks - Zhang's Wikipedia - CSDN博客


2018年05月24日 22:45:29[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：206个人分类：[正则](https://blog.csdn.net/lanchunhui/article/category/6955829)



## 1. cases
匹配任意单词（两侧可以有多个空格）：
( +[a-zA-Z]+ +)
上述表达式无法匹配句子末尾的单词，若想匹配句尾或者逗号前的单词，则可拓展为：
( +[a-zA-Z]+[?,.]? +)
中括号内的？表示问号这一标点，中括号后的？则表示匹配之前的项 1 次或 0 次；

匹配空行：
windows：^(\s*)\r\n
Linux：^(\s*)\n
匹配 ip：[0-9]{1, 3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}
也可使用 [[:digit:]]表示数字：[[:digit:]]{1,3}\.[[:digit:]]{1,3}\.[[:digit:]]{1,3}\.[[:digit:]]{1,3}


