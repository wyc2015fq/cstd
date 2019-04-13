
# Python 笔试 —— 效率与优雅 - Zhang's Wikipedia - CSDN博客


2018年05月30日 21:59:05[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：644



## 1. 效率
字符串拼接：
加号拼接字符串将造成对象的创建和垃圾的回收；
使用字符串的 join 方法对尤其是循环中的字符串进行拼接（先将不断出现的字符串 append 到 一个 list 中，再进行 join）；
>>''.join('hello').join('world')'whelloohellorhellolhellod'\# 会对 'hello' 迭代遍历>>''.join(['hello','world'])'helloworld'

