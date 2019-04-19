# python dsl - pyparsing介绍 - Arthur的随笔 - CSDN博客
2012年11月16日 11:31:35[largetalk](https://me.csdn.net/largetalk)阅读数：11011
写在前面： 10月份一直在外面出差，都没看书或写代码，也打破了保持这么久每个月都写几篇博客的习惯，现在11月份也过去一半了，还是没做什么事情。感觉这写东西都慢慢写不下去了，难的写不了，简单的网上到处都是，前面写的那些也都是从各个地方收集自己复诉一遍而已。就当刷点存在感吧。
昨天忽然想到ruby的dsl用的地方非常多，也比较容易写。基本上每个库都自己定义一套语言，我学ruby的时候就是受不了这个，不过现在想想，这种特性确实挺酷的，而且某些场合还是很有好处的，比如当时看到
[cucumber](https://github.com/cucumber/cucumber) 的时候确实被震撼住了。唉，扯远了，回归主题。当时我就想python 怎么就没有dsl呢，我就在网上找，发现一篇好文 [http://in.pycon.org/2011/static/files/talks/24/dsl_with_python.pdf](http://in.pycon.org/2011/static/files/talks/24/dsl_with_python.pdf) ，文章介绍了pyparsing库以及使用pyparsing
 解析自定义语言生成html的一个例子。非常简单，值得一看。
首先先简单介绍下DSL， DSL(Domain Specific Languages) 在特定领域为解决特定问题特别制定的语言， 比如SQL， 正则表达式。那怎么自己做一个DSL出来呢，dsl本质和我们一般使用的语言类似的（特别是脚本语言）， 一般的语言都有词法分析，语法分析等步骤，dsl也类似，可惜我没学过编译原理，看到什么lex/yacc就恐惧，幸好dsl可以简单点， 然后在python这里有个库 pyparsing 可以让你非常简单的写一个dsl。
第一步， pip install pyparsing
其次，你得对 上下文无关文法 和 BNF(Backus-Naur Form)稍微有一点了解。
使用pyparsing四个步骤：
• Import names from pyparsing module
• Define grammar using pyparsing classes and helper methods
• Use the grammar to parse the input text
• Process the results from parsing the input text
ok， 那我们先写一个parser design， 就借用上面文章的例子了：
```
form ::= form_name newline field+
field ::= field_name colon field_type [arrow property+]
property ::= key colon value
form_name ::= word
field_name ::= word
field_type ::= CharField | EmailField | PasswordField
key ::= word
value ::= alphanumeric+
word ::= alpha+
newline ::= \n
colon ::= :
arrow ::= ->
```
for BNF：
• ::= means "is defined as"
• + means "1 or more"
• * means "0 or more"
• items enclosed in []are optional
• succession of items means that matching tokens must occur in sequence
• | means either item may occur
然后用python来实现：
```python
from pyparsing import *
newline = "\n"
colon = ":"
arrow = "->"
word = Word(alphas)
key = word
value = Word(alphanums)
field_type = oneOf("CharField EmailField PasswordField")
field_name = word
form_name = word
field_property = key + colon + value
field = field_name + colon + field_type + 
     Optional(arrow + OneOrMore(field_property)) + newline
form = form_name + newline + OneOrMore(field)
input_str = '''
UserForm
name:CharField -> label:Username size:25
email:EmailField -> size:32
password:PasswordField
'''
print form.parseString(input_str)
```
好了，不再贴别人的东西了， 可以自己看那个文章。pyparsing 主页在这[http://pyparsing.wikispaces.com/](http://pyparsing.wikispaces.com/) ， 但它的文档比较少，可以看一本书 《Getting Started with Pyparsing》
整个用起来还是比较简单的，然后就剩下一些工程性和细节方面了，这需要去用才知道。
ps. 偏重数学，偏重模范化的人喜欢 Python；偏重乐趣，偏重人类语言化的人喜欢 Ruby. 深以为然
