# Python的format格式化输出 - 硬曲奇小屋 - CSDN博客





2018年01月08日 16:34:57[HardCookies](https://me.csdn.net/james_616)阅读数：7485








# Python的格式化输出

## 方法一：传统方法

与C语言类似，可以采用“%”作为格式化输出的标记。如： 
`print("The first number: %5d, the second number: %8.2f" % (123, 456.789))`

输出的结果为： 
`The first number:   123, the second number:   456.79`

不用做更多解释，分为3个部分，格式化的字符串、分隔符和数值元组。
## 方法二：format方法

与第一种方法相比，新的Python版本更倾向于使用**format**方法。它的使用也更加灵活，下面我们简单介绍一下。 

它的基本使用格式如下： 
`template.format(p0, p1, ..., k0=v0, k1=v1, ...)`

template是一个字符串，其中包括需要格式化输出的部分，这些部分各用大括号括起来{}。format后面的参数p表示第几个位置的需要格式化输出的变量。 

举个例子：
```python
>>> "First argument: {0}, second one: {1}".format(47,11) 
'First argument: 47, second one: 11'
>>> "Second argument: {1}, first one: {0}".format(47,11) 
'Second argument: 11, first one: 47'
```

大括号里面的数字就代表了第几个参数，python是从0开始起算的。位置后冒号之后的参数，则分别代表了每个格式化输出部分的格式。如{0:5d}, {1:8.2f}则分别代表占5位的整数和占8位的保留小数点2位的浮点数。 

还有些选项参数可以选择，加在”:”后。 

‘<’表示左对齐，‘>’表示右对齐，‘0’表示空位用0补齐，‘,’表示用’,’作为千位分隔符，‘^’能够使文本在所处位置居中。 

除了这些给定的选项，我们也可以使用自己给定的字符来补齐空位。 

下面给出一个例子：
```python
tplt = "{0:^10}\t{1:{3}^10}\t{2:^10}"   # {1:{3}^10} 1表示位置，{3}表示用第3个参数来填充，^表示居中，10表示占10个位置
print(tplt.format("排名","学校名称","总分",'*'))
```

运行后得到 
`排名       ***学校名称***      总分`

tplt中的{3}代表给的最后一个参数’*’，这样第2个位置的“学校名称”就会居中，空白位置用’*’代替。
## 小彩蛋

由于中文和英文字符占位大小不一样的问题，往往会导致格式化输出的中文字符不齐。比如在嵩天老师的python网络爬虫中的这个例子，学校名称这个位置非常难看。 
![这里写图片描述](https://img-blog.csdn.net/20180108163046379?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我们可以用中文空白字符chr(12288)来替代就像我们之前用’*’一样。这样，就能很好的解决中文对齐的问题。 
![这里写图片描述](https://img-blog.csdn.net/20180108163426238?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







