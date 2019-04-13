
# 浅析python 内置字符串处理函数的使用方法​​​​​​​ - jiahaowanhao的博客 - CSDN博客


2018年07月23日 21:35:20[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：41


**[浅析python 内置字符串处理函数的使用方法](http://cda.pinggu.org/view/26173.html)**
这篇文章主要介绍了python 内置字符串处理函数的使用方法,需要的朋友可以参考下
一、lower（）：将大写字母全部转为小写字母。如：
代码如下:
name='G'
b=name.lower()
二、title”":将字符串转化为标题，即所有单词的首字母大写，其他字母小写。使用方法同lower()
三、replace:返回某字符串的所有匹配项均被替换之后得到的字符串。
代码如下:
'This is a test'.replace('is','are')
四、split:将字符串分割成序列
代码如下:
'1+2+3+4+5'.split('+')
默认程序将所有空格作为分隔符。
五、strip:返回去除两侧(不包括内部)空格的字符串
代码如下:
'               in wh is kepy         '.strip()
六、translate:替换字符串中的某些部分。translate知识处理单个字符。但可以同时进行多个替换，多个替换指的是可以同时将a替换成b,将c替换成d，而replace一次只能将a替换成b。translate的用法较为复杂，需要使用的使用请翻阅专门介绍它的文档。

