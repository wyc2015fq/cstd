
# python中使用正则表达式的连接符示例代码 - jiahaowanhao的博客 - CSDN博客


2018年06月26日 22:06:37[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：51


[python中使用正则表达式的连接符示例代码](http://cda.pinggu.org/view/25929.html)
我们在前面的例子里，我们学习使用集合里字符或非集合里的字符，这时都是要把每个字符写出来的，但是有时需要把26个小写字母都放到集合里，那么按集合的方法，得输入26次，一个一个键入去，这样比较花时间，也容易出错，那么有没有更好的方法呢？这个是有的，就是使用正则表达式的连接符的功能：-，比如表示26个小写字符，就使用[a-z]就可以了。
本文详细的给大家介绍了关于python使用正则表达式的连接符的相关内容，分享出来供大家参考学习，下面话不多说了，来一起看看详细的介绍吧。
例子如下：
\#python 3.6
\#蔡军生
\#http://blog.csdn.net/caimouse/article/details/51749579
\#
from re_test_patterns import test_patterns
test_patterns(
'This is some text -- with punctuation.',
[('[a-z]+', 'sequences of lowercase letters'),
('[A-Z]+', 'sequences of uppercase letters'),
('[a-zA-Z]+', 'sequences of letters of either case'),
('[A-Z][a-z]+', 'one uppercase followed by lowercase')],
)
结果输出如下：
'[a-z]+' (sequences of lowercase letters)

'This is some text -- with punctuation.'
.'his'
.....'is'
........'some'
.............'text'
.....................'with'
..........................'punctuation'

'[A-Z]+' (sequences of uppercase letters)

'This is some text -- with punctuation.'
'T'

'[a-zA-Z]+' (sequences of letters of either case)

'This is some text -- with punctuation.'
'This'
.....'is'
........'some'
.............'text'
.....................'with'
..........................'punctuation'

'[A-Z][a-z]+' (one uppercase followed by lowercase)

'This is some text -- with punctuation.'
'This'
总结
以上就是这篇文章的全部内容了，希望本文的内容对大家的学习或者工作具有一定的参考学习价值.

