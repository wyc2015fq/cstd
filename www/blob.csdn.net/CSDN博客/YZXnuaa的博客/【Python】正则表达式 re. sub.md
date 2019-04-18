# 【Python】正则表达式 re. sub - YZXnuaa的博客 - CSDN博客
2018年02月22日 15:42:00[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：238
**背景：**
re.sub是re模块重要的组成部分，并且功能也非常强大，主要功能实现正则的替换。 
   re.sub定义： 
**sub**(**pattern**, **repl**, **string**, **count**=0, **flags**=0) 
    Return the string obtained by replacing the leftmost 
    non-overlapping occurrences of the pattern in string by the 
    replacement repl.  repl can be either a string or a callable; 
    if a string, backslash escapes in it are processed.  If it is 
    a callable, it’s passed the match object and must return 
    a replacement string to be used. 
    主要的意思为：对字符串string按照正则表达式pattern，将string的匹配项替换成字符串repl。 
    公式解析： 
**pattern**为表示正则中的模式字符串， 
**repl**为replacement，被替换的内容，repl可以是字符串，也可以是函数。 
**string**为正则表达式匹配的内容。 
**count**：由于正则表达式匹配到的结果是多个，使用count来限定替换的个数（顺序为从左向右），默认值为0，替换所有的匹配到的结果。 
[**flags**](http://blog.csdn.net/winterto1990/article/details/47361701)是匹配模式，可以使用按位或’|’表示同时生效，也可以在正则表达式字符串中指定。
import re
print(re.sub(r'\w+','10',"ji 43 af,geq",2,flags=re.I)
)10 10 af,geq
详解：首先导入re模块，使用re.sub函数，r’\w+’为正则表达式，匹配英文单词或数字，’10’为被替换的内容，”ji 43 af,geq”为re匹配的字符串内容，count为2 只替换前两个，flags=re.I 忽略大小写。 
输出部分自行理解。
