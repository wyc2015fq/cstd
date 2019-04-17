# Lua语法小贴士（七）错误处理 - 凯奥斯的注释 - CSDN博客





2016年10月28日 11:16:38[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：800
所属专栏：[Lua语法小贴士](https://blog.csdn.net/column/details/13146.html)









# 本文要介绍的Lua错误处理包括assert、pcall/xpcall以及debug.traceback。




assert断言，与两个参数，第一个参数为条件，第二个参数为条件不成立时的错误信息。

例如：


`assert(a," 'a' cannot be nil")`
当a为nil或者false的时候，会触发错误，并报错。


assert断言一般使用在不该出现的错误上，例如我们要实现一个平方根的方法，就要断言输入参数大于等于0。




pcall是保护调用（protected call）。使用方法：pcall(f,...)。f是被保护的方法，...是变长参数，代表f使用的参数。

例如：



```
local function test(a)
    a.b = 1
end
if pcall(test,1) then
    print("Success")
then
    print("Failure")
end

--Failure
```

xpcall是另外一种保护调用。使用方法：xpcall(f,err)。err代表错误处理方法。
例如：



```
local function errFunc(err)
    print(err)
end
if xpcall(function() test(1) end, errFunc) then
    print("Success")
else
    print("Failure")
end

--attempt to index local 'a'(a number value)
--Failure
```

我们修改一下errFunc：


```
local function errFunc(err)
    print(debug.traceback(err, 2))
end
```


报错时，我们就可以看到调用堆栈。
debug.traceback是回溯的方法，会返回一个string，第一个参数表示显示信息，第二个表示返回堆栈的起始深度（2是一个比较合适的值，大家可以自己调整试试）。

这个方法可以在任何地方调用，如果你需要的话。此外，这个方法有助于收集线上运行时触发的错误，可以将错误信息保存在本地文件或者发送给服务器。



