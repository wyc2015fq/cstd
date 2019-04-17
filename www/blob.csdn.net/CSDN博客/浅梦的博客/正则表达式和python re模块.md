# 正则表达式和python re模块 - 浅梦的博客 - CSDN博客





2017年08月08日 13:36:34[浅梦s](https://me.csdn.net/u012151283)阅读数：197标签：[Python																[正则表达式](https://so.csdn.net/so/search/s.do?q=正则表达式&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)
个人分类：[Python																[正则表达式](https://blog.csdn.net/u012151283/article/category/7084975)](https://blog.csdn.net/u012151283/article/category/6669030)







# 正则表达式语法
|字符|匹配|
|----|----|
|.|匹配任意字符（除了\n）|
|\d or \D|匹配数字or非数字|
|\s or \S|匹配空白or非空白字符|
|\w or \W|匹配单词字符[a-zA-Z0-9] or 非单词字符|
|*|匹配前一个字符0次或无限次|
|+|匹配前一个字符1次或无限次|
|?|匹配前一个字符0次或1次|
|{m} or {m,n}|匹配前一个字符m次 or 至少m至多n次|
|*?or+?or??|匹配模式变为**非贪婪**（尽可能少匹配字符）|
|^|匹配字符串开头|
|$|匹配字符串结尾|
|\A or \Z|指定的字符串匹必须出现在开头 or 结尾|
|||
|(ab)|括号中表达式作为一个分组|
|\< number>|引用编号为num的分组匹配到的字符串|
|(?P< name>)|分组起一个别名|
|(?P=name)|引用别名为name的分组匹配字符串|

# Python re模块

正则表达式模块`re`

```java
import re
```
- re.search(pattern,string,flags=0) 

在一个字符串中查找匹配，返回值为布尔类型 

与string的find()方法类似  - re.findall(pattern,string,flags=0) 

找到匹配，返回匹配部分的列表
- re.sub(pattern,repl,string,count=0,flags=0) 

将字符串中匹配正则表达式的部分替换为其他值
- re.split(pattern,string,maxsplit=0,flags=0) 

根据匹配分割字符串，返回分割字符串组成的列表






