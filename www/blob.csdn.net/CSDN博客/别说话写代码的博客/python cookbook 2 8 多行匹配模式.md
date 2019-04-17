# python  cookbook 2.8 多行匹配模式 - 别说话写代码的博客 - CSDN博客





2019年04月11日 09:56:28[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：15标签：[python cookbook 																[python 多行匹配模式](https://so.csdn.net/so/search/s.do?q=python 多行匹配模式&t=blog)](https://so.csdn.net/so/search/s.do?q=python cookbook &t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)








```
#你正在用正则表达式去匹配一大块的文本，你需要跨越多行去匹配。比如你用(.)去匹配字符，忘了(.)不能匹配
#换行符。比如假设想匹配c语言分割的注释：
comment=re.compile(r'/\*(.*?)\*/')
text1='/* this is a comment */'
text2='''/* this is a 
 multiline comment*/'''
print(comment.findall(text1))
print(comment.findall(text2))  #由于模式串不能匹配换行，只能匹配一行中是否满足
```

![](https://img-blog.csdnimg.cn/20190411095506367.png)

```
#为了修正这个问题，你可以修改模式字符串，增加对换行的支持
comment=re.compile(r'/\*((?:.|\n)*?)\*/')  
print(comment.findall(text2))
#在这个模式中，(?:.|\n)制定了一个非捕获组（也就是它定义了一个仅仅用来做匹配，而不能
#通过单独捕获或者编号的组）
```

![](https://img-blog.csdnimg.cn/20190411095520647.png)

```
#re.compilr()函数接受一个标志参数叫re.DITALL,它可以让正则表达式中的点(.)匹配包括换行符在内的任意字符
comment=re.compile(r'/\*(.*?)\*/',re.DOTALL)  #用了这个参数就可以使(.)匹配换行
print(comment.findall(text2))
```

![](https://img-blog.csdnimg.cn/20190411095547717.png)

但如果模式非常复杂或者是为了构造字符串令牌而将多个模式合并起来，这时候使用这个标记参数就可能出现一些问题。一般还是自己定义自己的正则表达式




