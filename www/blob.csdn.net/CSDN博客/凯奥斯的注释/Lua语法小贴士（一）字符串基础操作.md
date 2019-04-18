# Lua语法小贴士（一）字符串基础操作 - 凯奥斯的注释 - CSDN博客





2016年10月10日 13:38:28[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：13167
所属专栏：[Lua语法小贴士](https://blog.csdn.net/column/details/13146.html)









# 字符串是Lua里面非常常用的类型，本文就介绍一下字符串的一些相关操作。

替换：



```
local a = "Hello world"
local b = string.gsub(a, "world", "lua")
print(a) --Hello world
print(b) --Hello lua
```


连接：



```
local a = "Hello".."world"
local b = a..1
print(a) --Helloworld
print(b) --Helloworld1
```



多行：




```
local a= [[
hello
world
lua
is
good]]
```


自动转换数字：



```
local a = "10" + 1
print(a) --11
```





连接数字成字符串：


`print(10 .. 24) --1024`

数字字符串互转：


```
local a = tostring(11)
local b = tonumber("11")
local c = "" .. 11
print(a==c) --true
```


求字符串长度：



```
local a = "Hello"
print(#a) --5
print(#"lua") --3
```






参考：


[Lua语法小贴士（一）字符串基础操作](http://blog.csdn.net/ecidevilin/article/details/52775530)


[Lua语法小贴士（二）string库](http://blog.csdn.net/ecidevilin/article/details/52786404)


[Lua语法小贴士（三）魔法字符](http://blog.csdn.net/ecidevilin/article/details/52786488)






