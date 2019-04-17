# Python正则表达式：match(),search(),findall()与finditer()的用法 - Machine Learning with Peppa - CSDN博客





2018年03月06日 19:13:01[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：6612








### 一、模式串匹配

前几天了解到正则语法和有穷自动机的等价性，因此特意来复习一下RE的基本用法（太久没用了，手生）。日常来说，正则表达式的主要方法是re.method()这样的模式串匹配，一共有四种常用方法：

### 1、match

re.match(pattern, string[, flags])  

从首字母开始开始匹配，string如果包含pattern子串，则匹配成功，返回Match对象，失败则返回None，若要完全匹配，pattern要以$结尾。

## 2、search
re.search(pattern, string[, flags])  
若string中包含pattern子串，则返回Match对象，否则返回None，注意，如果string中存在多个pattern子串，只返回第一个。

## 3、findall
re.findall(pattern, string[, flags])  
返回string中所有与pattern相匹配的全部字串，返回形式为数组。

## 4、finditer
re.finditer(pattern, string[, flags])  
返回string中所有与pattern相匹配的全部字串，返回形式为迭代器。



### 二、目标串返回


若匹配成功，match()/search()返回的是Match对象，finditer()返回的也是Match对象的迭代器，获取匹配结果需要调用Match对象的group()、groups或group(index)方法。



1. 正则表达式中的三组括号把匹配结果分成三组
-  group() 同group（0）就是匹配正则表达式整体结果
-  group(1) 列出第一个括号匹配部分，group(2) 列出第二个括号匹配部分，group(3) 列出第三个括号匹配部分。
- |group(num=0)|匹配的整个表达式的字符串，group() 可以一次输入多个组号，在这种情况下它将返回一个包含那些组所对应值的元组。|
|----|----|
|groups()|返回一个包含所有小组字符串的元组，从 1 到 所含的小组号。|

2. 没有匹配成功的，re.search（）返回None
3. 当然郑则表达式中没有括号，group(1)肯定不对了。举个例子：


import re
a = "123abc456"
print re.search("([0-9]*)([a-z]*)([0-9]*)",a).group(0)   #123abc456,返回整体
print re.search("([0-9]*)([a-z]*)([0-9]*)",a).group(1)   #123
print re.search("([0-9]*)([a-z]*)([0-9]*)",a).group(2)   #abc
print re.search("([0-9]*)([a-z]*)([0-9]*)",a).group(3)   #456
### 三、正则表达式对象

不同于re.method()这种模式，正则包还提供了一种直接用compile返回的正则表达式对象作为主体的函数类型，究其本质其实和原本的模式串匹配一样。RgexObject实例有一些方法和属性：

![](https://img-blog.csdn.net/20160910102800514?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





如果没有匹配到的话，match() 和 search() 将返回 None。如果成功的话，就会返回一个 `MatchObject` 实例，其中有这次匹配的信息：它是从哪里开始和结束，它所匹配的子串等等。

![](https://img-blog.csdn.net/20180306190640626?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


函数re.complie将正则表达式（以字符串书写的）转换为模式对象（即上文所提到的：RegexObject），可以实现更有效率的匹配。如果在调用search或者match函数的时候使用字符串表达的正则表达式，它们也会在内部将字符串转换为正则表达式对象。



在学习正则表达式之初会有点困难。学习他们的关键是一次只学习一点--查找满足特定任务需要的那部分内容，预先将它们全部记住是没有必要的。但是预先需要一个概念，才知道哪里找，怎么找。




## 四、常用正则表达符

![](https://img-blog.csdn.net/20180306190924794?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)






