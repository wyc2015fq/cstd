# Python中隐藏了你不知道的10个彩蛋

2018年04月28日 17:15:58 [Allenlzcoder](https://me.csdn.net/Allenlzcoder) 阅读数：400



# Python中隐藏了你不知道的10个彩蛋

本博客转载自：<http://www.techug.com/post/10-tips-of-python-programming.html?utm_source=weibo&utm_medium=referral>

## 1、使用re.DEBUG查看正则表达式的匹配过程

正则表达式是Python的一大特色，但是调试起来会很痛苦，很容易得出一个bug。幸运的是，Python可以打印出正则表达式的解析树，通过re.debug来显示re.compile的完整过程。 
![这里写图片描述](https://img-blog.csdn.net/20180428170634232?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FsbGVubHpjb2Rlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
一旦你理解了语法，你就可以发现你的错误。在这里我们可以看到[/font]忘了去除[]

## 2、enumerate函数用于遍历列表中的元素以及它们的下标

![这里写图片描述](https://img-blog.csdn.net/20180428170727828?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FsbGVubHpjb2Rlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 3、对默认实参要多加小心

![这里写图片描述](https://img-blog.csdn.net/20180428170807772?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FsbGVubHpjb2Rlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
相反，你应该使用一个标记值表示“无定义”，来替换“[]”。 
![这里写图片描述](https://img-blog.csdn.net/20180428170842458?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FsbGVubHpjb2Rlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 4、对于C系的那些更喜欢括号而不是缩进的开发者，你只需使用以下一条命令：

```
from __future__ import braces1
```

## 5、切片操作中的tricks

```
a = [1,2,3,4,5] >>> a[::2] [1,3,5]1
```

特殊的例子是x[::-1]，它可以将列表反转

```
>>> a[::-1] [5,4,3,2,1]1
```

## 6、装饰器

装饰器实现了在一个函数中调用其它函数或方法来增加功能性，从而修改参数或结果等，在函数定义前加上装饰器，只需一个“@”符号。 
以下示例显示了一个print_args装饰器的用法： 
![这里写图片描述](https://img-blog.csdn.net/201804281712393?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FsbGVubHpjb2Rlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 7、取参的trick

你可以用*或者**来取出列表或字典作为函数参数 
![这里写图片描述](https://img-blog.csdn.net/20180428171318210?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FsbGVubHpjb2Rlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 8、Exception else语句

![这里写图片描述](https://img-blog.csdn.net/20180428171354213?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FsbGVubHpjb2Rlcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
使用“else”比在“try”语句中添加多余的代码更好，因为它避免了意外获取不被try语句保护的异常…除了声明之外。

## 9、嵌套列表推导式和生成器表达式

```
[(i,j) for i in range(3) for j in range(i) ]    
((i,j) for i in range(4) for j in range(i) )12
```

这些语句可以取代大量的嵌套循环代码块

## 10、主要句式

```
import this1
```

下面让我们再诵读一遍Python之禅的要义（The Zen of Python, by Tim Peters）：

```
Beautiful is better than ugly.
Explicit is better than implicit.
Simple is better than complex.
Complex is better than complicated.
Flat is better than nested.
Sparse is better than dense.
```