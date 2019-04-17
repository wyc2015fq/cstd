# Lua语法小贴士（十二）loadstring和loadfile - 凯奥斯的注释 - CSDN博客





2016年11月22日 22:30:11[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：3801
所属专栏：[Lua语法小贴士](https://blog.csdn.net/column/details/13146.html)









# Lua里面可以使用loadstring来加载一个字符串，字符串中包含了lua脚本。

示例：



```
local str = "print('TestLoadingString') return 1234"
local func = loadstring(str)
print(func())
```

虽然字符串里没有被function() end包围，但loadstring加载进来的却是一个函数。
这是一个没有参数的函数，如果你需要添加参数，可以这样：



```
local fmtStr = "local a='%d' print(a)"
local fmtFunc = loadstring(string.format(fmtStr, 9999))
fmtFunc()
```

或者还可以这样：


```
local fmtParamStr = "return function(s,x) print(s..x) end"
local fmtParamFunc = loadstring(fmtParamStr)()
fmtParamFunc("test parameters", "666")
```

如果你需要在配置表里自定义一些公式，loadstring可能比较好用。






# Lua里面还可以使用loadfile，可以加载一个lua文件。

（个人感觉意义不大）

例如，我们在一个文件TestLoading.lua里添加：



```
print("TestFileLoading")

return true
```

那么就可以在另一个lua文件中调用它：

`print(loadfile("TestLoading.lua")())`





