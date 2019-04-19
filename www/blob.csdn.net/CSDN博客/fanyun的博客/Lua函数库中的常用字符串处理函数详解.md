# Lua函数库中的常用字符串处理函数详解 - fanyun的博客 - CSDN博客
2017年04月04日 19:27:31[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：376
所属专栏：[巧用脚本](https://blog.csdn.net/column/details/15130.html)
**       说到Lua中的字符串处理函数，最常用的最重要的莫过于以下4种：**
1.find；2.match；3.gsub；4.gmatch
下边我们一个个详细讲解：
      简单实用的find
      string.find()函数用于在一个给定的目标字符串中搜索一个模式。最简单的模式就是一个单词，它只会匹配与自己完全相同的拷贝。当find找到一个模式后，它会返回两个值：匹配到的起始索引和结尾索引；如果没有找到任何匹配，它就返回nil。示例代码：
      复制代码代码如下:
local str = "Hello World"
local i, j = string.find(str,"Hello") -- 返回Hello在str中的起始位置和终止位置
print(i, j)
string.find函数还具有一个可选的第三个参数，它是一个索引，告诉函数应该从目标字符串的哪个位置开始搜索。当我们处理一个字符串中与给定模式相匹配的所以内容时，这个设置是非常有用的。我们可以循环的进行查找操作。当然了，这里只是说了最普通，也是最简单的一种方式，对于第二个参数，我们完全可以传递一个正则表达式，也就是一个pattern，进行正则匹配。示例代码：
复制代码代码如下:
local str = "Hello12345World"
local i, j = string.find(str,"%d+")
print(i, j) -- 6   10
find的兄弟match
函数string.match与string.find非常相似，它也是用于在一个字符串中搜索一种模式。区别在于，string.match返回的是目标字符串中与模式相匹配的那部分子串，并不是该模式所在的位置。示例代码：
复制代码代码如下:
local str = "Hello12345World"
local subStr = string.match(str,"%d+")
print(subStr)
local i, j =string.find(str,"%d+")
subStr =string.sub(str, i, j)
print(subStr)
-- 是不是可以认为match等于find + sub呢?
match和find是如此的相近，因此，在实际开发中，按照实际的需要，决定采用哪个。
替换利器gsub
string.gsub有3个参数：目标字符串、模式和替换字符串。它的基本用法是将目标字符串中所有出现模式的地方替换为目标字符串。来看一段简短的代码，就什么都明白了。
复制代码代码如下:
local str = "Hello World"
local strResult = string.gsub(str,"Hello", "Jelly")
print(strResult) -- Jelly World
另外gsub还有可选的第四个参数，可以限制替换的次数。示例代码如下：
复制代码代码如下:
local str = "Hello World"
-- 这里不限制替换次数
local strResult, cnt = string.gsub(str,"l", "o")
print(strResult) -- Heooo Worod
print(cnt) -- 实际替换的次数
-- 开始限制替换次数
strResult, cnt = string.gsub(str,"l", "o", 1)
print(strResult) -- Heolo World
print(cnt) -- 就替换了一次
这里还有重点需要介绍，也就是gsub的强大之处。先看一段代码，然后再做介绍。
复制代码代码如下:
local replaceTb = {hello ="Jelly", world = "Think"}
local str = "hello world"
-- 注意，第3个参数是一个table
local strResult = string.gsub(str,"hello", replaceTb)
print(strResult) -- Jelly world
strResult = string.gsub(strResult,"world", replaceTb)
print(strResult) -- Jelly Think
strResult = string.gsub(strResult,"hello", replaceTb)
print(strResult) -- Jelly Think
可以看到，gsub的第三个参数是一个table，也就是说，当gsub的第三个参数是一个table时，如果在查找的字符串中有与第二个参数相匹配的内容，就会将该内容作为key，在table中查找该key对应的value；如果该table中没有这个key，则不进行替换；上面的代码就是表明这个意思。接下来再来看一段比较神奇的代码：
复制代码代码如下:
local replaceFunc = function (str)
   return str .. "JellyThink"
end
local str = "hello world"
-- 注意，第3个参数是一个table
local strResult = string.gsub(str,"hello", replaceFunc)
print(strResult) -- helloJellyThink world
没有看错，gsub的第三个参数还可以是一个函数，该函数的参数是被匹配的字符串，该函数的返回值将会作为目标字符串去进行替换匹配的内容。
没法形容的gmatch
我也不知道要怎么形容gmatch，gmatch会返回一个迭代器函数，你可以使用这个迭代器函数去迭代匹配的所有内容。看下列代码：
复制代码代码如下:
local str = "Hello World"
local iteratorFunc = string.gmatch(str,"%a+") -- %a+表示匹配所有单词
for i in iteratorFunc do
   print(i)
end
如果你对迭代器还不熟悉的话，可以看这里。我现在就使用find来实现一个我们自己的gmatch，功能和gmatch是差不多的。代码如下：
复制代码代码如下:
local str = "Hello World"
-- 使用find来实现一个自己的gmatch
local myGmatch = function (s, pattern)
 local resultTb = {} -- 相当于迭代器中的恒定状态
 local index = 0 -- 相当于迭代器中的控制变量
 local i, j = string.find(s, pattern)
 while i do
   resultTb[#resultTb + 1] = string.sub(s, i, j)
   i, j = string.find(s, pattern, j + 1)
  end
 return function () -- 返回一个迭代器函数
   index = index + 1
   return resultTb[index]
  end
end
for i in myGmatch(str, "%a+") do
 print(i)
end
