# Lua语法小贴士（三）魔法字符 - 凯奥斯的注释 - CSDN博客





2016年10月11日 11:48:26[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2331
所属专栏：[Lua语法小贴士](https://blog.csdn.net/column/details/13146.html)









## Lua里的魔法字符包括：

. % + * - ? [ ] ^ ( ) $

## 可以使用它们对字符串进行模式匹配。




## 模式符号：




|.|所有字符|
|----|----|
|%a|字母|
|%c|控制字符|
|%d|数字|
|%l|小写字母|
|%p|标点符号|
|%s|空白字符|
|%u|大写字母|
|%w|字母或数字|
|%x|十六进制字符|
|%z|内部表示为0的字母|


大写形式表示它们的补集，例如%A表示非字母

```
local s = "Today is 11/10/2016"
print(string.match(s,"%d")) --1
print(string.match(s,"%D")) --T
```


修饰符号：|+|重复1次或多次|
|----|----|
|*|重复0次或多次|
|-|重复0此或多次|
|？|可选部分（重复0次或1次）|

示例：



```
local s = "Today is 11/10/2016"
local p = "%d+/%d+/%d+"
print(string.match(s,p)) --11/10/2016
```





其它符号：

使用[]可以创建字符集，例如：


`print(string.match("9 is a number", "[0123456789]")) --9`
并且可以使用-符号来简化它：



`print(string.match("9 is a number", "[0-9]")) --9`



类似的用法还有：


`print(string.match("9 is a number", "[a-nA-N]")) --i`


而加上^表示这个字符集的补集：



`print(string.match("9 is a number", "[^a-nA-N0-9 ]")) --s`
使用()可以将匹配的值单独捕获：




```
local date = "Today is 10/10/2016"
local p = "(%d+)/(%d+)/(%d+)"
local d,m,y = string.match(date,p)
print(d) --10
print(m) --10
print(y) --2016
```



%加数字表示捕获序号：




```
local s = "abcdefg"
print(string.gsub(s,"(%w)(%w)(%w)","%3%2%1")) --cbafedg
```
例如，将日期格式dd/mm/yyyy转换成yyyy-mm-dd




```
local s = "Tommorow is 11/10/2016"
print(string.gsub(s,"(%d+)/(%d+)/(%d+)", "%3-%2-%1")) --Tommorow is 2016-10-11
```

%加特殊符号表示该符号的字符：




```
local s = "[Date]2016-10-10"
local p = "[%[%]%-]"
for w in string.gmatch(s,p) do
    print(w)
end
```



$符号表示变量，例如：


```
name = "Lua"
status = "good"
print(string.gsub("$name is $status", "$(%w+)", _G)) --Lua is good
```

再例如：
```
local tbl = {}
tbl.name = "Lua"
tbl.status = "good"
print(string.gsub("$name is $status", "$(%w+)", tbl)) --Lua is good
```




参考：


[Lua语法小贴士（一）字符串基础操作](http://blog.csdn.net/ecidevilin/article/details/52775530)


[Lua语法小贴士（二）string库](http://blog.csdn.net/ecidevilin/article/details/52786404)


[Lua语法小贴士（三）魔法字符](http://blog.csdn.net/ecidevilin/article/details/52786488)




