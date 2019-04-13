
# everything 全盘文件查找工具及正则表达式的使用 - Zhang's Wikipedia - CSDN博客


2018年02月04日 00:30:58[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：1773


首先需要开启 everything 工具在（字符串）查找时，对正则表达式功能的支持：
【菜单栏】⇒ 【Search】⇒ 勾选【Enable Regex】
ctrl + i：字符大小写敏感/不敏感
## 1.文件后缀其全字查找
文件后缀，查询所有的 .sln 文件：
\.sln$，\是对.的转义，此时 . 就是点，而不是任意字符
全字查找，查找文件名为 algorithms.sln 的文件：
^algorithms\.sln$
2. 通配符与正则
通配符*/?在正则的环境下表示的含义在于：
*：前一个字符出现 0 次或多次
?：前一个字符出现（出现1次）或者不出现（出现 0 次）、
正则模式下，匹配任意一个字符：
.*：无法匹配换行符（\n）
[\s\S]*：\s 空白符，\S 非空白符


