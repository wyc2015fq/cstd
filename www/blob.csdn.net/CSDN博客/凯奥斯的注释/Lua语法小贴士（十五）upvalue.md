# Lua语法小贴士（十五）upvalue - 凯奥斯的注释 - CSDN博客





2017年09月08日 11:29:05[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：910
所属专栏：[Lua语法小贴士](https://blog.csdn.net/column/details/13146.html)









# 我们都知道，Lua里面的function实际上都是闭包（closure），而upvalue便是它引用到的上下文变量（“引用到”三个字很关键），业界一般又称为外部局部变量(external local variable），如果要翻译的话，个人偏好翻译为“上文变量”。

示例：



```
local test = 0
local function TestAdd1()
return test + 1
end
```
这里test就是TestAdd1的upvalue。

如何获取和设置upvalue？这两个方法在[debug库](http://blog.csdn.net/ecidevilin/article/details/53048445)里（也就是说，正常编程的时候，非万不得已不要用，就算是万不得已，也尽量不要用）。


debug.getupvalue这个方法的第二个参数是upvalue的序号，需要注意的是这个序号是在function里引用的序号，而不是变量声明的序号。

如果需要根据变量名来查看upvalue，那么就需要查找一下：



```
local function GetUpValue(func, name)
local i = 1
while true do
local k,v = debug.getupvalue(func, i)
if nil == k then break end
if name == k then return v end
i = i + 1
end
end
```

设置类似：




```
local function SetUpValue(func, name, val)
local i = 1
while true do
local k, v = debug.getupvalue(func,i)
if nil == k then break end
if name == k then
debug.setupvalue(func,i,val)
return
end
i = i + 1
end
end
```
测试：




```
print(TestAdd1())--1
print(GetUpValue(TestAdd1,"test"))--0
SetUpValue(TestAdd1,"test",100)
print(TestAdd1())--101
```








